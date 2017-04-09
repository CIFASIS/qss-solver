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

#ifndef MMO_UTILS_H_
#define MMO_UTILS_H_

#include <sbml/common/libsbml-namespace.h>
#include <sbml/math/ASTNode.h>
#include <map>
#include <string>

using namespace std;

LIBSBML_CPP_NAMESPACE_USE

/**
 *
 */
class MMOUtils
{
protected:
  /**
   *
   */
  MMOUtils ();
  /**
   *
   * @param copy
   */
  MMOUtils (MMOUtils const& copy);
  /**
   *
   * @param copy
   * @return
   */
  MMOUtils&
  operator= (MMOUtils const& copy);
private:
  int _varnum;
  map<string,map<string,string> > _predefinedFunctions;
  static bool _instance;
  static MMOUtils *_utils;
  bool _bioricaLanguage;
  string
  _applyList(ASTNode *node, string oper);
  bool
  _controlOperation(ASTNodeType_t type);
public:
  /**
   *
   */
  ~MMOUtils ()
  {
    _instance = false;
  };
  /**
   *
   * @return
   */
  static MMOUtils *
  getInstance ();
  /**
   *
   * @param vstr
   * @return
   */
  const char*
  getVar (string vstr = "tmpVar_");
  /**
   *
   * @param f
   */
  void
  bioricaLanguage(bool f);
  /**
   *
   * @param node
   * @param asgVariable
   * @return
   */
  string
  getExp (ASTNode *node, string asgVariable = "");
  /**
   *
   * @param n
   * @return
   */
  string
  indent (int n);
  /**
   *
   * @param node
   * @return
   */
  string checkPredefinedFunctions(ASTNode *node);
};

#endif /* MMO_UTILS_H_ */
