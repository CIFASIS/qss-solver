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

#include "sb_graph.h"

#include <boost/variant/get.hpp>

#include "../../ast/ast_builder.h"
#include "../../ir/expression.h"
#include "../../util/model_config.h"
#include "../../util/visitors/occurs.h"
#include "../../util/visitors/partial_eval_exp.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {
namespace SBG {

Label::Label(){};

Label::Label(PairSet pairs, EDGE::Direction dir) : _pairs(pairs), _dir(dir) {}

const PairSet &Label::pairs() const { return _pairs; }

std::ostream &operator<<(std::ostream &os, const Label &label)
{
  os << label._pairs;
  return os;
}

VertexProperty::VertexProperty()
    : _type(VERTEX::Equation), _exp(), _eq(), _stm(), _id(0), _var(), _visited(false), _num_deps(0), _var_deps(), _maps(), _name()
{
}

VertexProperty::VertexProperty(const VertexProperty &other)
{
  _type = other._type;
  _exp = other._exp;
  _eq = other._eq;
  _var = other._var;
  _stm = other._stm;
  _id = other._id;
  _visited = other._visited;
  _num_deps = other._num_deps;
  _var_deps = other._var_deps;
  _maps = other._maps;
  _name = other._name;
}

Util::Variable VertexProperty::var() { return _var; }

void VertexProperty::setVar(Util::Variable var) { _var = var; }

VERTEX::Type VertexProperty::type() { return _type; }

void VertexProperty::setType(VERTEX::Type type) { _type = type; }

IR::Expression VertexProperty::exp() { return _exp; }

void VertexProperty::setExp(IR::Expression exp) { _exp = exp; }

IR::Equation VertexProperty::eq() { return _eq; }

void VertexProperty::setEq(IR::Equation eq) { _eq = eq; }

StatementVertex VertexProperty::stm() { return _stm; }

void VertexProperty::setStm(StatementVertex stm) { _stm = stm; }

int VertexProperty::id() { return _id; }

void VertexProperty::setId(int id) { _id = id; }

bool VertexProperty::visited() const { return _visited; }

void VertexProperty::setVisited(bool visited) { _visited = visited; }

int VertexProperty::numDeps() const { return _num_deps; }

void VertexProperty::setNumDeps(int num_deps) { _num_deps = num_deps; }

void VertexProperty::setDepState(int id, VariableDep var_dep) { _var_deps[id] = var_dep; }

VariableDep VertexProperty::depStates(int id) { return _var_deps[id]; }

void VertexProperty::setMap(int id, Map map) { _maps[id] = map; }

Maps VertexProperty::maps() const { return _maps; }

Map VertexProperty::map(int id) { return _maps[id]; }

int VertexProperty::size()
{
  if (_eq.isEmpty() || !_eq.hasRange()) {
    return 1;
  }
  return _eq.range()->size();
}

string VertexProperty::name() const { return _name; }

void VertexProperty::setName(string name) { _name = name; }

VariableDep::VariableDep() : _var(), _dom(), _range(), _recursive(false), _alg_dom(), _alg_eq(), _alg_dep(false) {}

VariableDep::VariableDep(Variable var, MDI dom, MDI range)
    : _var(var), _dom(dom), _range(range), _recursive(false), _alg_dom(), _alg_eq(), _alg_dep(false)
{
}

VariableDep::VariableDep(Variable var, MDI dom, MDI range, bool recursive)
    : _var(var), _dom(dom), _range(range), _recursive(recursive), _alg_dom(), _alg_eq(), _alg_dep(false)
{
}

MDI VariableDep::dom() { return _dom; }

MDI VariableDep::range() { return _range; }

Variable VariableDep::var() { return _var; }

bool VariableDep::isRecursive() const { return _recursive; }

void VariableDep::setAlgDom(MDI alg_dom) { _alg_dom = alg_dom; }

MDI VariableDep::algDom() const { return _alg_dom; }
  
Equation VariableDep::algEq() const { return _alg_eq; }

void VariableDep::setAlgEq(Equation eq) { _alg_eq = eq; _alg_dep = true; }

bool VariableDep::hasAlgDeps() const { return _alg_dep; }

EvalOccur::EvalOccur(Expression exp, Option<Range> range)
    : _exp(exp), _cr(nullptr), _range(range), _offsets(), _factors(), _constants(), _usages(), _intervals()
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
        _factors.push_back(0);
        _constants.push_back(v);
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
          _factors.push_back(1);
          _constants.push_back(0);
        } else if (var->isConstant()) {
          _intervals.push_back(Interval(var->value(), var->value()));
          _usages.push_back(-1);
          _offsets.push_back(0);
          _factors.push_back(0);
          _constants.push_back(var->value());
        } else {
          LOG << "Wrong index expression." << endl;
          assert(false);
          _intervals.push_back(Interval(0, 0));
          _usages.push_back(-1);
          _offsets.push_back(0);
          _factors.push_back(1);
          _constants.push_back(0);
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
          _factors.push_back(s);
          _constants.push_back(offset);
        }
      }
    }
  }
}

