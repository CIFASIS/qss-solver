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
#include "../util/process_statement.h"
#include "../util/visitors/convert_condition.h"
#include "../util/visitors/convert_cont_red.h"
#include "../util/visitors/convert_equation.h"
#include "../util/visitors/convert_output_range.h"
#include "../util/visitors/convert_statement.h"
#include "../util/visitors/eval_init_exp.h"
#include "../util/visitors/variable_lookup.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Deps;
using namespace Util;
namespace IR {
/* Function Class Implementation*/

Function::Function(string name)
    : _imports(),
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

Function::~Function() {}

VarSymbolTable Function::symbols() const { return _symbols; }

void Function::insert(string n)
{
  _imports.insert(n, n);
  if (!Utils::instance().readPackage(n, _packages)) {
    Error::instance().add(0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str());
  }
}

void Function::insert(AST_Equation eq) { return; }

void Function::insert(AST_Statement stm) { _statements.insert(++_statementId, Statement(stm, _symbols)); }

void Function::insert(AST_Statement stm, bool initial) { insert(stm); }

void Function::insert(AST_External_Function_Call efc)
{
  string lvalue;
  VariableLookup vl(_symbols, _localSymbols);
  if (efc->hasComponentReference()) {
    AST_Expression_ComponentReference cr = efc->componentReference();
    if (!vl.apply(cr)) {
      Error::instance().add(efc->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "Insert external function call: %s",
                            cr->name().c_str());
      return;
    }
    lvalue = cr->name();
  }
  AST_ExpressionListIterator eli;
  if (efc->args() != nullptr) {
    foreach (eli, efc->args()) {
      if (!vl.apply(current_element(eli))) {
        Error::instance().add(efc->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "External function call.");
        return;
      }
    }
  }
  _externalFunctions.insert(++_externalFunctionId, ExternalFunction(lvalue, efc->name(), efc->args(), _symbols));
}

void Function::insert(VarName n, Variable &vi, DEC_Type type)
{
  EvalInitExp eval(_symbols);
  vi.setName(n);
  if (vi.typePrefix() & TP_CONSTANT) {
    vi.setValue(eval.apply(vi.modification()->getAsEqual()->exp()));
  }
  _symbols.insert(n, vi);
  if (type == DEC_PUBLIC) {
    if (vi.isOutput()) {
      _outputNbr++;
    }
    _arguments.insert(n, vi);
  } else {
    _localSymbols.insert(n, vi);
  }
}

void Function::insert(VarName n, Variable &vi) { insert(n, vi, DEC_PUBLIC); }

void Function::insert(AST_Argument_Modification x)
{
  if (!_annotations.insert(x)) {
    Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error, "%s", x->name()->c_str());
  }
}

string Function::name() const { return _name; }

ImportTable Function::imports() const { return _imports; }

StatementTable Function::statements() const { return _statements; }

ExternalFunctionTable Function::externalFunctions() const { return _externalFunctions; }

CompiledPackageTable Function::packages() const { return _packages; }

unsigned int Function::outputNbr() const { return _outputNbr; }

FunctionAnnotation Function::annotations() const { return _annotations; }

Util::VarSymbolTable Function::localSymbols() const { return _localSymbols; }

Util::VarSymbolTable Function::arguments() const { return _arguments; }

/* Package Class Implementation */

Package::Package(string name) : _imports(), _name(name), _functions() {}

VarSymbolTable Package::symbols() const { return VarSymbolTable(); }

void Package::insert(string n) { _imports[n] = n; }

void Package::insert(AST_Equation eq) { return; }

void Package::insert(AST_Statement stm) { return; }

void Package::insert(AST_Statement stm, bool initial) { return; }

void Package::setFunctions(FunctionTable &fs) { _functions.merge(fs); }

void Package::insert(AST_External_Function_Call efc) { return; }

void Package::insert(VarName n, Variable &vi, DEC_Type type) { return; }

void Package::insert(VarName n, Variable &vi) { return; }

void Package::insert(AST_Argument_Modification x) {}

string Package::name() const { return _name; }

ImportTable Package::imports() const { return _imports; }

FunctionTable Package::definitions() { return _functions; }

string Package::fileName() { return Utils::instance().packageName(_name); }

string Package::prefix() { return "__" + _name + "__"; }

