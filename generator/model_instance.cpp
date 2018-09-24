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
 along with QSSModelInstance Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "model_instance.h"

#include <sstream>
#include <utility>
#include <boost/optional/optional_io.hpp>

#include "macros.h"
#include "../ast/expression.h"
#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/equation.h"
#include "../ir/event.h"
#include "../ir/expression.h"
#include "../ir/statement.h"
#include "../util/error.h"
#include "../util/util.h"

namespace MicroModelica {
  
  namespace Generator {
  
    using namespace MicroModelica::IR;
    using namespace MicroModelica::Util;

    ModelInstance::ModelInstance(Model& model, CompileFlags& flags, WriterPtr writer) :
      _model(model),
      _flags(flags),
      _writer(writer)
    {
    }

    void
    ModelInstance::output()
    {
      stringstream buffer;
      EquationTable outputs = _model.outputs();
      EquationTable::iterator it;
      EquationDependencyMatrix eqdm = _model.dependencies().OA();
      Utils::instance().setLocalSymbols();
      FunctionPrinter fp;
      for(Equation out = outputs.begin(it); !outputs.end(it); out = outputs.next(it))
      {
        buffer << out;
        _writer->write(buffer.str(), (out.hasRange() ? WRITER::Output_Generic : WRITER::Output_Simple));
      }
      _writer->write(Utils::instance().localSymbols(), WRITER::Output);
      if(!_writer->isEmpty(WRITER::Output_Simple))
      { 
        _writer->write(fp.beginSwitch(), WRITER::Output); 
        _writer->write(fp.endSwitch(), WRITER::Output_Simple); 
      }
    }
    
    void
    ModelInstance::include()
    {
      stringstream buffer;
      buffer << "#include <stdlib.h>" << endl;
      buffer << "#include <stdio.h>" << endl;
      buffer << "#include <string.h>" << endl;
      buffer << "#include <math.h>" << endl;
      buffer << endl;
      buffer << "#include \"" << _model.name() << ".h\"" << endl;
      if(_model.externalFunctions())
      {
        buffer << "#include \"" << _model.name() << "_functions.h\"" << endl;
      }
      ImportTable imports = _model.imports();
      ImportTable::iterator it;
      for(string i = imports.begin(it); !imports.end(it); i = imports.next(it))
      {
        buffer << "#include \"" << Utils::instance().packageName(i) << ".h\"" << endl;
      }
      buffer << "#include <common/utils.h>" << endl;
      buffer << "#include <common/model.h>" << endl;
      buffer << "#include <common/commands.h>" << endl;
      buffer << "#include <qss/qss_model.h>" << endl;
      buffer << "#include <classic/classic_model.h>" << endl;
      buffer << endl;
      _writer->write(buffer,WRITER::Include); 
    }

    void 
    ModelInstance::initializeMatrix(VariableDependencyMatrix vdm, WRITER::Section alloc, WRITER::Section init)
    {
      stringstream buffer;
      VariableDependencyMatrix::iterator it;
      for(VariableDependency vd = vdm.begin(it); !vdm.end(it); vd = vdm.next(it))
      {
        buffer << vd.alloc();
        _writer->write(buffer, alloc);
        buffer << vd.init();
        _writer->write(buffer, init);
      }
    }

