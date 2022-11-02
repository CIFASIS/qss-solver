/*****************************************************************************

 This file is part of QSSModelInstance Solver.

 QSSModelInstance Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSSModelInstance Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSSModelInstance Solver.  If not, see
 <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "model_instance.h"

#include <boost/optional/optional_io.hpp>
#include <utility>

#include <deps/builders/eq_graph_builder.h>
#include <ast/expression.h>
#include <generator/macros.h>
#include <ir/annotation.h>
#include <ir/class.h>
#include <ir/equation.h>
#include <ir/event.h>
#include <ir/expression.h>
#include <ir/jacobian.h>
#include <ir/qss_model.h>
#include <ir/qss_model_deps.h>
#include <ir/statement.h>
#include <util/error.h>
#include <util/util.h>
#include <util/visitors/partial_eval_exp.h>

namespace MicroModelica {

namespace Generator {

using namespace MicroModelica::IR;
using namespace MicroModelica::Util;
using namespace MicroModelica::Deps;

ModelInstance::ModelInstance() : _model(), _flags(), _writer() {}

ModelInstance::ModelInstance(Model &model, CompileFlags &flags, WriterPtr writer) : _model(model), _flags(flags), _writer(writer) {}

void ModelInstance::include()
{
  stringstream buffer;
  buffer << "#include <stdlib.h>" << endl;
  buffer << "#include <stdio.h>" << endl;
  buffer << "#include <string.h>" << endl;
  buffer << "#include <math.h>" << endl;
  buffer << endl;
  buffer << "#include \"" << _model.name() << ".h\"" << endl;
  if (_model.externalFunctions()) {
    buffer << "#include \"" << _model.name() << "_functions.h\"" << endl;
  }
  ImportTable imports = _model.imports();
  ImportTable::iterator it;
  for (string i = imports.begin(it); !imports.end(it); i = imports.next(it)) {
    buffer << "#include \"" << Utils::instance().packageName(i) << ".h\"" << endl;
  }
  buffer << "#include <common/utils.h>" << endl;
  buffer << "#include <common/model.h>" << endl;
  buffer << "#include <common/commands.h>" << endl;
  buffer << "#include <qss/qss_model.h>" << endl;
  buffer << "#include <classic/classic_model.h>" << endl;
  buffer << endl;
  _writer->write(buffer, WRITER::Include);
}

void ModelInstance::allocateOutput()
{
  stringstream buffer;
  ModelAnnotation annot = _model.annotations();
  string period = "NULL";
  string indent = "";
  int ssize = 0;
  if (annot.commInterval() == "CI_Sampled") {
    indent = _writer->indent(1);
    list<double> sample = annot.sample();
    ssize = sample.size();
    period = "period";
    buffer << indent << "double period[" << ssize << "];" << endl;
    int n = 0;
    for (list<double>::iterator i = sample.begin(); i != sample.end(); i++) {
      buffer << indent << "period[" << n << "] = " << *i << ";" << endl;
    }
  }
  string outputFunction = (_model.outputNbr() ? "MOD_output" : "NULL");
  buffer << indent << "simulator->output = SD_Output(\"";
  buffer << _model.name() << "\",";
  buffer << _model.outputNbr() << ",";
  buffer << _model.discreteNbr() << ",";
  buffer << _model.stateNbr() << ",";
  buffer << period << ",";
  buffer << ssize << ",";
  buffer << annot.initialTime() << ",";
  buffer << annot.commInterval() << ",";
  buffer << annot.storeData() << ",";
  buffer << outputFunction << ");" << endl;
  buffer << indent << "SD_output modelOutput = simulator->output;" << endl;
  _writer->write(buffer, WRITER::Alloc_Output);
}

void ModelInstance::configOutput()
{
  if (!_model.outputNbr()) {
    return;
  }
  stringstream buffer;
  buffer << "SD_allocOutputMatrix(";
  buffer << "modelOutput, ";
  buffer << _model.stateNbr() << ", ";
  buffer << _model.discreteNbr() << ");";
  _writer->write(buffer, WRITER::Alloc_Output);
  EquationTable outputs = _model.outputs();
  EquationTable::iterator it;
  FunctionPrinter fp;
  for (Equation out = outputs.begin(it); !outputs.end(it); out = outputs.next(it)) {
    assert(out.isValid());
    Expression var = out.lhs();
    if (out.isRHSReference()) {
      var = out.rhs();
    }
    string tabs = "";
    Option<Range> range = out.range();
    if (range) {
      buffer << range.get();
      tabs = range->block();
    }
    PartialEvalExp partial_eval;
    Expression out_index_exp = partial_eval.apply(out.lhs().expression());
    buffer << tabs << "sprintf(modelOutput->variable[" << Index(out_index_exp) << "].name, " << fp.outputVariableName(var, range) << ");";
    if (range) {
      buffer << endl << range->end();
    }
    _writer->write(buffer, WRITER::Alloc_Output);
  }
}

void ModelInstance::output()
{
  generateDef<OutputModelGen>(_model.outputs(), WRITER::Output, WRITER::Output_Simple, WRITER::Output_Generic);
}

void ModelInstance::zeroCrossing()
{
  generateDef<ZCModelGen>(zeroCrossingTable(_model.events()), WRITER::Zero_Crossing, WRITER::ZC_Simple, WRITER::ZC_Generic);
}

void ModelInstance::handler()
{
  EventTable events = _model.events();
  EventTable::iterator it;
  VarSymbolTable symbols = _model.symbols();
  stringstream buffer;
  FunctionPrinter fp;
  ModelConfig::instance().clearLocalSymbols();
  for (Event event = events.begin(it); !events.end(it); event = events.next(it)) {
    _writer->write(event.handler(EVENT::Positive), (event.hasRange() ? WRITER::Handler_Pos_Generic : WRITER::Handler_Pos_Simple));
    _writer->write(event.handler(EVENT::Negative), (event.hasRange() ? WRITER::Handler_Neg_Generic : WRITER::Handler_Neg_Simple));
  }
  if (!_writer->isEmpty(WRITER::Handler_Pos_Generic)) {
    _writer->write(ModelConfig::instance().localSymbols(), WRITER::Handler_Pos);
  }
  if (!_writer->isEmpty(WRITER::Handler_Neg_Generic)) {
    _writer->write(ModelConfig::instance().localSymbols(), WRITER::Handler_Neg);
  }
  if (!_writer->isEmpty(WRITER::Handler_Pos_Simple)) {
    _writer->write(fp.beginSwitch(), WRITER::Handler_Pos);
    _writer->write(fp.endSwitch(), WRITER::Handler_Pos_Simple);
  }
  if (!_writer->isEmpty(WRITER::Handler_Neg_Simple)) {
    _writer->write(fp.beginSwitch(), WRITER::Handler_Neg);
    _writer->write(fp.endSwitch(), WRITER::Handler_Neg_Simple);
  }
}

void ModelInstance::configEvents()
{
  EventTable events = _model.events();
  EventTable::iterator it;
  for (Event event = events.begin(it); !events.end(it); event = events.next(it)) {
    _writer->write(event.config(), WRITER::Init_Data);
  }
}

void ModelInstance::settings()
{
  stringstream buffer;
  buffer << "settings->debug = " << _flags.debug() << ";";
  _writer->print(buffer);
  buffer << "settings->parallel = ";
  buffer << (_model.annotations().parallel() ? "TRUE" : "FALSE") << ";";
  _writer->print(buffer);
  buffer << "settings->hybrid = ";
  buffer << (_model.eventNbr() ? "TRUE" : "FALSE") << ";";
  _writer->print(buffer);
  buffer << "settings->method = " << _model.annotations().solver() << ";";
  _writer->print(buffer);
  unsigned long random_seed = _model.annotations().randomSeed();
  if (random_seed) {
    buffer << "settings->random_seed = " << _model.annotations().randomSeed() << ";";
    _writer->print(buffer);
  }
}

void ModelInstance::header()
{
  VarSymbolTable symbols = _model.symbols();
  VarSymbolTable::iterator it;
  stringstream buffer;
  _writer->write("// Model data access macro.\n\n", WRITER::Model_Header);
  Macros access_macro;
  buffer << access_macro.modelAccess(_model.discreteNbr(), _model.algebraicNbr());
  _writer->write(buffer, WRITER::Model_Header);
  if (ModelConfig::instance().isQss()) {
    _writer->write("// Coeff multipliers definition.\n\n", WRITER::Model_Header);
    buffer << access_macro.coeffMultipliers(_model.annotations().order());
    _writer->write(buffer, WRITER::Model_Header);
  }
  _writer->write("// Model Variables Macros\n\n", WRITER::Model_Header);
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (var.isModelVar()) {
      Macros macros(_model, var);
      buffer << "// Macros definition for variable: " << var.name() << endl;
      buffer << macros << endl;
      ;
    }
  }
  _writer->write(buffer, WRITER::Model_Header);
  if (symbols.parameters()) {
    _writer->write("\n// Model Parameters Declaration\n\n", WRITER::Model_Header);
  }
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (var.isParameter()) {
      buffer << "// Macro for parameter: " << var.name() << endl;
      buffer << var.declaration("__PAR__") << endl;
    }
    _writer->write(buffer, WRITER::Model_Header);
  }
  EquationTable derivatives = _model.derivatives();
  EquationTable::iterator eqit;
  if (derivatives.size()) {
    _writer->write("\n// Derivative Equations Macros\n\n", WRITER::Model_Header);
  }
  for (Equation e = derivatives.begin(eqit); !derivatives.end(eqit); e = derivatives.next(eqit)) {
    buffer << "// Macros for equation: " << e.id() << endl;
    buffer << e.macro() << endl;
    _writer->write(buffer, WRITER::Model_Header);
  }
  EquationTable algebraics = _model.algebraics();
  if (algebraics.size()) {
    _writer->write("\n// Algebraic Equations Macros\n\n", WRITER::Model_Header);
  }
  for (Equation e = algebraics.begin(eqit); !algebraics.end(eqit); e = algebraics.next(eqit)) {
    buffer << "// Macros for algebraic equation: " << e.id() << endl;
    buffer << e.macro() << endl;
    _writer->write(buffer, WRITER::Model_Header);
  }
  EventTable events = _model.events();
  if (events.size()) {
    _writer->write("\n// Event Macros\n\n", WRITER::Model_Header);
  }
  EventTable::iterator eit;
  for (Event e = events.begin(eit); !events.end(eit); e = events.next(eit)) {
    buffer << "// Macros for event: " << e.id() << endl;
    buffer << e.macro() << endl;
    _writer->write(buffer, WRITER::Model_Header);
  }
  if (events.size()) {
    if (ModelConfig::instance().isQss()) {
      _writer->write("#define _zc(coeff) zc[coeff]", WRITER::Model_Header);
    } else {
      _writer->write("#define _zc zc[0]", WRITER::Model_Header);
    }
  }
  EquationTable outputs = _model.outputs();
  if (outputs.size()) {
    _writer->write("\n// Output Equations Macros\n\n", WRITER::Model_Header);
  }
  for (Equation e = outputs.begin(eqit); !outputs.end(eqit); e = outputs.next(eqit)) {
    buffer << "// Macros for output equation: " << e.id() << endl;
    buffer << e.macro() << endl;
    _writer->write(buffer, WRITER::Model_Header);
  }
  if (outputs.size()) {
    _writer->write("#define _out out[0]", WRITER::Model_Header);
  }
  InputTable inputs = _model.inputs();
  if (inputs.size()) {
    _writer->write("\n// Input Matrix Macros\n\n", WRITER::Model_Header);
  }
  InputTable::iterator iit;
  for (Input i = inputs.begin(iit); !inputs.end(iit); i = inputs.next(iit)) {
    _writer->write(i.macro(), WRITER::Model_Header);
  }
  buffer << endl << "// Jacobian Macros definition. " << endl;
  _writer->write(buffer, WRITER::Model_Header);
  buffer << "#define _assign_jac(r, val) \\" << endl;
  buffer << "    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \\" << endl;
  buffer << "    dvdx->df_dx_t->index[r][0]++; \\" << endl;
  buffer << "    jac[col_t] = val;" << endl;
  _writer->write(buffer, WRITER::Model_Header);
  buffer << "#define _c_index(i) (i-1)";
  _writer->write(buffer, WRITER::Model_Header);
  buffer << endl;
  buffer << "#define _time t";
  _writer->write(buffer, WRITER::Model_Header);
}

string ModelInstance::componentDefinition(MODEL_INSTANCE::Component c)
{
  string handler_q_param = (ModelConfig::instance().isQss()) ? ", double* q" : "";
  switch (c) {
  case MODEL_INSTANCE::Model_Settings:
    return "void MOD_settings(SD_simulationSettings settings)";
  case MODEL_INSTANCE::Model: {
    stringstream buffer;
    buffer << "void MOD_definition(";
    if (ModelConfig::instance().isQss()) {
      buffer << "int idx, ";
    }
    buffer << "double *x, double *d, double *a, double t, double *dx)";
    return buffer.str();
  }
  case MODEL_INSTANCE::Deps:
    return "void MOD_dependencies(int idx, double *x, double *d, double *a, "
           "double t, double *dx, int *map)";
  case MODEL_INSTANCE::Zero_Crossing:
    return "void MOD_zeroCrossing(int idx, double *x, double *d, double *a, "
           "double t, double *zc)";
  case MODEL_INSTANCE::Handler_Pos:
    return "void MOD_handlerPos(int idx, double *x" + handler_q_param +
           ", double *d, double *a, "
           "double t)";
  case MODEL_INSTANCE::Handler_Neg:
    return "void MOD_handlerNeg(int idx, double *x" + handler_q_param +
           ", double *d, double *a, "
           "double t)";
  case MODEL_INSTANCE::Output:
    return "void MOD_output(int idx, double *x, double *d, double *a, double "
           "t, double *out)";
  case MODEL_INSTANCE::Jacobian:
    return "void MOD_jacobian(double *x, double *d, double *a, double t, "
           "SD_jacMatrices dvdx, double *jac)";
  case MODEL_INSTANCE::BdfModel:
    return "void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)";
  case MODEL_INSTANCE::CLC_Init:
    return "void CLC_initializeDataStructs(CLC_simulator simulator)";
  case MODEL_INSTANCE::QSS_Init:
    return "void QSS_initializeDataStructs(QSS_simulator simulator)";
  }
  return "";
}

void ModelInstance::initialCode()
{
  StatementTable stms = _model.initialCode();
  StatementTable::iterator it;
  stringstream buffer;
  ModelConfig::instance().setLocalInitSymbols();
  ModelConfig::instance().setInitialCode(true);
  VarSymbolTable symbols = _model.symbols();
  VarSymbolTable::iterator var_it;
  for (Variable var = symbols.begin(var_it); !symbols.end(var_it); var = symbols.next(var_it)) {
    if (var.isConstant()) {
      continue;
    }
    _writer->write(var.initialization(), WRITER::Init_Code);
  }
  for (Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it)) {
    _writer->write(stm, WRITER::Init_Code);
  }
  ModelConfig::instance().setInitialCode(false);
  ModelConfig::instance().unsetLocalInitSymbols();
}

void ModelInstance::inputs()
{
  InputTable inputs = _model.inputs();
  InputTable::iterator it;
  for (Input input = inputs.begin(it); !inputs.end(it); input = inputs.next(it)) {
    _writer->write(input, WRITER::Input);
  }
}

string ModelInstance::allocateModel()
{
  stringstream buffer;
  buffer << "MOD_zeroCrossing, ";
  buffer << "MOD_handlerPos, ";
  buffer << "MOD_handlerNeg, ";
  buffer << "MOD_jacobian";
  return buffer.str();
}

void ModelInstance::allocateVector(string name, int size) const
{
  stringstream buffer;
  if (size) {
    buffer << "int* " << name << " = (int*) malloc(" << size << "*sizeof(int));" << endl;
    _writer->write(buffer, WRITER::Prologue);
  }
}

void ModelInstance::freeVector(string name, int size) const
{
  stringstream buffer;
  if (size) {
    buffer << "free(" << name << ");" << endl;
    _writer->write(buffer, WRITER::Epilogue);
  }
}

void ModelInstance::allocateVectors() const
{
  allocateVector("states", _model.stateNbr());
  allocateVector("discretes", _model.discreteNbr());
  allocateVector("events", _model.eventNbr());
  allocateVector("outputs", _model.outputNbr());
}

void ModelInstance::freeVectors() const
{
  freeVector("states", _model.stateNbr());
  freeVector("discretes", _model.discreteNbr());
  freeVector("events", _model.eventNbr());
  freeVector("outputs", _model.outputNbr());
}

void ModelInstance::jacobian()
{
  Jacobian jac;
  ModelConfig::instance().clearLocalSymbols();
  jac.build();
  _writer->write("int row, row_t, eq_var, c_row, c_row_g;", WRITER::Jacobian);
  _writer->write("int col, col_g, col_t;", WRITER::Jacobian);
  _writer->write("int x_ind;", WRITER::Jacobian);
  _writer->write("double aux;", WRITER::Jacobian);
  _writer->write(ModelConfig::instance().localSymbols(), WRITER::Jacobian);
  _writer->write("SD_cleanJacMatrices(dvdx);", WRITER::Jacobian);
  _writer->write(jac.code(), WRITER::Jacobian);
  // Add local variables to the initialization procedure prologue.
  _writer->write("int row, eq_var, c_row;", WRITER::Prologue);
  _writer->write("int x_ind;", WRITER::Prologue);
}

void ModelInstance::generate()
{
  include();
  zeroCrossing();
  handler();
  output();
  initialCode();
  jacobian();
  // Print generated Model Instance.
  _writer->print(WRITER::Include);
  _writer->print(componentDefinition(MODEL_INSTANCE::Model_Settings));
  _writer->beginBlock();
  settings();
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::Model));
  _writer->beginBlock();
  _writer->print(WRITER::Model);
  _writer->print(WRITER::Model_Simple);
  _writer->print(WRITER::Model_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::Zero_Crossing));
  _writer->beginBlock();
  _writer->print(WRITER::Zero_Crossing);
  _writer->print(WRITER::ZC_Simple);
  _writer->print(WRITER::ZC_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::Handler_Pos));
  _writer->beginBlock();
  _writer->print(WRITER::Handler_Pos);
  _writer->print(WRITER::Handler_Pos_Simple);
  _writer->print(WRITER::Handler_Pos_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::Handler_Neg));
  _writer->beginBlock();
  _writer->print(WRITER::Handler_Neg);
  _writer->print(WRITER::Handler_Neg_Simple);
  _writer->print(WRITER::Handler_Neg_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::Output));
  _writer->beginBlock();
  _writer->print(WRITER::Output);
  _writer->print(WRITER::Output_Simple);
  _writer->print(WRITER::Output_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::Jacobian));
  _writer->beginBlock();
  _writer->print(WRITER::Jacobian);
  _writer->endBlock();
  _writer->write(ModelConfig::instance().localInitSymbols(), WRITER::Prologue);
}

/* QSSModelInstance Model Instance class. */

