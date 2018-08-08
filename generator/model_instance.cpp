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

    string
    QSSModelInstance::makefile(Makefile m)
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

    ClassicModelInstance::ClassicModelInstance(Model& model, CompileFlags& flags, WriterPtr writer) 
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

    string
    ClassicModelInstance::makefile(Makefile m)
    {
      return "";
    }
    
    void
    ClassicModelInstance::initializeDataStructures()
    {
    }
    
    void
    ClassicModelInstance::definition()
    {
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
    }
 
    Graph
    ClassicModelInstance::computationalGraph()
    {
      return Graph(0,0);
    }

  }
}
