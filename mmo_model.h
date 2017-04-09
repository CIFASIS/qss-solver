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

#ifndef MMO_MODEL_H_
#define MMO_MODEL_H_

#include <sbml/common/libsbml-namespace.h>
#include <sbml/Event.h>
#include <sbml/FunctionDefinition.h>
#include <sbml/math/ASTNode.h>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "mmo_decl.h"
#include "mmo_section.h"
#include "mmo_writer.h"

using namespace std;

LIBSBML_CPP_NAMESPACE_USE

/**
 *
 */
class MMOModel
{
public:
  /**
   *
   */
  ~MMOModel ();
  /**
   *
   * @param replace
   * @param type
   */
  MMOModel (string prefix = "", bool replace = false, WR_Type type = WR_MMO);
  /**
   *
   * @param name
   */
  MMOModel (string name);
  /**
   *
   * @return
   */
  string
  id ();
  /**
   *
   * @param x
   */
  void
  add (const Event &x);
  /**
   *
   * @param f
   */
  void
  add (const FunctionDefinition &f);
  /**
   *
   * @param id
   * @param isDerivative
   * @param value
   * @param type
   */
  void
  add (string id, bool isDerivative, const ASTNode *value, MMOSectionType type);
  /**
   *
   * @param id
   * @param isVar
   * @param value
   * @param type
   */
  void
  add (string id, bool isVar, double value, MMOSectionType type);
  /**
   *
   * @param id
   * @param isVar
   * @param type
   */
  void
  add (string id, bool isVar, MMOSectionType type);
  /**
   *
   * @param file
   */
  void
  write (string file);
private:
  bool
  _delayValue (const ASTNode *n);
  bool
  _searchAlg (string f, list<string> *s);
  void
  _addAlgs (list<string> algs, list<string> *equations);
  void
  _add (MMOExp *exp, MMOSectionType type);
  void
  _addAsgs (const Event &x, MMOMath zc, int pos, MMOEvent *ev);
  void
  _parseEquation (string id, const ASTNode *node, MMODeclType tydec,
		  MMOSectionType type);
  void
  _reorder ();
  void
  _addAlgebraicReplacement (MMOMath& eq);
  void
  addExpressionZeroCrossings (MMOMath eq, const string& id);
  void
  _addConstantAlgebraics();
  bool _replace;
  WR_Type _type;
  map<string,string> _imports;
  map<string, pair<list<string>,ASTNode* > > _functions;
  map<string,string> _variableParameters;
  string _prefix;
  list<string> _constantAlgebraics;
  string _name;
  map<string, list<string> > _variables;
  MMOSection _importSec;
  MMOSection _algorithm;
  MMOSection _equation;
  MMOSection _external_functions;
  MMOSection _declarations;
  MMOSection _initial_algorithm;
};

#endif /* MMO_MODEL_H_ */