QSSModelInstance::QSSModelInstance() : ModelInstance(), _model(), _flags(), _writer() {}

QSSModelInstance::QSSModelInstance(Model &model, CompileFlags &flags, WriterPtr writer)
    : ModelInstance(model, flags, writer), _model(model), _flags(flags), _writer(writer)
{
}

void QSSModelInstance::definition()
{
  generateDef<QSSModelGen>(_model.derivatives(), WRITER::Model, WRITER::Model_Simple, WRITER::Model_Generic);
}

void QSSModelInstance::dependencies()
{
  generateDef<QSSModelDeps>(_model.derivatives(), WRITER::Model_Deps, WRITER::Model_Deps_Simple, WRITER::Model_Deps_Generic);
}

void QSSModelInstance::bdfDefinition()
{
  bool sym_diff = ModelConfig::instance().symDiff();
  ModelConfig::instance().setSymDiff(false);
  _writer->write("int idx;", WRITER::Model_Bdf);
  _writer->write("int __bdf_it;", WRITER::Model_Bdf);
  _writer->write("for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {", WRITER::Model_Bdf);
  _writer->write("idx = BDFMap[__bdf_it];", WRITER::Model_Bdf);
  generateDef<QSSModelGen>(_model.BDFDerivatives(), WRITER::Model_Bdf, WRITER::Model_Bdf_Simple, WRITER::Model_Bdf_Generic);
  _writer->write("}", WRITER::Model_Bdf_Generic);
  ModelConfig::instance().setSymDiff(sym_diff);
}

