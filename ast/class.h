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

/**
 *
 */
class AST_Class_: public AST_Node_
{
  public:
    /**
     *
     * @param n
     * @param comp
     */
    AST_Class_(AST_String n, AST_Composition comp);
    /**
     *
     */
    ~AST_Class_();
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Class)
    ;
    /**
     *
     * @param c
     */
    void
    setComposition(AST_Composition c);
    /**
     *
     * @return
     */
    AST_Composition
    composition() const;
    /**
     *
     * @return
     */
    AST_String
    name() const;
    /**
     *
     * @return
     */
    AST_Element_ComponentList
    getComponents();
    /**
     *
     * @return
     */
    bool
    hasElementComponentList();
    /**
     *
     * @param c
     */
    void
    addComponent(AST_Element_Component c);
    /**
     *
     * @param c
     */
    void
    addClass(AST_Class c);
    /**
     *
     * @return
     */
    AST_ClassList
    getClasses();
    /**
     *
     * @param c
     */
    void
    setFather(AST_Class c);
    /**
     *
     * @return
     */
    bool
    hasFather();
    /**
     *
     * @return
     */
    AST_Class
    father() const;
    /**
     *
     * @param e
     */
    void
    addExtends(AST_String e);
    /**
     *
     * @return
     */
    AST_StringList
    getExtends();
    /**
     *
     * @return
     */
    bool
    isBasic();
    /**
     *
     */
    void
    setBasic();
    /**
     *
     */
    void
    setFinal();
    /**
     *
     * @return
     */
    bool
    isFinal();
    /**
     *
     */
    void
    setEncapsulated();
    /**
     *
     * @return
     */
    bool
    isEncapsulated();
    /**
     *
     * @return
     */
    bool
    hasExtends();
    /**
     *
     * @param cp
     */
    void
    setPrefixes(AST_ClassPrefix cp);
    /**
     *
     * @return
     */
    AST_ClassPrefix
    prefix() const;
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
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

#endif  /* AST_CLASS_H_ */
