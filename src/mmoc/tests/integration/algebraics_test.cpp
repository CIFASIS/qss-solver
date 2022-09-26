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

#include <gtest/gtest.h>
#include <iostream>

#include "util/model_config.h"
#include "util/visitors/algebraics.h"
#include "util/util.h"
#include "util/symbol_table.h"
#include <ast/parser/parse.h>

/// @addtogroup QSSSolverUTests
/// @{

using namespace MicroModelica::Util;

/// Unit tests for algebraics variable visitor.
class AlgebraicsTest : public ::testing::Test, public Algebraics {
  public:
  AlgebraicsTest() : Algebraics(false) {}

  virtual ~AlgebraicsTest() {}
};

TEST_F(AlgebraicsTest, testSimpleAlgebraicExp)
{
  stringstream code;
  VarSymbolTable symbols;
  vector<int> size;
  int res;
  code << "a" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  Variable var(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  var.setRealType(Variable::RealType::Algebraic);
  var.setName("a");
  symbols.insert("a", var);
  ModelConfig::instance().setSymbols(symbols);
  EXPECT_TRUE(apply(exp));
}

TEST_F(AlgebraicsTest, testSimpleStateExp)
{
  stringstream code;
  VarSymbolTable symbols;
  vector<int> size;
  int res;
  code << "a" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  Variable var(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  var.setRealType(Variable::RealType::State);
  var.setName("a");
  symbols.insert("a", var);
  ModelConfig::instance().setSymbols(symbols);
  EXPECT_FALSE(apply(exp));
}

TEST_F(AlgebraicsTest, testMixedExp)
{
  stringstream code;
  VarSymbolTable symbols;
  vector<int> size;
  int res;
  code << "a + b" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  Variable a(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  a.setRealType(Variable::RealType::Algebraic);
  a.setName("a");
  symbols.insert("a", a);
  Variable b(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  b.setRealType(Variable::RealType::State);
  b.setName("b");
  symbols.insert("b", a);
  ModelConfig::instance().setSymbols(symbols);
  EXPECT_TRUE(apply(exp));
}

TEST_F(AlgebraicsTest, testExcludeExp)
{
  stringstream code;
  VarSymbolTable symbols;
  vector<int> size;
  int res;
  code << "a" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  Variable var(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  var.setRealType(Variable::RealType::Algebraic);
  var.setName("a");
  symbols.insert("a", var);
  ModelConfig::instance().setSymbols(symbols);
  exclude(var);
  EXPECT_FALSE(apply(exp));
}

TEST_F(AlgebraicsTest, testOutputExp)
{
  stringstream code;
  VarSymbolTable symbols;
  vector<int> size;
  int res;
  code << "(a)" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  Variable var(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  var.setRealType(Variable::RealType::Algebraic);
  var.setName("a");
  symbols.insert("a", var);
  ModelConfig::instance().setSymbols(symbols);
  EXPECT_TRUE(apply(exp));
}

TEST_F(AlgebraicsTest, testFunctionCallExp)
{
  stringstream code;
  VarSymbolTable symbols;
  vector<int> size;
  int res;
  code << "sin(a)" << endl;
  AST_Expression exp = parseExpression(code.str(), &res);
  EXPECT_EQ(res, 0);
  Variable var(newType_Real(), TP_EQ, nullptr, nullptr, size, false);
  var.setRealType(Variable::RealType::Algebraic);
  var.setName("a");
  symbols.insert("a", var);
  ModelConfig::instance().setSymbols(symbols);
  EXPECT_TRUE(apply(exp));
}

/// @}
