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

#include "alg_usage.h"
#include "index.h"
#include "equation.h"
#include "expression.h"
#include "helpers.h"
#include "../deps/dependency_matrix.h"
#include "../util/error.h"
#include "../util/model_config.h"
#include "../util/symbol_table.h"
#include "../util/util.h"

namespace MicroModelica {
using namespace Deps;
using namespace Util;
namespace IR {

template <class EqGen>
class EquationMapper {
  public:
  EquationMapper() : _dict(), _mapper(), _scalar_alg_dict(), _vector_alg_dict(), _eq_gen(), _id(0) {}

  ~EquationMapper() = default;

  void compute(EqGen eq_gen, Deps::Paths deps)
  {
    _eq_gen = eq_gen;
    for (Deps::Path inf : deps) {
      processPath(inf);
    }
  }

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
  std::string traversePath(Equation eq, Equation der_eq, Index ife, AlgebraicPath path)
  {
    stringstream code;
    code << nonStateAlgebraics(eq, der_eq, ife);
    for (VariableDependency d : path) {
      code << insert(d, der_eq, ife);
    }
    code << eq << endl;
    return code.str();
  }

  void processPath(Deps::Path inf)
  {
    EquationTable derivatives = ModelConfig::instance().derivatives();
    stringstream buffer;
    Option<Equation> ifce = derivatives[inf.ifce.equationId()];
    if (ifce) {
      Equation ife_eq = ifce.get();
      Index ife = inf.ife();
      string idx_exp = ife.print();
      if (_dict.find(idx_exp) == _dict.end()) {
        DepInfo d = DepInfo(ife);
        _mapper[_id] = d;
        _dict[idx_exp] = _id++;
      }
      Equation eq = _eq_gen.generate(ife_eq, ife, inf.algs);
      buffer << traversePath(eq, ife_eq, ife, inf.algs);
      int id = _dict[idx_exp];
      DepInfo dep = _mapper[id];
      dep.addDependency(buffer.str());
      dep.addEquation(eq);
      _mapper[id] = dep;
    }
  }

  bool updateDict(string global_key, string local_key, map<string, map<string, string>>& dict)
  {
    map<string, string> local_alg_dict = dict[global_key];
    if (local_alg_dict.find(local_key) == local_alg_dict.end()) {
      local_alg_dict[local_key] = local_key;
      dict[global_key] = local_alg_dict;
      return true;
    }
    return false;
  }

  Equation getAlgEquation(int id)
  {
    EquationTable algebraic_eqs = ModelConfig::instance().algebraics();
    Option<Equation> alg = algebraic_eqs[id];
    if (alg) {
      return alg.get();
    } else {
      Error::instance().add(0, EM_CG | EM_NO_EQ, ER_Error, "Algebraic equation not found.");
    }
    return Equation();
  }

  std::string insert(VariableDependency alg, Equation der_eq, Index ife)
  {
    FunctionPrinter printer;
    stringstream code;
    Equation orig_alg = getAlgEquation(alg.equationId());
    Equation alg_eq = orig_alg.genAlgEquation(der_eq, alg.ife(), ife);
    if (alg.isReduction()) {
      alg_eq = orig_alg;
    }
    Index alg_idx(alg_eq.lhs());
    string local_key = alg_idx.print();
    if (ife.isConstant()) {
      string global_key = ife.print();
      if (updateDict(global_key, local_key, _scalar_alg_dict)) {
        code << printer.algebraic(alg_eq, alg.isReduction());
      }
    } else {
      string global_key = ife.variable().name();
      if (updateDict(global_key, local_key, _vector_alg_dict)) {
        Equation orig_equation = getAlgEquation(alg.equationId());
        code << printer.printAlgebraicGuards(orig_equation, alg_idx);
        code << printer.algebraic(orig_equation, alg.isReduction());
        code << printer.endDimGuards(orig_equation.range());
      }
    }
    return code.str();
  }

  std::string nonStateAlgebraics(Equation ifr, Equation der_eq, Index ife)
  {
    stringstream code;
    EquationDependencyMatrix eqdm = ifr.dependencyMatrix();
    Option<Paths> paths = eqdm[ifr.id()];
    if (paths) {
      Paths::iterator eq_it;
      for (eq_it = paths->begin(); eq_it != paths->end(); eq_it++) {
        AlgebraicPath algs = eq_it->algs;
        AlgebraicPath::reverse_iterator dep_it;
        for (dep_it = algs.rbegin(); dep_it != algs.rend(); dep_it++) {
          code << insert(*dep_it, der_eq, ife);
        }
      }
    }
    return code.str();
  }

  std::string dependencies(bool scalar) const
  {
    stringstream code, buffer, begin_exp, end_exp;
    static constexpr bool BEGIN = true;
    static constexpr bool END = false;
    for (auto mapper : _mapper) {
      DepInfo dep = mapper.second;
      if (dep.isScalar() == scalar) {
          begin_exp << generateGuards(dep, BEGIN);
          end_exp << generateGuards(dep, END);
        for (string eq : dep.deps()) {
          buffer << eq;
        }
          code << generate(begin_exp, buffer, end_exp, scalar);
      }
    }
    return code.str();
  }

  std::string generate(std::stringstream& begin, std::stringstream& code, std::stringstream& end, bool scalar) const
  {
    stringstream buffer;
    buffer << begin.str();
    buffer << code.str();
    buffer << end.str();
    if (!scalar) {
      buffer << endl;
    }
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
      if (dep.isScalar()) {
        buffer << TAB;
      } 
      buffer << TAB << fp.endExpression(range, FUNCTION_PRINTER::Break);
      if (dep.isScalar()) {
        buffer << endl;
      }
    }
    return buffer.str();
  }

  private:
  map<std::string, int> _dict;
  map<int, DepInfo> _mapper;
  map<std::string, map<std::string, std::string>> _scalar_alg_dict;
  map<std::string, map<std::string, std::string>> _vector_alg_dict;
  EqGen _eq_gen;
  int _id;
};
}  // namespace IR
}  // namespace MicroModelica

#endif /* MMO_EQUATION_MAPPER_H */
