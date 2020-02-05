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
#include "../ast/expression.h"
#include "../ast/equation.h"
#include "../ast/statement.h"
#include "../util/visitors/eval_init_exp.h"
#include "../util/visitors/get_index_usage.h"
#include "../util/visitors/is_constant_index.h"
#include "../util/visitors/replace_index.h"
#include "../util/visitors/revert_index.h"
#include "../util/util.h"
#include "../util/error.h"

namespace MicroModelica {
using namespace Deps;
using namespace Util;
namespace IR {

IndexDefinition::IndexDefinition() {}

std::ostream& operator<<(std::ostream& out, const IndexDefinition& id) { return out; }

Index::Index(Expression exp) : _indexes(), _dim(0), _exp(exp) {}

Index::Index(IndexDefinition id) : _indexes(), _dim(0), _exp() { _indexes[_dim++] = id; }

void Index::setMap(Expression exp) {}

bool Index::hasMap() const { return false; }

bool Index::operator==(const Index& other) const { return false; }

bool Index::isConstant() const
{
  IsConstantIndex constant_index;
  return constant_index.apply(_exp.expression());
}

Usage Index::usage() const
{
  GetIndexUsage usage;
  return usage.apply(_exp.expression());
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
  if (var) {
    return var.get();
  }
  assert(false);
  return Variable();
}

Range Index::range() const { return Range(variable()); }

Index Index::revert() const
{
  RevertIndex revert;
  VarSymbolTable symbols = Utils::instance().symbols();
  return Index(Expression(revert.apply(_exp.expression()), symbols));
}

Index Index::replace() const
{
  ReplaceIndex replace = ReplaceIndex(range(), usage());
  return Index(Expression(replace.apply(_exp.expression()), Utils::instance().symbols()));
}

string Index::usageExp() const { return _exp.usage(); }

string Index::modelicaExp() const
{
  stringstream buffer;
  string usage_exp = usageExp();
  buffer << variable().name();
  if (!usage_exp.empty()) {
    buffer << "[" << usageExp() << "]";
  }
  return buffer.str();
}

string Index::print() const
{
  stringstream buffer;
  buffer << "_idx" << _exp;
  return buffer.str();
}

std::ostream& operator<<(std::ostream& out, const Index& i)
{
  out << i.print();
  return out;
}

RangeDefinition::RangeDefinition(int begin, int end, int step) : _begin(begin), _end(end), _step(step) {}

std::ostream& operator<<(std::ostream& out, const RangeDefinition& rd) { return out; }

Range::Range() : _ranges(), _indexPos(), _size(0), _type(RANGE::For) {}

Range::Range(AST_Equation_For eqf, VarSymbolTable symbols, RANGE::Type type) : _ranges(), _indexPos(), _size(0), _type(type)
{
  AST_ForIndexList fil = eqf->forIndexList();
  setRangeDefinition(fil, symbols);
}

Range::Range(AST_Statement_For stf, VarSymbolTable symbols, RANGE::Type type) : _ranges(), _indexPos(), _size(0), _type(type)
{
  AST_ForIndexList fil = stf->forIndexList();
  setRangeDefinition(fil, symbols);
}

Range::Range(Variable var, RANGE::Type type) : _ranges(), _indexPos(), _size(0), _type(type) { generate(var); }

void Range::setRangeDefinition(AST_ForIndexList fil, VarSymbolTable symbols)
{
  AST_ForIndexListIterator filit;
  int pos = 0;
  foreach (filit, fil) {
    AST_ForIndex fi = current_element(filit);
    AST_Expression in = fi->in_exp();
    AST_ExpressionList el = in->getAsRange()->expressionList();
    AST_ExpressionListIterator eli;
    EvalInitExp eval(symbols);
    int size = el->size();
    int begin = eval.apply(AST_ListFirst(el));
    int end = eval.apply(AST_ListAt(el, size - 1));
    if (end < begin) {
      Error::instance().add(AST_ListFirst(el)->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong equation range.");
    }
    string index = fi->variable()->c_str();
    _ranges.insert(index, (size == 2 ? RangeDefinition(begin, end) : RangeDefinition(begin, end, eval.apply(AST_ListAt(el, 1)))));
    _indexPos.insert(index, pos++);
    Option<RangeDefinition> range = _ranges[index];
    if (range) {
      _size += range->size();
      _rowSize.push_back(range->size());
    } else {
      _rowSize.push_back(1);
    }
  }
}

void Range::generate(Variable var)
{
  int pos = 0;
  for (unsigned int i = 0; i < var.dimensions(); i++) {
    string index = getDimensionVar(i + 1);
    int begin = 1;
    int end = var.size(i);
    _ranges.insert(index, RangeDefinition(begin, end));
    _indexPos.insert(index, pos++);
    Option<RangeDefinition> range = _ranges[index];
    if (range) {
      _size += range->size();
      _rowSize.push_back(range->size());
    } else {
      _rowSize.push_back(1);
    }
    Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
    Utils::instance().symbols().insert(index, vi);
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
    _ranges.insert(index, RangeDefinition(begin, end));
    _indexPos.insert(index, pos++);
    Option<RangeDefinition> range = _ranges[index];
    if (range) {
      _size += range->size();
      _rowSize.push_back(range->size());
    } else {
      _rowSize.push_back(1);
    }
    Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
    Utils::instance().symbols().insert(index, vi);
  }
}

string Range::iterator(int dim)
{
  for (auto ip : _indexPos) {
    if (ip.second == dim) {
      return ip.first;
    }
  }
  return "";
}

string Range::getDimensionVars() const
{
  stringstream buffer;
  int size = _ranges.size();
  for (int i = 1; i <= size; i++) {
    buffer << getDimensionVar(i) << (i + 1 < size ? "," : "");
  }
  return buffer.str();
}

string Range::getDimensionVar(int i) const
{
  stringstream buffer;
  buffer << "_d" << i;
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

string Range::print() const
{
  stringstream buffer;
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  string block = "";
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    if (_type == RANGE::For) {
      string idx = ranges.key(it);
      buffer << block << "for(" << idx << " = " << r.begin() << "; ";
      buffer << idx << "<=" << r.end() << "; ";
      buffer << idx << "+=" << r.step() << ") {" << endl;
      block += TAB;
    }
  }
  addLocalVariables();
  return buffer.str();
}

string Range::indexes() const
{
  stringstream buffer;
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  int size = ranges.size(), i = 0;
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    string var = ranges.key(it);
    buffer << var << (++i < size ? "," : "");
  }
  return buffer.str();
}

void Range::addLocalVariables() const
{
  RangeDefinitionTable ranges = _ranges;
  RangeDefinitionTable::iterator it;
  int i = 1;
  for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
    string var = ranges.key(it);
    Utils::instance().addLocalSymbol("int " + var + ";");
    Utils::instance().addLocalSymbol("int " + getDimensionVar(i++) + ";");
  }
}

int Range::rowSize(int dim) const
{
  int total = 1;
  for (int it = _rowSize.size() - 1; it > dim; it--) {
    total *= _rowSize.at(it);
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
  Option<int> p = _indexPos[var];
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

std::ostream& operator<<(std::ostream& out, const Range& r) { return out << r.print(); }
}  // namespace IR
}  // namespace MicroModelica