    void 
    ModelInstance::allocateOutput()
    {
      stringstream buffer;
      ModelAnnotation annot = _model.annotations();
      string period = "NULL";
      string indent = "";
      int ssize = 0;
      if(annot.commInterval() == "CI_Sampled")
      {
        indent = _writer->indent(1);
        list<double> sample = annot.sample();
        ssize = sample.size();
        period = "period";
        buffer << indent << "double period[" << ssize << "];" << endl;
        int n = 0;
        for(list<double>::iterator i = sample.begin(); i != sample.end(); i++)
        {
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
      _writer->write(buffer, WRITER::Init_Output);
    }

    void 
    ModelInstance::zeroCrossing()
    {
      EventTable events = _model.events();
      EventTable::iterator it;
      FunctionPrinter fp;
      Utils::instance().setLocalSymbols();
      for(Event event = events.begin(it); !events.end(it); event = events.next(it))
      {
        stringstream buffer;
        Equation zc = event.zeroCrossing();
        buffer << zc;
        _writer->write(buffer, (zc.hasRange() ? WRITER::ZC_Generic : WRITER::ZC_Simple));
      }
      _writer->write(Utils::instance().localSymbols(), WRITER::Zero_Crossing);
      if(!_writer->isEmpty(WRITER::ZC_Simple))
      {
        _writer->write(fp.beginSwitch(), WRITER::Zero_Crossing);
        _writer->write(fp.endSwitch(), WRITER::ZC_Simple);
      }
    }
 
    void 
    ModelInstance::handler()
    {
      EventTable events = _model.events();
      EventTable::iterator it;
      VarSymbolTable symbols = _model.symbols();
      stringstream buffer;
      FunctionPrinter fp;
      Utils::instance().setLocalSymbols();
      for(Event event = events.begin(it); !events.end(it); event = events.next(it))
      {
        buffer << event.handler(EVENT::Positive);
        _writer->write(buffer, (event.hasRange() ? WRITER::Handler_Pos_Generic : WRITER::Handler_Pos_Simple)); 
        buffer << event.handler(EVENT::Negative);
        _writer->write(buffer, (event.hasRange() ? WRITER::Handler_Neg_Generic : WRITER::Handler_Neg_Simple)); 
      }
      if(!_writer->isEmpty(WRITER::Handler_Pos_Generic))
      {
        _writer->write(Utils::instance().localSymbols(), WRITER::Handler_Pos);
      }
      if(!_writer->isEmpty(WRITER::Handler_Neg_Generic))
      { 
        _writer->write(Utils::instance().localSymbols(), WRITER::Handler_Neg);
      }
      if(!_writer->isEmpty(WRITER::Handler_Pos_Simple))
      {
        _writer->write(fp.beginSwitch(), WRITER::Handler_Pos);
        _writer->write(fp.endSwitch(), WRITER::Handler_Pos_Simple);
      }
      if(!_writer->isEmpty(WRITER::Handler_Neg_Simple))
      {
        _writer->write(fp.beginSwitch(), WRITER::Handler_Neg);
        _writer->write(fp.endSwitch(), WRITER::Handler_Neg_Simple);
      }
    }

    void
    ModelInstance::settings()
    {
      stringstream buffer;
      buffer << "settings->debug = " << _flags.debug() << ";";
      _writer->print(buffer);
      buffer << "settings->parallel = ";
      buffer << (_flags.parallel() ? "TRUE" : "FALSE") << ";";
      _writer->print(buffer);
      buffer << "settings->hybrid = ";
      buffer << (_model.eventNbr() ? "TRUE" : "FALSE") << ";";
      _writer->print(buffer);
      buffer << "settings->method = " << _model.annotations().solver() << ";";
      _writer->print(buffer);
    }
    
    void 
    ModelInstance::header()
    {
      VarSymbolTable symbols = _model.symbols();
      VarSymbolTable::iterator it;
      stringstream buffer;
      _writer->write("// Model Variable and Parameters Macros\n", WRITER::Model_Header); 
      for(Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it))
      {
        if(var.isModelVar()) 
        { 
          Macros macros(_model, var);
          buffer << macros;
        }
      }
      _writer->write(buffer, WRITER::Model_Header);
      if(symbols.parameters()) { _writer->write("\n// Model Parameters Declaration\n", WRITER::Model_Header); }
      for(Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it))
      {
        if(var.isParameter()) { buffer << var.declaration("__PAR__"); }
        _writer->write(buffer, WRITER::Model_Header);
      }
      EquationTable derivatives = _model.derivatives();
      EquationTable::iterator eqit;
      if(!_model.annotations().classic()) { _writer->write("\n// Derivative Equations Macros\n", WRITER::Model_Header); }
      for(Equation e = derivatives.begin(eqit); !derivatives.end(eqit); e = derivatives.next(eqit))
      {
        _writer->write(e.macro(), WRITER::Model_Header);
      }
      _writer->write("\n// Event Macros\n", WRITER::Model_Header);
      EventTable events = _model.events();
      EventTable::iterator eit;
      for(Event e = events.begin(eit); !events.end(eit); e = events.next(eit))
      {
        _writer->write(e.macro(), WRITER::Model_Header);
      }
      if(events.size()) { _writer->write("#define _zc zc[0]", WRITER::Model_Header); }
      _writer->write("\n// Output Equations Macros\n", WRITER::Model_Header);
      EquationTable outputs = _model.outputs();
      for(Equation e = outputs.begin(eqit); !outputs.end(eqit); e = outputs.next(eqit))
      {
        _writer->write(e.macro(), WRITER::Model_Header);
      }
      if(outputs.size()) { _writer->write("#define _out out[0]", WRITER::Model_Header); }
      _writer->write("\n// Input Matrix Macros\n", WRITER::Model_Header);
      InputTable inputs = _model.inputs();
      InputTable::iterator iit;
      for(Input i = inputs.begin(iit); !inputs.end(iit); i = inputs.next(iit))
      {
        _writer->write(i.macro(), WRITER::Model_Header);
      }
      _writer->write("#define _time t", WRITER::Model_Header);
    }

