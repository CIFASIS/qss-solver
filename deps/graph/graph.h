/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef DEPS_GRAPH_H_
#define DEPS_GRAPH_H_

#include <iostream>
#include <utility>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../../ir/expression.h"
#include "../../ir/equation.h"
#include "../../util/symbol_table.h"
#include "graph_helpers.h"

namespace MicroModelica {
  namespace Deps {
    
    namespace VERTEX  
    {
      typedef enum
      {
        Equation,
        Influencer,
        Algebraic,
        Influencee
      } Type;
    }

    /// @brief This is the property for a vertex in the incidence graph. Nodes can be of two types: Equation or Unknown.
    struct VertexProperty {
  	  VERTEX::Type type;
      /// @brief This holds the unknown in the case of a Unknown node. 
  	  IR::Expression exp;
      IR::Equation eq;
      Util::Variable var;
    };

    class Label {
      public:
        inline Label() {};
        Label(IndexPairSet ips);
        Label(IndexPair ip);
        void RemovePairs(IndexPairSet ips);
        void RemoveUnknowns(MDI const unk2remove);
        void RemoveEquations(MDI const mdi);
        unsigned long int EdgeCount();
        inline bool IsEmpty() { return ips.size()==0; }
        inline const IndexPairSet & Pairs() const { return ips; }
        friend std::ostream& operator<<(std::ostream& os, const Label& label);
      private:
        IndexPairSet ips;
        IndexPair    _ip;
        void RemoveDuplicates();
    }; 
    
    /// @brief This is the definition of the Incidence graph for the scalar case.
    typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS, VertexProperty, Label> DepsGraph;
    /// @brief A vertex of the Incidence graph
    typedef boost::graph_traits<DepsGraph>::vertex_descriptor Vertex;
    /// @brief An equation vertex is the same as a regular vertex
    typedef Vertex EqVertex;
    /// @brief An unknown vertex is the same as a regular vertex
    typedef Vertex IfrVertex;
    /// @brief This is an edge of the scalar causalization graph
    typedef Vertex IfeVertex;
    /// @brief This is an edge of the scalar causalization graph
    typedef Vertex AlgVertex;
    /// @brief This is an edge of the scalar causalization graph
    typedef DepsGraph::edge_descriptor Edge;

    class GenerateEdge {
      public:
        GenerateEdge(struct VertexProperty eq, struct VertexProperty ifr, Util::VarSymbolTable symbols);
        ~GenerateEdge() {};
        inline bool
        exists() { return _exist; };
        inline IndexPairSet
        indexes() { return _ips; };
      protected:
        void
        initialize();
        void
        build(list<IR::Expression> exps);
      private:
        struct VertexProperty  _eq;
        struct VertexProperty  _ifr;
        bool                   _exist;
        Util::VarSymbolTable   _symbols;
        IndexPairSet           _ips;
    };

  }
}
#endif
