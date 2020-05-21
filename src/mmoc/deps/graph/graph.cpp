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

#include "graph.h"

#include <boost/variant/get.hpp>

#include "../../ast/ast_builder.h"
#include "../../ir/expression.h"
#include "../../util/visitors/occurs.h"
#include "../../util/visitors/partial_eval_exp.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {
/*****************************************************************************
 ****                              LABEL                                  ****
 *****************************************************************************/
Label::Label(IndexPairSet ips, EDGE::Direction dir) : ips(ips), _ip(), _dir(dir) { this->RemoveDuplicates(); }

Label::Label(IndexPair ip, EDGE::Direction dir) : ips(), _ip(ip), _dir(dir) {}

void Label::RemovePairs(IndexPairSet ipsToRemove)
{
  IndexPairSet newIps;
  foreach_(IndexPair ipRemove, ipsToRemove)
  {
    foreach_(IndexPair ip, this->ips)
    {
      //        newIps.erase(ip);
      foreach_(IndexPair ipRemaining, (ip - ipRemove)) { newIps.insert(ipRemaining); }
    }
  }
  this->ips = newIps;
}

void Label::RemoveUnknowns(MDI const mdi)
{
  IndexPairSet newIps;
  for (IndexPair ip : this->ips) {
    IndexPairSet afterRemove = ip.RemoveUnknowns(mdi);
    newIps.insert(afterRemove.begin(), afterRemove.end());
  }
  //    std::cout << "\nLabel::RemoveUnknowns result:\n" << newIps << "\n";
  this->ips = newIps;
}

void Label::RemoveEquations(MDI const mdi)
{
  IndexPairSet newIps;
  for (IndexPair ipOld : this->ips) {
    for (IndexPair ipNew : ipOld.RemoveEquations(mdi)) {
      newIps.insert(ipNew);
    }
  }
  this->ips = newIps;
}

std::ostream &operator<<(std::ostream &os, const Label &label)
{
  os << label._ip;
  return os;
}

