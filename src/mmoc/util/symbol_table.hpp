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

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include <ast/ast_types.hpp>
#include <ast/element.hpp>
#include <ast/modification.hpp>
#include <util/type.hpp>
#include <util/table.hpp>

namespace MicroModelica {

namespace Util {

class VarSymbolTable;

class Variable {
  public:
  Variable();
  Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c);
  Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c, const vector<int>& s, bool array);
  Variable& operator=(const Variable& other);
  bool operator==(const Variable& other);
  bool operator!=(const Variable& other);

  typedef enum { State, Algebraic, NotAssigned } RealType;

  inline void setRealType(RealType type) { _realType = type; };
  inline AST_TypePrefix typePrefix() const { return _tp; };
  inline AST_Comment comment() { return _comm; };
  inline void setComment(AST_Comment c) { _comm = c; };
  inline AST_Modification modification() { return _m; };
  inline void setModification(AST_Modification m)
  {
    _m = m;
    processModification();
  };
  inline Type type() { return _t; };
  inline void setType(Type t) { _t = t; };
  inline void setParameter()
  {
    _tp = TP_PARAMETER;
    unsetStartEach();
  };
  inline bool isParameter() const { return _tp & TP_PARAMETER; };
  inline bool isDiscrete() const { return (_tp & TP_DISCRETE); };
  inline bool builtIn() const { return _builtin; };
  inline void setBuiltIn() { _builtin = true; };
  inline bool isConstant() const { return _tp & TP_CONSTANT; };
  inline bool isInput() const { return _tp & TP_INPUT; };
  inline bool isOutput() const { return _tp & TP_OUTPUT; };
  inline bool isForType() const { return _tp & TP_FOR; };
  inline bool isEqType() const { return _tp & TP_EQ; };
  inline bool isLocal() const { return _tp & TP_LOCAL; };
  inline bool isState() const { return _realType == State; };
  inline bool isString() const { return _t->getType() == SymbolType::TYSTRING; };
  inline void setState() { unsetAssignment(); };
  inline bool isUnknown() const { return _unknown; };
  inline void setUnknown() { _unknown = true; };
  inline bool isTime() const { return _name.compare("time") == 0; };
  inline bool isAlgebraic() const { return _realType == Algebraic; };
  inline void setValue(int val) { _value = val; };
  inline int value() const { return _value; };
  unsigned int size();
  inline bool hasAssignment() const { return _hasAssigment; };
  inline bool hasStartModifier() const { return _hasStart; };
  inline bool hasEachModifier() const { return _hasEach; };
  inline void setEachModifier(bool each) { _hasEach = each; };
  inline string name() const { return _name; };
  void setName(string name);
  inline AST_Expression exp() { return _exp; };
  inline bool isArray() const { return _isArray; };
  inline bool isScalar() const { return !isArray(); };
  friend ostream& operator<<(ostream& os, const Variable& e);
  inline unsigned int size(int dim) const { return _size[dim]; };
  unsigned int rowSize(unsigned int dim) const;
  inline unsigned long dimensions() const { return _size.size(); };
  std::string declaration(std::string prefix = "");
  std::string initialization();
  inline bool hasOffset() const { return _hasOffset; };
  inline void setOffset(int offset)
  {
    _offset = offset;
    _hasOffset = true;
  };
  inline int offset() const { return _offset; };
  inline bool isModelVar() const { return isState() || isDiscrete() || isAlgebraic() || isParameter() || isEqType() || isOutput(); };
  std::string print() const;
  std::string castOperator() const;
  friend std::ostream& operator<<(std::ostream& out, const Variable& v);

  protected:
  void processModification();
  void unsetAssignment() { _hasAssigment = false; };
  inline void unsetStartEach()
  {
    _hasEach = false;
    _hasStart = false;
  };
  bool isDiscreteInteger() const;

  bool _unknown;
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
  void initialize(TypeSymbolTable tst);
  void insert(VarName name, Variable variable);
  inline bool parameters() const { return _parameters; };
  Option<Variable> lookup(const std::string& name) const;
  unsigned long maxDim() const;

  private:
  bool _parameters;
  unsigned long _max_dims;
};

using VariableList = std::list<Variable>;

}  // namespace Util
}  // namespace MicroModelica