    string 
    ModelInstance::componentDefinition(Component c)
    {
      switch(c)
      {
        case MODEL_SETTINGS: return "void\nMOD_settings(SD_simulationSettings settings)";
        case MODEL: return "void\nMOD_definition(double *x, double *d, double *a, double t, double *dx)";
        case DEPS: return "void\nMOD_dependencies(int idx, double *x, double *d, double *a, double t, double *der, int *map)";
        case ZC: return "void\nMOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)";
        case HANDLER_POS: return "void\nMOD_handlerPos(int idx, double *x, double *d, double *a, double t)";
        case HANDLER_NEG: return "void\nMOD_handlerNeg(int idx, double *x, double *d, double *a, double t)";
        case OUTPUT: return "void\nMOD_output(int idx, double *x, double *d, double *a, double t, double *out)";
        case JACOBIAN: return "void\nMOD_jacobian(double *x, double *d, double *a, double t, double *jac)";
        case CLC_INIT: return "void\nCLC_initializeDataStructs(CLC_simulator simulator)";
        case QSS_INIT: return "void\nQSS_initializeDataStructs(QSS_simulator simulator)";
      }
      return "";
    }

    void
    ModelInstance::initialCode()
    {
      StatementTable stms = _model.initialCode();
      StatementTable::iterator it;
      stringstream buffer;
      for(Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it))
      {
        buffer << stm;
        _writer->write(buffer, WRITER::Init_Code);
      }
    }

    void
    ModelInstance::inputs()
    {
      InputTable inputs = _model.inputs();
      InputTable::iterator it;
      for(Input input = inputs.begin(it); !inputs.end(it); input = inputs.next(it))
      {
        stringstream buffer;
        buffer << input;
        _writer->write(buffer, WRITER::Input);  
      }
    }

    string 
    ModelInstance::allocateModel()
    {
      stringstream buffer;
      buffer << (!_writer->isEmpty(WRITER::ZC_Simple) || !_writer->isEmpty(WRITER::ZC_Generic) ? "MOD_zeroCrossing" : "NULL") << ", ";
      buffer << (!_writer->isEmpty(WRITER::Handler_Pos_Simple) || !_writer->isEmpty(WRITER::Handler_Pos_Generic) ? "MOD_handlerPos" : "NULL") << ", ";
      buffer << (!_writer->isEmpty(WRITER::Handler_Neg_Simple) || !_writer->isEmpty(WRITER::Handler_Neg_Generic) ? "MOD_handlerNeg" : "NULL");
      return buffer.str();
    }
    
