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

#ifndef AST_COMPOSITION_H_
#define AST_COMPOSITION_H_

#include <string>

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

class AST_CompositionElement_;

using namespace std;

class AST_Composition_ : public AST_Node_ {
  public:
  AST_Composition_(AST_ElementList el, AST_CompositionElementList cl);
  DEFINE_CLASS_PRINTER(AST_Composition);
  AST_CompositionElementList compositionList() const;
  bool hasCompositionList();
  AST_ElementList elementList() const;
  bool hasElementList();
  void setExternalFunctionCall(AST_External_Function_Call);
  AST_External_Function_Call externalCall() const;
  bool hasExternalFunctionCall();
  void setAnnotation(AST_ArgumentList);
  bool hasAnnotation();
  void accept(AST_Visitor *visitor);

  private:
  AST_CompositionElementList _composition_list;
  AST_ElementList _element_list;
  AST_ArgumentList _annot;
  AST_External_Function_Call _ext;
};

class AST_CompositionElement_ : public AST_Node_ {
  public:
  AST_CompositionElement_(AST_CompositionEqsAlgs eqs_algs);
  AST_CompositionElement_(AST_ElementList el);
  DEFINE_CLASS_PRINTER(AST_CompositionElement);
  AST_CompositionEqsAlgs getEquationsAlgs();
  AST_ElementList getElementList();
  bool hasEquations();
  bool hasStatements();
  bool hasElements();
  void accept(AST_Visitor *visitor);

  private:
  AST_CompositionEqsAlgs _eqs_algs;
  AST_ElementList _el;
};

class AST_CompositionEqsAlgs_ : public AST_Node_ {
  public:
  AST_CompositionEqsAlgs_(AST_EquationList eq);
  AST_CompositionEqsAlgs_(AST_StatementList st);
  AST_CompositionEqsAlgs_(AST_StatementList st, bool initial);
  AST_CompositionEqsAlgs_(AST_EquationList eq, bool initial);
  AST_EquationList getEquations();
  AST_StatementList getAlgorithms();
  bool isInitial();
  bool hasEquations();
  bool hasStatements();
  void accept(AST_Visitor *visitor);

  private:
  bool _initial;
  AST_EquationList _eq;
  AST_StatementList _st;
};

class AST_External_Function_Call_ : public AST_Node_ {
  public:
  AST_External_Function_Call_(AST_String, AST_Expression_ComponentReference, AST_Expression, AST_ArgumentList);
  AST_String language();
  string languageString();
  AST_ArgumentList annotation();
  AST_ExpressionList args();
  string name();
  AST_Expression exp();
  AST_Expression_ComponentReference componentReference();
  bool hasComponentReference();
  void accept(AST_Visitor *visitor);

  private:
  AST_String _lang;
  AST_ArgumentList _annot;
  AST_Expression_Call _call;
  AST_Expression _exp;
  AST_Expression_ComponentReference _cr;
};

#endif /* AST_COMPOSITION_H_ */
