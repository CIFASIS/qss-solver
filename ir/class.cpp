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
      _statements[++_statementId] = Statement(stm, _symbols);
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
      _externalFunctions.insert(++_externalFunctionId, ExternalFunction(lvalue, efc->name(),efc->args(), _symbols));
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
      _name(name),
      _functions()
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
    Package::setFunctions(FunctionTable& fs) 
    {
      _functions.merge(fs);
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
      _types(),
      _annotations(_symbols),
      _calledFunctions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependecies(),
      _packages(),
      _initialCode(),
      _libraryDirectories(),
      _linkLibraries(),
      _includeDirectories(),
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
      _symbols.initialize(_types);
    }

    Model::Model(string name) :
      _name(name),
      _imports(),
      _symbols(),
      _types(),
      _annotations(_symbols),
      _calledFunctions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependecies(),
      _packages(),
      _initialCode(),
      _libraryDirectories(),
      _linkLibraries(),
      _includeDirectories(),
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
      _symbols.initialize(_types);
    }

    void
    Model::insert(VarName n, Variable& vi, DEC_Type type)  
    {
      insert(n, vi);
    }

    void
    Model::insert(VarName n, Variable& vi) 
    {
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

    Option<Variable>  
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
      }
      return var;
    }

    void 
    Model::setRealVariableOffset(AST_Expression exp, Util::Variable::RealType type, int& offset)
    {
        Option<Variable> var = variable(exp);
        if(!var->hasOffset())
        {
          var->setOffset(offset);
          var->setRealType(type);
          offset += var->size();
        }
        _symbols.insert(var->name(), var.get());
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
        _initialCode.insert(_statementId++, Statement(stm, _symbols, initial));
      }
      else
      {
        _astStatements.push_back(st);
      }
    }

    void 
    Model::addFunction(SymbolTable symbols, FunctionTable& fs)
    {
      SymbolTable::iterator fit;
      for(string s = symbols.begin(fit); !symbols.end(fit); s = symbols.next(fit))
      {
        Option<Function> ef = fs[s];
        if(ef) 
        { 
          SymbolTable libraries;
          _calledFunctions.insert(s, ef.get()); 
          FunctionAnnotation fa = ef->annotations();
          if(fa.hasIncludeDirectory())
          {
            string in = fa.includeDirectory();
            _includeDirectories.insert(in, in);
          }
          if(fa.hasLibraryDirectory())
          {
            string in = fa.libraryDirectory();
            _libraryDirectories.insert(in, in);
          }
          if(fa.hasLibraries())
          {
            libraries = fa.libraries();
            _linkLibraries.merge(libraries);
          }
          CompiledFunction cf(s, fa.includeDirectory(), fa.libraryDirectory(), libraries);
          Utils::instance().addCompiledFunction(cf);
        }
      }
    }

    void
    Model::setCalledFunctions(FunctionTable &fs) 
    {
      EquationTable::iterator it;
      for(Equation eq = _derivatives.begin(it); !_derivatives.end(it); eq = _derivatives.next(it))
      {
        addFunction(eq.calledFunctions(), fs);
      }
      for(Equation eq = _algebraics.begin(it); !_algebraics.end(it); eq = _algebraics.next(it))
      {
        addFunction(eq.calledFunctions(), fs);
      }
      EventTable::iterator eit;
      for(Event ev = _events.begin(eit); !_events.end(eit); ev = _events.next(eit))
      {
        addFunction(ev.zeroCrossing().calledFunctions(), fs);
        StatementTable::iterator sit;
        StatementTable stms = ev.positiveHandler();
        for(Statement stm = stms.begin(sit); !stms.end(sit); stm = stms.next(sit))
        {
          addFunction(stm.calledFunctions(), fs);
        }
        stms = ev.negativeHandler();
        for(Statement stm = stms.begin(sit); !stms.end(sit); stm = stms.next(sit))
        {
          addFunction(stm.calledFunctions(), fs);
        }
      }
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
      Option<CompiledPackage> cp = _packages[n];
      if(cp)
      {
        Utils::instance().addCompiledFunctions(cp->definitions());
        _linkLibraries.merge(cp->linkLibraries());
        _libraryDirectories.merge(cp->libraryDirectories());
        _includeDirectories.merge(cp->includeDirectories());
      }
    }

    void 
    Model::addInput(Equation eq, int eqId, EQUATION::Type type)
    {
      if(!eq.autonomous())
      {
        _inputs[_inputNbr++] = Input(eqId, type);
      }
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
        Equation mse(eq, _symbols, range, EQUATION::Derivative);
        addInput(mse, _derivativeId, EQUATION::Derivative);
        _derivatives.insert(_derivativeId++, mse);
      }
      else if(eqe->left()->expressionType() == EXPCOMPREF)
      {
        variable(eqe->left());
        Equation mse(eq, _symbols, range, EQUATION::Algebraic);
        addInput(mse, _algebraicId, EQUATION::Algebraic);
        _algebraics.insert(_algebraicId++, mse);
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
          Equation mse(eq, _symbols, range, EQUATION::Algebraic);
          addInput(mse, _algebraicId, EQUATION::Algebraic);
          _algebraics.insert(_algebraicId++, mse);
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
      Utils::instance().setSymbols(_symbols);
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
    Model::addEvent(AST_Statement stm, Option<Range> range)
    {
      if(stm->statementType() == STWHEN)
      {
        _eventNbr += (range ? range->size() : 1);
        AST_Statement_When sw = stm->getAsWhen();
        if(sw->hasComment())
        {
          _annotations.eventComment(sw->comment());
        }
        Event event(sw->condition(), _symbols);
        AST_StatementList stl = sw->statements();
        AST_StatementListIterator it;
        foreach(it,stl)
        {
          event.add(current_element(it));
        }
        if(sw->hasElsewhen())
        {
          int newEvent = false;
          AST_Statement_ElseList ewl = sw->else_when();
          AST_Statement_ElseListIterator ewit;
          foreach(ewit,ewl)
          {
            AST_Statement_Else se = current_element(ewit);
            Event event2 = event;
            if(!event.compare(se->condition()))
            {
              event2 = Event(se->condition(), _symbols);
              _eventNbr += range->size();
              newEvent = true;
            }
            AST_StatementList stel = se->statements();
            AST_StatementListIterator steit;
            foreach(steit,stel)
            {
              event2.add(current_element(steit));
            }
            if(newEvent)
            {
              _eventNbr += (range ? range->size() : 1);
              _events.insert(_eventId++, event2);
            }
          }
        }
        _events.insert(_eventId++, event);
      }
    }
    
    void 
    Model::setEvents()
    {
      list<AST_Statement>::iterator st;
      for(st = _astStatements.begin(); st != _astStatements.end(); st++)
      {
        AST_Statement stm = *st;
        if(stm->statementType() == STWHEN)
        {
          addEvent(stm, Option<Range>());
        }
        else if(stm->statementType() == STFOR)
        {
          AST_Statement_For stf = stm->getAsFor();
          Range range(stf, _symbols);
          AST_StatementList sts = stf->statements();
          AST_StatementListIterator stit;
          foreach(stit,sts)
          {
            addEvent(current_element(stit), range);
          }
        }
      }
    }

    void 
    Model::setOutputs()
    {
      list<AST_Expression> astOutputs = _annotations.output();
      list<AST_Expression>::iterator it;
      for(it = astOutputs.begin(); it != astOutputs.end(); it++)
      {
        _outputs.insert(_outputNbr++, Equation(*it, _symbols, EQUATION::Output));
      }
    }
  }
}
