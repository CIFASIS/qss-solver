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

#ifndef MMO_EQUATION_MAPPER_H
#define MMO_EQUATION_MAPPER_H

#include <string>

#include "index.h"
#include "equation.h"
#include "expression.h"
#include "helpers.h"
#include "../deps/dependency_matrix.h"
#include "../util/symbol_table.h"
#include "../util/util.h"

namespace MicroModelica {
using namespace Deps;
namespace IR {

template <class EqGen>
class EquationMapper {
  public:
  EquationMapper(EqGen eq_gen, Util::Variable var, Deps::VariableDependencies deps) : _dict(), _mapper(), _eq_gen(eq_gen), _id(0)
  {
    initialize(deps);
  }
  ~EquationMapper() = default;

  std::string scalar() const
  {
    static bool constexpr SCALAR = true;
    return dependencies(SCALAR);
  }

  std::string vector() const
  {
    static bool constexpr VECTOR = false;
    return dependencies(VECTOR);
  }

  protected:
  void initialize(Deps::VariableDependencies deps)
  {
    for (Deps::Influences inf : deps) {
      processInf(inf);
    }
  }

  void processInf(Deps::Influences inf)
  {
    EquationTable derivatives = ModelConfig::instance().derivatives();
    stringstream buffer;
    FunctionPrinter fp;
    Option<Equation> ifce = derivatives[inf.ifce.equationId()];
    if (ifce) {
      Equation orig_eq = ifce.get();
      Index ifr = inf.ifr();
      string idx_exp = ifr.print();
      if (_dict.find(idx_exp) == _dict.end()) {
        DepInfo d = DepInfo(ifr);
        _mapper[_id] = d;
        _dict[idx_exp] = _id++;
      }
      Equation eq = _eq_gen.generate(ifce.get(), ifr, inf.algs);
      if (eq.isJacobian()) {
        buffer << fp.jacobianTerms(_eq_gen.terms());
      } else {
        buffer << nonStateAlgebraics(orig_eq, inf.algs);
        buffer << fp.algebraics(inf.algs);
      }
      buffer << eq << endl;
      int id = _dict[idx_exp];
      DepInfo dep = _mapper[id];
      dep.addDependency(buffer.str());
      _mapper[id] = dep;
    }
  }

  std::string nonStateAlgebraics(Equation eq, AlgebraicDependencies algs)
  {
    stringstream code;
    EquationDependencyMatrix eqdm = eq.dependencyMatrix();
    Option<VariableDependencies> eqd = eqdm[eq.id()];
    if (eqd) {
      VariableDependencies::iterator eq_it;
      for (eq_it = eqd->begin(); eq_it != eqd->end(); eq_it++) {
        bool found = false;
        int alg_id = eq_it->ifce.equationId();
        for (VariableDependency alg : algs) {
          if (alg_id == alg.equationId()) {
            found = true;
            break;
          }
        }
        if (!found) {
          FunctionPrinter fp;
          code << fp.algebraic(alg_id, eq_it->ifce.isReduction());
          code << fp.algebraics(eq_it->algs);
        }
      }
    }
    return code.str();
  }

  std::string dependencies(bool scalar) const
  {
    stringstream code, buffer, begin_exp, end_exp;
    bool generate_guards = true;
    static constexpr bool BEGIN = true;
    static constexpr bool END = false;
    for (auto mapper : _mapper) {
      DepInfo dep = mapper.second;
      if (dep.isScalar() == scalar) {
        if (generate_guards) {
          begin_exp << generateGuards(dep, BEGIN);
          end_exp << generateGuards(dep, END);
        }
        for (string eq : dep.deps()) {
          buffer << eq;
        }
        if (scalar) {
          code << generate(begin_exp, buffer, end_exp);
        }
        if (!scalar && generate_guards) {
          generate_guards = false;
        }
      }
    }
    if (!scalar) {
      code << generate(begin_exp, buffer, end_exp);
    }
    return code.str();
  }

  std::string generate(std::stringstream& begin, std::stringstream& code, std::stringstream& end) const
  {
    stringstream buffer;
    buffer << begin.str();
    buffer << code.str();
    buffer << end.str();
    begin.str("");
    code.str("");
    end.str("");
    return buffer.str();
  }

  std::string generateGuards(DepInfo dep, bool begin) const
  {
    stringstream buffer;
    FunctionPrinter fp;
    Index index = dep.index();
    Option<Range> range;
    if (!dep.isScalar()) {
      range = index.range();
    }
    if (begin) {
      buffer << fp.beginExpression(index.identifier(), range);
    } else {
      buffer << TAB << fp.endExpression(range, FUNCTION_PRINTER::Break);
    }
    return buffer.str();
  }

  private:
  map<std::string, int> _dict;
  map<int, DepInfo> _mapper;
  EqGen _eq_gen;
  int _id;
};
}  // namespace IR
}  // namespace MicroModelica

#endif /* MMO_EQUATION_MAPPER_H */
