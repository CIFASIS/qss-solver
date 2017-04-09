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

#ifndef MMO_DECL_H_
#define MMO_DECL_H_

#include <string>

#include "mmo_exp.h"

using namespace std;

/**
 *
 */
typedef enum
{
  DEC_CONSTANT,
  DEC_PARAMETER,
  DEC_STATE,
  DEC_DISCRETE,
  DEC_ALGEBRAIC,
  DEC_DERIVATIVE,
  DEC_ASSIGNMENT,
  DEC_CONDITIONAL_ASSIGNMENT,
  DEC_INITIAL_ASSIGNMENT,
  DEC_ZC_RELATION,
  DEC_ZC_OPPOSITE_RELATION,
  DEC_ALGEBRAIC_EQUATION,
  DEC_IMPLICIT_EQUATION,
  DEC_REINIT,
  DEC_CONDITION,
  DEC_FUNCTION_INPUT,
  DEC_FUNCTION_OUTPUT,
  DEC_FUNCTION_DEFINITION,
  DEC_FUNCTION_FORMULA,
  DEC_IMPORT
} MMODeclType;

/**
 *
 */
class MMODecl : public MMOExp
{
public:
  /**
   *
   * @param id
   * @param value
   * @param type
   */
  MMODecl (string id, double value, MMODeclType type);
  /**
   *
   * @param id
   * @param exp
   * @param type
   */
  MMODecl (string id, string exp, MMODeclType type, bool conditional = false);
  /**
   *
   * @param exp
   * @param type
   */
  MMODecl (string exp, MMODeclType type);
  /**
   *
   */
  MMODecl ();
  /**
   *
   */
  ~MMODecl ();
  /**
   *
   * @param visitor
   */
  void
  accept (MMOVisitor *visitor);
  /**
   *
   * @return
   */
  string
  getId ();
  /**
   *
   * @param i
   */
  void
  id (string i);
  /**
   *
   * @param type
   */
  void
  setType (MMODeclType type);
  /**
   *
   * @return
   */
  string
  getExp ();
  /**
   *
   * @param i
   */
  void
  exp (string i);
  /**
   *
   * @param i
   */
  void
  value (double i);
  /**
   *
   * @return
   */
  double
  getValue ();
  /**
   *
   * @return
   */
  bool
  hasExp ();
  /**
   *
   * @return
   */
  bool
  hasValue ();
  /**
   *
   * @return
   */
  bool
  isAlgebraicEquation ();
  /**
   *
   * @return
   */
  bool
  isInitialAssignment ();
  /**
   *
   * @return
   */
  bool
  isAssignment ();
  /**
   *
   * @return
   */
  bool
  isZeroCrossing ();
  /**
   *
   * @return
   */
  bool
  isOppositeZeroCrossing ();
  /**
   *
   * @return
   */
  bool
  isDerivative ();
  /**
   *
   * @return
   */
  bool
  isParameter ();
  /**
   *
   * @return
   */
  bool
  isConstant ();
  /**
   *
   * @return
   */
  bool
  isState ();
  /**
   *
   * @return
   */
  bool
  isDiscrete ();
  /**
   *
   * @return
   */
  bool
  isAlgebraic ();
  /**
   *
   * @return
   */
  bool
  isCondition ();
  /**
   *
   * @return
   */
  bool
  isFunctionInput ();
  /**
   *
   * @return
   */
  bool
  isFunctionOutput ();
  /**
   *
   * @return
   */
  bool
  isFunctionDefinition ();
  /**
   *
   * @return
   */
  bool
  isFunctionFormula ();
  /**
   *
   * @return
   */
  bool
  isReinit ();
  /**
   *
   * @return
   */
  bool
  isImport();
  /**
   *
   * @return
   */
  bool
  isImplicit ();
  /**
   *
   * @return
   */
  bool
  isConditionalAssignment ();
private:
  string _id;
  string _exp;
  double _value;
  MMODeclType _type;
  bool _init;
};

#endif  /* MMO_DECL_H_ */