    /* QSSModelInstance Model Instance class. */

    QSSModelInstance::QSSModelInstance(Model& model, CompileFlags& flags, WriterPtr writer) : 
      ModelInstance(model, flags, writer),
      _model(model),
      _flags(flags),
      _writer(writer)
    {
    }

    void 
    QSSModelInstance::allocateSolver()
    {
      return;
    }

    void
    QSSModelInstance::initializeDataStructures()
    {
      return;
    }

    void
    QSSModelInstance::definition()
    {
      return;
    }

    void
    QSSModelInstance::dependencies()
    {
      return;
    }

    Graph
    QSSModelInstance::computationalGraph()
    {
      return Graph(0, 0);
    }

    void
    QSSModelInstance::generate()
    {
    }

    void 
    QSSModelInstance::header()
    {
    }

    /* ClassicModelInstance Model Instance class. */

    ClassicModelInstance::ClassicModelInstance(Model& model, CompileFlags& flags, WriterPtr writer) :
      ModelInstance(model, flags, writer),
      _model(model),
      _flags(flags),
      _writer(writer)
    {
    }

    void
    ClassicModelInstance::initializeDataStructures()
    {
      stringstream buffer;
      Utils::instance().setLocalSymbols();
      allocateSolver();
      initialCode();
      ModelDependencies deps = _model.dependencies();
      // Initialize Solver Data Structures.
      initializeMatrix(deps.SD(), WRITER::Alloc_Matrix_SD, WRITER::Init_Matrix_SD);
      initializeMatrix(deps.SD(), WRITER::Alloc_Matrix_DS, WRITER::Init_Matrix_DS);
      inputs();
      // Initialize Output Data Structures.
      allocateOutput();
      initializeMatrix(deps.OS(), WRITER::Alloc_Output_States, WRITER::Init_Output_States);
      initializeMatrix(deps.OD(), WRITER::Alloc_Output_Discretes, WRITER::Init_Output_Discretes);
      allocateModel();
      _writer->write(Utils::instance().localSymbols(), WRITER::Alloc_Matrix);
    }
    
    void
    ClassicModelInstance::definition()
    {
      EquationTable derivatives = _model.derivatives();
      EquationTable algebraics = _model.algebraics();
      EquationTable::iterator it;
      VarSymbolTable symbols = _model.symbols();
      stringstream buffer;
      Utils::instance().setLocalSymbols();
      for(Equation alg = algebraics.begin(it); !algebraics.end(it); alg = algebraics.next(it))
      {
         buffer << alg;
         _writer->write(buffer, WRITER::Model_Simple);
      }
      for(Equation der = derivatives.begin(it); !derivatives.end(it); der = derivatives.next(it))
      {
         buffer << der;
         _writer->write(buffer, WRITER::Model_Simple);
      }
      _writer->write(Utils::instance().localSymbols(), WRITER::Model);
    }
    
    void
    ClassicModelInstance::dependencies()
    {
    }
    
    Graph
    ClassicModelInstance::computationalGraph()
    {
      return Graph(0,0);
    }

    void 
    ClassicModelInstance::allocateSolver()
    {
      ModelAnnotation annot = _model.annotations();
      stringstream buffer;
      buffer << "simulator->data = CLC_Data(" << _model.stateNbr() << ",";
      buffer << _model.discreteNbr() << ",";
      buffer << _model.eventNbr() << ",";
      buffer << _model.inputNbr() << ",";
      buffer << _model.algebraicNbr() << ",\"";
      buffer << _model.name() << "\");" << endl;
      buffer << "modelData = simulator->data;" << endl;
      buffer << "const double t = " << annot.initialTime() << ";" << endl;
      buffer << "double* x = modelData->x;" << endl;
      buffer << "double* d = modelData->d;" << endl;
      buffer << "double* a = modelData->a;" << endl;
      _writer->write(buffer, WRITER::Alloc_Matrix);
    }

