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
 * causalization_strategy.h
 *
 *  Created on: 12/05/2013
 *      Author: fede
 */

#include <causalize/graph/graph_definition.h>
#include <mmo/mmo_class.h>


namespace Causalize {
class CausalizationStrategy {
public:
  CausalizationStrategy(Modelica::MMO_Class &mmo_class);
  void Causalize();
  void CausalizeSimple();
  void CausalizeTarjan();
private:
  void SimpleCausalizationStrategy();
  Edge GetUniqueEdge(Vertex v);
  void CollectDegree1Verts(Vertex v, std::list<Vertex> &degree1Verts);
  void MakeCausalBegining(Modelica::AST::Equation eq, Modelica::AST::Expression unknown);
  void MakeCausalMiddle();
  void MakeCausalEnd(Modelica::AST::Equation eq, Modelica::AST::Expression unknown);

  CausalizationGraph _graph;
  Modelica::MMO_Class &_mmo_class;
  Modelica::AST::EquationList _causalEqsBegining;
  Modelica::AST::EquationList _causalEqsMiddle;
  std::vector<Modelica::AST::Equation> _causalEqsEnd;
  int _causalEqsEndIndex;
  Modelica::AST::ClassList _cl;
  Modelica::AST::ExpList _all_unknowns;
  std::list<std::string> c_code;
};
}
