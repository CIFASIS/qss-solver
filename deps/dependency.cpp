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

#include "dependency.h"

#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

#include "../ir/equation.h"
#include "../util/symbol_table.h"

using namespace std;

namespace MicroModelica {
using namespace Util;
using namespace IR;
namespace Deps {

void Dependency::print(DepsGraph graph)
{
  for (Vertex vertex : boost::make_iterator_range(vertices(graph))) {
    VertexProperty vertex_info = graph[vertex];
    if (vertex_info.type() == VERTEX::Influencer) {
      Variable var = vertex_info.var();
      cout << "Print edges for: " << var << " Source range: " << variableRange(var) << endl;
      printEdges(graph, vertex, variableRange(vertex_info.var()));
    }
  }
}

void Dependency::printEdges(DepsGraph graph, Vertex source_vertex, MDI source_range)
{
  VertexProperty source_vertex_info = graph[source_vertex];
  boost::graph_traits<DepsGraph>::out_edge_iterator edge, out_edge_end;
  stringstream buffer;
  for (boost::tie(edge, out_edge_end) = out_edges(source_vertex, graph); edge != out_edge_end; ++edge) {
    Label lbl = graph[*edge];
    MDI dom = lbl.Pair().Dom();
    MDI ran = source_range;
    buffer << "Processing new output edge" << endl;
    Option<MDI> intersect = source_range.Intersection(dom);
    if (intersect) {
      buffer << "Intersection found for " << source_range << " and " << dom;
      MDI intersection = boost::get<MDI>(intersect);
      ran = lbl.getImage(intersection);
      buffer << " IMAGE " << ran;
    } else {
      buffer << "NO Intersection found for " << source_range << " and " << dom;
    }
    cout << buffer.str() << endl;
    buffer.str("");
    auto target_vertex = boost::target(*edge, graph);
    if (source_vertex_info.type() == VERTEX::Influencer) {
      // Store the influencer index pair.
      buffer.str("");
      buffer << "Source " << source_vertex_info.var().name();
      buffer << " Exp: " << lbl.Pair().exp();
      buffer << " Dom: " << lbl.Pair().Dom();
      buffer << " Ran: " << lbl.Pair().Ran() << " -----> ";
    }
    VertexProperty target_vertex_info = graph[target_vertex];
    if (target_vertex_info.type() == VERTEX::Influencee) {
      assert(source_vertex_info.type() == VERTEX::Equation || source_vertex_info.type() == VERTEX::Statement);
      int id;
      if (source_vertex_info.type() == VERTEX::Equation) {
        id = source_vertex_info.eq().id();
      } else {
        // @TODO Set statement id.
        id = 0;
      }
      buffer << " Equation " << id << " to variable " << target_vertex_info.var().name() << endl;
      cout << buffer.str();
    } else if (target_vertex_info.type() == VERTEX::Algebraic) {
      assert(source_vertex_info.type() == VERTEX::Equation);
      int id = source_vertex_info.eq().id();
      buffer << " Algebraic Equation " << id << " to variable " << target_vertex_info.var().name();
      cout << buffer.str();
      printEdges(graph, target_vertex, ran);
    } else {
      buffer << " Following equation ";
      cout << buffer.str();
      printEdges(graph, target_vertex, ran);
    }
  }
}

MDI Dependency::variableRange(Variable var)
{
  IntervalList intervals;

  if (var.isArray()) {
    int i, d = var.dimensions();
    for (i = 0; i < d; i++) {
      intervals.push_back(Interval(1, var.size(i)));
    }
  }
  return MDI(intervals);
}

VariableDependency Dependency::getVariableDependency(string name, MDI dom, MDI ran, int id)
{
  VariableDependency var_dep;
  var_dep.setVariable(name);
  var_dep.setDom(dom);
  var_dep.setRan(ran);
  var_dep.setEquationId(id);
  return var_dep;
}

bool Dependency::isRecursive(VertexProperty source, VertexProperty target)
{
  if (source.type() == VERTEX::Algebraic && target.type() == VERTEX::Equation && target.eq().type() == IR::EQUATION::Algebraic) {
    // Check if the LHS of the equation is equal to
    // the source variable.
    Equation eq = target.eq();
    Option<Variable> target_variable = eq.LHSVariable();
    assert(target_variable);
    if (source.var().name().compare(target_variable->name()) == 0 && eq.isRecursive()) {
      return true;
    }
  }
  return false;
}

void Dependency::paths(DepsGraph graph, Vertex source_vertex, MDI source_range, VariableDependencies& var_deps, AlgebraicDependencies& algs)
{
  VertexProperty source_vertex_info = graph[source_vertex];
  boost::graph_traits<DepsGraph>::out_edge_iterator edge, out_edge_end;
  for (boost::tie(edge, out_edge_end) = out_edges(source_vertex, graph); edge != out_edge_end; ++edge) {
    Label lbl = graph[*edge];
    MDI dom = lbl.Pair().Dom();
    Option<MDI> intersect = source_range.Intersection(dom);
    if (intersect) {
      MDI intersection = intersect.get();
      auto target_vertex = boost::target(*edge, graph);
      if (source_vertex_info.type() == VERTEX::Influencer) {
        // Store the influencer index pair.
        _ifr = lbl.Pair();
        _ifr_dom = intersection;
      }
      // First look if the target node is terminal.
      VertexProperty target_vertex_info = graph[target_vertex];
      MDI ran = lbl.getImage(intersection);
      if (target_vertex_info.type() == VERTEX::Influencee) {
        assert(source_vertex_info.type() == VERTEX::Equation || source_vertex_info.type() == VERTEX::Statement);
        int id = 0;
        if (source_vertex_info.type() == VERTEX::Equation) {
          id = source_vertex_info.eq().id();
        } else {
          // @TODO Set statement id.
          id = 0;
        }
        VariableDependency var_dep = getVariableDependency(target_vertex_info.var().name(), _ifr_dom, ran, id);
        var_dep.setIfr(_ifr);
        var_dep.setIfe(lbl.Pair());
        var_dep.setRange();
        cout << "dependency from: " << _ifr.exp() << " to " << target_vertex_info.var().name() << endl;
        Influences inf = {algs, var_dep};
        var_deps.push_back(inf);
      } else if (isRecursive(source_vertex_info, target_vertex_info)) {
        int id = target_vertex_info.eq().id();
        VariableDependency var_dep = getVariableDependency(source_vertex_info.var().name(), intersection, lbl.Pair().Ran(), id);
        algs.push_back(var_dep);
      } else if (target_vertex_info.type() == VERTEX::Algebraic) {
        assert(source_vertex_info.type() == VERTEX::Equation);
        if (source_vertex_info.eq().type() == IR::EQUATION::Algebraic) {
          int id = source_vertex_info.eq().id();
          VariableDependency var_dep = getVariableDependency(target_vertex_info.var().name(), intersection, ran, id);
          algs.push_back(var_dep);
        }
        paths(graph, target_vertex, ran, var_deps, algs);
      } else {
        paths(graph, target_vertex, ran, var_deps, algs);
      }
    }
  }
}

void Dependency::append(VariableDependencyMatrix& a, VariableDependencyMatrix& b)
{
  VariableDependencyMatrix::const_iterator source_it;
  for (source_it = b.begin(); source_it != b.end(); source_it++) {
    a.insert(source_it->first, source_it->second);
  }
}

void Dependency::merge(VariableDependencyMatrix& source, VariableDependencyMatrix& target, VariableDependencyMatrix& merge)
{
  VariableDependencyMatrix::const_iterator source_it;
  for (source_it = source.begin(); source_it != source.end(); source_it++) {
    // First look if the source variable affects some equation in the target matrix.
    Option<VariableDependencies> has_target_deps = target[source_it->first];
    if (!has_target_deps) {
      continue;
    }
    VariableDependencies target_var_deps = has_target_deps.get();
    VariableDependencies source_var_deps = source_it->second;
    for (auto source_var_dep : source_var_deps) {
      for (auto target_var_dep : target_var_deps) {
        // Intersect the merging variable
        MDI target_dom = target_var_dep.ifce.dom();
        Option<MDI> intersect = source_var_dep.ifce.dom().Intersection(target_dom);
        if (intersect) {
          // We found a match and must adjust the ranges and generate the new var_dep.
          MDI intersection = intersect.get();
          VariableDependency target = target_var_dep.ifce;
          VariableDependency source = source_var_dep.ifce;
          VariableDependency var_dep =
              getVariableDependency(source.variable(), source.getImage(intersection), target.getImage(intersection), target.equationId());
          var_dep.setIfr(source.ifePair());
          var_dep.setIfe(target.ifePair());
          var_dep.setRange();

          // Finally insert the new variable in the merge matrix.
          Option<VariableDependencies> has_merge_deps = merge[source.variable()];
          VariableDependencies new_merge_deps;
          if (has_merge_deps) {
            new_merge_deps = has_merge_deps.get();
          }
          Influences inf = {AlgebraicDependencies(), var_dep};
          new_merge_deps.push_back(inf);
          merge.insert(source.variable(), new_merge_deps);
        }
      }
    }
  }
}
}  // namespace Deps
}  // namespace MicroModelica