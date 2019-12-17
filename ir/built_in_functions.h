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

#ifndef MMO_BUILT_IN_FUNCTIONS_H
#define MMO_BUILT_IN_FUNCTIONS_H

#include "helpers.h"
#include "../util/table.h"

namespace MicroModelica {
namespace IR {

class BuiltInFunctionPrinter {
  public:
  BuiltInFunctionPrinter(){};
  ~BuiltInFunctionPrinter(){};
  friend std::ostream& operator<<(std::ostream& out, const BuiltInFunctionPrinter& b);
  virtual std::string print() const { return ""; };
  virtual std::string code() { return ""; };
};

class MaxFunction : public BuiltInFunctionPrinter {
  public:
  MaxFunction(){};
  ~MaxFunction(){};
  std::string print() const;
  std::string code();
};

class MinFunction : public BuiltInFunctionPrinter {
  public:
  MinFunction(){};
  ~MinFunction(){};
  std::string print() const;
  std::string code();
};

class SumFunction : public BuiltInFunctionPrinter {
  public:
  SumFunction(){};
  ~SumFunction(){};
  std::string print() const;
  std::string code();
};

class ProductFunction : public BuiltInFunctionPrinter {
  public:
  ProductFunction(){};
  ~ProductFunction(){};
  std::string print() const;
  std::string code();
};

class InnerProductFunction : public BuiltInFunctionPrinter {
  public:
  InnerProductFunction(){};
  ~InnerProductFunction(){};
  std::string print() const;
  std::string code();
};

typedef ModelTable<std::string, BuiltInFunctionPrinter> BuiltInFunctionPrinterTable;

class BuiltInFunction {
  public:
  static BuiltInFunction& instance()
  {
    static BuiltInFunction _instance;
    return _instance;
  }
  ~BuiltInFunction(){};
  inline CompiledFunctionTable functions() { return _functions; };
  inline Option<BuiltInFunctionPrinter> reductionFunctions(string name) { return _reduction[name]; };
  inline bool isValid(std::string func) { return _functions[func].is_initialized(); };

  private:
  BuiltInFunction();
  CompiledFunctionTable _functions;
  BuiltInFunctionPrinterTable _reduction;
};
}  // namespace IR
}  // namespace MicroModelica

#endif /* MMO_BUILT_IN_FUNCTIONS_H */