/* Model Class Implementation */

Model::Model()
    : _name(),
      _imports(),
      _symbols(),
      _types(),
      _annotations(_symbols),
      _calledFunctions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependencies(),
      _packages(),
      _initialCode(),
      _libraryDirectories(),
      _linkLibraries(),
      _includeDirectories(),
      _ast_equations(),
      _ast_statements(),
      _stateNbr(0),
      _discreteNbr(0),
      _algebraicNbr(0),
      _eventNbr(0),
      _outputNbr(0),
      _inputNbr(0),
      _derivativeId(1),
      _algebraicId(1),
      _eventId(1),
      _outputId(1),
      _inputId(1),
      _externalFunctions(false)
{
  _symbols.initialize(_types);
}

Model::Model(string name)
    : _name(name),
      _imports(),
      _symbols(),
      _types(),
      _annotations(_symbols),
      _calledFunctions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependencies(),
      _packages(),
      _initialCode(),
      _libraryDirectories(),
      _linkLibraries(),
      _includeDirectories(),
      _ast_equations(),
      _ast_statements(),
      _stateNbr(0),
      _discreteNbr(0),
      _algebraicNbr(0),
      _eventNbr(0),
      _outputNbr(0),
      _inputNbr(0),
      _derivativeId(1),
      _algebraicId(1),
      _eventId(1),
      _outputId(1),
      _inputId(1),
      _externalFunctions(false)
{
  _symbols.initialize(_types);
}

void Model::insert(VarName n, Variable &vi, DEC_Type type) { insert(n, vi); }

void Model::insert(VarName n, Variable &vi)
{
  vi.setName(n);
  if (vi.typePrefix() & TP_CONSTANT) {
    EvalInitExp eval(_symbols);
    vi.setValue(eval.apply(vi.exp()));
  }
  if (vi.typePrefix() & TP_DISCRETE) {
    vi.setOffset(_discreteNbr);
    _discreteNbr += vi.size();
  }
  _symbols.insert(n, vi);
}

void Model::setVariableOffset(Variable var, unsigned int &offset, Util::Variable::RealType type, bool set_variable_count)
{
  if (!var.hasOffset()) {
    var.setOffset(offset);
    var.setRealType(type);
    if (set_variable_count) {
      offset += var.size();
    }
  }
  _symbols.insert(var.name(), var);
}

void Model::setRealVariables(AST_Equation eq)
{
  AST_Equation_Equality eqe = eq->getAsEquality();
  if (eqe->left()->expressionType() == EXPDERIVATIVE) {
    AST_Expression_Derivative ed = eqe->left()->getAsDerivative();
    AST_Expression derArg = AST_ListFirst(ed->arguments());
    setVariableOffset(Utils::instance().variable(derArg, _symbols), _stateNbr, Variable::RealType::State);
  } else if (eqe->left()->expressionType() == EXPCOMPREF) {
    setVariableOffset(Utils::instance().variable(eqe->left(), _symbols), _algebraicNbr, Variable::RealType::Algebraic);
  } else if (eqe->left()->expressionType() == EXPOUTPUT) {
    AST_Expression_Output eout = eqe->left()->getAsOutput();
    AST_ExpressionList el = eout->expressionList();
    AST_ExpressionListIterator it;
    list<string> lvars;
    list<Index> lidx;
    foreach (it, el) {
      setVariableOffset(Utils::instance().variable(current_element(it), _symbols), _algebraicNbr, Variable::RealType::Algebraic);
    }
  } else {
    Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
  }
}

void Model::insert(AST_Equation eq)
{
  AST_Equation teq = ConvertEquation(eq, _symbols).get();
  _ast_equations.push_back(teq);
  if (teq->equationType() == EQEQUALITY) {
    setRealVariables(teq);
  } else if (teq->equationType() == EQFOR) {
    AST_Equation_For eqf = teq->getAsFor();
    AST_EquationList eqs = eqf->equationList();
    AST_EquationListIterator it;
    foreach (it, eqs) {
      if (current_element(it)->equationType() == EQFOR) {
        insert(current_element(it));
      } else {
        setRealVariables(current_element(it));
      }
    }
  } else {
    Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
  }
}

void Model::insert(AST_Statement stm) { insert(stm, false); }

void Model::insert(AST_External_Function_Call efc) { return; }

