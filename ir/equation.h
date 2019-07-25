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
typedef enum {
  ClassicDerivative,
  QSSDerivative,
  Algebraic,
  Dependency,
  Output,
  ZeroCrossing,
  Handler,
  Jacobian
} Type;
}

// class EquationPrinter;

class Equation {
public:
  Equation(){};
  Equation(AST_Expression eq, Util::VarSymbolTable &symbols,
           EQUATION::Type type, int id, int offset);
  Equation(AST_Expression eq, Util::VarSymbolTable &symbols,
           Option<Range> range, EQUATION::Type type, int id, int offset);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, EQUATION::Type type,
           int id);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, Range r,
           EQUATION::Type type, int id);
  Equation(AST_Equation eq, Util::VarSymbolTable &symbols, Option<Range> r,
           EQUATION::Type type, int id);
  /**
   *
   */
  ~Equation(){};
  inline bool hasRange() { return _range.is_initialized(); };
  inline Expression lhs() { return _lhs; };
  inline Expression rhs() { return _rhs; };
  inline AST_Expression equation() { return _rhs.expression(); };
  inline bool autonomous() { return _autonomous; };
  inline Util::SymbolTable calledFunctions() { return _calledFunctions; };
  std::string print() const;
  std::string macro() const;
  inline Option<Range> range() { return _range; };
  inline int id() { return _id; };
  static std::string functionId(EQUATION::Type type);
  inline EQUATION::Type type() { return _type; }
  inline bool isDerivative() {
    return _type == EQUATION::QSSDerivative ||
           _type == EQUATION::ClassicDerivative;
  }
  inline bool isZeroCrossing() { return _type == EQUATION::ZeroCrossing; }
  inline bool isOutput() { return _type == EQUATION::Output; }
  inline bool isAlgebraic() { return _type == EQUATION::Algebraic; }
  Option<Util::Variable> LHSVariable();
  friend std::ostream &operator<<(std::ostream &out, const Equation &e);
  bool isValid() const;
  bool hasAlgebraics();
  std::string functionId() const;
  bool isRHSReference() const;

private:
  void process(AST_Equation eq);
  void process(AST_Expression exp);
  Deps::EquationDependencyMatrix dependencyMatrix() const;
  std::string prefix() const;
  std::string lhsStr() const;
  void initializeDerivatives();
  std::string generateDerivatives() const;
  // EquationPrinter getPrinter() const;
  AST_Equation _eq;
  Expression _lhs;
  Expression _rhs;
  Expression _derivatives[3];
  Option<Range> _range;
  bool _autonomous;
  Util::VarSymbolTable _symbols;
  Util::SymbolTable _calledFunctions;
  EQUATION::Type _type;
  int _id;
  int _offset;
};

typedef ModelTable<int, Equation> EquationTable;

/*class EquationPrinter
{
public:
    EquationPrinter(Equation eq) : _eq(eq) {};
    ~EquationPrinter() = default;
    virtual std::string print() const { return ""; };
    std::string prefix() const;
    std::string lhs() const;
private:
    Equation _eq;
};

class ClassicConfig : public EquationPrinter
{
public:
    ClassicConfig(Equation eq) : EquationPrinter(eq), _eq(eq) {};
    ~ClassicConfig() = default;
    std::string print() const;

private:
    Equation _eq;
};

class OutputConfig : public EquationPrinter
{
public:
    OutputConfig(Equation eq) : EquationPrinter(eq), _eq(eq) {};
    ~OutputConfig() = default;
    std::string print() const;

private:
    Equation _eq;
};

class EquationConfig : public EquationPrinter
{
public:
    EquationConfig(Equation eq, const Util::VarSymbolTable& symbols) :
EquationPrinter(eq), _eq(eq), _symbols(symbols) {};
    ~EquationConfig() = default;
    std::string print() const;

protected:
  void initializeDerivatives();
  std::string generateDerivatives() const;

private:
  Equation _eq;
  Util::VarSymbolTable _symbols;
  Expression _derivatives[3];
};*/

} // namespace IR
} // namespace MicroModelica

#endif /* EQUATION_H_ */
