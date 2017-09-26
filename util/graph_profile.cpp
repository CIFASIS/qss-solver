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

#include <stdlib.h>

#include "graph_profile.h"

double
GRP_nodeWeight(GRP_graphProfile g, GRP_EdgeType type)
{
  if(type == GRP_CONT)
  {
    return g->state->contEdgeWeight;
  }
  else if(type == GRP_DSC)
  {
    return g->state->dscEdgeWeight;
  }
  else
  {
    return g->state->virtEdgeWeight;
  }
  return 0;
}

GRP_graphProfile
GRP_GraphProfile()
{
  GRP_graphProfile p = (GRP_graphProfile) malloc(sizeof(*p));
  p->ops = GRP_GraphProfileOps();
  p->state = GRP_GraphProfileState();
  return p;
}

void
GRP_freeGraphProfile(GRP_graphProfile g)
{
  GRP_freeGraphProfileOps(g->ops);
  GRP_freeGraphProfileState(g->state);
  free(g);
}

GRP_graphProfileOps
GRP_GraphProfileOps()
{
  GRP_graphProfileOps p = (GRP_graphProfileOps) malloc(sizeof(*p));
  p->weight = GRP_nodeWeight;
  return p;
}

void
GRP_freeGraphProfileOps(GRP_graphProfileOps ops)
{
  free(ops);
}

GRP_graphProfileState
GRP_GraphProfileState()
{
  GRP_graphProfileState p = (GRP_graphProfileState) malloc(sizeof(*p));
  p->states = 0;
  p->events = 0;
  p->contEdgeWeight = 10;
  p->dscEdgeWeight = 10;
  p->virtEdgeWeight = 1;
  return p;
}

void
GRP_freeGraphProfileState(GRP_graphProfileState state)
{
  free(state);
}

double
GRP_Weight(GRP_graphProfile g, GRP_EdgeType type)
{
  return g->ops->weight(g, type);
}

