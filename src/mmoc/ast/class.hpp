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

#ifndef AST_CLASS_H_
#define AST_CLASS_H_

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

class AST_Class_ : public AST_Node_ {
  public:
  AST_Class_(AST_String n, AST_Composition comp);
  ~AST_Class_();
  DEFINE_CLASS_PRINTER(AST_Class);
  void setComposition(AST_Composition c);
  AST_Composition composition() const;
  AST_String name() const;
  AST_Element_ComponentList getComponents();
  bool hasElementComponentList();
  void addComponent(AST_Element_Component c);
  void addClass(AST_Class c);
  AST_ClassList getClasses();
  void setFather(AST_Class c);
  bool hasFather();
  AST_Class father() const;
  void addExtends(AST_String e);
  AST_StringList getExtends();
  bool isBasic();
  void setBasic();
  void setFinal();
  bool isFinal();
  void setEncapsulated();
  bool isEncapsulated();
  bool hasExtends();
  void setPrefixes(AST_ClassPrefix cp);
  AST_ClassPrefix prefix() const;
  void accept(AST_Visitor *visitor);

  private:
  AST_Element_ComponentList _components;
  AST_StringList _extends_list;
  AST_String _name;
  AST_Composition _composition;
  AST_ClassList _sub_classes;
  AST_Class _father;
  AST_Boolean _basic, _encapsulated, _final;
  AST_ClassPrefix _prefix;
};

#endif /* AST_CLASS_H_ */
