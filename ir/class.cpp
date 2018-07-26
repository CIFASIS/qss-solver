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

#include "class.h"

#include <iterator>
#include <sstream>
#include <utility>

namespace MicroModelica {
  namespace IR
  {
    /* Function class */

    Function::Function(string name) 
    {
    }

    Function::~Function()
    {
    }

    VarSymbolTable
    Function::varTable()
    {
      return NULL;
    }

    void
    Function::insert(string n)
    {
    }

    void
    Function::insert(AST_Equation eq)
    {
      return;
    }

    void
    Function::insert(AST_Statement stm)
    {
    }

    void
    Function::insert(AST_Statement stm, bool initial)
    {
    }

    void
    Function::insert(AST_External_Function_Call efc)
    {
    }

    void
    Function::insert(VarName n, VarInfo vi, DEC_Type type)
    {
    }

    void
    Function::insert(VarName n, VarInfo vi)
    {
    }

    void
    Function::insert(AST_Argument_Modification x)
    {
    }

    string
    Function::name() const
    {
      return ""; 
    }

    MMO_ImportTable 
    Function::imports()
    {
    }

    Package::Package(string name) 
    {
    }

    Package::~Package()
    {
    }

    VarSymbolTable
    Package::varTable()
    {
      return NULL;
    }

    void
    Package::insert(string n)
    {
      return;
    }

    void
    Package::insert(AST_Equation eq)
    {
      return;
    }

    void
    Package::insert(AST_Statement stm)
    {
      return;
    }

    void
    Package::insert(AST_Statement stm, bool initial)
    {
      return;
    }

    void
    Package::insert(Function &f)
    {
      return;
    }

    void
    Package::insert(AST_External_Function_Call efc)
    {
      return;
    }

    void
    Package::insert(VarName n, VarInfo vi, DEC_Type type)
    {
      return;
    }

    void
    Package::insert(VarName n, VarInfo vi)
    {
      return;
    }

    void
    Package::insert(AST_Argument_Modification x)
    {
    }

    string
    Package::name() const
    {
      return "";
    }
    Model::Model(string name)
    {
    }

    Model::~Model()
    {
    }

    void
    Model::insert(VarName n, VarInfo vi, DEC_Type type)
    {
    }

    void
    Model::insert(VarName n, VarInfo vi)
    {
    }

    void
    Model::insert(AST_Equation eq)
    {
    }

    void
    Model::insert(AST_Statement stm)
    {
    }

    void
    Model::insert(AST_External_Function_Call efc)
    {
      return;
    }

    void
    Model::insert(AST_Statement stm, bool initial)
    {
    }

    void
    Model::insert(Function &f)
    {
    }

    void
    Model::insert(AST_Argument_Modification x)
    {
    }

    VarSymbolTable
    Model::varTable()
    {
      return NULL;
    }

    void
    Model::insert(string n)
    {
    }

    string
    Model::name() const
    {
      return "";
    }

    MMO_ImportTable
    Model::imports()
    {
    }

  }
}
