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

#include "../ast/ast_builder.h"
#include "../ast/composition.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../util/error.h"
#include "helpers.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR
  {
    /* Function Class Implementation*/

    Function::Function(string name) :
      _imports(),
      _name(name),
      _symbols(),
      _localSymbols(),
      _statements(),
      _types(),
      _packages(),
      _arguments(),
      _outputNbr(0),
      _externalFunctionId(0),
      _statementId(0),
      _externalFunctions()
    {
    }

    Function::~Function()
    {
    }

    VarSymbolTable
    Function::symbols() const
    {
      return _symbols;
    }

    void
    Function::insert(string n)
    {
      _imports.insert(n, n);
      if(!Utils::instance().readPackage(n, _packages))
      {
        Error::instance().add(0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str());
      }
    }

    void
    Function::insert(AST_Equation eq)  
    {
      return;
    }

    void
    Function::insert(AST_Statement stm) 
    {
      _statements[++_statementId] = Statement(stm);
    }

    void
    Function::insert(AST_Statement stm, bool initial) 
    {
      insert(stm);
    }

    void
    Function::insert(AST_External_Function_Call efc)   
    {
      string lvalue;
      VariableLookup vl(_symbols, _localSymbols);
      if(efc->hasComponentReference())
      {
        AST_Expression_ComponentReference cr = efc->componentReference();
        if(!vl.apply(cr))
        {
          Error::instance().add(efc->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", cr->name().c_str());
          return;
        }
        lvalue = cr->name();
      }
      AST_ExpressionListIterator eli;
      if(efc->args() != NULL)
      {
        foreach(eli,efc->args())
        {
          if(!vl.apply(current_element(eli)))
          {
            Error::instance().add(efc->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "External function call.");
            return;
          }
        }
      }
      _externalFunctions.insert(++_externalFunctionId, ExternalFunction(lvalue, efc->name(),efc->args()));
    }

    void
    Function::insert(VarName n, Variable& vi, DEC_Type type)  
    {
      EvalInitExp eval(_symbols);
      vi.setName(n);
      if(vi.typePrefix() & TP_CONSTANT)
      {
        vi.setValue(eval.apply(vi.modification()->getAsEqual()->exp()));
      }
      _symbols.insert(n, vi);
      if(type == DEC_PUBLIC)
      {
        if(vi.isOutput())
        {
          _outputNbr++;
        }
        _arguments.insert(n,vi);
      }
      else
      {
        _localSymbols.insert(n, vi);
      }
    }

    void
    Function::insert(VarName n, Variable& vi)  
    {
      insert(n, vi, DEC_PUBLIC);
    }

    void
    Function::insert(AST_Argument_Modification x)  
    { 
      if(!_annotations.insert(x))
      {
        Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error, "%s", x->name()->c_str());
      }
    }

    string
    Function::name() const
    {
      return _name; 
    }

    ImportTable 
    Function::imports() const
    {
      return _imports;
    }

    StatementTable 
    Function::statements() const 
    {
      return _statements;
    }
    
    ExternalFunctionTable 
    Function::externalFunctions() const 
    {
      return _externalFunctions;
    }

    CompiledPackageTable 
    Function::packages() const 
    {
      return _packages;
    }
    
    unsigned int 
    Function::outputNbr() const
    {
      return _outputNbr;
    }
      
    FunctionAnnotation 
    Function::annotations() const
    {
      return _annotations;
    }
     
    Util::VarSymbolTable 
    Function::localSymbols() const
    {
      return _localSymbols;
    }
 
    Util::VarSymbolTable 
    Function::arguments() const
    {
      return _arguments;
    }

    /* Package Class Implementation */

    
    Package::Package(string name) :
      _imports(),
      _name(),
      _functions()
    {
    }

    Package::~Package()
    {
    }

    VarSymbolTable
    Package::symbols() const 
    {
      return VarSymbolTable();
    }

    void
    Package::insert(string n) 
    {
      _imports[n] = n;
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
      _functions[f.name()] = f;
    }

    void
    Package::insert(AST_External_Function_Call efc)  
    {
      return;
    }

    void
    Package::insert(VarName n, Variable& vi, DEC_Type type)  
    {
      return;
    }

    void
    Package::insert(VarName n, Variable& vi)  
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
      return _name;
    }

    ImportTable 
    Package::imports() const
    {
      return _imports;
    }
    
    FunctionTable
    Package::definitions()
    {
      return _functions;
    }

    string
    Package::fileName() 
    {
      return Utils::instance().packageName(_name);
    }

    string
    Package::prefix()
    {
      return "__" + _name + "__";
    }

    /* Model Class Implementation */
    
    Model::Model() :
      _name(),
      _imports(),
      _symbols(),
      _annotations(_symbols),
      _calledFunctions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependecies(),
      _packages(),
      _initialCode(),
      _astEquations(),
      _astStatements(),
      _stateNbr(0),
      _discreteNbr(0),
      _algebraicNbr(0),
      _outputNbr(0),
      _derivativeId(0),
      _algebraicId(0),
      _eventId(0),
      _externalFunctions(false)
    {
    }

    Model::Model(string name) :
      _name(name),
      _imports(),
      _symbols(),
      _annotations(_symbols),
      _calledFunctions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependecies(),
      _packages(),
      _initialCode(),
      _astEquations(),
      _astStatements(),
      _stateNbr(0),
      _discreteNbr(0),
      _algebraicNbr(0),
      _outputNbr(0),
      _derivativeId(0),
      _algebraicId(0),
      _eventId(0),
      _externalFunctions(false)
    {
    }

    void
    Model::insert(VarName n, Variable& vi, DEC_Type type)  
    {
      insert(n, vi);
    }

    void
    Model::insert(VarName n, Variable& vi) 
    {
      cout << "Inserta " << n << endl;
      vi.setName(n);
      if(vi.typePrefix() & TP_CONSTANT)
      {
        EvalInitExp eval(_symbols); 
        vi.setValue(eval.apply(vi.exp()));
      }
      if(vi.typePrefix() & TP_DISCRETE)
      {
        vi.setOffset(_discreteNbr);
        _discreteNbr += vi.size();
      }
      _symbols.insert(n, vi);
    }

    Variable  
    Model::variable(AST_Expression exp)
    {
      string varName;
      if(exp->expressionType() == EXPCOMPREF)
      {
        AST_Expression_ComponentReference ecr = exp->getAsComponentReference();
        varName = *AST_ListFirst(ecr->names());
      }
      Option<Variable> var = _symbols[varName];
        if(!var)
        {
          Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", varName.c_str());
          return Variable();
        }

      return var.get();
    }

    void 
    Model::setRealVariableOffset(AST_Expression exp, Util::Variable::RealType type, int& offset)
    {
        Variable var = variable(exp);
        if(!var.hasOffset())
        {
          var.setOffset(offset);
          var.setRealType(type);
          offset += var.size();
        }
    }

    void 
    Model::setRealVariables(AST_Equation eq)
    {
      AST_Equation_Equality eqe = eq->getAsEquality();
      if(eqe->left()->expressionType() == EXPDERIVATIVE)
      {
        AST_Expression_Derivative ed = eqe->left()->getAsDerivative();
        AST_Expression derArg = AST_ListFirst(ed->arguments());
        setRealVariableOffset(derArg, Variable::RealType::State, _stateNbr);
      }
      else if(eqe->left()->expressionType() == EXPCOMPREF)
      {
        setRealVariableOffset(eqe->left(), Variable::RealType::Algebraic, _algebraicNbr);
      }
      else if(eqe->left()->expressionType() == EXPOUTPUT)
      {
        AST_Expression_Output eout = eqe->left()->getAsOutput();
        AST_ExpressionList el = eout->expressionList();
        AST_ExpressionListIterator it;
        list<string> lvars;
        list<Index> lidx;
        foreach(it,el)
        {
          setRealVariableOffset(current_element(it), Variable::RealType::Algebraic, _algebraicNbr);
        }
      }
      else
      {
        Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
      }
    }

    void
    Model::insert(AST_Equation eq)  
    {
      AST_Equation teq = ConvertEquation(eq, _symbols).get();
      _astEquations.push_back(teq);
      if(teq->equationType() == EQEQUALITY)
      {
        setRealVariables(teq);
      }
      else if(teq->equationType() == EQFOR)
      {
        AST_Equation_For eqf = teq->getAsFor();
        AST_EquationList eqs = eqf->equationList();
        AST_EquationListIterator it;
        foreach(it,eqs)
        {
          if(current_element(it)->equationType() == EQFOR)
          {
            insert(current_element(it));
          }
          else
          {
            setRealVariables(current_element(it));
          }
        }
      }
      else
      {
        Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
      }
    }

    void
    Model::insert(AST_Statement stm)  
    {
      insert(stm, false);
    }

    void
    Model::insert(AST_External_Function_Call efc) 
    {
      return;
    }

    void
    Model::insert(AST_Statement stm, bool initial) 
    {
      AST_Statement st = ConvertStatement(stm, _symbols).get();
      if(initial)
      {
        _initialCode[_statementId++] = Statement(stm, initial);
      }
      else
      {
        _astStatements.push_back(st);
      }
    }

    void
    Model::setCalledFunctions(FunctionTable &fs) 
    {
    }

    void
    Model::insert(AST_Argument_Modification x) 
    {
      if(!_annotations.insert(x))
      {
        Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error, "%s", x->name()->c_str());
      }
    }

    void
    Model::insert(string n) 
    {
      _imports.insert(n, n);
      if(!Utils::instance().readPackage(n, _packages))
      {
        Error::instance().add(0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str());
      }
    }

    SymbolTable 
    Model::linkLibraries() const 
    {
      return SymbolTable();
    }

    SymbolTable 
    Model::includeDirectories() const 
    {
      return SymbolTable();
    }
    
    SymbolTable 
    Model::libraryDirectories() const 
    {
      return SymbolTable();
    }

    void 
    Model::addEquation(AST_Equation eq, Option<Range> range)
    {
      assert(eq->equationType() == EQEQUALITY);
      AST_Equation_Equality eqe = eq->getAsEquality();
      if(eqe->left()->expressionType() == EXPDERIVATIVE)
      {
        AST_Expression_Derivative ed = eqe->left()->getAsDerivative();
        variable(AST_ListFirst(ed->arguments()));
        Equation mse(eq, range, EQUATION::Derivative);
        _derivatives[_derivativeId++] = mse;
      }
      else if(eqe->left()->expressionType() == EXPCOMPREF)
      {
        variable(eqe->left());
        Equation mse(eq, range, EQUATION::Algebraic);
        _algebraics[_algebraicId++] = mse;
      }
      else if(eqe->left()->expressionType() == EXPOUTPUT)
      {
        if(eqe->right()->expressionType() != EXPCALL)
        {
          Error::instance().add(eqe->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
        }
        AST_Expression_Output eout = eqe->left()->getAsOutput();
        AST_ExpressionList el = eout->expressionList();
        AST_ExpressionListIterator it;
        foreach(it,el)
        {
          variable(current_element(it));
          Equation mse(eq, range, EQUATION::Algebraic);
          _algebraics[_algebraicId++] = mse;
        }
      }
      else
      {
        Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
      }
    }

    void 
    Model::setEquations()
    {
      list<AST_Equation>::iterator it;
      for(it = _astEquations.begin(); it != _astEquations.end(); it++)
      {
        AST_Equation eq = current_element(it);
        if(eq->equationType() == EQEQUALITY)
        {
          addEquation(eq, Option<Range>());
        }
        else if(eq->equationType() == EQFOR)
        {
          vector<int> begin, end;
          AST_Equation_For eqf = eq->getAsFor();
          Range range(eqf, _symbols);
          AST_EquationList eqs = eqf->equationList();
          AST_EquationListIterator it;
          foreach(it,eqs)
          {
            addEquation(current_element(it), range);
          }
        }
      }
    }

    void 
    Model::setEvents()
    {
    }

  }
}
