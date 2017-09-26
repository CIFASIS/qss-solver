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

#ifndef TREE_H_
#define TREE_H_

/**
 *
 */
typedef double *leave;
/**
 *
 */
typedef struct BT_node_ *BT_node;
/**
 *
 */
struct BT_node_
{
    int value; //!<
    int father; //!<
    short size; //!<
};
/**
 *
 * @return
 */
BT_node
BT_Node(void);
/**
 *
 */
typedef struct BT_tree_ *BT_tree;
/**
 *
 */
struct BT_tree_
{
    BT_node *nodes; //!<
    leave leaves; //!<
    int *map;
    int *father; //!<
    int height; //!<
    int size; //!<
};
/**
 *
 * @param dim
 * @param leaves
 * @return
 */
BT_tree
BT_Tree(int dim, leave leaves, int leavesNumber, int *map);

void
BT_freeTree(BT_tree tree);
/**
 *
 */
typedef struct BT_info_ *BT_info;
/**
 *
 */
struct BT_info_
{
    int size; //!<
    int **fathers; //!<
};
/**
 *
 * @param mh
 * @param mn
 * @return
 */
BT_info
BT_Info(int mh, int mn);

void
BT_freeInfo(BT_info info);

/** Random binary tree structures */

/**
 *
 */
typedef struct BTR_node_ *BTR_node;

/**
 *
 */
struct BTR_node_
{
    int value; //!<
    int father; //!<
    short size; //!<
    int visit[3]; //!<
};

/**
 *
 * @return
 */
BTR_node
BTR_Node(void);

/**
 *
 */
typedef struct BTR_tree_ *BTR_tree;
/**
 *
 */
struct BTR_tree_
{
    BTR_node *nodes; //!<
    leave leaves; //!<
    int *father; //!<
    int *equals; //!<
    int *weightedEquals; //!<
    int height; //!<
    int size; //!<
    int numEquals; //!<
    int randomRange; //!<
    int num; //!<
    double *weights; //!<
    double minimum; //!<
};
/**
 *
 * @param dim
 * @param leaves
 * @param weights
 * @return
 */
BTR_tree
BTR_Tree(int dim, leave leaves, double *weights);

#endif /* TREE_H_ */