    string 
    ClassicModelInstance::allocateModel()
    {
      stringstream buffer;
      buffer << "simulator->model = CLC_Model(MOD_definition, ";
      buffer << ModelInstance::allocateModel();
      buffer << ", MOD_jacobian);";
      return buffer.str();
    }

    void 
    ClassicModelInstance::generate()
    {
      include();
      definition();
      dependencies();
      zeroCrossing();
      handler();
      output();
      initializeDataStructures();
      // Print generated Model Instance.
      _writer->print(WRITER::Include);
      _writer->print(componentDefinition(MODEL_SETTINGS));
      _writer->beginBlock();
      settings();
      _writer->endBlock();
      _writer->print(componentDefinition(MODEL));
      _writer->beginBlock();
      _writer->print(WRITER::Model);
      _writer->print(WRITER::Model_Simple);
      _writer->endBlock();
      _writer->print(componentDefinition(JACOBIAN));
      _writer->beginBlock();
      _writer->print(WRITER::Jacobian);
      _writer->endBlock();
      _writer->print(componentDefinition(ZC));
      _writer->beginBlock();
      _writer->print(WRITER::Zero_Crossing);
      _writer->print(WRITER::ZC_Simple);
      _writer->print(WRITER::ZC_Generic);
      _writer->endBlock();
      _writer->print(componentDefinition(HANDLER_POS));
      _writer->beginBlock();
      _writer->print(WRITER::Handler_Pos);
      _writer->print(WRITER::Handler_Pos_Simple);
      _writer->print(WRITER::Handler_Pos_Generic);
      _writer->endBlock();
      _writer->print(componentDefinition(HANDLER_NEG));
      _writer->beginBlock();
      _writer->print(WRITER::Handler_Neg);
      _writer->print(WRITER::Handler_Neg_Simple);
      _writer->print(WRITER::Handler_Neg_Generic);
      _writer->endBlock();
      _writer->print(componentDefinition(OUTPUT));
      _writer->beginBlock();
      _writer->print(WRITER::Output);
      _writer->print(WRITER::Output_Simple);
      _writer->print(WRITER::Output_Generic);
      _writer->endBlock();
      _writer->print(componentDefinition(CLC_INIT));
      _writer->beginBlock();
      _writer->print(WRITER::Alloc_Matrix);
      _writer->print(WRITER::Init_Code);
      _writer->print(WRITER::Alloc_Matrix_SD);
      _writer->print(WRITER::Alloc_Matrix_DS);
      _writer->print(WRITER::Init_Matrix);
      _writer->print(WRITER::Init_Matrix_DS);
      _writer->print(WRITER::Init_Matrix_SD);
      _writer->print(WRITER::Input);
      _writer->print(WRITER::Init_Output);
      _writer->print(allocateModel());
      _writer->endBlock();
      _writer->print(componentDefinition(QSS_INIT));
      _writer->beginBlock();
      _writer->endBlock();
    }

    void 
    ClassicModelInstance::header()
    {
      ModelInstance::header();
      VarSymbolTable symbols = _model.symbols();
      VarSymbolTable::iterator it;
      stringstream buffer;
      buffer << endl;
      buffer << "// Derivative Macros definition. ";
      _writer->write(buffer, WRITER::Model_Header);
      for(Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it))
      {
        if(!var.isState()) { continue; }
        stringstream buffer;
        Macros macros(_model, var);
        buffer << "#define _der" << var << macros.parameters() << " dx[0]"; 
        _writer->write(buffer, WRITER::Model_Header);
      }
      buffer << endl << "// Jacobian Macros definition. ";
      _writer->write(buffer, WRITER::Model_Header);
      buffer << "#define _jac(i) jac[i++]";
      _writer->write(buffer, WRITER::Model_Header);
    }

  }
}
