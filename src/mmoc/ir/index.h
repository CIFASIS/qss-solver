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

class RangeDefinition {
  public:
  RangeDefinition(){};
  RangeDefinition(int begin, int end, int step = 1);
  ~RangeDefinition(){};
  inline int begin() { return _begin; };
  inline int end() { return _end; };
  inline int step() { return _step; };
  inline int size() const { return (_begin == _end) ? 1 : (_end - _begin + 1) / _step; };
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

  ~Range(){};
  inline int size() const { return _size; };
  inline bool isEmpty() const { return _size == 0; };
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
  std::string getDimensionVars() const;
  std::string getDimensionVar(int i) const;
  friend std::ostream& operator<<(std::ostream& out, const Range& r);

  protected:
  void generate(Util::Variable var);

  private:
  void setRangeDefinition(AST_ForIndexList fil, Util::VarSymbolTable symbols);
  RangeDefinitionTable _ranges;
  ModelTable<std::string, int> _indexPos;
  int _size;
  RANGE::Type _type;
  std::vector<int> _rowSize;
};

class IndexDefinition {
  public:
  IndexDefinition();
  ~IndexDefinition(){};

  friend std::ostream& operator<<(std::ostream& out, const IndexDefinition& id);

  private:
  string _variable;
  Expression _exp;
};

class Index {
  public:
  Index() : _indexes(), _dim(0), _exp(){};
  Index(IndexDefinition id);
  Index(Expression exp);
  ~Index(){};
  void setMap(IR::Expression exp);
  bool hasMap() const;
  bool operator==(const Index& other) const;
  bool isConstant() const;
  inline int dimension() { return _dim; };
  inline void setExp(Expression exp) { _exp = exp; };
  std::string print() const;
  std::string identifier() const;
  Range range();
  Deps::Usage usage() const;
  Index revert() const;
  void replace();
  std::string usageExp() const;
  std::string modelicaExp() const;
  friend std::ostream& operator<<(std::ostream& out, const Index& i);
  Util::Variable variable() const;

  private:
  map<int, IndexDefinition> _indexes;
  int _dim;
  Expression _exp;
};

typedef ModelTable<std::string, Index> IndexTable;

}  // namespace IR
}  // namespace MicroModelica
#endif /* INDEX_H_ */