void QSSModelInstance::allocateSolver()
{
  ModelAnnotation annot = _model.annotations();
  stringstream buffer;
  buffer << "simulator->data = QSS_Data(" << _model.stateNbr() << ",";
  buffer << _model.discreteNbr() << ",";
  buffer << _model.eventNbr() << ",";
  buffer << _model.inputNbr() << ",";
  buffer << _model.algebraicNbr() << ",";
  buffer << _model.derivatives().size() << ",";
  buffer << _model.algebraics().size() << ",\"";
  buffer << _model.name() << "\");" << endl;
  buffer << "QSS_data modelData = simulator->data;" << endl;
  buffer << "MODEL_DATA_ACCESS(modelData)" << endl;
  _writer->write(buffer, WRITER::Prologue);
}

string QSSModelInstance::allocateModel()
{
  stringstream buffer;
  buffer << "simulator->model = QSS_Model(MOD_definition, ";
  buffer << "MOD_dependencies, ";
  buffer << ModelInstance::allocateModel();
  buffer << ", MOD_BDF_definition);";
  return buffer.str();
}

void QSSModelInstance::initTime()
{
  stringstream buffer;
  buffer << "simulator->time = QSS_Time(" << _model.stateNbr() << "," << _model.eventNbr() << "," << _model.inputNbr() << ","
         << _model.annotations().initialTime() << "," << _model.annotations().scheduler() << ", NULL);" << endl;
  _writer->write(buffer, WRITER::Init_Data);
}