void Label::RemoveDuplicates()
{
  bool removeSomething = true;
  IndexPairSet newIPS = ips;
  while (removeSomething) {
    for (IndexPairSet::iterator checkingIP = ips.begin(); checkingIP != ips.end(); checkingIP++) {
      // Ignore pairs 1-1 => should not be equal pairs in a set
      if (checkingIP->Dom().Size() == 1 && checkingIP->Ran().Size() == 1) continue;
      for (IndexPairSet::iterator otherIP = ips.begin(); otherIP != ips.end(); otherIP++) {
        // Ignore the same pair
        if (checkingIP == otherIP) continue;
        switch (checkingIP->Type()) {
        case INDEX_PAIR::RN_N:
          switch (otherIP->Type()) {
          case INDEX_PAIR::RN_N:
            if (checkingIP->GetUsage() == otherIP->GetUsage()) {
              if (checkingIP->GetOffset() == otherIP->GetOffset()) {  // Same usage same offset => are equals: SHOULD NOT EvalOccur
                // ERROR("This case should not EvalOccur since should not be equal pairs in a set");
                abort();
              } else {  // Same usage different offset => there is no intersection, nothing to remove
                removeSomething = false;
                continue;
              }
            } else {  // Different usage => ERROR: Not supported yet
              // ERROR("Multiple usages of a same vector with different index usages in a same for equation not supported");
              abort();
            }
          case INDEX_PAIR::RN_1:
            if (checkingIP->Ran().Contains(otherIP->Ran())) {  // There is intersection => remove it from the N-1 Index Pair
              newIPS.erase(*otherIP);
              MDI domToRemove = otherIP->Ran().RevertUsage(checkingIP->GetUsage(), checkingIP->Dom()).ApplyOffset(checkingIP->GetOffset());
              for (MDI remainingDom : otherIP->Dom() - domToRemove) {
                newIPS.insert(IndexPair(remainingDom, otherIP->Ran(), otherIP->GetOffset(), otherIP->GetUsage()));
              }
              removeSomething = true;
              continue;
            } else {  // No intersection => nothing to remove
              removeSomething = false;
              continue;
            }
          case INDEX_PAIR::R1_N:
            if (checkingIP->Dom().Contains(
                    otherIP->Dom())) {  // There is intersection => remove the N-N Index Pair, since it must be a 1-1 pair.
              newIPS.erase(*checkingIP);
              removeSomething = true;
              continue;
            }
          }
        case INDEX_PAIR::RN_1:
          switch (otherIP->Type()) {
          case INDEX_PAIR::RN_N:
            if (otherIP->Ran().Contains(checkingIP->Ran())) {  // There is intersection => remove it from the N-1 Index Pair
              newIPS.erase(*checkingIP);
              MDI domToRemove = checkingIP->Ran().RevertUsage(otherIP->GetUsage(), otherIP->Dom()).ApplyOffset(otherIP->GetOffset());
              for (MDI remainingDom : checkingIP->Dom() - domToRemove) {
                newIPS.insert(IndexPair(remainingDom, checkingIP->Ran(), checkingIP->GetOffset(), checkingIP->GetUsage()));
              }
              removeSomething = true;
              continue;
            } else {  // No intersection => nothing to remove
              removeSomething = false;
              continue;
            }
          case INDEX_PAIR::RN_1:
            if (checkingIP->Ran() == otherIP->Ran()) {  // Same range => are equals: SHOULD NOT EvalOccur
              // ERROR("This case should not EvalOccur since should not be equal pairs in a set");
              abort();
            } else {  // No intersection => nothing to remove
              removeSomething = false;
              continue;
            }
          case INDEX_PAIR::R1_N:
            // This case should not EvalOccur
            // ERROR("This case should not EvalOccur since should could not be N-1 and 1-N pairs in a same label");
            abort();
          }
        case INDEX_PAIR::R1_N:
          switch (otherIP->Type()) {
          case INDEX_PAIR::RN_N:
            if (otherIP->Dom().Contains(
                    checkingIP->Dom())) {  // There is intersection => remove the N-N Index Pair, since it must be a 1-1 pair.
              newIPS.erase(*otherIP);
              removeSomething = true;
              continue;
            }
          case INDEX_PAIR::RN_1:
            // This case should not EvalOccur
            // ERROR("This case should not EvalOccur since should could not be N-1 and 1-N pairs in a same label");
            abort();
          case INDEX_PAIR::R1_N:
            if (checkingIP->Dom() == otherIP->Dom()) {  // Same range => are equals: SHOULD NOT EvalOccur
              // ERROR("This case should not EvalOccur since should not be equal pairs in a set");
              abort();
            } else {  // No intersection => nothing to remove
              removeSomething = false;
              continue;
            }
          }
        }
      }
    }
    removeSomething = false;
  }
  ips = newIPS;
}

MDI Label::getImage(MDI intersection) const
{
  IndexPair orig = Pair();
  IndexPair p(intersection, orig.Ran(), orig.GetOffset(), orig.GetUsage(), orig.exp());
  INDEX_PAIR::Rel rel = p.Type();
  if (_dir == EDGE::Input) {
    return intersection.getImage(orig);
  } else {
    return intersection.revertImage(orig);
  }
}

EvalOccur::EvalOccur(Expression exp, VarSymbolTable symbols, Option<Range> range)
    : _exp(exp), _cr(nullptr), _symbols(symbols), _range(range)
{
  if (_exp.expression()->expressionType() == EXPCOMPREF) {
    _cr = _exp.expression()->getAsComponentReference();
  } else {
    _cr = newAST_Expression_ComponentReference();
  }
  initialize();
}

