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

#include <string>
#include <map>

#include <ir/expression.h>
#include <deps/sbg_graph/deps_graph.h>
#include <util/symbol_table.h>
#include <util/table.h>

namespace MicroModelica {
namespace IR {

namespace RANGE {
typedef enum { For, If } Type;
}

namespace INDEX {
typedef enum { Iterator, Dimension } Type;
}

class Index;

typedef vector<int> Usage;

class RangeDefinition {
  public:
  RangeDefinition(){};
  RangeDefinition(int begin, int end, int step = 1);
  RangeDefinition(Expression begin_exp, Expression end_exp, int begin, int end, int step = 1);
  ~RangeDefinition() = default;
  inline int begin() const { return _begin; }
  inline int cBegin() const { return begin() - 1; }
  inline int end() const { return _end; };
  inline int cEnd() const { return end() - 1; };
  inline int step() const { return _step; };
  inline int size() const { return (_begin == _end) ? 1 : (_end - _begin + 1) / _step; };
  void setBegin(int begin);
  void setEnd(int end);
  std::string beginExp(bool convert_params, bool c_index) const;
  std::string endExp(bool convert_params, bool c_index) const;

  protected:
  std::string generateExp(Expression exp, int limit, bool convert_params) const;
  int _begin;
  int _end;
  int _step;
  Expression _begin_exp;
  Expression _end_exp;
};

typedef ModelTable<std::string, RangeDefinition> RangeDefinitionTable;

class Range {
  public:
  Range();
  Range(AST_Equation_For eqf, RANGE::Type type = RANGE::For);
  Range(AST_Statement_For stf, RANGE::Type type = RANGE::For);
  Range(Util::Variable var, RANGE::Type type = RANGE::For);
  Range(AST_Expression exp);
  Range(SB::Set set, int offset, std::vector<std::string> vars = std::vector<std::string>());

  ~Range() = default;

  int size() const;
  bool isEmpty() const;
  inline RangeDefinitionTable definition() const { return _ranges; };
  int dim() const;
  std::string print(bool range = false, bool c_index = false, bool convert_params = false) const;
  std::string end() const;
  std::string indexes() const;
  std::vector<std::string> getIndexes() const;
  std::vector<std::string> getInitValues() const;
  void addLocalVariables() const;
  void addRangeLocalVariables() const;
  int rowSize(int dim) const;
  std::string block(int dim = -1) const;
  int pos(std::string var);
  std::string iterator(int dim, bool range_idx = false);
  std::string iterator(std::string var, int dim, bool range_idx = false);
  std::string getPrintDimensionVarsString() const;
  std::string getDimensionVarsString(bool range = false) const;
  std::vector<std::string> getDimensionVars(bool range = false) const;
  std::string getDimensionVar(int i, bool range = false) const;
  bool intersect(Range other);
  void applyUsage(Index usage);
  bool checkUsage(Index usage, Index def);
  std::string in(ExpressionList exps);
  std::string in(std::vector<std::string> exps);
  map<std::string, AST_Expression> initExps();
  void replace(Index usage);
  friend std::ostream& operator<<(std::ostream& out, const Range& r);
  void generate(SB::Set set, int offset, std::vector<std::string> vars, std::vector<Expression> begin_exps = std::vector<Expression>(),
                std::vector<Expression> end_exps = std::vector<Expression>());

  bool fixed() const;

  protected:
  void generate(Util::Variable var);
  void generate(AST_Expression exp);
  void updateRangeDefinition(std::string index_def, RangeDefinition def, int pos);
  void addRangeVariables(int i, string index) const;
  bool isVariable(std::string var);
  bool testExpression(AST_Expression exp);
  Expression getExp(std::vector<Expression> exps, size_t pos);
  bool isDimensionVar(std::string var);

  private:
  void setRangeDefinition(AST_ForIndexList fil);
  RangeDefinitionTable _ranges;
  ModelTable<std::string, int> _index_pos;
  int _size;
  RANGE::Type _type;
  std::vector<int> _row_size;
  bool _fixed;
};

class IndexDefinition {
  public:
  IndexDefinition();
  IndexDefinition(std::string variable, int constant, int factor);
  ~IndexDefinition() = default;
  int constant();
  int factor();
  std::string variable();

  private:
  string _variable;
  int _constant;
  int _factor;
};

class Index {
  public:
  Index() : _indexes(), _exp(){};
  Index(Expression exp);
  ~Index() = default;
  void setMap(IR::Expression exp);
  bool hasMap() const;
  bool operator==(const Index& other) const;
  bool isConstant() const;
  int dimension();
  void setExp(Expression exp);
  std::string print() const;
  std::string identifier() const;
  Range range() const;
  Index revert() const;
  Index replace(bool range_idx = false) const;
  std::string usageExp() const;
  std::string modelicaExp() const;
  friend std::ostream& operator<<(std::ostream& out, const Index& i);
  Util::Variable variable() const;
  bool hasVariable(int dim);
  std::string variable(int dim);
  bool hasFactor(int dim);
  int factor(int dim);
  bool hasConstant(int dim);
  int constant(int dim);
  Expression expression() const;
  bool isEmpty() const;
  std::vector<std::string> variables();

  protected:
  void parseIndexes();

  private:
  map<int, IndexDefinition> _indexes;
  Expression _exp;
};

typedef ModelTable<std::string, Index> IndexTable;

}  // namespace IR
}  // namespace MicroModelica
