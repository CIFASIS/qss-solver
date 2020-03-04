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

#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "../ast/ast_types.h"
#include "../ast/element.h"
#include "../ast/modification.h"
#include "type.h"
#include "table.h"

namespace MicroModelica {

namespace Util {

class VarSymbolTable;

class Variable {
  public:
  Variable();
  Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c);
  Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c, vector<int> s, bool array);
  Variable& operator=(const Variable& other);

  typedef enum { State, Algebraic, NotAssigned } RealType;

  virtual void setRealType(RealType type) { _realType = type; };
  virtual AST_TypePrefix typePrefix() { return _tp; };
  virtual AST_Comment comment() { return _comm; };
  virtual void setComment(AST_Comment c) { _comm = c; };
  virtual AST_Modification modification() { return _m; };
  virtual void setModification(AST_Modification m)
  {
    _m = m;
    processModification();
  };
  virtual Type type() { return _t; };
  virtual void setType(Type t) { _t = t; };
  virtual void setParameter()
  {
    _tp = TP_PARAMETER;
    unsetStartEach();
  };
  virtual bool isParameter() const { return _tp & TP_PARAMETER; };
  virtual bool isDiscrete() const { return (_tp & TP_DISCRETE) || _discrete; };
  virtual bool builtIn() const { return _builtin; };
  virtual void setDiscrete()
  {
    _discrete = true;
    unsetAssignment();
  };
  virtual void setBuiltIn() { _builtin = true; };
  virtual bool isConstant() const { return _tp & TP_CONSTANT; };
  virtual bool isInput() const { return _tp & TP_INPUT; };
  virtual bool isOutput() const { return _tp & TP_OUTPUT; };
  virtual bool isForType() const { return _tp & TP_FOR; };
  virtual bool isEqType() const { return _tp & TP_EQ; };
  virtual bool isState() const { return _realType == State; };
  virtual bool isString() const { return _t->getType() == TYSTRING; };
  virtual void setState() { unsetAssignment(); };
  virtual bool isUnknown() { return _unknown; };
  virtual void setUnknown() { _unknown = true; };
  virtual bool isTime() { return _name.compare("time") == 0; };
  virtual bool isAlgebraic() const { return _realType == Algebraic; };
  virtual void setAlgebraic()
  {
    unsetAssignment();
    unsetStartEach();
  };
  virtual void setValue(int val) { _value = val; };
  virtual int value() { return _value; };
  virtual unsigned int size();
  virtual bool hasAssignment() { return _hasAssigment; };
  virtual bool hasStartModifier() { return _hasStart; };
  virtual bool hasEachModifier() { return _hasEach; };
  virtual void setEachModifier(bool each) { _hasEach = each; };
  virtual string name() const { return _name; };
  virtual void setName(string n) { _name = n; };
  virtual AST_Expression exp() { return _exp; };
  virtual bool isArray() { return _isArray; };
  virtual bool isScalar() { return !isArray(); };
  friend ostream& operator<<(ostream& os, const Variable& e);
  virtual unsigned int size(int dim) const { return _size[dim]; };
  virtual unsigned int rowSize(unsigned int dim) const;
  virtual unsigned int dimensions() const { return _size.size(); };
  virtual std::string declaration(std::string prefix = "");
  virtual std::string initialization(const VarSymbolTable& symbols);
  virtual bool hasOffset() { return _hasOffset; };
  virtual void setOffset(int offset)
  {
    _offset = offset;
    _hasOffset = true;
  };
  virtual int offset() const { return _offset; };
  virtual bool isModelVar() const { return isState() || isDiscrete() || isAlgebraic() || isParameter() || isEqType() || isOutput(); };
  virtual std::string print() const;
  friend std::ostream& operator<<(std::ostream& out, const Variable& v);

  private:
  std::string variableExpression(const VarSymbolTable& symbols);
  void processModification();
  void unsetAssignment() { _hasAssigment = false; };
  virtual void unsetStartEach()
  {
    _hasEach = false;
    _hasStart = false;
  };

  bool _unknown;
  bool _discrete;
  Type _t;
  AST_TypePrefix _tp;
  AST_Modification _m;
  AST_Comment _comm;
  bool _builtin;
  vector<int> _size;
  int _value;
  AST_Expression _exp;
  bool _hasStart;
  bool _hasEach;
  bool _hasAssigment;
  string _name;
  bool _isArray;
  bool _hasOffset;
  int _offset;
  RealType _realType;
};

class TypeSymbolTable : public ModelTable<TypeName, Type> {
  public:
  TypeSymbolTable();
};

class VarSymbolTable : public ModelTable<VarName, Variable> {
  public:
  VarSymbolTable();
  ~VarSymbolTable() = default;
  virtual void initialize(TypeSymbolTable tst);
  virtual void insert(VarName n, Variable vi);
  virtual bool parameters() { return _parameters; };

  private:
  bool _parameters;
};
}  // namespace Util
}  // namespace MicroModelica

#endif /* SYMBOL_TABLE_H_ */
