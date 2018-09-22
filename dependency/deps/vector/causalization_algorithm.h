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

#include <causalize/vector/vector_graph_definition.h>
#include <mmo/mmo_class.h>

namespace Causalize {
class CausalizationStrategyVector{
	public:
		CausalizationStrategyVector(Causalize::VectorCausalizationGraph g, Modelica::MMO_Class &m);			
		bool Causalize();
		void PrintCausalizationResult();
	private:
		void SolveEquations();
		void Causalize1toN(const Unknown unknown, const Equation equation, const IndexPairSet ips);
		void CausalizeNto1(const Unknown unknown, const Equation equation, const IndexPairSet ips);
    Vertex GetEquation(Edge e);
    Vertex GetUnknown(Edge e);
    Option<std::pair<VectorEdge,IndexPairSet> > CanCausalizeEquation(VectorEquationVertex eq);
    Option<std::pair<VectorEdge,IndexPairSet> > CanCausalizeUnknown(VectorUnknownVertex eq);

    int step;
		int equationNumber;
		int unknownNumber;
		Causalize::VectorCausalizationGraph graph;
		std::list<Causalize::VectorVertex> equationDescriptors, unknownDescriptors;
		std::vector<Causalize::CausalizedVar> equations1toN;
		std::vector<Causalize::CausalizedVar> equationsNto1;
    Modelica::MMO_Class &mmo;
};
}
