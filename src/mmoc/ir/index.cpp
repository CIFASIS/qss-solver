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

#include "index.h"

#include <sstream>

#include <ir/helpers.h>
#include <ast/ast_builder.h>
#include <ast/expression.h>
#include <ast/equation.h>
#include <ast/statement.h>
#include <util/error.h>
#include <util/model_config.h>
#include <util/util.h>
#include <util/visitors/eval_init_exp.h>
#include <util/visitors/get_index_usage.h>
#include <util/visitors/is_constant_index.h>
#include <util/visitors/parse_index.h>
#include <util/visitors/partial_eval_exp.h>
#include <util/visitors/replace_index.h>
#include <util/visitors/revert_index.h>

namespace MicroModelica {
using namespace Deps;
using namespace Util;
namespace IR {

IndexDefinition::IndexDefinition() : _variable(), _constant(0), _factor(1) {}

IndexDefinition::IndexDefinition(string variable, int constant, int factor) : _variable(variable), _constant(constant), _factor(factor) {}

int IndexDefinition::constant() { return _constant; }

int IndexDefinition::factor() { return _factor; }

string IndexDefinition::variable() { return _variable; }

Index::Index(Expression exp) : _indexes(), _exp(exp) { parseIndexes(); }

void Index::parseIndexes()
{
  // Iterate over the expression indexes and generate IndexDefinition for each one.
  AST_Expression exp = _exp.expression();
  assert(exp->expressionType() == EXPCOMPREF);
  AST_Expression_ComponentReference ref = exp->getAsComponentReference();
  int dim = 0;
  if (ref->hasIndexes()) {
    AST_ExpressionList indexes = ref->firstIndex();
    AST_ExpressionListIterator it;
    foreach (it, indexes) {
      ParseIndex parse_index;
      VarSymbolTable symbols = ModelConfig::instance().symbols();
      PartialEvalExp partial_eval;
      parse_index.apply(partial_eval.apply(current_element(it)));
      _indexes[dim++] = IndexDefinition(parse_index.variable(), parse_index.constant(), parse_index.factor());
    }
  }
}

vector<int> Index::initValues(Option<Range> range)
{
  assert(!isEmpty());
  vector<int> init_values;
  size_t size = _indexes.size();
  if (_exp.reference()->isScalar()) {
    init_values.push_back(0);
  } else if (isConstant()) {
    assert(_indexes.size());
    for (size_t i = 0; i < size; i++) {
      IndexDefinition idx_def = _indexes[i];
      init_values.push_back(idx_def.constant());
    }
  } else {
    assert(range);
    for (size_t i = 0; i < size; i++) {
      IndexDefinition idx_def = _indexes[i];
      if (idx_def.variable().empty()) {
        init_values.push_back(idx_def.constant());
      } else {
        RangeDefinitionTable ranges = range->definition();
        Option<RangeDefinition> dim_range = ranges[idx_def.variable()];
        assert(dim_range);
        init_values.push_back(idx_def.constant() + idx_def.factor() * dim_range->begin());
      }
    }
  }
  return init_values;
}

int Index::dimension() { return _indexes.size(); }

Expression Index::expression() const { return _exp; }

void Index::setMap(Expression exp) {}

bool Index::hasMap() const { return false; }

bool Index::operator==(const Index& other) const { return false; }

bool Index::isConstant() const
{
  if (isEmpty()) {
    return false;
  }
  CheckIndexExpression constant_index;
  return constant_index.apply(_exp.expression());
}

void Index::setExp(Expression exp)
{
  _exp = exp;
  parseIndexes();
}

string Index::identifier(bool recursive_use) const
{
  stringstream buffer;
  if (isConstant() && !recursive_use) {
    buffer << "_eval" << _exp;
  } else {
    buffer << variable();
  }
  return buffer.str();
}

Variable Index::variable() const
{
  Option<Variable> var = _exp.reference();
  assert(var);
  return var.get();
}

Range Index::range() const { return Range(variable()); }

Index Index::revert() const
{
  RevertIndex revert;
  return Index(Expression(revert.apply(_exp.expression())));
}

Index Index::replace(bool range_idx) const { return replace(range(), range_idx); }

Index Index::replace(Option<Range> for_range, bool range_idx) const
{
  Range var_range = range();
  if (for_range) {
    ReplaceIndex replace = ReplaceIndex(for_range.get(), _exp.expression(), range_idx);
    return Index(Expression(replace.apply(_exp.expression())));
  }
  ReplaceIndex replace = ReplaceIndex(var_range, _exp.expression(), range_idx);
  return Index(Expression(replace.apply(_exp.expression())));
}

string Index::usageExp() const { return _exp.usage(); }

string Index::modelicaExp() const
{
  stringstream buffer;
  string usage_exp = _exp.usage();
  buffer << variable().name();
  if (!usage_exp.empty()) {
    buffer << "[" << usage_exp << "]";
  }
  return buffer.str();
}

string Index::print() const
{
  stringstream buffer;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  PartialEvalExp partial_eval;
  Expression exp = Expression(partial_eval.apply(_exp.expression()));
  buffer << "_idx" << exp;
  return buffer.str();
}

vector<string> Index::variables()
{
  vector<string> vars;
  for (unsigned int i = 0; i < _indexes.size(); i++) {
    if (_indexes[i].variable().empty()) {
      stringstream constant;
      constant << _indexes[i].constant();
      vars.push_back(constant.str());
    } else {
      vars.push_back(_indexes[i].variable());
    }
  }
  return vars;
}

bool Index::hasVariable(int dim) { return !_indexes[dim].variable().empty(); }

string Index::variable(int dim) { return _indexes[dim].variable(); }

bool Index::hasFactor(int dim) { return _indexes[dim].factor() != 1; }

int Index::factor(int dim) { return _indexes[dim].factor(); }

bool Index::hasConstant(int dim) { return _indexes[dim].constant() != 0; }

int Index::constant(int dim) { return _indexes[dim].constant(); }

bool Index::isEmpty() const { return _exp.isEmpty(); }

std::ostream& operator<<(std::ostream& out, const Index& i)
{
  out << i.print();
  return out;
}

RangeDefinition::RangeDefinition(int begin, int end, int step) : _begin(begin), _end(end), _step(step), _begin_exp(), _end_exp() {}

RangeDefinition::RangeDefinition(Expression begin_exp, Expression end_exp, int begin, int end, int step)
    : _begin(begin), _end(end), _step(step), _begin_exp(begin_exp), _end_exp(end_exp)
{
}

void RangeDefinition::setBegin(int begin) { _begin = begin; }

void RangeDefinition::setEnd(int end) { _end = end; }

string RangeDefinition::generateExp(Expression exp, int limit, bool convert_params) const
{
  stringstream code;
  if (exp.isEmpty()) {
    code << limit;
  } else {
    Index index(exp);
    if (convert_params) {
      index = index.replace();
    }
    code << index.expression();
  }
  return code.str();
}

string RangeDefinition::beginExp(bool convert_params, bool c_index) const
{
  return generateExp(_begin_exp, ((c_index) ? cBegin() : begin()), convert_params);
}

string RangeDefinition::endExp(bool convert_params, bool c_index) const
{
  return generateExp(_end_exp, ((c_index) ? cEnd() : end()), convert_params);
}

Range::Range() : _ranges(), _index_pos(), _size(1), _type(RANGE::For), _fixed(true), _merged_dims(false) {}

Range::Range(AST_Equation_For eqf, RANGE::Type type) : _ranges(), _index_pos(), _size(1), _type(type), _fixed(true), _merged_dims(false)
{
  AST_ForIndexList fil = eqf->forIndexList();
  setRangeDefinition(fil);
}

Range::Range(AST_Statement_For stf, RANGE::Type type, bool from_event)
    : _ranges(), _index_pos(), _size(1), _type(type), _fixed(true), _merged_dims(false)
{
  AST_ForIndexList fil = stf->forIndexList();
  setRangeDefinition(fil, from_event);
}

Range::Range(Variable var, RANGE::Type type) : _ranges(), _index_pos(), _size(1), _type(type), _fixed(true), _merged_dims(false)
{
  generate(var);
}

Range::Range(AST_Expression exp) : _ranges(), _index_pos(), _size(1), _type(RANGE::For), _fixed(true), _merged_dims(false)
{
  generate(exp);
}

Range::Range(SB::Set set, int offset, vector<string> vars, Option<Range> orig_range)
    : _ranges(), _index_pos(), _size(1), _type(RANGE::For), _fixed(true), _merged_dims(false)
{
  generate(set, offset, vars, orig_range);
}

void Range::updateRangeDefinition(std::string index_def, RangeDefinition def, int pos)
{
  _ranges.insert(index_def, def);
  _index_pos.insert(index_def, pos);
  Option<RangeDefinition> range = _ranges[index_def];
  assert(range);
  _size *= range->size();
  _row_size.push_back(range->size());
  Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
  ModelConfig::instance().addVariable(index_def, vi);
}

bool Range::testExpression(AST_Expression exp)
{
  if (exp->expressionType() == EXPCOMPREF) {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "index.cpp:264 %s", cr->name().c_str());
      return false;
    }
    if (var->isParameter()) {
      _fixed = false;
      return true;
    }
  }
  return false;
}