string EvalOccur::reference(AST_Expression exp)
{
  if (exp->expressionType() == EXPBINOP) {
    if (exp->getAsBinOp()->left()->expressionType() == EXPCOMPREF) {
      return exp->getAsBinOp()->left()->getAsComponentReference()->name();
    }
    if (exp->getAsBinOp()->right()->expressionType() == EXPCOMPREF) {
      return exp->getAsBinOp()->right()->getAsComponentReference()->name();
    }
  }
  return "";
}

int EvalOccur::getBinopInteger(AST_Expression exp)
{
  assert(exp->expressionType() == EXPBINOP);
  if (exp->getAsBinOp()->left()->expressionType() == EXPINTEGER) {
    return exp->getAsBinOp()->left()->getAsInteger()->val();
  }
  if (exp->getAsBinOp()->right()->expressionType() == EXPINTEGER) {
    return exp->getAsBinOp()->right()->getAsInteger()->val();
  }
  return 0;
}

int EvalOccur::constant(AST_Expression exp)
{
  if (exp->expressionType() == EXPBINOP) {
    if (!(exp->getAsBinOp()->binopType() == BINOPSUB || exp->getAsBinOp()->binopType() == BINOPADD)) {
      return 0;
    }
    int umin = 1;
    if (exp->getAsBinOp()->binopType() == BINOPSUB) {
      umin = -1;
    }
    return umin * getBinopInteger(exp);
  }
  return 0;
}

int EvalOccur::step(AST_Expression exp)
{
  if (exp->expressionType() == EXPBINOP) {
    if (exp->getAsBinOp()->binopType() == BINOPMULT) {
      return getBinopInteger(exp);
    }
    AST_Expression op = exp->getAsBinOp()->left();
    if (op->expressionType() == EXPBINOP) {
      if (op->getAsBinOp()->binopType() == BINOPMULT) {
        return getBinopInteger(op);
      }
    }
    op = exp->getAsBinOp()->right();
    if (op->expressionType() == EXPBINOP) {
      if (op->getAsBinOp()->binopType() == BINOPMULT) {
        return getBinopInteger(op);
      }
    }
  }
  return 1;
}

void EvalOccur::initialize()
{
  if (_cr->hasIndexes()) {
    std::vector<int> offset_vector;
    AST_ExpressionList indexes = _cr->firstIndex();
    AST_ExpressionListIterator it;
    foreach (it, indexes) {
      PartialEvalExp pexp(_symbols);
      AST_Expression cur = pexp.apply(current_element(it));
      if (cur->expressionType() == EXPINTEGER) {
        int v = cur->getAsInteger()->val();
        _intervals.push_back(Interval(v, v));
        _usages.push_back(-1);
        _offsets.push_back(0);
      } else if (cur->expressionType() == EXPCOMPREF) {
        AST_Expression_ComponentReference cr = cur->getAsComponentReference();
        Option<Variable> var = _symbols[cr->name()];
        if (var && _range) {
          RangeDefinitionTable rdt = _range->definition();
          Option<RangeDefinition> rd = rdt[var->name()];
          int begin = 0, end = 0, step = 1;
          if (rd) {
            begin = rd->begin();
            end = rd->end();
            step = rd->step();
          }
          _intervals.push_back(Interval(begin, end, step));
          _usages.push_back(_range->pos(var->name()));
          _offsets.push_back(0);
        } else if (var->isConstant()) {
          _intervals.push_back(Interval(var->value(), var->value()));
          _usages.push_back(-1);
          _offsets.push_back(0);
        } else {
          cout << "Wrong index expression." << endl;
          assert(false);
          _intervals.push_back(Interval(0, 0));
          _usages.push_back(-1);
          _offsets.push_back(0);
        }
      } else if (cur->expressionType() == EXPBINOP) {
        int offset = constant(cur);
        string name = reference(cur);
        int s = step(cur);
        Option<Variable> var = _symbols[name];
        if (var && _range) {
          RangeDefinitionTable rdt = _range->definition();
          Option<RangeDefinition> rd = rdt[name];
          int begin = 0, end = 0;
          if (rd) {
            begin = rd->begin();
            end = rd->end();
          }
          _intervals.push_back(Interval(s * begin + offset, s * end + offset));
          _usages.push_back(_range->pos(name));
          _offsets.push_back(offset);
        }
      }
    }
  }
}

