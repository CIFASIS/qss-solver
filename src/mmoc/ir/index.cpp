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

#include "helpers.h"
#include "../ast/ast_builder.h"
#include "../ast/expression.h"
#include "../ast/equation.h"
#include "../ast/statement.h"
#include "../util/error.h"
#include "../util/model_config.h"
#include "../util/util.h"
#include "../util/visitors/eval_init_exp.h"
#include "../util/visitors/get_index_usage.h"
#include "../util/visitors/is_constant_index.h"
#include "../util/visitors/parse_index.h"
#include "../util/visitors/partial_eval_exp.h"
#include "../util/visitors/replace_index.h"
#include "../util/visitors/revert_index.h"

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
  IsConstantIndex constant_index;
  return constant_index.apply(_exp.expression());
}

Usage Index::usage() const
{
  GetIndexUsage usage;
  return usage.apply(_exp.expression());
}

void Index::setExp(Expression exp)
{
  _exp = exp;
  parseIndexes();
}

string Index::identifier() const
{
  stringstream buffer;
  if (isConstant()) {
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

Index Index::replace(bool range_idx) const
{
  ReplaceIndex replace = ReplaceIndex(range(), usage(), range_idx);
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
  for(unsigned int i = 0; i < _indexes.size(); i++){
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

RangeDefinition::RangeDefinition(int begin, int end, int step) : _begin(begin), _end(end), _step(step) {}

void RangeDefinition::setBegin(int begin) { _begin = begin; }

void RangeDefinition::setEnd(int end) { _end = end; }

std::ostream& operator<<(std::ostream& out, const RangeDefinition& rd) { return out; }

Range::Range() : _ranges(), _index_pos(), _size(1), _type(RANGE::For) {}

Range::Range(AST_Equation_For eqf, RANGE::Type type) : _ranges(), _index_pos(), _size(1), _type(type)
{
  AST_ForIndexList fil = eqf->forIndexList();
  setRangeDefinition(fil);
}

Range::Range(AST_Statement_For stf, RANGE::Type type) : _ranges(), _index_pos(), _size(1), _type(type)
{
  AST_ForIndexList fil = stf->forIndexList();
  setRangeDefinition(fil);
}

Range::Range(Variable var, RANGE::Type type) : _ranges(), _index_pos(), _size(1), _type(type) { generate(var); }

Range::Range(AST_Expression exp) : _ranges(), _index_pos(), _size(1), _type(RANGE::For) { generate(exp); }

Range::Range(SBG::MDI mdi) : _ranges(), _index_pos(), _size(1), _type(RANGE::For) { generate(mdi); }

Range::Range(SB::Set set, int offset, vector<string> vars) : _ranges(), _index_pos(), _size(1), _type(RANGE::For) { generate(set, offset, vars); }

void Range::updateRangeDefinition(std::string index_def, RangeDefinition def, int pos)
{
  _ranges.insert(index_def, def);
  _index_pos.insert(index_def, pos);
  Option<RangeDefinition> range = _ranges[index_def];
  assert(range);
  _size *= range->size();
  _row_size.push_back(range->size());
}

void Range::setRangeDefinition(AST_ForIndexList fil)
{
  AST_ForIndexListIterator filit;
  int pos = 0;
  foreach (filit, fil) {
    AST_ForIndex fi = current_element(filit);
    AST_Expression in = fi->in_exp();
    AST_ExpressionList el = in->getAsRange()->expressionList();
    AST_ExpressionListIterator eli;
    EvalInitExp eval;
    int size = el->size();
    int begin = eval.apply(AST_ListFirst(el));
    int end = eval.apply(AST_ListAt(el, size - 1));
    if (end < begin) {
      Error::instance().add(AST_ListFirst(el)->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong equation range.");
    }
    string index = fi->variable()->c_str();
    updateRangeDefinition(index, (size == 2 ? RangeDefinition(begin, end) : RangeDefinition(begin, end, eval.apply(AST_ListAt(el, 1)))),
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
    Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
    ModelConfig::instance().addVariable(index, vi);
  }
}

void Range::generate(AST_Expression exp)
{
  assert(exp->expressionType() == EXPCOMPREF);
  IsConstantIndex constant_index;
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

void Range::generate(SBG::MDI mdi)
{
  int pos = 0;
  for (auto interval : mdi.intervals()) {
    int begin = interval.lower();
    int end = interval.upper();
    if (end < begin) {
      Error::instance().add(0, EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong range in dependency matrix.");
    }
    string index = Utils::instance().iteratorVar(pos);
    updateRangeDefinition(index, RangeDefinition(begin, end), pos++);
    Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
    ModelConfig::instance().addVariable(index, vi);
  }
}

bool Range::isVariable(std::string var)
{
  bool not_number = true;
  try
  {
      std::stoi(var);
      not_number = false;
  }
  catch(const std::exception &) {}
  return not_number;
}

void Range::generate(SB::Set set, int offset, vector<string> vars)
{
  int pos = 0;
  SB::UnordAtomSet a_sets = set.atomicSets();
  for (SB::AtomSet a_set : a_sets) {
    SB::MultiInterval intervals =  a_set.atomicSets();
    for(SB::Interval interval : intervals.intervals()) {
      int begin = interval.lo() - offset + 1;
      int end = begin + interval.hi() - interval.lo();
      if (end < begin) {
        Error::instance().add(0, EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong range in dependency matrix.");
      }
      string index = Utils::instance().iteratorVar(pos);
      if (!vars.empty() && isVariable(vars[pos])) {
        index = vars[pos];
      } 
      updateRangeDefinition(index, RangeDefinition(begin, end), pos++);
      Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
      ModelConfig::instance().addVariable(index, vi);
    }
  }
}

void Range::generate(MDI mdi)
{
  int pos = 0;
  for (auto interval : mdi.mdi()) {
    int begin = interval.lower();
    int end = interval.upper();
    if (end < begin) {
      Error::instance().add(0, EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong range in dependency matrix.");
    }
    string index = Utils::instance().iteratorVar(pos);
    updateRangeDefinition(index, RangeDefinition(begin, end), pos++);
    Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
    ModelConfig::instance().addVariable(index, vi);
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
  return "";
}

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

string Range::print(bool range, bool c_index) const
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
      int begin = (c_index) ? r.cBegin() : r.begin();
      int end = (c_index) ? r.cEnd() : r.end();
      buffer << block << "for(" << idx << " = " << begin << "; ";
      buffer << idx << "<=" << end << "; ";
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

MDI Range::getMDI()
{
  IntervalList intervals;
  RangeDefinitionTable::iterator it;
  for (RangeDefinition rd = _ranges.begin(it); !_ranges.end(it); rd = _ranges.next(it)) {
    intervals.push_back(Interval(rd.begin(), rd.end(), rd.step()));
  }
  return MDI(intervals);
}

bool Range::intersect(Range other)
{
  MDI other_mdi = other.getMDI();
  Option<MDI> intersect = getMDI().Intersection(other_mdi);
  return intersect.is_initialized();
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
  assert(exps.size() == (size_t)_ranges.size());
  // If empty generate a default true condition to handle scalar cases.
  if (exps.empty()) {
    return "1";
  }
  stringstream code;
  RangeDefinitionTable::iterator it;
  int i = 0;
  int size = _ranges.size();
  for (RangeDefinition r = _ranges.begin(it); !_ranges.end(it); r = _ranges.next(it), i++) {
    string exp_str = exps[i];
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

void Range::replace(Index usage)
{
  vector<string> variables = usage.variables();
  set<string> added_vars;
  vector<string> old_keys;
  int pos = 1;
  for (string var : variables) {
    if (isVariable(var)) {
      if (added_vars.find(var) == added_vars.end()) {
        added_vars.insert(var);
        Option<RangeDefinition> r = _ranges[var];
        assert(r);
        string index = getDimensionVar(pos);
        if (index != var) {
          old_keys.push_back(var);
          _ranges.insert(index, r.get());
        }
      }
    }
    pos++;
  }
  for (string key : old_keys) {
    _ranges.remove(key);
  }
}

std::ostream& operator<<(std::ostream& out, const Range& r) { return out << r.print(); }
}  // namespace IR
}  // namespace MicroModelica
