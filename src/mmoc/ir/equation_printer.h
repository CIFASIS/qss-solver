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

#include <ast/ast_types.h>
#include <ir/helpers.h>
#include <ir/index.h>
#include <util/table.h>

namespace MicroModelica {
namespace Util {
typedef ModelTable<std::string, std::string> SymbolTable;
}

namespace IR {

class Equation;

class EquationPrinter {
  public:
  EquationPrinter(Equation eq);
  virtual ~EquationPrinter() = default;
  virtual std::string print() const { return ""; };
  virtual std::string macro() const { return ""; };
  std::string identifier() const { return _identifier; };
  std::string algCode() const { return _alg_code; }
  std::string prefix() const;
  std::string lhs(int order = 0) const;
  virtual std::string equationId() const;
  FUNCTION_PRINTER::ReturnStatementType returnStm() const;

  protected:
  void setup(Equation eq);

  private:
  std::string _identifier;
  int _id;
  int _type;
  Expression _lhs;
  std::string _alg_code;
  FUNCTION_PRINTER::ReturnStatementType _return_stm;
};

EquationPrinter* getPrinter(Equation eq);

class DerivativePrinter : public EquationPrinter {
  public:
  DerivativePrinter(Equation eq);
  ~DerivativePrinter() = default;
  std::string print() const override;
  std::string macro() const override;
  inline void factorialInit(int fact_init) { _fact_init = fact_init; };

  protected:
  void initializeDerivatives();
  std::string generateDerivatives(std::string tabs, int init = 1) const;

  private:
  Expression _derivatives[3];
  int _fact_init;
  int _id;
  Option<Range> _range;
  Expression _lhs;
  Expression _rhs;
};

class ClassicPrinter : public DerivativePrinter {
  public:
  ClassicPrinter(Equation eq);
  ~ClassicPrinter() = default;
  std::string print() const override;

  private:
  Option<Range> _range;
  Expression _rhs;
};

class OutputPrinter : public DerivativePrinter {
  public:
  OutputPrinter(Equation eq);
  ~OutputPrinter() = default;

  std::string print() const override;
  std::string equationId() const override;

  private:
  int _id;
  Option<Range> _range;
  Expression _rhs;
};

class AlgebraicPrinter : public DerivativePrinter {
  public:
  AlgebraicPrinter(Equation eq);
  ~AlgebraicPrinter() = default;
  std::string print() const override;
  std::string equationId() const override;

  private:
  Option<Range> _range;
  Expression _rhs;
  Expression _lhs;
  int _id;
};

class DependencyPrinter : public DerivativePrinter {
  public:
  DependencyPrinter(Equation eq);
  ~DependencyPrinter() = default;
  std::string print() const override;

  protected:
  std::string beginParallelMap(std::string &tabs) const;
  std::string endParallelMap() const;

  private:
  Index _usage;
  Option<Range> _range;
  Expression _rhs;
  Index _var_idx;
  bool _parallel;
};

class ZeroCrossingPrinter : public DerivativePrinter {
  public:
  ZeroCrossingPrinter(Equation eq);
  ~ZeroCrossingPrinter() = default;
  std::string equationId() const override;

  private:
  int _id;
};

}  // namespace IR
}  // namespace MicroModelica
