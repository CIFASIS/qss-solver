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

#ifndef MMO_UTIL_GRAPH_H_
#define MMO_UTIL_GRAPH_H_

#include <map>
#include <set>

#include "graph_profile.h"

using namespace std;

class Graph
{
  public:
    /**
     *
     * @param states
     * @param events
     */
    Graph(int states, int events);
    /**
     *
     */
    ~Graph();
    /**
     *
     * @return
     */
    map<int, set<int> >
    graph();
    /**
     *
     * @return
     */
    map<int, set<int> >
    hyperGraph();
    /**
     *
     * @param orig
     * @param dest
     */
    void
    addGraphEdge(int orig, int dest);
    /**
     *
     * @param orig
     * @param dest
     */
    void
    addHyperGraphEdge(int orig, int dest);
    /**
     *
     * @param node
     * @param edge
     * @return
     */
    int
    graphEdgeWeight(int node, int inf);
    /**
     *
     * @param node
     * @param edge
     * @return
     */
    int
    hyperGraphEdgeWeight(int node);
    /**
     *
     */
    void
    connectGraphs();
    /**
     *
     * @return
     */
    int
    graphEdges();
    /**
     *
     * @param node
     * @return
     */
    int
    graphNodeEdges(int node);
    /**
     *
     * @return
     */
    bool
    empty();
    /**
     *
     * @return
     */
    int
    hyperGraphEdges();
    /**
     *
     */
    int
    nodeWeight(int node);
    /**
     *
     */
    void
    addNodeWeight(int node, int weight);
    private:
    int
    edgeWeight(int node);
    int _states;
    int _events;
    int _nvtxs;
    int _graphEdges;
    map<int, set<int> > _graph;
    map<int, set<int> > _graphInputs;
    map<int, int> _graphDiscretes;
    map<int, set<int> > _hyperGraph;
    map<int, set<int> > _wmap;
    map<int, set<int> > _hwmap;
    GRP_graphProfile _profile;
};

#endif /* MMO_UTIL_GRAPH_H_ */
