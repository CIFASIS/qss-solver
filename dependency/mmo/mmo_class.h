/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef MMO_CLASS
#define MMO_CLASS


#include <ast/class.h>
#include <util/table.h>
#include <util/type.h>


namespace Modelica {
using namespace Modelica::AST;
struct MMO_Class;


struct MMO_Class {
  MMO_Class();
  MMO_Class(Class &c);
  member_(Name,name);
  member_(ClassPrefixes,prefixes);
  member_(EquationSection, initial_eqs);
  member_(EquationSection, equations);
  member_(StatementSection, initial_sts);
  member_(StatementSection, statements);
  
  member_(Option<Annotation>,annotation);
  member_(Option<Annotation>,external_annot);
  member_(Option<String>,language);
  member_(Option<External>,external);
  
  member_(VarSymbolTable,syms);
  member_(TypeSymbolTable,tyTable);
  member_(std::vector<Name>, variables);
  member_(std::vector<Name>, types);
  
  printable(MMO_Class);
  bool isConnector();	
  void insertElement(Element);
  
  void addEquation(Equation);
  void addInitEquation(Equation);
  void addStatement(Statement);
  void addInitStatement(Statement);
  
  member_(ExtendList,extends);
  member_(ImportList,imports);
  member_(MMO_Class *,father);
  void addVar(Name n , VarInfo var);
  Option<VarInfo> getVar(Name n);
  bool isLocal(Name n);

  
};
}
#endif

