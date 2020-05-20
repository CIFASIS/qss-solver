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

#ifndef EQUATION_H_
#define EQUATION_H_

#include "../ast/ast_types.h"
#include "../deps/dependency_matrix.h"
#include "../util/table.h"
#include "index.h"

namespace MicroModelica {
namespace Util {
typedef ModelTable<std::string, std::string> SymbolTable;
}

namespace IR {
namespace EQUATION {
typedef enum { ClassicDerivative, QSSDerivative, Algebraic, Dependency, Output, ZeroCrossing, Handler, Jacobian, JacobianTerm } Type;
}
class EquationVariable {
  public:
  static std::string modelVariables(int id, EQUATION::Type type)
  {
    stringstream buffer;
    if (type == EQUATION::Type::Output) {
      buffer << "_out_exp_";
    } else {
      buffer << "_event_";
    }
    buffer << id;
    return buffer.str();
  };
};

class Equation {
  public:
  Equation();
  Equation(AST_Expression lhs, AST_Expression rhs, Util::VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id);
  Equation(AST_Expression eq, Util::VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id, int offset);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, EQUATION::Type type, int id);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, Range r, EQUATION::Type type, int id);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, Option<Range> r, EQUATION::Type type, int id);
  ~Equation() = default;
  inline bool hasRange() const { return _range.is_initialized(); };
  inline Expression lhs() const { return _lhs; };
  Index index() const;
  inline Expression rhs() const { return _rhs; };
  inline AST_Expression equation() { return _rhs.expression(); };
  inline bool autonomous() { return _autonomous; };
  inline Util::SymbolTable calledFunctions() { return _calledFunctions; };
  std::string print() const;
  std::string macro() const;
  inline Option<Range> range() const { return _range; };
  inline int id() const { return _id; };
  inline EQUATION::Type type() const { return _type; }
  inline bool isDerivative() const { return _type == EQUATION::QSSDerivative || _type == EQUATION::ClassicDerivative; }
  inline bool isZeroCrossing() const { return _type == EQUATION::ZeroCrossing; }
  inline bool isOutput() const { return _type == EQUATION::Output; }
  inline bool isAlgebraic() const { return _type == EQUATION::Algebraic; }
  inline bool isJacobian() const { return _type == EQUATION::Jacobian; }
  Option<Util::Variable> LHSVariable() const;
  friend std::ostream &operator<<(std::ostream &out, const Equation &e);
  bool isValid() const;
  bool hasAlgebraics();
  std::string identifier() const;
  bool isRHSReference() const;
  Deps::EquationDependencyMatrix dependencyMatrix() const;
  inline void setUsage(Index usage) { _usage = usage; };
  inline Index usage() const { return _usage; };
  void setType(EQUATION::Type type);
  bool isRecursive() const;
  void applyUsage(Index usage);
  bool isEmpty() const;
  std::string applyId() const;
  Equation genAlgEquation(Equation der_eq, Index rhs_usage, Index lhs_usage);

  protected:
  void initialize(AST_Equation eq);
  void initialize(AST_Expression exp);
  void initialize(AST_Expression lhs, AST_Expression rhs);
  void setup();

  private:
  AST_Equation _eq;
  Expression _lhs;
  Expression _rhs;
  Option<Range> _range;
  bool _autonomous;
  Util::VarSymbolTable _symbols;
  Util::SymbolTable _calledFunctions;
  EQUATION::Type _type;
  int _id;
  int _offset;
  std::string _lhs_exp;
  Index _usage;
};

typedef ModelTable<int, Equation> EquationTable;

class Jacobian {
  public:
  Jacobian() : _symbols(), _jac_terms(){};
  Jacobian(Util::VarSymbolTable &symbols) : _symbols(symbols), _jac_terms(){};
  ~Jacobian() = default;
  Equation generate(Equation eq, Index idx, Deps::AlgebraicPath algs);
  list<Equation> terms();

  private:
  Util::VarSymbolTable _symbols;
  list<Equation> _jac_terms;
};

class Dependency {
  public:
  ~Dependency() = default;
  Equation generate(Equation eq, Index idx, Deps::AlgebraicPath algs);
  list<Equation> terms();
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* EQUATION_H_ */
