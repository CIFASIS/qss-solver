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

#pragma once

#include <ir/index.h>
#include <ir/equation.h>
#include <ir/expression.h>
#include <ir/event.h>
#include <deps/sbg_graph/deps_graph.h>

namespace MicroModelica {
namespace Deps {

class EQSelector {
  public:
  EQSelector() : _eqs() {};
  EQSelector(IR::EquationTable eqs);
  ~EQSelector() = default;
  IR::EquationTable getNodes() const;
  IR::Expression exp(IR::Equation eq);
  IR::Equation getNode(int id);
  IR::Equation setUsage(IR::Index ifr_idx, IR::Equation eq, Option<IR::Range> range);
  IR::Equation getScalarUsage(SB::Set range, IR::Expression exp, IR::Equation eq, IR::Index ifr_idx);
  bool isAlgebraic(SB::Deps::SetVertex vertex);
  bool multipleNodes() const;
  int id(SB::Deps::SetVertex vertex);
  
  protected:
  IR::EquationTable _eqs;
};

class EVSelector {
  public:
  EVSelector() : _evs() {};
  EVSelector(IR::EventTable evs);
  ~EVSelector() = default;
  IR::EventTable getNodes() const;
  IR::Expression exp(IR::Event ev);
  IR::Event getNode(int id);
  IR::Event setUsage(IR::Index ifr_idx, IR::Event ev, Option<IR::Range> range);
  IR::Event getScalarUsage(SB::Set range, IR::Expression exp, IR::Event ev, IR::Index ifr_idx);
  bool isAlgebraic(SB::Deps::SetVertex vertex);
  bool multipleNodes() const;
  int id(SB::Deps::SetVertex vertex);

  protected:
  IR::EventTable _evs;
};

}  // namespace Deps
}  // namespace MicroModelica
