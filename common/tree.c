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

#include "tree.h"

#include <stdlib.h>

#include "utils.h"

BT_node
BT_Node()
{
  BT_node p = checkedMalloc(sizeof(*p));
  p->value = 0;
  p->father = 0;
  p->size = 0;
  return p;
}

BT_tree
BT_Tree(int dim, leave leaves, int leavesNumber, int *map)
{
  BT_tree p = checkedMalloc(sizeof(*p));
  if(dim == 0)
  {
    p->nodes = checkedMalloc(sizeof(BT_node*));
    p->nodes[0] = checkedMalloc(sizeof(struct BT_node_));
    p->height = 0;
    p->father = checkedMalloc(sizeof(int));
    p->size = 1;
    p->leaves = leaves;
    p->map = map;
    return p;
  }
  int height = (int) LOG2(dim);
  if(height)
  {
    int j, size = DIV2(dim);
    p->nodes = checkedMalloc(height * sizeof(BT_node*));
    for(j = 0; j < height; j++)
    {
      p->nodes[j] = checkedMalloc(size * sizeof(struct BT_node_));
      size = DIV2(size);
    }
    p->height = height - 1;
    p->father = checkedMalloc(leavesNumber * sizeof(int));
    for(j = 0; j < leavesNumber; j++)
    {
      p->father[j] = NOT_ASSIGNED;
    }
  }
  else
  {
    p->nodes = checkedMalloc(sizeof(BT_node*));
    p->nodes[0] = checkedMalloc(sizeof(struct BT_node_));
    p->height = 0;
    p->father = checkedMalloc(sizeof(int));
  }
  p->size = dim;
  p->leaves = leaves;
  p->map = map;
  return p;
}

void
BT_freeTree(BT_tree tree)
{
  int j;
  for(j = 0; j <= tree->height; j++)
  {
    free(tree->nodes[j]);
  }
  free(tree->nodes);
  if(tree->father)
  {
    free(tree->father);
  }
  free(tree);
}

BT_info
BT_Info(int mh, int mn)
{
  int j;
  BT_info p = checkedMalloc(sizeof(*p));
  p->fathers = checkedMalloc((mh + 1) * sizeof(*p->fathers));
  p->size = mh;
  mn = DIV2(mn);
  for(j = 0; j <= mh; j++)
  {
    p->fathers[j] = checkedMalloc(mn * sizeof(int));
    mn = DIV2(mn);
  }
  return p;
}

void
BT_freeInfo(BT_info info)
{
  int j, mh = info->size;
  for(j = 0; j <= mh; j++)
  {
    free(info->fathers[j]);
  }
  free(info->fathers);
  free(info);
}

BTR_node
BTR_Node()
{
  BTR_node p = checkedMalloc(sizeof(*p));
  p->value = 0;
  p->father = 0;
  p->size = 0;
  p->visit[0] = NOT_EQUAL;
  p->visit[1] = NOT_EQUAL;
  p->visit[2] = NOT_EQUAL;
  return p;
}

BTR_tree
BTR_Tree(int dim, leave leaves, double *weights)
{
  int i;
  int height = (int) LOG2(dim);
  BTR_tree p = checkedMalloc(sizeof(*p));
  if(height)
  {
    int j, size = DIV2(dim);
    p->nodes = checkedMalloc(height * sizeof(BTR_node*));
    for(j = 0; j < height; j++)
    {
      p->nodes[j] = checkedMalloc(size * sizeof(struct BTR_node_));
      size = DIV2(size);
    }
    p->height = height - 1;
    p->father = checkedMalloc(dim * sizeof(int));
  }
  else
  {
    p->nodes = checkedMalloc(sizeof(BTR_node*));
    p->nodes[0] = checkedMalloc(sizeof(struct BTR_node_));
    p->height = 0;
    p->father = checkedMalloc(sizeof(int));
  }
  p->size = dim;
  p->leaves = leaves;
  p->equals = (int*) malloc(dim * sizeof(int));
  p->weights = (double*) malloc(dim * sizeof(double));
  int totalWeights = 0;
  if(weights == NULL)
  {
    for(i = 0; i < dim; i++)
    {
      p->weights[i] = 1;
      totalWeights += p->weights[i];
    }
  }
  else
  {
    for(i = 0; i < dim; i++)
    {
      p->weights[i] = weights[i];
      totalWeights += p->weights[i];
    }
  }
  p->weightedEquals = (int*) malloc(totalWeights * sizeof(int));
  p->numEquals = 0;
  return p;
}
