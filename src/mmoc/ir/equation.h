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
#include <util/table.h>
#include <ir/index.h>

namespace MicroModelica {
namespace Util {
typedef ModelTable<std::string, std::string> SymbolTable;
}

namespace IR {
namespace EQUATION {
typedef enum { ClassicDerivative, QSSDerivative, Algebraic, Dependency, Output, ZeroCrossing, QSSBDFDerivative } Type;
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
  Equation(AST_Expression lhs, AST_Expression rhs, Option<Range> range, EQUATION::Type type, int id);
  Equation(AST_Expression eq, Option<Range> range, EQUATION::Type type, int id, int offset);
  Equation(AST_Equation eq, EQUATION::Type type, int id);
  Equation(AST_Equation eq, Range r, EQUATION::Type type, int id);
  Equation(AST_Equation eq, Option<Range> r, EQUATION::Type type, int id);
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
  inline void setRange(Option<Range> range) { _range = range; };
  inline int id() const { return _id; };
  inline EQUATION::Type type() const { return _type; }
  inline bool isDerivative() const { return _type == EQUATION::QSSDerivative || _type == EQUATION::ClassicDerivative || _type == EQUATION::QSSBDFDerivative; }
  inline bool isZeroCrossing() const { return _type == EQUATION::ZeroCrossing; }
  inline bool isOutput() const { return _type == EQUATION::Output; }
  inline bool isAlgebraic() const { return _type == EQUATION::Algebraic; }
  Option<Util::Variable> LHSVariable() const;
  friend std::ostream &operator<<(std::ostream &out, const Equation &e);
  bool isValid() const;
  bool hasAlgebraics();
  std::string identifier() const;
  bool isRHSReference() const;
  inline void setUsage(Index usage) { _usage = usage; };
  inline Index usage() const { return _usage; };
  void setType(EQUATION::Type type);
  bool isRecursive() const;
  void applyUsage(Index usage);
  bool isEmpty() const;
  std::string applyId() const;
  Equation genAlgEquation(Equation der_eq, Index rhs_usage, Index lhs_usage);
  int arrayId() const;
  void setAlgCode(std::string alg_code);
  std::string algCode() const;

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
  Util::SymbolTable _calledFunctions;
  EQUATION::Type _type;
  int _id;
  int _offset;
  std::string _lhs_exp;
  Index _usage;
  std::string _alg_code;
};

class EquationDefOrder {
  public:
  EquationDefOrder() : _var_name(), _var_init() {};
  EquationDefOrder(std::string var_name, std::vector<int> var_init) : _var_name(var_name), _var_init(var_init) {};

  bool operator<(const EquationDefOrder& other) const
  {
    int var_compare = _var_name.compare(other._var_name); 
    if (var_compare == 0) {
      assert(_var_init.size() == other._var_init.size());
      size_t size = _var_init.size();
      for (size_t i = 0; i < size; i++) {
        if (_var_init[i] < other._var_init[i]) {
          return true;
        }
      }
      return false;
    }
    return var_compare < 0;
  }

  std::string variable() const { return _var_name; }
  
  protected:
  std::string _var_name;
  std::vector<int> _var_init;
};

typedef std::map<int, std::list<Equation>> OrderedEquations; 

typedef std::map<EquationDefOrder, Equation> EquationOrderMap; 

typedef ModelTable<int, Equation> EquationTable;

}  // namespace IR
}  // namespace MicroModelica