void Range::setRangeDefinition(AST_ForIndexList fil, bool from_event)
{
  AST_ForIndexListIterator filit;
  int pos = 0;
  foreach (filit, fil) {
    AST_ForIndex fi = current_element(filit);
    AST_Expression in = fi->in_exp();
    AST_ExpressionList el = in->getAsRange()->expressionList();
    EvalInitExp eval;
    int size = el->size();
    AST_Expression ast_begin_exp = AST_ListFirst(el);
    AST_Expression ast_end_exp = AST_ListAt(el, size - 1);
    Expression begin_exp;
    Expression end_exp;
    int begin = 1;
    int end = 1;
    if (testExpression(ast_begin_exp)) {
      begin_exp = Expression(ast_begin_exp);
    } else {
      begin = eval.apply(ast_begin_exp);
    }
    if (testExpression(ast_end_exp)) {
      end_exp = Expression(ast_end_exp);
    } else {
      end = eval.apply(ast_end_exp);
    }
    if (from_event) {
      end = eval.apply(ast_end_exp) - begin + 1;
      begin = 1;
    }
    if (end < begin) {
      Error::instance().add(AST_ListFirst(el)->lineNum(), EM_IR | EM_WRONG_EXP, ER_Fatal, "Wrong equation range.");
    }
    string index = fi->variable()->c_str();
    updateRangeDefinition(index,
                          (size == 2 ? RangeDefinition(begin_exp, end_exp, begin, end)
                                     : RangeDefinition(begin_exp, end_exp, begin, end, eval.apply(AST_ListAt(el, 1)))),
                          pos++);
  }
}

