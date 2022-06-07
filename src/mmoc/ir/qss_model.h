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

#include <string>
#include <map>

#include <ir/compute_deps.h>
#include <ir/equation.h>
#include <deps/builders/eq_graph_builder.h>
#include <deps/sbg_graph/deps_graph.h>
#include <util/symbol_table.h>
#include <util/table.h>

namespace MicroModelica {
namespace IR {

struct QSSModelDef {
  std::string simple;
  std::string generic;
};


class QSSModelGenerator {
  public:
  QSSModelGenerator();
  ~QSSModelGenerator() = default;

  void init(SB::Deps::SetVertex vertex);
  void end();
  void postProcess(SB::Deps::SetVertex vertex);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp, Expression use_exp, SB::PWLMap def_map,
              SB::Deps::LMapExp def_map_exp, SB::Set intersection);
  void initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge);
  QSSModelDef def();

  void setup(EquationTable eqs);
  EquationTable config() { return EquationTable(); }

  protected:
  QSSModelDef _qss_model_def;
  int _tabs;
  AlgDepsMap _der_deps;
  AlgDepsMap _alg_deps;
  bool _post_process_eval;
  EquationTable _eqs;
};

template<typename GraphBuilder>
class QSSModel {
  public:
  QSSModel();
  ~QSSModel() = default;

  void build(EquationTable eqs);

  std::string simpleDef();

  std::string genericDef();

  protected:
  QSSModelDef _qss_model_def;
};

typedef QSSModel<Deps::SDSBGraphBuilder> QSSModelGen;

typedef QSSModel<Deps::SZSBGraphBuilder> ZCModelGen;

typedef QSSModel<Deps::SOSBGraphBuilder> OutputModelGen;


}  // namespace IR
}  // namespace MicroModelica
