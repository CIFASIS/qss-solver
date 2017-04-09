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

#ifndef MMO_ZEROCROSSING_H_
#define MMO_ZEROCROSSING_H_

#include <sbml/common/libsbml-namespace.h>
#include <sbml/math/ASTNode.h>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

LIBSBML_CPP_NAMESPACE_USE

/**
 *
 */
class MMOZeroCrossing
{
public:
  /**
   *
   */
  MMOZeroCrossing ();
  /**
   *
   * @param math
   */
  MMOZeroCrossing (ASTNode *math);
  /**
   *
   */
  ~MMOZeroCrossing ();
  /**
   *
   * @return
   */
  string
  getReverseRelation ();
  /**
   *
   * @return
   */
  pair<string, string>
  first ();
  /**
   *
   * @return
   */
  pair<string, string>
  next ();
  /**
   *
   * @return
   */
  bool
  end ();
  /**
   *
   * @param i
   * @return
   */
  bool
  isPositive (int i);
private:
  void
  _parseLogicalOperators (ASTNode *node, ASTNode *cond);
  void
  _parseZeroCrossing (ASTNode *node, ASTNode *cond);
  ASTNode *
  _reverseRelation (ASTNode *node);
  ASTNode *
  _addCond (ASTNode *cond, ASTNode *node);
  void
  _negateNode (ASTNode *node, ASTNode *cond);
  list<pair<ASTNode *, ASTNode *> > _zero_crossings;
  vector<bool> _types;
  ASTNode *_zero_crossing;
  list<pair<ASTNode *, ASTNode *> >::iterator _it;
};

#endif /* MMO_ZEROCROSSING_H_ */
