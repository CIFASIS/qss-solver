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

#include "symbol_table.h"

#include <sstream>
#include <math.h>

#include "../ast/expression.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "../ir/helpers.h"
#include "util.h"

namespace MicroModelica {
  using namespace IR;
  namespace Util {
    
    /* Variable class. */

    Variable::Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c) :
        _discrete(false), 
        _t(t), 
        _tp(tp), 
        _m(m), 
        _comm(c), 
        _builtin(false), 
        _size(), 
        _value(0), 
        _exp(NULL), 
        _hasStart(false), 
        _hasEach(false), 
        _hasAssigment(false), 
        _name(), 
        _isArray(false),
        _hasOffset(false),
        _offset(0),
        _realType(NotAsigned)
    {
      processModification();
    }

    Variable::Variable(Type t, AST_TypePrefix tp, AST_Modification m, AST_Comment c, vector<int> s, bool array) :
        _discrete(false), 
        _t(t), 
        _tp(tp), 
        _m(m), 
        _comm(c), 
        _builtin(false), 
        _size(s), 
        _value(0), 
        _exp(NULL), 
        _hasStart(false), 
        _hasEach(false), 
        _hasAssigment(false), 
        _name(), 
        _isArray(array),
        _hasOffset(false),
        _offset(0),
        _realType(NotAsigned)
    {
      processModification();
    }

    /*! \brief Process the argument modification to determine the variable modifiers if any.
     *
     * 	\note{We don't look for errors here, if there's an error in the code, it should be detected in 
     * 	an earlier stage (the model checker and the intermediate code generation.}
     */

    void
    Variable::processModification()
    {
      _hasAssigment = false;
      _hasEach = false;
      _hasStart = false;
      if(_m != NULL)
      {
        ModificationType t = _m->modificationType();
        if(t == MODEQUAL)
        {
          _hasAssigment = true;
          _exp = _m->getAsEqual()->exp();
        }
        else if(t == MODCLASS)
        {
          AST_ArgumentList al = _m->getAsClass()->arguments();
          AST_ArgumentListIterator it;
          AST_Expression asgExp = _m->getAsClass()->exp();
          if(asgExp->expressionType() != EXPNULL)
          {
            _hasAssigment = true;
            _exp = asgExp;
          }
          else
          {
            if(!isParameter())
            {
              foreach(it,al)
              {
                if(current_element(it)->argumentType() == AR_MODIFICATION)
                {
                  AST_Argument_Modification am =
                  current_element(it)->getAsModification();
                  if(am->modification()->modificationType() == MODEQUAL)
                  {
                    _exp = am->modification()->getAsEqual()->exp();
                    if(current_element(it)->hasEach())
                    {
                      _hasEach = true;
                    }
                    else if(am->name()->compare("start") == 0)
                    {
                      _hasStart = true;
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    int
    Variable::size()
    {
      vector<int>::iterator it;
      int total = 1;
      for(it = _size.begin(); it != _size.end(); it++)
      {
        total *= *it;
      }
      return total;
    }

    ostream &
    operator<<(ostream &ret, const Variable &e)
    {
      if(e.isForType() || e.isInput() || e.isOutput()) 
      {
        ret << e._name;
      }
      else if(e.isConstant())
      {
        ret << e._value;
      }
      else 
      {
        ret << "_" << e._name;
      }
      return ret;
    }

    string 
    Variable::declaration(string prefix)
    {
      stringstream buffer;
      buffer << "double " << prefix << name();
      if(isArray())
      {
        buffer << "[" << size() << "]";
      }
      buffer << ";";
      return buffer.str();
    }

    string 
    Variable::initialization(const VarSymbolTable& symbols)
    {
      stringstream buffer;
      string index;
      if(hasEachModifier())
      {
        index = Utils::instance().iteratorVar();
        buffer << "int " << index << ";" << endl;
      }
      if(hasAssignment() || hasStartModifier() || hasEachModifier())
      {
        Expression ex(exp(), symbols);
        if(hasAssignment() || hasStartModifier())
        {
            buffer << this << " = " << ex << ";";
        }
        else if(hasEachModifier())
        {
          buffer << "for(" << index << " = 0; " << index << " <= " << size() << ";" << index << "++)" << endl;
          buffer << TAB << "{" << endl;
          buffer << TAB << TAB << this << "[" << index << "]" << " = " << ex << ";" << endl;
          buffer << TAB << "}";
        }
      }
      return buffer.str();
    }
  
    /* TypeSymbolTable_ class.*/

    TypeSymbolTable::TypeSymbolTable()
    {
      insert("String", new Type_String_());
      insert("Real", new Type_Real_());
      insert("Integer", new Type_Integer_());
      insert("Boolean", new Type_Boolean_());
    }

    /* VarSymbolTable class */

    VarSymbolTable::VarSymbolTable() :
        _coeffs(1), 
        _parameters()
    {
    }

    void
    VarSymbolTable::initialize(TypeSymbolTable ty)
    {
      Variable v(ty["Real"].get(), 0, NULL, NULL, vector<int>(1, 0), false);
      v.setBuiltIn();
      v.setName("time");
      insert("time", v);
    }

    void
    VarSymbolTable::insert(VarName n, Variable vi)
    {
      ModelTable<VarName, Variable>::insert(n, vi);
      if(vi.isParameter())
      {
        _parameters.push_back(vi);
      }
    }

    list<Variable>
    VarSymbolTable::parameters()
    {
      return _parameters;
    }
  }
}