void QSSModelInstance::initializeDataStructures()
{
  stringstream buffer;
  ModelConfig::instance().setLocalInitSymbols();
  const bool PARALLEL = _model.annotations().parallel();
  allocateSolver();
  allocateVectors();
  freeVectors();
  ModelDependencies deps = _model.dependencies();
  // Initialize Solver Data Structures.
  initializeMatrix(deps.SD(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());
  initializeMatrix(deps.DS(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());
  inputs();

  // Initialize Jacobian matrices.
  initializeMatrix(deps.JAC(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());

  // Initialize Event Data Structures.
  initializeMatrix(deps.SZ(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());
  initializeMatrix(deps.ZS(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());

  initializeMatrix(deps.HZ(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
  initializeMatrix(deps.HD(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
  initializeMatrix(deps.LHSDsc(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
  initializeMatrix(deps.LHSSt(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
  initializeMatrix(deps.RHSSt(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
  if (PARALLEL) {
    initializeMatrix(deps.DH(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
    initializeMatrix(deps.SH(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());
    initializeMatrix(deps.HH(), WRITER::Alloc_Data, WRITER::Init_Data, _model.eventNbr());
  }
  configEvents();
  _writer->write("QSS_allocDataMatrix(modelData);", WRITER::Alloc_Data);
  _writer->write("SD_setupJacMatrices(modelData->jac_matrices);", WRITER::Init_Data);
  // Initialize Output Data Structures.
  allocateOutput();
  initializeMatrix(deps.OS(), WRITER::Alloc_Output, WRITER::Init_Output, _model.outputNbr());
  initializeMatrix(deps.OD(), WRITER::Alloc_Output, WRITER::Init_Output, _model.outputNbr());
  initializeMatrix(deps.SO(), WRITER::Alloc_Output, WRITER::Init_Output, _model.stateNbr());
  initializeMatrix(deps.DO(), WRITER::Alloc_Output, WRITER::Init_Output, _model.discreteNbr());
  initTime();
  configOutput();
  allocateModel();
  ModelConfig::instance().unsetLocalInitSymbols();
}

Graph QSSModelInstance::computationalGraph() { return Graph(0, 0); }

void QSSModelInstance::generate()
{
  definition();
  dependencies();
  bdfDefinition();
  initializeDataStructures();
  ModelInstance::generate();
  _writer->print(componentDefinition(MODEL_INSTANCE::Deps));
  _writer->beginBlock();
  _writer->print(WRITER::Model_Deps);
  _writer->print(WRITER::Model_Deps_Simple);
  _writer->print(WRITER::Model_Deps_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::BdfModel));
  _writer->beginBlock();
  _writer->print(WRITER::Model_Bdf);
  _writer->print(WRITER::Model_Bdf_Simple);
  _writer->print(WRITER::Model_Bdf_Generic);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::QSS_Init));
  _writer->beginBlock();
  _writer->print(WRITER::Prologue);
  _writer->print(WRITER::Init_Code);
  _writer->print(WRITER::Alloc_Data);
  _writer->print(WRITER::Init_Data);
  _writer->print(WRITER::Input);
  _writer->print(WRITER::Alloc_Output);
  _writer->print(WRITER::Init_Output);
  _writer->print(allocateModel());
  _writer->print(WRITER::Epilogue);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::CLC_Init));
  _writer->beginBlock();
  _writer->endBlock();
}

void QSSModelInstance::header()
{
  ModelInstance::header();
  VarSymbolTable symbols = _model.symbols();
  VarSymbolTable::iterator it;
  stringstream buffer;
  buffer << endl;
  buffer << "// Derivative Macros definition. " << endl;
  _writer->write(buffer, WRITER::Model_Header);
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (var.isState()) {
      stringstream buffer;
      Macros macros(_model, var);
      buffer << "// Derivative definition for variable: " << var.name() << endl;
      buffer << "#define _der" << var << macros.parameters() << " dx[coeff+1]";
      _writer->write(buffer, WRITER::Model_Header);
    }
  }
}

/* ClassicModelInstance Model Instance class. */

ClassicModelInstance::ClassicModelInstance(Model &model, CompileFlags &flags, WriterPtr writer)
    : ModelInstance(model, flags, writer), _model(model), _flags(flags), _writer(writer)
{
}

void ClassicModelInstance::definition()
{
  EquationTable derivatives = _model.derivatives();
  EquationTable algebraics = _model.algebraics();
  EquationTable::iterator it;
  VarSymbolTable symbols = _model.symbols();
  stringstream buffer;
  ModelConfig::instance().clearLocalSymbols();
  for (Equation alg = algebraics.begin(it); !algebraics.end(it); alg = algebraics.next(it)) {
    _writer->write(alg, WRITER::Model_Simple);
  }
  for (Equation der = derivatives.begin(it); !derivatives.end(it); der = derivatives.next(it)) {
    _writer->write(der, WRITER::Model_Simple);
  }
  _writer->write(ModelConfig::instance().localSymbols(), WRITER::Model);
}

void ClassicModelInstance::initializeDataStructures()
{
  stringstream buffer;
  ModelConfig::instance().setLocalInitSymbols();
  allocateSolver();
  allocateVectors();
  freeVectors();
  ModelDependencies deps = _model.dependencies();
  // Initialize Solver Data Structures.
  initializeMatrix(deps.SD(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());
  initializeMatrix(deps.DS(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());
  configEvents();
  inputs();
  // Initialize Jacobian matrices.
  initializeMatrix(deps.JAC(), WRITER::Alloc_Data, WRITER::Init_Data, _model.stateNbr());

  _writer->write("CLC_allocDataMatrix(modelData);", WRITER::Alloc_Data);
  _writer->write("SD_setupJacMatrices(modelData->jac_matrices);", WRITER::Init_Data);

  // Initialize Output Data Structures.
  allocateOutput();
  initializeMatrix(deps.OS(), WRITER::Alloc_Output, WRITER::Init_Output, _model.outputNbr());
  initializeMatrix(deps.OD(), WRITER::Alloc_Output, WRITER::Init_Output, _model.outputNbr());
  initializeMatrix(deps.SO(), WRITER::Alloc_Output, WRITER::Init_Output, _model.stateNbr());
  initializeMatrix(deps.DO(), WRITER::Alloc_Output, WRITER::Init_Output, _model.discreteNbr());
  configOutput();
  allocateModel();
  ModelConfig::instance().unsetLocalInitSymbols();
}

void ClassicModelInstance::allocateSolver()
{
  ModelAnnotation annot = _model.annotations();
  stringstream buffer;
  buffer << "simulator->data = CLC_Data(" << _model.stateNbr() << ",";
  buffer << _model.discreteNbr() << ",";
  buffer << _model.eventNbr() << ",";
  buffer << _model.inputNbr() << ",";
  buffer << _model.algebraicNbr() << ",";
  buffer << _model.derivatives().size() << ",";
  buffer << _model.algebraics().size() << ",\"";
  buffer << _model.name() << "\");" << endl;
  buffer << "CLC_data modelData = simulator->data;" << endl;
  buffer << "MODEL_DATA_ACCESS(modelData)" << endl;
  _writer->write(buffer, WRITER::Prologue);
}

string ClassicModelInstance::allocateModel()
{
  stringstream buffer;
  buffer << "simulator->model = CLC_Model(MOD_definition, ";
  buffer << ModelInstance::allocateModel();
  buffer << ");";
  return buffer.str();
}

void ClassicModelInstance::generate()
{
  definition();
  initializeDataStructures();
  ModelInstance::generate();
  _writer->print(componentDefinition(MODEL_INSTANCE::CLC_Init));
  _writer->beginBlock();
  _writer->print(WRITER::Prologue);
  _writer->print(WRITER::Init_Code);
  _writer->print(WRITER::Alloc_Data);
  _writer->print(WRITER::Init_Data);
  _writer->print(WRITER::Input);
  _writer->print(WRITER::Alloc_Output);
  _writer->print(WRITER::Init_Output);
  _writer->print(allocateModel());
  _writer->print(WRITER::Epilogue);
  _writer->endBlock();
  _writer->print(componentDefinition(MODEL_INSTANCE::QSS_Init));
  _writer->beginBlock();
  _writer->endBlock();
}

void ClassicModelInstance::header()
{
  ModelInstance::header();
  VarSymbolTable symbols = _model.symbols();
  VarSymbolTable::iterator it;
  stringstream buffer;
  buffer << endl;
  buffer << "// Derivative Macros definition. " << endl;
  _writer->write(buffer, WRITER::Model_Header);
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (var.isState()) {
      stringstream buffer, arguments;
      Macros macros(_model, var);
      arguments << macros.engineIndexArguments();
      buffer << "// Derivative definition for variable: " << var.name() << endl;
      buffer << "#define _der" << var << macros.parameters() << " dx[" << arguments.str() << "]";
      _writer->write(buffer, WRITER::Model_Header);
    }
  }
}

}  // namespace Generator
}  // namespace MicroModelica