void Range::generate(Variable var)
{
  int pos = 0;
  for (unsigned int i = 0; i < var.dimensions(); i++) {
    string index = getDimensionVar(i + 1);
    int begin = 1;
    int end = var.size(i);
    updateRangeDefinition(index, RangeDefinition(begin, end), pos++);
  }
}

void Range::generate(AST_Expression exp)
{
  assert(exp->expressionType() == EXPCOMPREF);
  CheckIndexExpression constant_index;
  assert(constant_index.apply(exp));
  AST_Expression_ComponentReference ref = exp->getAsComponentReference();
  assert(ref->hasIndexes());
  AST_ExpressionList indexes = ref->firstIndex();
  AST_ExpressionListIterator it;
  int i = 0;
  int pos = 0;
  foreach (it, indexes) {
    AST_Expression index_exp = current_element(it);
    string index = getDimensionVar(i + 1);
    EvalInitExp eval_exp;
    int scalar_value = eval_exp.apply(index_exp);
    updateRangeDefinition(index, RangeDefinition(scalar_value, scalar_value), pos++);
  }
}

bool Range::isVariable(std::string var)
{
  bool not_number = true;
  try {
    std::stoi(var);
    not_number = false;
  } catch (const std::exception&) {
  }
  return not_number;
}

Expression Range::getExp(std::vector<Expression> exps, size_t pos)
{
  if (exps.empty()) {
    return Expression();
  }
  assert(exps.size() > pos);
  _fixed = false;
  return exps[pos];
}

