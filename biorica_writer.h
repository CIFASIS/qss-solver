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

#ifndef BIORICA_WRITER_H_
#define BIORICA_WRITER_H_

#include <fstream>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "mmo_event.h"
#include "mmo_function.h"
#include "mmo_section.h"
#include "mmo_visitor.h"

class MMODecl;
class MMOEvent;
class MMOExp;
class MMOFunction;
class MMOMath;
class MMOSection;

using namespace std;

class BioRicaWriter : public MMOVisitor
{
public:
  /**
   *
   * @param file
   */
  BioRicaWriter (string file);
  /**
   *
   */
  ~BioRicaWriter ();
  /**
   *
   * @param x
   */
  void
  visit (MMOExp *x);
  /**
   *
   * @param x
   */
  void
  visit (MMODecl *x);
  /**
   *
   * @param x
   */
  void
  visit (MMODecl x);
  /**
   *
   * @param x
   */
  void
  leave (MMODecl *x);
  /**
   *
   * @param x
   */
  void
  visit (MMOEvent *x);
  /**
   *
   * @param x
   */
  void
  visit (MMOEvent x);
  /**
   *
   * @param x
   */
  void
  leave (MMOEvent *x);
  /**
   *
   * @param x
   */
  void
  visit (MMOFunction *x);
  /**
   *
   * @param x
   */
  void
  visit (MMOFunction x);
  /**
   *
   * @param x
   */
  void
  leave (MMOFunction *x);
  /**
   *
   * @param x
   */
  void
  visit (MMOSection *x);
  /**
   *
   * @param x
   */
  void
  visit (MMOSection x);
  /**
   *
   * @param x
   */
  void
  leave (MMOSection *x);
private:
  string
  _flatId (MMODecl *x);
  void
  _print (list<string> *l, string sep = "\n", string finalSep = "");
  list<string>
  _getList (map<string, string> m);
  int _indent;
  string _modelName;
  fstream _out;
  list<string> _consts;
  list<string> _formulas;
  list<string> _domains;
  list<string> _nodeElementStates;
  list<string> _nodeElementFlows;
  list<string> _nodeElementEvents;
  list<string> _nodeFieldEqDiffs;
  map<string, pair<string, list<string>*> > _nodeFieldTransitions;
  list<string> _nodeFieldAssertions;
  list<string> _nodeFieldExternals;
  list<string> _nodeFieldSynchs;
  map<string, string> _nodeFieldInits;
  string _currentEvent;
};

#endif /* BIORICA_WRITER_H_ */