bool EvalOccur::hasIndex() { return _cr->hasIndexes(); }

IntervalList EvalOccur::intervals() { return _intervals; }

Usage EvalOccur::usages() { return _usages; }

Offset EvalOccur::offsets() { return Offset(_offsets); }

GenerateEdge::GenerateEdge(VertexProperty source, VertexProperty sink, VarSymbolTable symbols, EDGE::Direction dir, VERTEX::Eval eval)
    : _source(source), _sink(sink), _exist(false), _symbols(symbols), _ips(), _dir(dir), _eval(eval)
{
  initialize();
}

void GenerateEdge::initialize()
{
  Occurs occurs(_source.var().name(), _symbols);
  if (_eval == VERTEX::LHS) {
    if (_source.type() == VERTEX::Influencer && _sink.type() == VERTEX::Equation && !_sink.eq().isAlgebraic() &&
        _source.id() == _sink.eq().id()) {
      ExpressionList exps;
      _exist = true;
      exps.push_back(_sink.eq().lhs());
      build(exps);
    } else if (_source.type() == VERTEX::Algebraic && _dir == EDGE::Output) {
      _exist = occurs.apply(_sink.eq().lhs().expression());
      if (_exist) {
        build(occurs.occurrences());
      }
    } else if (_source.type() == VERTEX::Algebraic && _dir == EDGE::Input) {
      assert(_sink.type() == VERTEX::Equation);
      _exist = occurs.apply(_sink.eq().equation());
      if (_exist) {
        build(occurs.occurrences());
      }
    } else if (_source.type() == VERTEX::Influencee) {
      assert(_sink.type() == VERTEX::Equation);
      ExpressionList exps;
      if (sinkIsEvent() && _source.id() == _sink.id()) {
        _exist = true;
        exps.push_back(_sink.stm().event());
        build(exps);
      } else if (_source.id() >= 0 && _source.id() == _sink.eq().id()) {
        _exist = true;
        exps.push_back(_sink.eq().lhs());
        build(exps);
      } else {
        assert(_sink.type() == VERTEX::Equation);
        _exist = occurs.apply(_sink.eq().equation());
        if (_exist) {
          build(occurs.occurrences());
        }
      }
    }
  } else {
    if (_source.type() == VERTEX::Influencer || (_source.type() == VERTEX::Algebraic && _dir == EDGE::Output)) {
      if (_sink.type() == VERTEX::Equation) {
        _exist = occurs.apply(_sink.eq().equation());
      } else if (_sink.type() == VERTEX::Statement) {
        _exist = occurs.apply(_sink.stm().exp().expression());
      }
      if (_exist) {
        build(occurs.occurrences());
      }
    } else if (_source.type() == VERTEX::Algebraic && _dir == EDGE::Input) {
      assert(_sink.type() == VERTEX::Equation);
      _exist = occurs.apply(_sink.eq().lhs().expression());
      if (_exist) {
        build(occurs.occurrences());
      }
    } else if (_source.type() == VERTEX::Influencee) {
      ExpressionList exps;
      if (sinkIsEvent()) {
        if (_source.id() == _sink.id()) {
          _exist = true;
          exps.push_back(_sink.stm().event());
          build(exps);
        }
      } else if (sinkIsOutput()) {
        if (_source.id() == _sink.eq().id()) {
          _exist = true;
          exps.push_back(_sink.eq().lhs());
          build(exps);
        }
      } else {
        _exist = occurs.apply(_sink.eq().lhs().expression());
        if (_exist) {
          build(occurs.occurrences());
        }
      }
    }
  }
}