void Range::generate(SB::Set set, int offset, vector<string> vars, Option<Range> orig_range)
{
  unsigned int pos = 0;
  const bool MAP_RANGE = orig_range.has_value();
  SB::UnordAtomSet a_sets = set.atomicSets();
  for (SB::AtomSet a_set : a_sets) {
    SB::MultiInterval intervals = a_set.atomicSets();
    int exp_pos = 0;
    for (SB::Interval interval : intervals.intervals()) {
      int begin = interval.lo() - offset + 1;
      int end = begin + interval.hi() - interval.lo();
      int step = interval.step();
      if (MAP_RANGE) {
        RangeDefinition orig_def = orig_range->definition().value(exp_pos);
        begin = orig_def.begin() + (begin - 1) * orig_def.step();
        end = orig_def.begin() + (end - 1) * orig_def.step();
      }
      if (end < begin) {
        Error::instance().add(0, EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong range in dependency matrix.");
      }
      string index = Utils::instance().iteratorVar(pos);
      if (!vars.empty() && pos < vars.size() && isVariable(vars[pos])) {
        index = vars[pos];
      }
      updateRangeDefinition(index, RangeDefinition(begin, end, step), pos++);
      exp_pos++;
    }
  }
}

void Range::generate(SB::Set set, int offset, vector<string> vars, std::vector<Expression> begin_exps, std::vector<Expression> end_exps)
{
  unsigned int pos = 0;
  SB::UnordAtomSet a_sets = set.atomicSets();
  for (SB::AtomSet a_set : a_sets) {
    SB::MultiInterval intervals = a_set.atomicSets();
    int exp_pos = 0;
    for (SB::Interval interval : intervals.intervals()) {
      int begin = interval.lo() - offset + 1;
      int end = begin + interval.hi() - interval.lo();
      if (end < begin) {
        Error::instance().add(0, EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong range in dependency matrix.");
      }
      string index = Utils::instance().iteratorVar(pos);
      if (!vars.empty() && pos < vars.size() && isVariable(vars[pos])) {
        index = vars[pos];
      }
      updateRangeDefinition(index, RangeDefinition(getExp(begin_exps, exp_pos), getExp(end_exps, exp_pos), begin, end), pos++);
      exp_pos++;
    }
  }
}

string Range::iterator(int dim, bool range_idx)
{
  if (range_idx) {
    return getDimensionVar(dim + 1, range_idx);
  }
  for (auto ip : _index_pos) {
    if (ip.second == dim) {
      return ip.first;
    }
  }
  return getDimensionVar(dim + 1);
}

string Range::iterator(string var, int dim, bool range_idx)
{
  if (isDimensionVar(var)) {
    iterator(dim, range_idx);
  }
  for (auto ip : _index_pos) {
    if (ip.first == var) {
      return getDimensionVar(ip.second + 1);
    }
  }
  return iterator(dim, range_idx);
}

bool Range::isDimensionVar(string var) { return (var.rfind("_d", 0) == 0); }

string Range::getPrintDimensionVarsString() const
{
  stringstream buffer;
  int size = _ranges.size();
  for (int i = 1; i <= size; i++) {
    buffer << "%d" << (i + 1 <= size ? "," : "");
  }
  return buffer.str();
}

string Range::getDimensionVarsString(bool range) const
{
  stringstream buffer;
  int size = _ranges.size();
  for (int i = 1; i <= size; i++) {
    buffer << getDimensionVar(i, range) << (i + 1 <= size ? "," : "");
  }
  return buffer.str();
}

vector<string> Range::getDimensionVars(bool range) const
{
  vector<string> buffer;
  int size = _ranges.size();
  for (int i = 1; i <= size; i++) {
    buffer.push_back(getDimensionVar(i, range));
  }
  return buffer;
}

string Range::getDimensionVar(int i, bool range) const
{
  stringstream buffer;
  if (range) {
    buffer << "_rg";
  }
  buffer << "_d" << i;
  if (range) {
    addRangeVariables(i, buffer.str());
  }
  return buffer.str();
}

string Range::end() const
{
  stringstream buffer;
  int size = _ranges.size();
  for (int i = size; i > 0; i--) {
    buffer << block(i - 1) << "}" << (i - 1 > 0 ? "\n" : "");
  }
  return buffer.str();
}

string Range::print(bool range, bool c_index, bool convert_params) const
{
  stringstream buffer;
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  string block = "";
  int idx_count = 0;
  vector<string> idxs = getIndexes();
  if (range) {
    idxs = getDimensionVars(range);
  }
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    if (_type == RANGE::For) {
      string idx = idxs[idx_count];
      buffer << block << "for(" << idx << " = " << r.beginExp(convert_params, c_index) << "; ";
      buffer << idx << "<=" << r.endExp(convert_params, c_index) << "; ";
      buffer << idx << "+=" << r.step() << ") {" << endl;
      block += TAB;
      idx_count++;
    }
  }
  if (range) {
    addRangeLocalVariables();
  } else {
    addLocalVariables();
  }
  return buffer.str();
}

string Range::indexes() const
{
  stringstream buffer;
  vector<string> indexes = getIndexes();
  long unsigned int size = _ranges.size(), i = 0;
  for (string index : indexes) {
    buffer << index << (++i < size ? "," : "");
  }
  return buffer.str();
}

vector<string> Range::getInitValues() const
{
  vector<string> indexes;
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    stringstream val;
    val << r.begin();
    indexes.push_back(val.str());
  }
  return indexes;
}

