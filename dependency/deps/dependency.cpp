
/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <ast/ast_types.h>
#include <parser/parser.h>
#include <ast/equation.h>
#include <mmo/mmo_class.h>
#include <util/debug.h>
#include <deps/dependency.h>
#include <deps/for_unrolling/process_for_equations.h>
#include <deps/vector/graph_builder.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <boost/variant/get.hpp>
#include <deps/vector/splitfor.h>

using namespace std;
using namespace Modelica;
using namespace Modelica::AST;
using namespace Causalize;

namespace Dependency {

  Dependency::Dependency(string filename, bool vectorial) :
    _filename(filename),
    _vectorial(vectorial)
  {
  }

  Dependency::~Dependency()
  {
  }

  string 
  Dependency::compute()
  {
    stringstream code;
    StoredDef sd;
    bool r;
    sd=Parser::ParseFile(_filename,r);
   
    if (!r) return "";

    Class ast_c = boost::get<Class>(sd.classes().front());
    MMO_Class mmo(ast_c);
    if (_vectorial) {
      SplitFor sf(mmo);
      sf.splitFor();
      ReducedGraphBuilder gb(mmo);
      VectorCausalizationGraph g = gb.makeGraph();
      return code.str();
    }
    foreach_(const Equation &e, mmo.equations_ref().equations_ref()) {
      if (debugIsEnabled('c'))
        code << e << std::endl;
    }
    code << mmo << endl;
    return code.str();
  }
}
