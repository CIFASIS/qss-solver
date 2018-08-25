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

    /* QSSModelInstance Model Instance class. */

    QSSModelInstance::QSSModelInstance(Model& model, CompileFlags& flags, WriterPtr writer) 
    {
    }

    QSSModelInstance::~QSSModelInstance()
    {
    }

    string
    QSSModelInstance::header()
    {
      return "";
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

    void
    QSSModelInstance::output()
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
      _model(model),
      _flags(flags),
      _writer(writer)
    {
    }

    ClassicModelInstance::~ClassicModelInstance()
    {
    }

    string
    ClassicModelInstance::header()
    {
      return "";
    }

    void
    ClassicModelInstance::initializeDataStructures()
    {
      ModelDependencies deps = _model.dependencies();
      // Initialize output data structures.
      DependencyMatrix OS = deps.OS();
      
      DependencyMatrix SD = deps.SD();

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
         _writer->write(&buffer, MODEL_SIMPLE);
      }
      for(Equation der = derivatives.begin(it); !derivatives.end(it); der = derivatives.next(it))
      {
         buffer << der;
         _writer->write(&buffer, MODEL_SIMPLE);
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
    
    void
    ClassicModelInstance::output()
    {
      EquationTable outputs = _model.outputs();
      EquationTable::iterator it;
      stringstream buffer;
      for(Equation out = outputs.begin(it); !outputs.end(it); out = outputs.next(it))
      {
        buffer << out;
        _writer->write(buffer.str(), (out.hasRange() ? OUTPUT_GENERIC : OUTPUT_SIMPLE));
      }
    }
 
    Graph
    ClassicModelInstance::computationalGraph()
    {
      return Graph(0,0);
    }

  }
}