vector<string> Range::getIndexes() const
{
  vector<string> indexes;
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    string var = ranges.key(it);
    indexes.push_back(var);
  }
  return indexes;
}

void Range::addLocalVariables() const
{
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  int i = 1;
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    string var = ranges.key(it);
    ModelConfig::instance().addLocalSymbol("int " + var + ";");
    ModelConfig::instance().addLocalSymbol("int " + getDimensionVar(i++) + ";");
  }
}

void Range::addRangeLocalVariables() const
{
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  int i = 1;
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    string index = getDimensionVar(i++, true);
    ModelConfig::instance().addLocalSymbol("int " + index + ";");
  }
}

void Range::addRangeVariables(int i, string index) const
{
  Variable range_var(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
  ModelConfig::instance().addVariable(index, range_var);
}

int Range::rowSize(int dim) const
{
  int total = 1;
  for (int it = _row_size.size() - 1; it > dim; it--) {
    total *= _row_size.at(it);
  }
  return total;
}

string Range::block(int dim) const
{
  int size = _ranges.size();
  if (dim >= 0) {
    size = dim;
  }
  string block = "";
  for (int i = 0; i < size; i++) {
    block += TAB;
  }
  return block;
}

int Range::pos(string var)
{
  Option<int> p = _index_pos[var];
  if (p) {
    return boost::get<int>(p);
  }
  return 0;
}

void Range::applyUsage(Index usage)
{
  int dimension = usage.dimension();
  assert(_ranges.size() == dimension);
  RangeDefinitionTable::iterator it;
  int i = 0;
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), i++) {
    if (usage.hasVariable(i)) {
      int new_begin = r.begin() * usage.factor(i) + usage.constant(i);
      int new_end = r.end() * usage.factor(i) + usage.constant(i);
      RangeDefinition new_range(new_begin, new_end, r.step());
      _ranges.insert(_ranges.key(it), new_range);
    }
  }
}

void Range::update(int offset)
{
  RangeDefinitionTable::iterator it;
  int i = 0;
  map<string, RangeDefinition> updated_ranges;
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), i++) {
    int new_begin = r.begin() + offset;
    int new_end = r.end() + offset;
    RangeDefinition new_range(new_begin, new_end, r.step());
    updated_ranges[_ranges.key(it)] = new_range;
  }
  for (auto update_range : updated_ranges) {
    _ranges.insert(update_range.first, update_range.second);
  }
}

void Range::update(Range other)
{
  RangeDefinitionTable::iterator it;
  RangeDefinitionTable::iterator other_it;
  RangeDefinitionTable other_ranges = other.definition();
  assert(other_ranges.size() == _ranges.size());
  map<string, RangeDefinition> updated_ranges;
  RangeDefinition other_def = other_ranges.begin(other_it);
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), other_def = other_ranges.next(other_it)) {
    int new_begin = r.begin() + other_def.begin() - 1;
    int new_end = r.end() + other_def.begin() - 1;
    RangeDefinition new_range(new_begin, new_end, r.step());
    updated_ranges[_ranges.key(it)] = new_range;
  }
  for (auto update_range : updated_ranges) {
    _ranges.insert(update_range.first, update_range.second);
  }
}

bool Range::checkUsage(Index usage, Index def)
{
  int dimension = def.dimension();
  assert(_ranges.size() == dimension);
  assert(usage.isConstant());
  RangeDefinitionTable::iterator it;
  int i = 0;
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), i++) {
    if (def.hasVariable(i)) {
      int new_begin = r.begin() * def.factor(i) + def.constant(i);
      int new_end = r.end() * def.factor(i) + def.constant(i);
      int usage_val = usage.constant(i);
      if (usage_val < new_begin || usage_val > new_end) {
        return false;
      }
    }
  }
  return true;
}

string Range::in(ExpressionList exps)
{
  vector<string> str_exps;
  for (auto e : exps) {
    str_exps.push_back(e.print());
  }
  return in(str_exps);
}

string Range::in(vector<string> exps)
{
  assert(exps.size() <= (size_t)_ranges.size());
  // If empty generate a default true condition to handle scalar cases.
  if (exps.empty()) {
    return "1";
  }
  stringstream code;
  RangeDefinitionTable::iterator it;
  int i = 0;
  int size = _ranges.size();
  int exps_size = exps.size();
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), i++) {
    string exp_str = (i < exps_size) ? exps[i] : getDimensionVar(i);

    code << "(" << r.begin() << " <= " << exp_str << " && " << exp_str << " <= " << r.end() << ")";
    code << ((i + 1 < size) ? " && " : "");
  }
  return code.str();
}

