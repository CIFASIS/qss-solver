/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

/*
 * apply_tarjan.cpp
 *
 *  Created on: 21/12/2014
 *      Author: fede
 */

#include <causalize/apply_tarjan.h>
#include <util/debug.h>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <map>
#include <stdio.h>


namespace Causalize {
typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS> DirectedGraph;
typedef boost::graph_traits < DirectedGraph >::vertex_descriptor DGVertex;
typedef boost::graph_traits < DirectedGraph >::edge_descriptor DGEdge;

std::map<Vertex, Vertex> _matching;
std::map<DGVertex, Vertex> _collapsed2original;
using namespace Causalize;

DGVertex original2collapsed(Vertex value) {
  std::map<DGVertex, Vertex>::iterator it;
  for (it = _collapsed2original.begin(); it != _collapsed2original.end(); it++){
    if ((*it).second == value) {
      return (*it).first;
    }
  }
  ERROR("Can't find collapsed vertex from original.");
  return (*it).first;
}

void buildCollapsedGraph(Causalize::CausalizationGraph& graph, DirectedGraph& digraph) {
  // Create the vertices on the directed graph
  Causalize::CausalizationGraph::vertex_iterator vi, vi_end;
  for(boost::tie(vi,vi_end) = vertices(graph); vi != vi_end; ++vi) {
   if (graph[*vi].type == E) {
     DGVertex v = add_vertex(digraph);
     _collapsed2original[v]=*vi;
   }
  }
  // Create the edges on the directed graph
  DirectedGraph::vertex_iterator vj, vj_end;
  for(boost::tie(vj,vj_end) = vertices(digraph); vj != vj_end; ++vj) {
    CausalizationGraph::out_edge_iterator ek, ek_end;
    Vertex originalEqVertex = _collapsed2original[*vj];
    Vertex uMatchingVertex = _matching[originalEqVertex];
    for(boost::tie(ek,ek_end) = out_edges(uMatchingVertex, graph); ek != ek_end; ++ek) {
        Vertex eqAdjacentVertex = target(*ek, graph);
        if(eqAdjacentVertex != originalEqVertex) {
          boost::add_edge(original2collapsed(eqAdjacentVertex), *vj, digraph);
        }
    }
  }
}

//void replaceMMOClassEquations(MMO_Class mmoClass, MMO_EquationList causalEqs) {
//  MMO_EquationList oldEquations = mmoClass->getEquations();
//  MMO_EquationListIterator iter, auxiliaryIter;
//  auxiliaryIter = oldEquations->begin();
//  for(iter = auxiliaryIter; iter != oldEquations->end(); iter = auxiliaryIter) {
//    ++auxiliaryIter;
//    mmoClass->removeEquation(current_element(iter));
//  }
//  foreach(iter, causalEqs) {
//    mmoClass->addEquation(current_element(iter));
//  }
//}

int apply_tarjan(CausalizationGraph &graph, std::map<int, Causalize::ComponentPtr> &components) {

  boost::associative_property_map< std::map<Vertex, Vertex> >  matching_map(_matching);

  // Vertex Index Map required for checked_edmonds_maximum_cardinality_matching.
  // This is to allow the causalization graph, which is an adjacency list, to
  // use as VertexList either vecS or listS.
  std::map<Vertex, int> vertex2index;
  CausalizationGraph::vertex_iterator i, iend;
  int ic = 0;
  for (boost::tie(i, iend) = vertices(graph); i != iend; ++i, ++ic) {
    vertex2index[*i] = ic;
  }
  boost::associative_property_map< std::map<Vertex, int> >  index_map(vertex2index);

  DEBUG('c', "Calculating maximum cardinality matching over causalization graph...\n");

  bool success = checked_edmonds_maximum_cardinality_matching(graph, matching_map, index_map);
  if (!success) {
    ERROR("Can't find a maximum cardinality matching.\n");
  }

  for (std::map<Vertex, Vertex>::iterator it=_matching.begin(); it!=_matching.end(); ++it) {
        char se[10];
        Vertex v1 = it->first;
        if(graph[v1].type == E) {
          sprintf(se, "E%d", vertex2index[v1]);
        } else {
          sprintf(se, "U%d", vertex2index[v1]);
        }
        char su[10];
        Vertex v2 = it->second;
        if(graph[v2].type == E) {
          sprintf(su, "E%d", vertex2index[v2]);
        } else {
          sprintf(su, "U%d", vertex2index[v2]);
        }
        DEBUG('c', "%s matches %s\n", se, su);
    }

  DirectedGraph collapsedGraph;

  DEBUG('c', "Collapsing matching vertices...\n");

  buildCollapsedGraph(graph, collapsedGraph);

  std::map<DGVertex, int> vertex2component;
  boost::associative_property_map< std::map<DGVertex, int> >  component_map(vertex2component);
  std::map<DGVertex, int> dg_vertex2index;
  DirectedGraph::vertex_iterator j, jend;
  int jc = 0;
  for (boost::tie(j, jend) = vertices(collapsedGraph); j != jend; ++j, ++jc) {
    dg_vertex2index[*j] = jc;
  }
  boost::associative_property_map< std::map<DGVertex, int> >  dg_index_map(dg_vertex2index);

  DEBUG('c', "Running tarjan algorithm over collapsed graph...\n");

  int numComponents = strong_components(collapsedGraph, component_map, boost::vertex_index_map(dg_index_map));

  DEBUG('c', "%d strong components identifed.\n", numComponents);

  for (std::map<DGVertex,int>::iterator it=vertex2component.begin(); it!=vertex2component.end(); ++it) {
        DGVertex dgVertex = it->first;
        int componentIndex = it->second;
        DEBUG('c', "Vertex: %d -- Component: %d\n", dg_vertex2index[dgVertex], componentIndex);
        Vertex eqVertex = _collapsed2original[dgVertex];
        Vertex uVertex = _matching[eqVertex];
        std::map<int, Causalize::ComponentPtr>::iterator componentsIt = components.find(componentIndex);
        if(componentsIt == components.end()){
          Causalize::ComponentPtr component = new Causalize::Component;
          std::list<Vertex> *uVertices = new std::list<Vertex>;
          uVertices->push_back(uVertex);
          component->uVertices = uVertices;
          std::list<Vertex> *eqVertices = new std::list<Vertex>;
          eqVertices->push_back(eqVertex);
          component->eqVertices = eqVertices;
          components[componentIndex] = component;
        } else {
          Causalize::ComponentPtr component = componentsIt->second;
          std::list<Vertex> *uVertices = component->uVertices;
          uVertices->push_back(uVertex);
          std::list<Vertex> *eqVertices = component->eqVertices;
          eqVertices->push_back(eqVertex);
        }
  }

  return components.size();

}

}

