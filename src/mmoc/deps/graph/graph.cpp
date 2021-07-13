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

#include <ast/ast_builder.h>
#include <ir/expression.h>
#include <util/model_config.h>
#include <util/logger.h>
#include <util/visitors/occurs.h>
#include <util/visitors/partial_eval_exp.h>

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

/*****************************************************************************
 ****                              LABEL                                  ****
 *****************************************************************************/
Label::Label(IndexPairSet ips, EDGE::Direction dir) : ips(ips), _ip(), _dir(dir) {}

Label::Label(IndexPair ip, EDGE::Direction dir) : ips(), _ip(ip), _dir(dir) {}

std::ostream &operator<<(std::ostream &os, const Label &label)
{
  os << label._ip;
  return os;
}

MDI Label::getImage(MDI intersection) const
{
  IndexPair orig = Pair();
  if (_dir == EDGE::Input) {
    return intersection.getImage(orig);
  } else {
    return intersection.revertImage(orig);
  }
}

EvalOccur::EvalOccur(Expression exp, Option<Range> range)
    : _exp(exp), _cr(nullptr), _range(range)
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
      PartialEvalExp pexp;
      AST_Expression cur = pexp.apply(current_element(it));
      if (cur->expressionType() == EXPINTEGER) {
        int v = cur->getAsInteger()->val();
        _intervals.push_back(Interval(v, v));
        _usages.push_back(-1);
        _offsets.push_back(0);
      } else if (cur->expressionType() == EXPCOMPREF) {
        AST_Expression_ComponentReference cr = cur->getAsComponentReference();
        Option<Variable> var = ModelConfig::instance().lookup(cr->name());
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
          LOG << "Wrong index expression." << endl;
          assert(false);
          _intervals.push_back(Interval(0, 0));
          _usages.push_back(-1);
          _offsets.push_back(0);
        }
      } else if (cur->expressionType() == EXPBINOP) {
        int offset = constant(cur);
        string name = reference(cur);
        int s = step(cur);
        Option<Variable> var = ModelConfig::instance().lookup(name);
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

GenerateEdge::GenerateEdge(VertexProperty source, VertexProperty sink, EDGE::Direction dir, VERTEX::Eval eval)
    : _source(source), _sink(sink), _exist(false), _ips(), _dir(dir), _eval(eval)
{
  initialize();
}

void GenerateEdge::initialize()
{
  Occurs occurs(_source.var().name());
  if (_eval == VERTEX::LHS) {
    if (_source.type() == VERTEX::Influencer && _sink.type() == VERTEX::Equation && !_sink.eq().isAlgebraic() &&
        _source.id() == _sink.eq().id()) {
      set<Expression> exps;
      _exist = true;
      exps.insert(_sink.eq().lhs());
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
      set<Expression> exps;
      if (sinkIsEvent() && _source.id() == _sink.id()) {
        _exist = true;
        exps.insert(_sink.stm().event());
        build(exps);
      } else if (_source.id() >= 0 && _source.id() == _sink.eq().id()) {
        _exist = true;
        exps.insert(_sink.eq().lhs());
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
      set<Expression> exps;
      if (sinkIsEvent()) {
        if (_source.id() == _sink.id()) {
          _exist = true;
          exps.insert(_sink.stm().event());
          build(exps);
        }
      } else if (sinkIsOutput()) {
        if (_source.id() == _sink.eq().id()) {
          _exist = true;
          exps.insert(_sink.eq().lhs());
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

void GenerateEdge::build(set<Expression> exps)
{
  Option<Range> sink_range = range(_sink);
  IntervalList sink_interval;
  if (sink_range) {
    for (auto rd : sink_range->definition()) {
      sink_interval.push_back(Interval(rd.second.begin(), rd.second.end()));
    }
  }
  for (Expression exp : exps) {
    LOG << "BUILDER EXP: " << exp << endl;
    assert(exp.isReference());
    EvalOccur eval_occur(exp, sink_range);
    MDI mdi_dom(eval_occur.intervals());
    MDI mdi_ran(sink_interval);
    if (_dir == EDGE::Input) {
      mdi_dom = MDI(sink_interval);
      mdi_ran = MDI(eval_occur.intervals());
    }
    if (sink_range) {
      if (eval_occur.hasIndex()) {
        LOG << "Found use, adding casse N -> N: " << exp << endl;
        _ips.insert(IndexPair(mdi_dom, mdi_ran, eval_occur.offsets(), eval_occur.usages(), exp));
      } else {  
        LOG << "Found use, adding casse 1 -> N: " << exp << endl;
        _ips.insert(IndexPair(MDI(0), mdi_ran, Offset(), Usage(), exp));
      }
    } else {
      if (_dir != EDGE::Input && _eval != VERTEX::LHS) {
        mdi_ran = getScalarMDI();
      }
      if (eval_occur.hasIndex()) {  
        LOG << "Found use, adding casse 1 -> 1 (integer expression index): " << exp << endl;
        _ips.insert(IndexPair(mdi_dom, mdi_ran, eval_occur.offsets(), eval_occur.usages(), exp));
      } else {  
        LOG << "Found use, adding casse 1 -> 1: " << exp << endl;
        _ips.insert(IndexPair(MDI(0), mdi_ran, Offset(), Usage(), exp));
      }
    }
    LOG << "Indexes: " << _ips.size() << endl;
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
      PartialEvalExp pexp;
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