bool EvalOccur::hasIndex() { return _cr->hasIndexes(); }

IntervalList EvalOccur::intervals() { return _intervals; }

Usage EvalOccur::usages() { return _usages; }

Offset EvalOccur::offsets() { return Offset(_offsets); }

LinearFunction EvalOccur::linearFunctions() { return LinearFunction(_constants, _factors); }

BuildEdge::BuildEdge(VertexProperty source, VertexProperty sink, EDGE::Direction dir, VERTEX::Eval eval)
    : _source(source), _sink(sink), _exist(false), _pairs(), _dir(dir), _eval(eval)
{
  initialize();
}

void BuildEdge::initialize()
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

void BuildEdge::build(set<Expression> exps)
{
  Option<Range> sink_range = range(_sink);
  IntervalList sink_interval;
  if (sink_range) {
    for (auto rd : sink_range->definition()) {
      sink_interval.push_back(Interval(rd.second.begin(), rd.second.end()));
    }
  }
  for (Expression exp : exps) {
    assert(exp.isReference());
    EvalOccur eval_occur(exp, sink_range);
    MDI mdi_dom(eval_occur.intervals());
    MDI mdi_ran(sink_interval);
    if (_dir == EDGE::Input) {
      if (sink_range) {
        mdi_dom = MDI(sink_interval);
      } else {
        mdi_dom = getScalarMDI();
      }
      mdi_ran = MDI(eval_occur.intervals());
    }
    if (sink_range) {
      if (eval_occur.hasIndex()) {  // N N also includes 1 N
        _pairs.insert(Pair(mdi_dom, mdi_ran, Map(eval_occur.usages(), eval_occur.offsets(), exp, eval_occur.linearFunctions())));
      } else {  // 1 N
        _pairs.insert(Pair(MDI(0), mdi_ran, Map(exp)));
      }
    } else {
      if (_dir != EDGE::Input && _eval != VERTEX::LHS) {
        mdi_ran = getScalarMDI();
      }
      if (eval_occur.hasIndex()) {  // 1 1 In this case the index must be an integer expression.
        _pairs.insert(Pair(mdi_dom, mdi_ran, Map(eval_occur.usages(), eval_occur.offsets(), exp, eval_occur.linearFunctions())));
      } else {  // 1 1
        _pairs.insert(Pair(MDI(0), mdi_ran, Map(exp)));
      }
    }
  }
}

MDI BuildEdge::getScalarMDI(Expression exp)
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

MDI BuildEdge::getScalarMDI()
{
  if (_sink.type() == VERTEX::Equation && (_sink.eq().isDerivative() || _sink.eq().isAlgebraic())) {
    return getScalarMDI(_sink.eq().lhs());
  }
  return MDI(0);
}

Option<Range> BuildEdge::range(struct VertexProperty sink)
{
  if (_sink.type() == VERTEX::Equation) {
    Option<Range> eq_range = _sink.eq().range();
    if (eq_range) {
      return eq_range;
    }
  } else if (_sink.type() == VERTEX::Statement) {
    return _sink.stm().range();
  }
  return Option<Range>();
}

bool BuildEdge::sinkIsEvent()
{
  if (_sink.type() == VERTEX::Statement) {
    return true;
  }
  if (_sink.type() == VERTEX::Equation && _sink.eq().isZeroCrossing()) {
    return true;
  }
  return false;
}

bool BuildEdge::sinkIsOutput()
{
  if (_sink.type() == VERTEX::Equation && _sink.eq().isOutput()) {
    return true;
  }
  return false;
}

bool BuildEdge::exists() { return _exist; };

PairSet BuildEdge::indexes() { return _pairs; };

}  // namespace SBG
}  // namespace Deps
}  // namespace MicroModelica