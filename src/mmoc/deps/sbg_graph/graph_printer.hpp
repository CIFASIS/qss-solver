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

using namespace std;

#define SB_MAKE_SPACE \
  for (int __i = 0; __i < _depth; __i++) stri << " ";
#define SB_TAB_SPACE 2
#define SB_INSERT_TAB _depth += SB_TAB_SPACE;
#define SB_DELETE_TAB _depth -= SB_TAB_SPACE;

namespace SB {

template <typename G, typename V, typename E, typename S, typename VIT, typename EIT>
class GraphPrinter {
  public:
  GraphPrinter(const G &g) : _graph(g), _depth(0){};

  void printGraph(std::string name)
  {
    stringstream stri;
    ofstream out(name.c_str());

    stri << "digraph G{" << endl;
    SB_INSERT_TAB
    SB_MAKE_SPACE
    stri << "  ratio=\"fill\"" << endl;
    SB_MAKE_SPACE
    stri << "  node[shape=\"ellipse\"]" << endl;
    SB_INSERT_TAB
    SB_MAKE_SPACE

    // Vertices printing
    printVertices(stri);
    stri << "\n";

    SB_DELETE_TAB
    SB_DELETE_TAB

    SB_INSERT_TAB
    SB_INSERT_TAB
    stringstream colors;

    SB_DELETE_TAB
    stri << colors.str();
    SB_DELETE_TAB

    SB_INSERT_TAB
    SB_INSERT_TAB

    // Edge printing
    printEdges(stri);

    SB_DELETE_TAB
    SB_DELETE_TAB
    stri << "\n";
    stri << "}" << endl;
    out << stri.str();
    out.close();
  };

  protected:
  void printVertices(stringstream &stri)
  {
    VIT vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(_graph); vi != vi_end; ++vi) {
      stri << vPrinter(_graph[*vi]) << " [label=\"" << vLabelPrinter(_graph[*vi]) << "\"]";
      stri << "\n";
      SB_MAKE_SPACE
    }
  };

  void printEdges(stringstream &stri)
  {
    EIT ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(_graph); ei != ei_end; ++ei) {
      S v1 = boost::source(*ei, _graph);
      S v2 = boost::target(*ei, _graph);
      stri << vPrinter(_graph[v1]) << " -> " << vPrinter(_graph[v2]);
      stri << " [label=\"" << ePrinter(_graph[*ei]) << "\", arrowhead=\"none\"]";
      stri << "\n";
      SB_MAKE_SPACE
    }
  };

  std::string vPrinter(SetVertex<V> v)
  {
    std::stringstream label;
    label << v.name() << " " << v.id() << " " << v.index();
    return label.str();
  };
  std::string vLabelPrinter(SetVertex<V> v)
  {
    std::stringstream label;
    SB::Set dom = v.range();
    label << "{ " << v.name() << " Dom: " << dom << " }";
    return label.str();
  };

  std::string ePrinter(SetEdge<E> e)
  {
    std::stringstream label;
    PWLMap mapF = e.mapF();
    PWLMap mapU = e.mapU();
    label << "{ " << e.name() << " mapF: " << mapF << " mapU: " << mapU << " }";
    return label.str();
  };

  private:
  const G &_graph;
  int _depth;
};

}  // namespace SB
