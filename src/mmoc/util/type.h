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

#include <iostream>
#include <list>
#include <string>

#include <ast/ast_types.h>
#include <ast/expression.h>
#include <util/macros.h>

enum class SymbolType { TYREAL, TYINTEGER, TYBOOLEAN, TYSTRING, TYARRAY, TYTUPLA, TYFUNCTION };

DEFINE_TYPE(Type);
DEFINE_TYPE(Type_Real);
DEFINE_TYPE(Type_Integer);
DEFINE_TYPE(Type_Boolean);
DEFINE_TYPE(Type_String);
DEFINE_TYPE(Type_Array);
DEFINE_TYPE(Type_Tupla);
DEFINE_TYPE(Type_Function);
DEFINE_LIST(Type);

class Type_ {
  public:
  virtual ~Type_() = default;
  virtual SymbolType getType() = 0;
  virtual string print() const = 0;
  friend ostream &operator<<(ostream &os, const Type_ &e);
  friend ostream &operator<<(ostream &os, const Type &e);
  friend int operator==(Type_ &e1, Type_ &e2);
  friend int operator==(Type_ &e1, Type e2);
  friend int operator!=(Type_ &e1, Type_ &e2);
  friend int operator!=(Type_ &e1, Type e2);
  Type_Array getAsArray();
  Type_Tupla getAsTupla();
  Type_Function getAsFunction();
};

class Type_Real_ : public Type_ {
  public:
  Type_Real_() = default;
  ~Type_Real_() override = default;
  SymbolType getType() override { return SymbolType::TYREAL; };
  string print() const override;
};

Type_Real newType_Real();
void deleteType_Real(Type_Real m);

class Type_Integer_ : public Type_ {
  public:
  Type_Integer_() = default;
  ~Type_Integer_() override = default;
  SymbolType getType() override { return SymbolType::TYINTEGER; };
  string print() const override;
};

Type_Integer newType_Integer();
void deleteType_Integer(Type_Integer m);

class Type_Boolean_ : public Type_ {
  public:
  ~Type_Boolean_() override = default;
  ;
  SymbolType getType() override { return SymbolType::TYBOOLEAN; };
  string print() const override;
};

class Type_String_ : public Type_ {
  public:
  ~Type_String_() override = default;
  SymbolType getType() override { return SymbolType::TYSTRING; };
  string print() const override;
};

Type_String newType_String();
void deleteType_String(Type_String m);

class Type_Array_ : public Type_ {
  public:
  Type_Array_(Type t, AST_Expression dim);
  ~Type_Array_() override = default;
  SymbolType getType() override { return SymbolType::TYARRAY; }
  string print() const override;
  Type arrayOf();
  AST_Expression dimension() { return _dim; };

  private:
  Type _t;
  AST_Expression _dim;
};

class Type_Tupla_ : public Type_ {
  public:
  explicit Type_Tupla_(TypeList tyl);
  ~Type_Tupla_() override = default;
  string print() const override;
  TypeList tupla() { return _tyl; };
  SymbolType getType() override { return SymbolType::TYTUPLA; }

  private:
  TypeList _tyl;
};

class Type_Function_ : public Type_ {
  public:
  Type_Function_(Type output, TypeList input);
  ~Type_Function_() override = default;
  string print() const override;
  TypeList input() { return _input; };
  Type output() { return _output; };
  SymbolType getType() override { return SymbolType::TYFUNCTION; };

  private:
  TypeList _input;
  Type _output;
};
