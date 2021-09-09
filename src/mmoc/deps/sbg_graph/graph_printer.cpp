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

#include <deps/sbg_graph/graph_printer.h>
#include <deps/sbg_graph/deps_graph.h>

#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

#define MAKE_SPACE \
  for (int __i = 0; __i < _depth; __i++) stri << " ";
#define TAB_SPACE 2
#define INSERT_TAB _depth += TAB_SPACE;
#define DELETE_TAB _depth -= TAB_SPACE;

namespace SB {

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
GraphPrinter<G,V,E,S,VIT,EIT>::GraphPrinter(const G &g)

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
void GraphPrinter<G,V,E,S,VIT,EIT>::printGraph(std::string name)
{
  stringstream stri;
  ofstream out(name.c_str());

  stri << "digraph G{" << endl;
  INSERT_TAB
  MAKE_SPACE
  stri << "  ratio=\"fill\"" << endl;
  MAKE_SPACE
  stri << "  node[shape=\"ellipse\"]" << endl;
  INSERT_TAB
  MAKE_SPACE

  // Vertices printing
  printVertices(stri);
  stri << "\n";

  DELETE_TAB
  DELETE_TAB

  INSERT_TAB
  INSERT_TAB
  stringstream colors;

  DELETE_TAB
  stri << colors.str();
  DELETE_TAB

  INSERT_TAB
  INSERT_TAB

  // Edge printing
  printEdges(stri);

  DELETE_TAB
  DELETE_TAB
  stri << "\n";
  stri << "}" << endl;
  out << stri.str();
  out.close();
}

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
void GraphPrinter<G,V,E,S,VIT,EIT>::printVertices(stringstream &stri)
{
  VIT vi, vi_end;
  for (boost::tie(vi, vi_end) = boost::vertices(_graph); vi != vi_end; ++vi) {
    stri << vPrinter(_graph[*vi]) << " [label=\"" << vLabelPrinter(_graph[*vi]) << "\"]";
    stri << "\n";
    MAKE_SPACE
  }
}

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
void GraphPrinter<G,V,E,S,VIT,EIT>::printEdges(stringstream &stri)
{
  EIT ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(_graph); ei != ei_end; ++ei) {
    S v1 = boost::source(*ei, _graph);
    S v2 = boost::target(*ei, _graph);
    stri << vPrinter(_graph[v1]) << " -> " << vPrinter(_graph[v2]);
    stri << " [label=\"" << ePrinter(_graph[*ei]) << "\", arrowhead=\"none\"]";
    stri << "\n";
    MAKE_SPACE
  }
}

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
std::string GraphPrinter<G,V,E,S,VIT,EIT>::vPrinter(SetVertex<V> v) { return v.name(); }

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
std::string GraphPrinter<G,V,E,S,VIT,EIT>::vLabelPrinter(SetVertex<V> v)
{
  std::stringstream label;
  SB::Set dom = v.range();
  label << "{ " << v.name() << " Dom: " << dom << " }";
  return label.str();
}

template<typename G, typename V, typename E, typename S, typename VIT, typename EIT>
std::string GraphPrinter<G,V,E,S,VIT,EIT>::ePrinter(SetEdge<E> e)
{
  std::stringstream label;
  PWLMap mapF = e.mapF();
  PWLMap mapU = e.mapU();
  label << "{ " << e.name() << " mapF: " << mapF << " mapU: " << mapU << " }";
  return label.str();
}
}  // namespace SB
