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

#ifndef MMO_ASSIGNMENT_H_
#define MMO_ASSIGNMENT_H_

#include <sbml/common/libsbml-namespace.h>
#include <sbml/math/ASTNode.h>
#include <list>
#include <string>

using namespace std;

LIBSBML_CPP_NAMESPACE_USE

/**
 *
 */
class MMOAssignment
{
public:
  /**
   *
   * @param node
   */
  MMOAssignment (ASTNode *node);
  /**
   *
   * @param node
   */
  MMOAssignment (const ASTNode *node);
  /**
   *
   */
  ~MMOAssignment ();
  /**
   *
   * @return
   */
  ASTNode *
  getAssignment ();
  /**
   *
   * @return
   */
  list<string>
  replacedVars ();
private:
  ASTNode *_exp;
};

#endif /* MMO_ASSIGNMENT_H_ */
