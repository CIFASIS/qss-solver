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
typedef enum { ClassicDerivative, QSSDerivative, Algebraic, Dependency, Output, ZeroCrossing, Handler, Jacobian } Type;
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
  Equation(){};
  Equation(AST_Expression lhs, AST_Expression rhs, Util::VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id);
  Equation(AST_Expression eq, Util::VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id, int offset);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, EQUATION::Type type, int id);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, Range r, EQUATION::Type type, int id);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, Option<Range> r, EQUATION::Type type, int id);
  ~Equation();
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
  Option<Util::Variable> LHSVariable();
  friend std::ostream &operator<<(std::ostream &out, const Equation &e);
  bool isValid() const;
  bool hasAlgebraics();
  std::string identifier() const;
  bool isRHSReference() const;
  Deps::EquationDependencyMatrix dependencyMatrix() const;
  inline void setUsage(Index usage) { _usage = usage; };
  inline Index usage() const { return _usage; };
  inline void setType(EQUATION::Type type) { _type = type; };

  private:
  void initialize(AST_Equation eq);
  void initialize(AST_Expression exp);
  void initialize(AST_Expression lhs, AST_Expression rhs);
  void setup();
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
  Jacobian(Util::VarSymbolTable &symbols) : _symbols(symbols){};
  ~Jacobian() = default;
  Equation generate(Equation eq, Index idx);

  private:
  Util::VarSymbolTable _symbols;
};

class Dependency {
  public:
  Dependency(){};
  ~Dependency() = default;
  Equation generate(Equation eq, Index idx);
};

class EquationPrinter {
  public:
  EquationPrinter(Equation eq, Util::VarSymbolTable symbols);
  ~EquationPrinter() = default;
  virtual std::string print() const { return ""; };
  virtual std::string macro() const { return ""; };
  std::string identifier() const { return _identifier; };
  std::string prefix() const;
  std::string lhs(int order = 0) const;
  virtual std::string equationId() const;

  protected:
  void setup();

  private:
  Equation _eq;
  Util::VarSymbolTable _symbols;
  std::string _identifier;
};

class JacobianConfig : public EquationPrinter {
  public:
  JacobianConfig(Equation eq, Util::VarSymbolTable symbols) : EquationPrinter(eq, symbols), _eq(eq){};
  ~JacobianConfig() = default;
  std::string print() const override;

  private:
  Equation _eq;
};

class EquationConfig : public EquationPrinter {
  public:
  EquationConfig(Equation eq, Util::VarSymbolTable symbols);
  ~EquationConfig() = default;
  std::string print() const override;
  std::string macro() const override;
  inline void factorialInit(int fact_init) { _fact_init = fact_init; };

  protected:
  void initializeDerivatives();
  std::string generateDerivatives(std::string tabs, int init = 1) const;

  private:
  Equation _eq;
  Util::VarSymbolTable _symbols;
  Expression _derivatives[3];
  int _fact_init;
};

class ClassicConfig : public EquationConfig {
  public:
  ClassicConfig(Equation eq, Util::VarSymbolTable symbols) : EquationConfig(eq, symbols), _eq(eq){};
  ~ClassicConfig() = default;
  std::string print() const override;

  private:
  Equation _eq;
};

class OutputConfig : public EquationConfig {
  public:
  OutputConfig(Equation eq, Util::VarSymbolTable symbols) : EquationConfig(eq, symbols), _eq(eq){};
  ~OutputConfig() = default;
  std::string print() const override;
  std::string equationId() const override;

  private:
  Equation _eq;
};

class AlgebraicConfig : public EquationConfig {
  public:
  AlgebraicConfig(Equation eq, Util::VarSymbolTable symbols) : EquationConfig(eq, symbols), _eq(eq){};
  ~AlgebraicConfig() = default;
  std::string print() const override;

  private:
  Equation _eq;
  Util::VarSymbolTable _symbols;
};

class DependencyConfig : public EquationConfig {
  public:
  DependencyConfig(Equation eq, Util::VarSymbolTable symbols) : EquationConfig(eq, symbols), _eq(eq){};
  ~DependencyConfig() = default;
  std::string print() const override;

  private:
  Equation _eq;
  Util::VarSymbolTable _symbols;
};

class ZeroCrossingConfig : public EquationConfig {
  public:
  ZeroCrossingConfig(Equation eq, Util::VarSymbolTable symbols) : EquationConfig(eq, symbols), _eq(eq) { factorialInit(1); };
  ~ZeroCrossingConfig() = default;
  std::string equationId() const override;

  private:
  Equation _eq;
  Util::VarSymbolTable _symbols;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* EQUATION_H_ */
