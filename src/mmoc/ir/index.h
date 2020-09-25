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

#ifndef INDEX_H_
#define INDEX_H_

#include <string>
#include <map>

#include "expression.h"
#include "../deps/graph/graph_helpers.h"
#include "../deps/graph/sb_graph_helpers.h"
#include "../util/symbol_table.h"
#include "../util/table.h"

namespace MicroModelica {
namespace IR {

namespace RANGE {
typedef enum { For, If } Type;
}

namespace INDEX {
typedef enum { Iterator, Dimension } Type;
}

class Index;

class RangeDefinition {
  public:
  RangeDefinition(){};
  RangeDefinition(int begin, int end, int step = 1);
  ~RangeDefinition() = default;
  inline int begin() { return _begin; };
  inline int end() { return _end; };
  inline int step() { return _step; };
  inline int size() const { return (_begin == _end) ? 1 : (_end - _begin + 1) / _step; };
  void setBegin(int begin);
  void setEnd(int end);
  friend std::ostream& operator<<(std::ostream& out, const RangeDefinition& rd);

  private:
  int _begin;
  int _end;
  int _step;
};

typedef ModelTable<std::string, RangeDefinition> RangeDefinitionTable;

class Range {
  public:
  Range();
  Range(AST_Equation_For eqf, Util::VarSymbolTable symbols, RANGE::Type type = RANGE::For);
  Range(AST_Statement_For stf, Util::VarSymbolTable symbols, RANGE::Type type = RANGE::For);
  Range(Util::Variable var, RANGE::Type type = RANGE::For);
  Range(AST_Expression exp);
  Range(Deps::SBG::MDI mdi);
  ~Range() = default;

  int size() const;
  bool isEmpty() const;
  inline RangeDefinitionTable definition() const { return _ranges; };
  std::string print() const;
  std::string end() const;
  string indexes() const;
  void addLocalVariables() const;
  int rowSize(int dim) const;
  std::string block(int dim = -1) const;
  int pos(std::string var);
  void generate(Deps::MDI mdi);
  inline bool empty() { return _size == 0; };
  std::string iterator(int dim);
  std::string getDimensionVarsString() const;
  std::vector<std::string> getDimensionVars() const;
  std::string getDimensionVar(int i) const;
  bool intersect(Range other);
  Deps::MDI getMDI();
  void applyUsage(Index usage);
  std::string in(ExpressionList exps);
  std::string in(std::vector<std::string> exps);
  friend std::ostream& operator<<(std::ostream& out, const Range& r);

  protected:
  void generate(Util::Variable var);
  void generate(AST_Expression exp);
  void generate(Deps::SBG::MDI mdi);
  void updateRangeDefinition(std::string index_def, RangeDefinition def, int pos);

  private:
  void setRangeDefinition(AST_ForIndexList fil, Util::VarSymbolTable symbols);
  RangeDefinitionTable _ranges;
  ModelTable<std::string, int> _index_pos;
  int _size;
  RANGE::Type _type;
  std::vector<int> _row_size;
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
  Deps::Usage usage() const;
  Index revert() const;
  Index replace() const;
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

  protected:
  void parseIndexes();

  private:
  map<int, IndexDefinition> _indexes;
  Expression _exp;
};

typedef ModelTable<std::string, Index> IndexTable;

}  // namespace IR
}  // namespace MicroModelica
#endif /* INDEX_H_ */