void GenerateEdge::build(list<Expression> exps)
{
  Option<Range> sink_range = range(_sink);
  IntervalList sink_interval;
  if (sink_range) {
    for (auto rd : sink_range->definition()) {
      sink_interval.push_back(Interval(rd.second.begin(), rd.second.end()));
    }
  }
  for (Expression exp : exps) {
    cout << "BUILDER EXP: " << exp << endl;
    assert(exp.isReference());
    EvalOccur eval_occur(exp, _symbols, sink_range);
    MDI mdi_dom(eval_occur.intervals());
    MDI mdi_ran(sink_interval);
    if (_dir == EDGE::Input) {
      mdi_dom = MDI(sink_interval);
      mdi_ran = MDI(eval_occur.intervals());
    }
    if (sink_range) {
      if (eval_occur.hasIndex()) {  // N N also includes 1 N
        cout << "Intenta agregar caso 0: " << exp << endl;
        _ips.insert(IndexPair(mdi_dom, mdi_ran, eval_occur.offsets(), eval_occur.usages(), exp));
      } else {  // 1 N
        cout << "Intenta agregar caso 1: " << exp << endl;
        _ips.insert(IndexPair(MDI(0), mdi_ran, Offset(), Usage(), exp));
      }
    } else {
      if (_dir != EDGE::Input && _eval != VERTEX::LHS) {
        mdi_ran = getScalarMDI();
      }
      if (eval_occur.hasIndex()) {  // 1 1 In this case the index must be an integer expression.
        cout << "Intenta agregar caso 2:" << exp << endl;
        _ips.insert(IndexPair(mdi_dom, mdi_ran, eval_occur.offsets(), eval_occur.usages(), exp));
      } else {  // 1 1
        cout << "Intenta agregar caso 3:" << exp << endl;
        _ips.insert(IndexPair(MDI(0), mdi_ran, Offset(), Usage(), exp));
      }
    }
    cout << "Indexes: " << _ips.size() << endl;
  }
}

MDI GenerateEdge::getScalarMDI(Expression exp)
{
  assert(exp.isReference());
  IntervalList interval;
  AST_Expression_ComponentReference cr = exp.expression()->getAsComponentReference();
  if (cr->hasIndexes()) {
    AST_ExpressionList indexes = cr->firstIndex();
    AST_ExpressionListIterator it;
    foreach (it, indexes) {
      PartialEvalExp pexp(_symbols);
      AST_Expression cur = pexp.apply(current_element(it));
      if (cur->expressionType() == EXPINTEGER) {
        int v = cur->getAsInteger()->val();
        interval.push_back(Interval(v, v));
      } else {
        // @TODO: Add error message
        assert(false);
      }
    }
  }
  return MDI(interval);
}

MDI GenerateEdge::getScalarMDI()
{
  if (_sink.type() == VERTEX::Equation && (_sink.eq().isDerivative() || _sink.eq().isAlgebraic())) {
    return getScalarMDI(_sink.eq().lhs());
  }
  return MDI(0);
}

Option<Range> GenerateEdge::range(struct VertexProperty sink)
{
  if (_sink.type() == VERTEX::Equation) {
    Option<Range> eq_range = _sink.eq().range();
    if (eq_range) {
      return eq_range;
    } else {
      MDI eq_mdi = getScalarMDI();
      if (!eq_mdi.isEmpty()) {
        Range scalar;
        scalar.generate(eq_mdi);
        return scalar;
      }
    }
  } else if (_sink.type() == VERTEX::Statement) {
    return _sink.stm().range();
  }
  return Option<Range>();
}

bool GenerateEdge::sinkIsEvent()
{
  if (_sink.type() == VERTEX::Statement) {
    return true;
  }
  if (_sink.type() == VERTEX::Equation && _sink.eq().isZeroCrossing()) {
    return true;
  }
  return false;
}

bool GenerateEdge::sinkIsOutput()
{
  if (_sink.type() == VERTEX::Equation && _sink.eq().isOutput()) {
    return true;
  }
  return false;
}
}  // namespace Deps
}  // namespace MicroModelica