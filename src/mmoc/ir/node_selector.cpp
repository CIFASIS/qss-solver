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

#include "node_selector.h"

#include <deps/sbg_graph/build_from_exps.h>

namespace MicroModelica {
using namespace IR;

namespace Deps {

EQSelector::EQSelector(EquationTable eqs) : _eqs(eqs), _select_states(false), _lhs_states(false) {};

IR::EquationTable EQSelector::getNodes() const { return _eqs; };

IR::Equation EQSelector::getNode(int id)
{
  Option<Equation> eq = _eqs[id];
  assert(eq);
  return eq.get();
}

IR::Equation EQSelector::setUsage(Index ifr_idx, Equation eq, Option<Range> range)
{
  Equation der = eq;
  der.setUsage(ifr_idx);
  if (range && der.hasRange()) {
    der.setRange(range);
  }
  return der;
}

IR::Equation EQSelector::getScalarUsage(SB::Set range, Expression exp, Equation eq, Index ifr_idx)
{
  Equation use_eq = eq;
  if ((range.size() == 1) && exp.isScalar() && eq.hasRange()) {
    use_eq.applyUsage(ifr_idx);
  }
  return use_eq;
}

bool EQSelector::isAlgebraic(SB::Deps::SetVertex vertex)
{
  Equation eq = getEquation(vertex, _eqs);
  return eq.type() == EQUATION::Algebraic;
}

int EQSelector::id(SB::Deps::SetVertex vertex) { return getEquation(vertex, _eqs).id(); }; 

IR::Expression EQSelector::exp(Equation eq) { return eq.lhs(); }

bool EQSelector::multipleNodes() const { return false; }

bool EQSelector::lhsStates() { return _lhs_states; }

std::string EQSelector::nodeName(int id) { return getNode(id).LHSVariable()->name(); }

bool EQSelector::validVariable(Util::Variable var)
{
  if (_select_states) {
    return var.isState();
  }
  return (var.name().find("_event_") != std::string::npos);
}

SB::PWLMap EQSelector::mapU() { return _map_u; }

SB::PWLMap EQSelector::mapF() { return _map_f; }

void EQSelector::setMapU(SB::PWLMap map_u) { _map_u = map_u; }

void EQSelector::setMapF(SB::PWLMap map_f) { _map_f = map_f; }

EVSelector::EVSelector(IR::EventTable evs) : _evs(evs) {}

IR::EventTable EVSelector::getNodes() const { return _evs; }

IR::Event EVSelector::getNode(int id)
{ 
  Option<Event> ev = _evs[id];
  assert(ev);
  return ev.get();
}

IR::Event EVSelector::setUsage(IR::Index ifr_idx, IR::Event ev, Option<IR::Range> range) { return ev; }

IR::Event EVSelector::getScalarUsage(SB::Set range, IR::Expression exp, IR::Event ev, IR::Index ifr_idx) { return ev; }

bool EVSelector::isAlgebraic(SB::Deps::SetVertex vertex)
{ 
  if (vertex.desc().type() == SB::Deps::VERTEX::Equation) {
    Equation eq = getEquation(vertex);  
    return eq.type() == EQUATION::Algebraic;
  }
  return false; 
}

int EVSelector::id(SB::Deps::SetVertex vertex) 
{ 
  return getEvent(vertex).id();
}; 

IR::Expression EVSelector::exp(Event ev) { return ev.zeroCrossing().index().expression(); }

bool EVSelector::multipleNodes() const { return true; }

bool EVSelector::lhsStates() { return false; }

std::string EVSelector::nodeName(int id) { return getNode(id).zeroCrossing().LHSVariable()->name(); }

bool EVSelector::validVariable(Util::Variable var) { return (var.name().find("_event_") != std::string::npos); }

SB::PWLMap EVSelector::mapU() { return _map_u; }

SB::PWLMap EVSelector::mapF() { return _map_f; }

void EVSelector::setMapU(SB::PWLMap map_u) { _map_u = map_u; }

void EVSelector::setMapF(SB::PWLMap map_f) { _map_f = map_f; }

}  // namespace Deps
}  // namespace MicroModelica