void Model::insert(AST_Statement stm, bool initial)
{
  AST_Statement st = ConvertStatement(stm, _symbols).get();
  if (initial) {
    _initialCode.insert(_statementId++, Statement(stm, _symbols, initial));
  } else {
    _ast_statements.push_back(st);
  }
}

void Model::addFunction(SymbolTable symbols, FunctionTable &fs)
{
  SymbolTable::iterator fit;
  for (string s = symbols.begin(fit); !symbols.end(fit); s = symbols.next(fit)) {
    Option<Function> ef = fs[s];
    if (ef) {
      SymbolTable libraries;
      _calledFunctions.insert(s, ef.get());
      FunctionAnnotation fa = ef->annotations();
      if (fa.hasIncludeDirectory()) {
        string in = fa.includeDirectory();
        _includeDirectories.insert(in, in);
      }
      if (fa.hasLibraryDirectory()) {
        string in = fa.libraryDirectory();
        _libraryDirectories.insert(in, in);
      }
      if (fa.hasLibraries()) {
        libraries = fa.libraries();
        _linkLibraries.merge(libraries);
      }
      CompiledFunction cf(s, fa.includeDirectory(), fa.libraryDirectory(), libraries);
      Utils::instance().addCompiledFunction(cf);
    }
  }
}

void Model::setCalledFunctions(FunctionTable &fs)
{
  EquationTable::iterator it;
  for (Equation eq = _derivatives.begin(it); !_derivatives.end(it); eq = _derivatives.next(it)) {
    addFunction(eq.calledFunctions(), fs);
  }
  for (Equation eq = _algebraics.begin(it); !_algebraics.end(it); eq = _algebraics.next(it)) {
    addFunction(eq.calledFunctions(), fs);
  }
  EventTable::iterator eit;
  for (Event ev = _events.begin(eit); !_events.end(eit); ev = _events.next(eit)) {
    addFunction(ev.zeroCrossing().calledFunctions(), fs);
    StatementTable::iterator sit;
    StatementTable stms = ev.positiveHandler();
    for (Statement stm = stms.begin(sit); !stms.end(sit); stm = stms.next(sit)) {
      addFunction(stm.calledFunctions(), fs);
    }
    stms = ev.negativeHandler();
    for (Statement stm = stms.begin(sit); !stms.end(sit); stm = stms.next(sit)) {
      addFunction(stm.calledFunctions(), fs);
    }
  }
}

void Model::insert(AST_Argument_Modification x)
{
  if (!_annotations.insert(x)) {
    Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error, "%s", x->name()->c_str());
  }
}

void Model::insert(string n)
{
  _imports.insert(n, n);
  if (!Utils::instance().readPackage(n, _packages)) {
    Error::instance().add(0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str());
  }
  Option<CompiledPackage> cp = _packages[n];
  if (cp) {
    Utils::instance().addCompiledFunctions(cp->definitions());
    _linkLibraries.merge(cp->linkLibraries());
    _libraryDirectories.merge(cp->libraryDirectories());
    _includeDirectories.merge(cp->includeDirectories());
  }
}

void Model::addEquation(AST_Equation eq, Option<Range> range)
{
  assert(eq->equationType() == EQEQUALITY);
  AST_Equation_Equality eqe = eq->getAsEquality();
  EQUATION::Type t = (_annotations.isClassic() ? EQUATION::ClassicDerivative : EQUATION::QSSDerivative);
  if (eqe->left()->expressionType() == EXPDERIVATIVE) {
    Equation mse(eq, _symbols, range, t, _derivativeId);
    _derivatives.insert(_derivativeId++, mse);
  } else if (eqe->left()->expressionType() == EXPCOMPREF) {
    Equation mse(eq, _symbols, range, EQUATION::Algebraic, _algebraicId);
    _algebraics.insert(_algebraicId++, mse);
  } else if (eqe->left()->expressionType() == EXPOUTPUT) {
    if (eqe->right()->expressionType() != EXPCALL) {
      Error::instance().add(eqe->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
    }
    AST_Expression_Output eout = eqe->left()->getAsOutput();
    AST_ExpressionList el = eout->expressionList();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      Equation mse(eq, _symbols, range, EQUATION::Algebraic, _algebraicId);
      _algebraics.insert(_algebraicId++, mse);
    }
  } else {
    Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
  }
}

