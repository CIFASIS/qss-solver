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
      EquationTable algebraics = _model.algebraics();
      EquationTable::iterator algIt;
      EquationDependencyMatrix eqdm = _model.dependencies().OA();
      for(Equation out = outputs.begin(it); !outputs.end(it); out = outputs.next(it))
      {
        Option<EquationDependency> eqd = eqdm[outputs.key(it)];
        if(eqd)
        {
          EquationDependency::iterator eqIt;
          for(eqIt = eqd->begin(); eqIt != eqd->end(); eqIt++)
          {
            Option<Equation> alg = algebraics[*eqIt];
            if(alg)
            {
              buffer << alg;
            }
            else 
            {
              Error::instance().add(0, EM_CG | EM_NO_EQ, ER_Error, "Algebraic equation not found.");
            }
          }
        }
        buffer << out;
        _writer->write(buffer.str(), (out.hasRange() ? OUTPUT_GENERIC : OUTPUT_SIMPLE));
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
      buffer << endl;
      _writer->write(buffer, INCLUDE);
      buffer << "#include <common/model.h>" << endl;
      buffer << "#include <common/commands.h>" << endl;
      buffer << "#include <qss/qss_model.h>" << endl;
      buffer << "#include <classic/classic_model.h>" << endl;
      _writer->write(buffer,INCLUDE); 
    }

    void 
    ModelInstance::initializeMatrix(VariableDependencyMatrix vdm, Section alloc, Section init)
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
      _writer->write(buffer, INIT_OUTPUT);
    }


    /* QSSModelInstance Model Instance class. */

    QSSModelInstance::QSSModelInstance(Model& model, CompileFlags& flags, WriterPtr writer) : 
      ModelInstance(model, flags, writer)
    {
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

    void
    QSSModelInstance::zeroCrossing()
    {
      return;
    }

    void
    QSSModelInstance::handler()
    {
      return;
    }

    Graph
    QSSModelInstance::computationalGraph()
    {
      return Graph(0, 0);
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
      ModelDependencies deps = _model.dependencies();
      // Initialize Solver Data Structures.
      allocateSolver();
      initializeMatrix(deps.OS(), ALLOC_OUTPUT_STATES, INIT_OUTPUT_STATES);
      // Initialize Output Data Structures.
      allocateOutput();
      initializeMatrix(deps.OS(), ALLOC_OUTPUT_STATES, INIT_OUTPUT_STATES);
      initializeMatrix(deps.OD(), ALLOC_OUTPUT_DSC, INIT_OUTPUT_DSC);

    }
    
    void
    ClassicModelInstance::definition()
    {
      EquationTable derivatives = _model.derivatives();
      EquationTable algebraics = _model.algebraics();
      EquationTable::iterator it;
      VarSymbolTable symbols = _model.symbols();
      stringstream buffer;
      for(Equation alg = algebraics.begin(it); !algebraics.end(it); alg = algebraics.next(it))
      {
         buffer << alg;
         _writer->write(buffer, MODEL_SIMPLE);
      }
      for(Equation der = derivatives.begin(it); !derivatives.end(it); der = derivatives.next(it))
      {
         buffer << der;
         _writer->write(buffer, MODEL_SIMPLE);
      }
    }
    
    void
    ClassicModelInstance::dependencies()
    {
    }
    
    void
    ClassicModelInstance::zeroCrossing()
    {
    }
    
    void
    ClassicModelInstance::handler()
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
      buffer << "  modelData = simulator->data;" << endl;
      buffer << "  const double t = " << annot.initialTime() << ";" << endl;
      _writer->write(buffer, ALLOC_LD);
    }

  }
}
