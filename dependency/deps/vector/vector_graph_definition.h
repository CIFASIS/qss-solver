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

#ifndef VECTOR_GRAPH_DEFINITION_
#define VECTOR_GRAPH_DEFINITION_

#include <utility>
#include <set>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/icl/interval_set.hpp>

#include <ast/equation.h>
#include <mmo/mmo_class.h>
#include <causalize/graph/graph_definition.h>

namespace Causalize {
  /// @brief This is the property for a vertex in the incidence graph. Nodes can be of two types: Equation or Unknow.
  struct VectorVertexProperty: VertexProperty {
  /// @brief The number of equations or unknowns left to causalize in this node
    int count;
  };
  
  /// @brief A pair representing a usage of a variable in an equation
  typedef std::pair<int,int> IndexPair;
  typedef std::set<IndexPair> IndexPairSet;

  struct VectorEdgeProperty {

    friend std::ostream & operator << (std::ostream &os, const VectorEdgeProperty &ep) {
      os << "{";
      foreach_(IndexPair ip, ep.labels) 
        os << "(" << ip.first << "," << ip.second << ") ";
      os << "}";
      return os;
    }

    /* This is the new version */
    IndexPairSet labels;
    std::set<int> getDom() const {
      std::set<int> dom;
      foreach_(IndexPair ip, labels)
        dom.insert(ip.first);
      return dom;
    }
    std::set<int> getRan() const {
      std::set<int> ran;
      foreach_(IndexPair ip, labels)
        ran.insert(ip.second);
      return ran;
    }

/// @brief This function removes a set of pairs from this Edge
///
/// @param ips set of pairs to remove
    void RemovePairs(IndexPairSet ips) {
      foreach_(IndexPair ip, ips) 
        labels.erase(ip);
    } 
    void RemoveUnknowns(IndexPairSet ips_remove) {
      IndexPairSet new_labels;
      foreach_(IndexPair ip, labels)  {
        bool found=false;
        foreach_(IndexPair ip_remove, ips_remove)  {
          if (ip_remove.second == ip.second) {
            found=true;
            break;
          }
        }
        if (!found)
          new_labels.insert(ip);
      }
      labels = new_labels;
    } ;
    void RemoveEquations(IndexPairSet ips_remove) {
      IndexPairSet new_labels;
      foreach_(IndexPair ip, labels)  {
        bool found=false;
        foreach_(IndexPair ip_remove, ips_remove)  {
          if (ip_remove.first == ip.first) {
            found=true;
            break;
          }
        }
        if (!found)
          new_labels.insert(ip);
      }
      labels = new_labels;
    }
    bool IsEmpty() {
      return labels.size()==0;
    }
  };

  /// @brief This is the definition of the Incidence graph for the vector case.
  typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, VectorVertexProperty, VectorEdgeProperty> VectorCausalizationGraph;
  /// @brief This a node from the vectorized incidence graph
  typedef Causalize::VectorCausalizationGraph::vertex_descriptor VectorVertex;
  /// @brief An equation vertex is the same as a regular vertex
  typedef VectorVertex VectorEquationVertex;
  /// @brief An unknown vertex is the same as a regular vertex
  typedef VectorVertex VectorUnknownVertex;
  /// @brief This is an edge of the vectorized causalization graph
  typedef VectorCausalizationGraph::edge_descriptor VectorEdge;
  /// @brief This struct represents a set of causalized vars for the vector algorithm

  struct CausalizedVar{
    Unknown unknown;
    Equation equation;
    IndexPairSet pairs;
  };
}
#endif