void Model::reduceEquation(AST_Equation_Equality eq, list<AST_Equation> &new_eqs)
{
  ReductionFunctions<AST_Equation, ConvertContRed> reduction_functions(eq->right(), _symbols,
                                                                       Utils::instance().variable(eq->left(), _symbols));
  AST_Expression new_exp = reduction_functions.apply();
  eq->setRight(new_exp);
  list<AST_Equation> code = reduction_functions.code();
  new_eqs.insert(new_eqs.end(), code.begin(), code.end());
  list<Variable> variables = reduction_functions.variables();
  for (Variable v : variables) {
    setVariableOffset(v, _algebraicNbr, Variable::RealType::Algebraic);
  }
}

void Model::setEquations()
{
  list<AST_Equation> new_eqs;
  list<AST_Equation>::iterator it;
  for (it = _ast_equations.begin(); it != _ast_equations.end(); it++) {
    AST_Equation eq = current_element(it);
    if (eq->equationType() == EQEQUALITY) {
      reduceEquation(eq->getAsEquality(), new_eqs);
    } else if (eq->equationType() == EQFOR) {
      AST_Equation_For eqf = eq->getAsFor();
      AST_EquationList eqs = eqf->equationList();
      AST_EquationListIterator it;
      foreach (it, eqs) {
        assert(current_element(it)->equationType() == EQEQUALITY);
        reduceEquation(current_element(it)->getAsEquality(), new_eqs);
      }
    }
  }
  _ast_equations.insert(_ast_equations.end(), new_eqs.begin(), new_eqs.end());
  for (it = _ast_equations.begin(); it != _ast_equations.end(); it++) {
    AST_Equation eq = current_element(it);
    if (eq->equationType() == EQEQUALITY) {
      addEquation(eq, Option<Range>());
    } else if (eq->equationType() == EQFOR) {
      vector<int> begin, end;
      AST_Equation_For eqf = eq->getAsFor();
      Range range(eqf, _symbols);
      AST_EquationList eqs = eqf->equationList();
      AST_EquationListIterator it;
      foreach (it, eqs) {
        addEquation(current_element(it), range);
      }
    }
  }
}

void Model::addVariable(int id, Option<Range> range, EQUATION::Type type, unsigned int &offset)
{
  vector<int> s;
  if (range) {
    s.push_back(range->size());
  }
  TypePrefix eq_type = TP_EQ;
  if (type == EQUATION::Type::Output) {
    eq_type = TP_OUTPUT;
  }
  Variable var(newType_Integer(), eq_type, nullptr, nullptr, s, false);
  string var_name = EquationVariable::modelVariables(id, type);
  insert(var_name, var);
  Option<Variable> variable = _symbols[var_name];
  static bool DONT_INCREASE_OFFSET = false;
  setVariableOffset(variable.get(), offset, Variable::RealType::NotAssigned, DONT_INCREASE_OFFSET);
}

void Model::addEvent(AST_Statement stm, Option<Range> range)
{
  if (stm->statementType() == STWHEN) {
    AST_Statement_When sw = stm->getAsWhen();
    _annotations.eventComment(sw->comment());
    addVariable(_eventId, range, EQUATION::Type::ZeroCrossing, _eventNbr);
    Event event(sw->condition(), _eventId, _eventNbr, _symbols, range);
    _eventNbr += (range ? range->size() : 1);
    AST_StatementList stl = sw->statements();
    AST_StatementListIterator it;
    foreach (it, stl) {
      event.add(current_element(it));
    }
    if (sw->hasElsewhen()) {
      int new_event = false;
      AST_Statement_ElseList ewl = sw->else_when();
      AST_Statement_ElseListIterator ewit;
      foreach (ewit, ewl) {
        AST_Statement_Else se = current_element(ewit);
        Event else_event = event;
        if (!else_event.compare(se->condition())) {
          addVariable(_eventId + 1, range, EQUATION::Type::ZeroCrossing, _eventNbr);
          else_event = Event(se->condition(), _eventId + 1, _eventNbr, _symbols, range);
          _eventNbr += (range ? range->size() : 1);
          new_event = true;
        }
        AST_StatementList stel = se->statements();
        AST_StatementListIterator steit;
        foreach (steit, stel) {
          else_event.add(current_element(steit));
        }
        if (new_event) {
          _events.insert(_eventId++, else_event);
        } else {
          event = else_event;
        }
      }
    }
    _events.insert(_eventId++, event);
  }
}

