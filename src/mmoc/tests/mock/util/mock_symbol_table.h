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

#ifndef __MOCK_SYMBOL_TABLE_HPP__
#define __MOCK_SYMBOL_TABLE_HPP__

#include <gmock/gmock.h>

#include "../util/symbol_table.h"

class MockVariable : public Variable {
  public:
  virtual ~MockVariable() = default;
  MOCK_METHOD1(setRealType, void(RealType));
  MOCK_METHOD0(typePrefix, AST_TypePrefix());
  MOCK_METHOD0(comment, AST_Comment());
  MOCK_METHOD1(setComment, void(AST_Comment));
  MOCK_METHOD0(modification, AST_Modification());
  MOCK_METHOD1(setModification, void(AST_Modification);
  MOCK_METHOD0(type, Type());
  MOCK_METHOD1(setType, void(Type));
  MOCK_METHOD0(setParameter, void());
  MOCK_CONST_METHOD0(isParameter, bool());
  MOCK_CONST_METHOD0(isDiscrete, bool());
  MOCK_CONST_METHOD0(builtIn, bool());
  MOCK_METHOD0(setDiscrete, void());
  MOCK_METHOD0(setBuiltIn, void());
  MOCK_CONST_METHOD0(isConstant, bool());
  MOCK_CONST_METHOD0(isInput, bool());
  MOCK_CONST_METHOD0(isOutput, bool());
  MOCK_CONST_METHOD0(isForType, bool());
  MOCK_CONST_METHOD0(isEqType, bool());
  MOCK_CONST_METHOD0(isState, bool());
  MOCK_CONST_METHOD0(isString, bool());
  MOCK_METHOD0(setState, void());
  MOCK_METHOD0(isUnknown, bool());
  MOCK_METHOD0(setUnknown, void());
  MOCK_METHOD0(isTime, bool());
  MOCK_CONST_METHOD0(isAlgebraic, bool());
  MOCK_METHOD0(setAlgebraic, void());
  MOCK_METHOD1(setValue, void(int));
  MOCK_METHOD0(value, int());
  MOCK_METHOD0(size, unsigned int());
  MOCK_METHOD0(hasAssignment, bool());
  MOCK_METHOD0(hasStartModifier, bool());
  MOCK_METHOD0(hasEachModifier, bool());
  MOCK_METHOD1(setEachModifier, void(bool));
  MOCK_CONST_METHOD0(name, string());
  MOCK_METHOD1(setName, void(std::string));
  MOCK_METHOD0(exp, AST_Expression());
  MOCK_METHOD0(isArray, bool());
  MOCK_METHOD0(isScalar, bool());
  MOCK_CONST_METHOD1(size, unsigned int(int));
  MOCK_CONST_METHOD1(rowSize, unsigned int(unsigned int));
  MOCK_CONST_METHOD1(dimensions, unsigned int());
  MOCK_METHOD1(declaration, std::string(std::string));
  MOCK_METHOD1(initialization, std::string(const VarSymbolTable&));
  MOCK_METHOD0(hasOffset, bool());
  MOCK_METHOD1(setOffset, void(int));
  MOCK_METHOD0(offset, int());
  MOCK_CONST_METHOD0(isModelVar, bool());
  MOCK_CONST_METHOD0(print, std::string());
};

class MockVarSymbolTable : public VarSymbolTable {
  public:
  virtual ~MockVarSymbolTable() = default;
  MOCK_METHOD1(initialize, void(TypeSymbolTable));
  MOCK_METHOD2(insert, void(VarName, Variable vi));
  MOCK_METHOD0(parameters, bool());
};

#endif  // __MOCK_SYMBOL_TABLE_HPP__