int Range::size() const { return _size; };

bool Range::isEmpty() const { return _row_size.size() == 0; };

map<std::string, AST_Expression> Range::initExps()
{
  map<string, AST_Expression> init_exps;
  RangeDefinitionTable::iterator it;
  int i = 0;
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), i++) {
    init_exps[_ranges.key(it)] = newAST_Expression_Integer(r.begin());
  }
  return init_exps;
}

bool Range::checkRangeVariable(string var, set<string>& added_vars, vector<string>& old_keys, int& pos)
{
  if (added_vars.find(var) == added_vars.end()) {
    Option<RangeDefinition> r = _ranges[var];
    if (r.has_value()) {
      added_vars.insert(var);
      string index = getDimensionVar(pos);
      if (index != var) {
        old_keys.push_back(var);
        _ranges.insert(index, r.get());
      }
      pos++;
      return true;
    }
  }
  return false;
}

bool Range::checkRangeVariables(string ife_idx, string ifr_idx, set<string>& added_vars, vector<string>& old_keys, int& pos)
{
  if ((!isVariable(ife_idx) || ife_idx.empty()) && (!isVariable(ifr_idx) || ifr_idx.empty())) {
    return true;
  }
  bool found = false;
  if (isVariable(ife_idx)) {
    found = checkRangeVariable(ife_idx, added_vars, old_keys, pos);
  }
  if (!found && isVariable(ifr_idx)) {
    found = checkRangeVariable(ifr_idx, added_vars, old_keys, pos);
  }
  return found;
}

void Range::replace(Index ife_usage, Index ifr_usage)
{
  const bool SELECT_IFE_USAGE = ife_usage.variables().size() > ifr_usage.variables().size();
  vector<string> max_usage = (SELECT_IFE_USAGE) ? ife_usage.variables() : ifr_usage.variables();
  vector<string> min_usage = (!SELECT_IFE_USAGE) ? ife_usage.variables() : ifr_usage.variables();
  vector<pair<string, string>> used_variables;

  int min_usage_idx = 0;
  int min_usage_size = min_usage.size();
  for (string max_usage_str : max_usage) {
    string min_usage_str = "";
    if (min_usage_idx < min_usage_size) {
      min_usage_str = min_usage[min_usage_idx];
    }
    used_variables.push_back(make_pair(max_usage_str, min_usage_str));
    min_usage_idx++;
  }

  // Add only variables from the ifr index that are higher than the ife usage
  // the rest should be replaced by variables in the original range.
  /*int variables_size = variables.size();
  int ifr_vars = 1;
  if (!ifr_usage.isEmpty()) {
    for (string ifr_var : ifr_usage.variables()) {
      if (ifr_vars > variables_size) {
        variables.push_back(ifr_var);
      }
      ifr_vars++;
    }
    sort(variables.begin(), variables.end());
    variables.erase(unique(variables.begin(), variables.end()), variables.end());
  }*/
  // In case of a scalar usage in N<->1 relations.
  if (used_variables.empty()) {
    max_usage = getIndexes();
    for (string max_usage_str : max_usage) {
      string min_usage_str = "";
      used_variables.push_back(make_pair(max_usage_str, min_usage_str));
    }
  }
  set<string> added_vars;
  vector<string> old_keys;
  int pos = 1;
  for (pair<string, string> vars : used_variables) {
    assert(checkRangeVariables(vars.first, vars.second, added_vars, old_keys, pos));
  }
  for (string key : old_keys) {
    _ranges.remove(key);
  }
}

bool Range::fixed() const { return _fixed; }

void Range::merge(Range other)
{
  RangeDefinitionTable::iterator it;
  int pos = _ranges.size();
  for (RangeDefinition r = other._ranges.begin(it); !other._ranges.end(it); r = other._ranges.next(it)) {
    updateRangeDefinition(other._ranges.key(it), RangeDefinition(r.begin(), r.end(), r.step()), pos++);
  }
  _merged_dims = true;
}

int Range::dim() const { return _ranges.size(); }

bool Range::hasMergedDims() const { return _merged_dims; }

std::ostream& operator<<(std::ostream& out, const Range& r) { return out << r.print(); }

}  // namespace IR
}  // namespace MicroModelica
