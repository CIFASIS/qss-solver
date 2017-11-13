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

#ifndef GRAPH_GRAPH_PROFILE_H_
#define GRAPH_GRAPH_PROFILE_H_

typedef enum
{
  GRP_CONT, 
  GRP_DSC, 
  GRP_VIRT
} GRP_EdgeType;

/**
 *
 */
typedef struct GRP_graphProfileOps_ *GRP_graphProfileOps;

/**
 *
 */
typedef struct GRP_graphProfileState_ *GRP_graphProfileState;

/**
 *
 */
typedef struct GRP_graphProfile_ *GRP_graphProfile;

/**
 *
 * @param
 * @param
 * @param
 */
typedef double
(*GRP_graphProfileWeight)(GRP_graphProfile, GRP_EdgeType);

/**
 *
 */
struct GRP_graphProfileOps_
{
    GRP_graphProfileWeight weight; //!<
};

/**
 *
 */
struct GRP_graphProfileState_
{
    int states; //!<
    int events; //!<
    double contEdgeWeight;
    double dscEdgeWeight;
    double virtEdgeWeight;
};

/**
 *
 */
struct GRP_graphProfile_
{
    GRP_graphProfileOps ops; //!<
    GRP_graphProfileState state; //!<
};

/**
 *
 * @return
 */

GRP_graphProfile
GRP_GraphProfile();

/**
 *
 * @param g
 */
void
GRP_freeGraphProfile(GRP_graphProfile g);

/**
 *
 * @return
 */
GRP_graphProfileOps
GRP_GraphProfileOps();

/**
 *
 * @param ops
 */
void
GRP_freeGraphProfileOps(GRP_graphProfileOps ops);

/**
 *
 * @return
 */
GRP_graphProfileState
GRP_GraphProfileState();

/**
 *
 * @param state
 */
void
GRP_freeGraphProfileState(GRP_graphProfileState state);

double
GRP_Weight(GRP_graphProfile g, GRP_EdgeType type);

#endif /* GRAPH_GRAPH_PROFILE_H_ */
