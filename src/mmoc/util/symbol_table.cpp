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

#include "symbol_table.h"

#include <sstream>
#include <math.h>

#include "../ast/ast_builder.h"
#include "../ast/expression.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "../ir/helpers.h"
#include "util.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

/* Variable class. */

Variable::Variable()
    : _unknown(false),
      _discrete(false),
      _t(nullptr),
      _tp(TP_CONSTANT),
      _m(nullptr),
      _comm(nullptr),
      _builtin(false),
      _size(),
      _value(0),
      _exp(nullptr),
      _hasStart(false),
      _hasEach(false),
      _hasAssigment(false),
      _name(),
      _isArray(false),
      _hasOffset(false),
      _offset(0),
      _realType(NotAssigned)
{
}

Variable::Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c)
    : _unknown(false),
      _discrete(false),
      _t(t),
      _tp(tp),
      _m(m),
      _comm(c),
      _builtin(false),
      _size(),
      _value(0),
      _exp(nullptr),
      _hasStart(false),
      _hasEach(false),
      _hasAssigment(false),
      _name(),
      _isArray(false),
      _hasOffset(false),
      _offset(0),
      _realType(NotAssigned)
{
  processModification();
}

Variable::Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c, vector<int> s, bool array)
    : _unknown(false),
      _discrete(false),
      _t(t),
      _tp(tp),
      _m(m),
      _comm(c),
      _builtin(false),
      _size(s),
      _value(0),
      _exp(nullptr),
      _hasStart(false),
      _hasEach(false),
      _hasAssigment(false),
      _name(),
      _isArray(array),
      _hasOffset(false),
      _offset(0),
      _realType(NotAssigned)
{
  processModification();
}

Variable &Variable::operator=(const Variable &other)
{
  _discrete = other._discrete;
  _t = other._t;
  _tp = other._tp;
  _m = other._m;
  _comm = other._comm;
  _builtin = other._builtin;
  _size = other._size;
  _value = other._value;
  _exp = other._exp;
  _hasStart = other._hasStart;
  _hasEach = other._hasEach;
  _hasAssigment = other._hasAssigment;
  _name = other._name;
  _isArray = other._isArray;
  _hasOffset = other._hasOffset;
  _offset = other._offset;
  _realType = other._realType;

  return *this;
}

bool Variable::operator==(const Variable &other) { return print() == other.print(); }

bool Variable::operator!=(const Variable &other) { return !((*this) == other); }

/*! \brief Process the argument modification to determine the variable modifiers if any.
 *
 * 	\note{We don't look for errors here, if there's an error in the code, it should be detected in
 * 	an earlier stage (the model checker and the intermediate code generation.}
 */

void Variable::processModification()
{
  _hasAssigment = false;
  _hasEach = false;
  _hasStart = false;
  if (_m != nullptr) {
    ModificationType t = _m->modificationType();
    if (t == MODEQUAL) {
      _hasAssigment = true;
      _exp = _m->getAsEqual()->exp();
    } else if (t == MODCLASS) {
      AST_ArgumentList al = _m->getAsClass()->arguments();
      AST_ArgumentListIterator it;
      AST_Expression asgExp = _m->getAsClass()->exp();
      if (asgExp->expressionType() != EXPNULL) {
        _hasAssigment = true;
        _exp = asgExp;
      } else {
        if (!isParameter()) {
          foreach (it, al) {
            if (current_element(it)->argumentType() == AR_MODIFICATION) {
              AST_Argument_Modification am = current_element(it)->getAsModification();
              if (am->modification()->modificationType() == MODEQUAL) {
                _exp = am->modification()->getAsEqual()->exp();
                if (current_element(it)->hasEach()) {
                  _hasEach = true;
                } else if (am->name()->compare("start") == 0) {
                  _hasStart = true;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
}

unsigned int Variable::size()
{
  vector<int>::const_iterator it;
  unsigned int total = 1;
  for (it = _size.begin(); it != _size.end(); it++) {
    total *= *it;
  }
  return total;
}

unsigned int Variable::rowSize(unsigned int dim) const
{
  unsigned int total = 1;
  for (unsigned int it = dim + 1; it < _size.size(); it++) {
    total *= _size.at(it);
  }
  return total;
}

string Variable::print() const
{
  stringstream buffer;
  if (isForType() || isInput() || isOutput() || isEqType() || isLocal()) {
    buffer << _name;
  } else if (isConstant()) {
    buffer << _value;
  } else {
    buffer << "_" << _name;
  }
  return buffer.str();
}

ostream &operator<<(ostream &out, const Variable &v)
{
  out << v.print();
  return out;
}

string Variable::declaration(string prefix)
{
  stringstream buffer;
  buffer << "double " << prefix << name();
  if (isArray()) {
    buffer << "[" << size() << "]";
  }
  buffer << ";";
  return buffer.str();
}

string Variable::initialization()
{
  stringstream buffer;
  if (hasAssignment() || hasStartModifier() || hasEachModifier()) {
    Range range = Range(*this);
    Expression ex(exp());
    Expression var = Utils::instance().variableExpression(name(), range);
    if (hasEachModifier()) {
      buffer << range;
      buffer << TAB << TAB << var << " = " << ex << ";" << endl;
      buffer << range.end();
    } else if (hasAssignment() || hasStartModifier()) {
      buffer << var << " = " << ex << ";";
    }
  }
  return buffer.str();
}

/* TypeSymbolTable_ class.*/

TypeSymbolTable::TypeSymbolTable()
{
  insert("String", new Type_String_());
  insert("Real", new Type_Real_());
  insert("Integer", new Type_Integer_());
  insert("Boolean", new Type_Boolean_());
}

/* VarSymbolTable class */

VarSymbolTable::VarSymbolTable() : _parameters(false), _max_dims(1) {}

void VarSymbolTable::initialize(TypeSymbolTable ty)
{
  Variable t(ty["Real"].get(), 0, nullptr, nullptr, vector<int>(1, 0), false);
  t.setBuiltIn();
  t.setName("time");
  insert("time", t);
  Variable reinit(ty["Real"].get(), 0, nullptr, nullptr, vector<int>(1, 0), false);
  reinit.setBuiltIn();
  reinit.setName("reinit");
  insert("reinit", reinit);
  Variable chain_rule(ty["Real"].get(), TP_LOCAL, nullptr, nullptr, vector<int>(1, 0), false);
  chain_rule.setBuiltIn();
  chain_rule.setName("aux");
  insert("aux", chain_rule);
  Variable row(ty["Integer"].get(), TP_LOCAL, nullptr, nullptr, vector<int>(1, 0), false);
  row.setBuiltIn();
  row.setName("row");
  insert("row", row);
}

void VarSymbolTable::insert(VarName name, Variable variable)
{
  assert(name != "");
  variable.setName(name);
  ModelTable<VarName, Variable>::insert(name, variable);
  if (variable.isParameter()) {
    _parameters = true;
  }
  if (variable.dimensions() > _max_dims) {
    _max_dims = variable.dimensions();
  }
}

Option<Variable> VarSymbolTable::lookup(string name)
{
  std::map<string, Variable> table = map();
  Option<Variable> var = table[name];
  if (var) {
    return var;
  }
  return Option<Variable>();
}

unsigned int VarSymbolTable::maxDim() const { return _max_dims; }

}  // namespace Util
}  // namespace MicroModelica