void Model::reduceEvent(AST_Statement_When event)
{
  assert(event->statementType() == STWHEN);
  AST_StatementList stms = event->statements();
  AST_StatementListIterator stm_it;
  foreach (stm_it, stms) {
    reduceStatement(current_element(stm_it), stms, stm_it);
  }
  if (event->hasElsewhen()) {
    AST_Statement_ElseList else_when = event->else_when();
    AST_Statement_ElseListIterator else_when_it;
    foreach (else_when_it, else_when) {
      AST_Statement_Else else_stm = current_element(else_when_it);
      stms = else_stm->statements();
      foreach (stm_it, stms) {
        reduceStatement(current_element(stm_it), stms, stm_it);
      }
    }
  }
}

void Model::setEvents()
{
  Utils::instance().setSymbols(_symbols);
  list<AST_Statement>::iterator it;
  for (it = _ast_statements.begin(); it != _ast_statements.end(); it++) {
    AST_Statement stm = *it;
    if (stm->statementType() == STWHEN) {
      reduceEvent(stm->getAsWhen());
    } else if (stm->statementType() == STFOR) {
      AST_Statement_For for_stm = stm->getAsFor();
      AST_StatementList stms = for_stm->statements();
      AST_StatementListIterator stm_it;
      foreach (stm_it, stms) {
        reduceEvent(current_element(stm_it)->getAsWhen());
      }
    }
  }
  for (it = _ast_statements.begin(); it != _ast_statements.end(); it++) {
    AST_Statement stm = *it;
    if (stm->statementType() == STWHEN) {
      addEvent(stm, Option<Range>());
    } else if (stm->statementType() == STFOR) {
      AST_Statement_For stf = stm->getAsFor();
      Range range(stf, _symbols);
      AST_StatementList sts = stf->statements();
      AST_StatementListIterator stit;
      foreach (stit, sts) {
        addEvent(current_element(stit), range);
      }
    }
  }
}

void Model::setOutputs()
{
  list<AST_Expression> astOutputs = _annotations.output();
  list<AST_Expression>::iterator it;
  for (it = astOutputs.begin(); it != astOutputs.end(); it++) {
    ConvertOutputRange convert = ConvertOutputRange(_symbols);
    AST_Expression converted = convert.apply(*it);
    Option<Range> range = convert.range();
    addVariable(_outputId, range, EQUATION::Type::Output, _outputNbr);
    Equation eq(converted, _symbols, range, EQUATION::Output, _outputId, _outputNbr);
    _outputNbr += (range ? range->size() : 1);
    _outputs.insert(_outputId++, eq);
  }
}

void Model::addInput(Equation eq)
{
  if (!eq.autonomous()) {
    _inputNbr += (eq.hasRange() ? eq.range()->size() : 1);
    _inputs.insert(_inputId, Input(Index(eq.lhs()), eq.range(), _inputId));
    _inputId++;
  }
}

void Model::setInputs()
{
  EquationTable::iterator it;
  for (Equation eq = _derivatives.begin(it); !_derivatives.end(it); eq = _derivatives.next(it)) {
    if (!eq.autonomous()) {
      addInput(eq);
      continue;
    }
    EquationDependencyMatrix eqdm = _dependencies.DA();
    Option<Paths> eqd = eqdm[_derivatives.key(it)];
    if (eqd) {
      Paths::iterator eit;
      for (eit = eqd->begin(); eit != eqd->end(); eit++) {
        Option<Equation> alg = _algebraics[eit->ifce.equationId()];
        if (alg && !alg->autonomous()) {
          addInput(eq);
          break;
        }
      }
    }
  }
}

void Model::computeDependencies()
{
  _dependencies.compute(derivatives(), outputs(), algebraics(), events(), _symbols);
  ModelConfig::instance().setDependencies(_dependencies);
}

void Model::setModelConfig()
{
  ModelConfig::instance().setAlgebraics(_algebraics);
  ModelConfig::instance().setModelAnnotations(_annotations);
  ModelConfig::instance().setDerivatives(_derivatives);
}
}  // namespace IR
}  // namespace MicroModelica
