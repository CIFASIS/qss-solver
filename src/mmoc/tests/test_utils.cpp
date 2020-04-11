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

#include "test_utils.h"

#include "ast/element.h"
#include "ast/ast_builder.h"
#include "parser/parse.h"
#include "util/visitors/eval_init_exp.h"

using namespace MicroModelica::Util;

void insert(VarName name, Variable &var, VarSymbolTable &symbols)
{
  var.setName(name);
  if (var.typePrefix() & TP_CONSTANT) {
    EvalInitExp eval(symbols);
    var.setValue(eval.apply(var.exp()));
  }
  symbols.insert(name, var);
}

VarSymbolTable getSymbols(string variable_definition)
{
  VarSymbolTable symbols;
  int ret;
  AST_ElementList elements = parseVariables(variable_definition, &ret);
  assert(ret == 0);
  AST_ElementListIterator it;
  foreach (it, elements) {
    AST_Element element = current_element(it);
    if (element->elementType() == COMPONENT) {
      AST_Element_Component comp = element->getAsComponent();
      AST_TypePrefix type_prefix = comp->typePrefix();
      AST_DeclarationListReverseIterator dec_it;
      AST_DeclarationList decs = comp->nameList();
      foreachReverse(dec_it, decs)
      {
        AST_Declaration dec = current_element(dec_it);
        vector<int> size;
        bool array = dec->hasIndexes();
        if (array) {
          EvalInitExp eval(symbols);
          AST_ExpressionList exps = dec->indexes();
          AST_ExpressionListIterator exp_it;
          foreach (exp_it, exps) {
            size.push_back(eval.apply(current_element(exp_it)));
          }
        }
        if (type_prefix & TP_CONSTANT) {
          Variable var(newType_Integer(), type_prefix, dec->modification(), nullptr, size, array);
          insert(dec->name(), var, symbols);
        } else {
          if ((type_prefix & TP_PARAMETER) && comp->isInteger()) {
            Variable var(newType_Integer(), type_prefix, dec->modification(), nullptr, size, array);
            insert(dec->name(), var, symbols);
          } else if (comp->isString()) {
            Variable var(newType_String(), type_prefix, dec->modification(), nullptr, size, array);
            insert(dec->name(), var, symbols);
          } else {
            Variable var(newType_Real(), type_prefix, dec->modification(), nullptr, size, array);
            insert(dec->name(), var, symbols);
          }
        }
      }
    }
  }
  return symbols;
}
