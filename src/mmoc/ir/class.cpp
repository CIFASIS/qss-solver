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
#include "../util/model_config.h"
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
      _localSymbols(),
      _statements(),
      _packages(),
      _arguments(),
      _output_nbr(0),
      _external_function_id(0),
      _statement_id(0),
      _external_functions()
{
}

Function::~Function() {}

VarSymbolTable Function::symbols() const { return ModelConfig::instance().symbols(); }

void Function::insert(string n)
{
  _imports.insert(n, n);
  if (!Utils::instance().readPackage(n, _packages)) {
    Error::instance().add(0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str());
  }
}

void Function::insert(AST_Equation eq) { return; }

void Function::insert(AST_Statement stm) { _statements.insert(++_statement_id, Statement(stm)); }

void Function::insert(AST_Statement stm, bool initial) { insert(stm); }

void Function::insert(AST_External_Function_Call efc)
{
  string lvalue;
  VariableLookup vl(symbols(), localSymbols());
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
  _external_functions.insert(++_external_function_id, ExternalFunction(lvalue, efc->name(), efc->args()));
}

void Function::insert(VarName n, Variable &vi, DEC_Type type)
{
  EvalInitExp eval;
  vi.setName(n);
  if (vi.typePrefix() & TP_CONSTANT) {
    vi.setValue(eval.apply(vi.modification()->getAsEqual()->exp()));
  }
  ModelConfig::instance().addVariable(n, vi);
  if (type == DEC_PUBLIC) {
    if (vi.isOutput()) {
      _output_nbr++;
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

ExternalFunctionTable Function::externalFunctions() const { return _external_functions; }

CompiledPackageTable Function::packages() const { return _packages; }

unsigned int Function::outputNbr() const { return _output_nbr; }

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
      _annotations(),
      _called_functions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependencies(),
      _packages(),
      _initial_code(),
      _library_directories(),
      _link_libraries(),
      _include_directories(),
      _ast_equations(),
      _ast_statements(),
      _state_nbr(0),
      _discrete_nbr(0),
      _algebraic_nbr(0),
      _event_nbr(0),
      _output_nbr(0),
      _input_nbr(0),
      _derivative_id(1),
      _algebraic_id(1),
      _event_id(1),
      _output_id(1),
      _input_id(1),
      _external_functions(false)
{
}

Model::Model(string name)
    : _name(name),
      _imports(),
      _annotations(),
      _called_functions(),
      _derivatives(),
      _algebraics(),
      _events(),
      _dependencies(),
      _packages(),
      _initial_code(),
      _library_directories(),
      _link_libraries(),
      _include_directories(),
      _ast_equations(),
      _ast_statements(),
      _state_nbr(0),
      _discrete_nbr(0),
      _algebraic_nbr(0),
      _event_nbr(0),
      _output_nbr(0),
      _input_nbr(0),
      _derivative_id(1),
      _algebraic_id(1),
      _event_id(1),
      _output_id(1),
      _input_id(1),
      _external_functions(false)
{
}

VarSymbolTable Model::symbols() const { return ModelConfig::instance().symbols(); };

void Model::insert(VarName n, Variable &vi, DEC_Type type) { insert(n, vi); }

void Model::insert(VarName n, Variable &vi)
{
  vi.setName(n);
  if (vi.typePrefix() & TP_CONSTANT) {
    EvalInitExp eval;
    vi.setValue(eval.apply(vi.exp()));
  }
  if (vi.typePrefix() & TP_DISCRETE) {
    vi.setOffset(_discrete_nbr);
    _discrete_nbr += vi.size();
  }
  ModelConfig::instance().addVariable(n, vi);
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
  ModelConfig::instance().addVariable(var.name(), var);
}

void Model::setRealVariables(AST_Equation eq)
{
  AST_Equation_Equality eqe = eq->getAsEquality();
  if (eqe->left()->expressionType() == EXPDERIVATIVE) {
    AST_Expression_Derivative ed = eqe->left()->getAsDerivative();
    AST_Expression derArg = AST_ListFirst(ed->arguments());
    setVariableOffset(Utils::instance().variable(derArg), _state_nbr, Variable::RealType::State);
  } else if (eqe->left()->expressionType() == EXPCOMPREF) {
    setVariableOffset(Utils::instance().variable(eqe->left()), _algebraic_nbr, Variable::RealType::Algebraic);
  } else if (eqe->left()->expressionType() == EXPOUTPUT) {
    AST_Expression_Output eout = eqe->left()->getAsOutput();
    AST_ExpressionList el = eout->expressionList();
    AST_ExpressionListIterator it;
    list<string> lvars;
    list<Index> lidx;
    foreach (it, el) {
      setVariableOffset(Utils::instance().variable(current_element(it)), _algebraic_nbr, Variable::RealType::Algebraic);
    }
  } else {
    Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
  }
}

void Model::insert(AST_Equation eq)
{
  AST_Equation teq = ConvertEquation(eq).get();
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
  AST_Statement st = ConvertStatement(stm).get();
  if (initial) {
    _initial_code.insert(_statement_id++, Statement(stm, initial));
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
      _called_functions.insert(s, ef.get());
      FunctionAnnotation fa = ef->annotations();
      if (fa.hasIncludeDirectory()) {
        string in = fa.includeDirectory();
        _include_directories.insert(in, in);
      }
      if (fa.hasLibraryDirectory()) {
        string in = fa.libraryDirectory();
        _library_directories.insert(in, in);
      }
      if (fa.hasLibraries()) {
        libraries = fa.libraries();
        _link_libraries.merge(libraries);
      }
      CompiledFunction cf(s, fa.includeDirectory(), fa.libraryDirectory(), libraries);
      Utils::instance().addCompiledFunction(cf);
      _external_functions = true;
    }
  } 
}

void Model::setCalledFunctions(FunctionTable &fs)
{
  StatementTable::iterator stm_it;
  for (Statement stm = _initial_code.begin(stm_it); !_initial_code.end(stm_it); stm = _initial_code.next(stm_it)) {
    addFunction(stm.calledFunctions(), fs);
  }
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
    _link_libraries.merge(cp->linkLibraries());
    _library_directories.merge(cp->libraryDirectories());
    _include_directories.merge(cp->includeDirectories());
  }
}

void Model::addEquation(AST_Equation eq, Option<Range> range)
{
  assert(eq->equationType() == EQEQUALITY);
  AST_Equation_Equality eqe = eq->getAsEquality();
  EQUATION::Type t = (_annotations.isClassic() ? EQUATION::ClassicDerivative : EQUATION::QSSDerivative);
  if (eqe->left()->expressionType() == EXPDERIVATIVE) {
    Equation mse(eq, range, t, _derivative_id);
    _annotations.expComment(eqe->comment(), _derivative_id);
    _derivatives.insert(_derivative_id++, mse);
  } else if (eqe->left()->expressionType() == EXPCOMPREF) {
    Equation mse(eq, range, EQUATION::Algebraic, _algebraic_id);
    _algebraics.insert(_algebraic_id++, mse);
  } else if (eqe->left()->expressionType() == EXPOUTPUT) {
    if (eqe->right()->expressionType() != EXPCALL) {
      Error::instance().add(eqe->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
    }
    AST_Expression_Output eout = eqe->left()->getAsOutput();
    AST_ExpressionList el = eout->expressionList();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      Equation mse(eq, range, EQUATION::Algebraic, _algebraic_id);
      _algebraics.insert(_algebraic_id++, mse);
    }
  } else {
    Error::instance().add(eq->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
  }
}

void Model::reduceEquation(AST_Equation_Equality eq, list<AST_Equation> &new_eqs)
{
  ReductionFunctions<AST_Equation, ConvertContRed> reduction_functions(eq->right(), 
                                                                       Utils::instance().variable(eq->left()));
  AST_Expression new_exp = reduction_functions.apply();
  eq->setRight(new_exp);
  list<AST_Equation> code = reduction_functions.code();
  new_eqs.insert(new_eqs.end(), code.begin(), code.end());
  list<Variable> variables = reduction_functions.variables();
  for (Variable v : variables) {
    setVariableOffset(v, _algebraic_nbr, Variable::RealType::Algebraic);
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
      Range range(eqf);
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
    RangeDefinitionTable ranges = range->definition();
    RangeDefinitionTable::iterator it;
    for (RangeDefinition def = ranges.begin(it); !ranges.end(it); def = ranges.next(it)) {
      s.push_back(def.size());
    }
  }
  TypePrefix eq_type = TP_EQ;
  if (type == EQUATION::Type::Output) {
    eq_type = TP_OUTPUT;
  }
  Variable var(newType_Integer(), eq_type, nullptr, nullptr, s, !s.empty());
  string var_name = EquationVariable::modelVariables(id, type);
  insert(var_name, var);
  Option<Variable> variable = ModelConfig::instance().lookup(var_name);
  static bool DONT_INCREASE_OFFSET = false;
  setVariableOffset(variable.get(), offset, Variable::RealType::NotAssigned, DONT_INCREASE_OFFSET);
}

void Model::addEvent(AST_Statement stm, Option<Range> range)
{
  if (stm->statementType() == STWHEN) {
    AST_Statement_When sw = stm->getAsWhen();
    _annotations.expComment(sw->comment(), _event_id);
    addVariable(_event_id, range, EQUATION::Type::ZeroCrossing, _event_nbr);
    Event event(sw->condition(), _event_id, _event_nbr, range, _annotations.EventId());
    _event_nbr += (range ? range->size() : 1);
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
          addVariable(_event_id + 1, range, EQUATION::Type::ZeroCrossing, _event_nbr);
          else_event = Event(se->condition(), _event_id + 1, _event_nbr, range, _annotations.EventId());
          _event_nbr += (range ? range->size() : 1);
          new_event = true;
        }
        AST_StatementList stel = se->statements();
        AST_StatementListIterator steit;
        foreach (steit, stel) {
          else_event.add(current_element(steit));
        }
        if (new_event) {
          _events.insert(_event_id++, else_event);
        } else {
          event = else_event;
        }
      }
    }
    _events.insert(_event_id++, event);
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
      Range range(stf);
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
  list<AST_Expression> ast_outputs = _annotations.output();
  list<AST_Expression>::iterator it;
  for (it = ast_outputs.begin(); it != ast_outputs.end(); it++) {
    ConvertOutputRange convert;
    AST_Expression converted = convert.apply(*it);
    Option<Range> range = convert.range();
    addVariable(_output_id, range, EQUATION::Type::Output, _output_nbr);
    Equation eq(converted, range, EQUATION::Output, _output_id, _output_nbr);
    _output_nbr += (range ? range->size() : 1);
    _outputs.insert(_output_id++, eq);
  }
}

void Model::addInput(Equation eq)
{
  if (!eq.autonomous()) {
    _input_nbr += (eq.hasRange() ? eq.range()->size() : 1);
    _inputs.insert(_input_id, Input(Index(eq.lhs()), eq.range(), _input_id));
    _input_id++;
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
  }
}

void Model::computeDependencies()
{
  _dependencies.compute(derivatives(), outputs(), algebraics(), events());
  ModelConfig::instance().setDependencies(_dependencies);
}

void Model::setModelConfig()
{
  ModelConfig::instance().setAlgebraics(_algebraics);
  ModelConfig::instance().setModelAnnotations(_annotations);
  ModelConfig::instance().setDerivatives(_derivatives);
  ModelConfig::instance().setStateNbr(_state_nbr);
  ModelConfig::instance().setEvents(_events);
}
}  // namespace IR
}  // namespace MicroModelica
