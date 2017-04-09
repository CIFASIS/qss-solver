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

#ifndef MMO_FUNCTION_H_
#define MMO_FUNCTION_H_

#include <list>
#include <string>

#include "mmo_decl.h"

using namespace std;

/**
 *
 */
class MMOFunction : public MMOExp
{
public:
  /**
   *
   * @param id
   */
  MMOFunction (string id);
  /**
   *
   */
  ~MMOFunction ();
  /**
   *
   * @param visitor
   */
  void
  accept (MMOVisitor *visitor);
  /**
   *
   * @param decl
   */
  void
  add (MMODecl *decl);
  /**
   *
   * @return
   */
  string
  getId ()
  {
    return (_id);
  };
private:
  string _id;
  MMODecl *_definition;
  list<MMODecl*> _inputs;
  MMODecl *_output;
};

#endif  /* MMO_FUNCTION_H_ */
