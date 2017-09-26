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

#include <parser/parse.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../ast/class.h"
#include "../ast/composition.h"
#include "../ast/equation.h"
#include "../ast/stored_definition.h"
#include "mocc_parser.h"

using namespace std;

AST_StoredDefinition
parseFile(string filename, int *r)
{
  fstream in;
  int ret;
  MCC_Parser parser(false);
  if(filename.size())
  {
    in.open(filename.c_str(), fstream::in);
    if(in.fail())
    {
      cerr << "Could not open file " << filename.c_str() << endl;
      exit(-1);
    }
    ret = parser.parseFile(&in);
  }
  else
  { // read from stdin
    ret = parser.parseFile(&cin);
  }
  if(ret == 0)
  {
    *r = 0;
    in.close();
    return parser.root();
  }
  *r = -1;
  in.close();
  return NULL;
}

AST_Expression
parseExpression(string exp, int *r)
{
  fstream in;
  string tmpFile;
#ifndef _WIN32
  tmpFile = "/tmp/t";
#else
  tmpFile = "t";
#endif
  in.open(tmpFile.c_str(), fstream::out);
  in << "model A equation x = " << exp << "; end A;";
  in.close();
  int ret;
  AST_Class c = parseClass(tmpFile.c_str(), &ret);
  if(ret == 0)
  {
    *r = 0;
    return c->composition()->compositionList()->front()->getEquationsAlgs()->getEquations()->front()->getAsEquality()->right();
  }
  *r = -1;
  return NULL;
}

AST_Equation
parseEquation(string exp, int *r)
{
  fstream in;
  string tmpFile;
#ifndef _WIN32
  tmpFile = "/tmp/t";
#else
  tmpFile = "t";
#endif
  in.open(tmpFile.c_str(), fstream::out);
  in << "model A equation " << exp << "; end A;";
  in.close();
  int ret;
  AST_Class c = parseClass(tmpFile.c_str(), &ret);
  if(ret == 0)
  {
    *r = 0;
    return c->composition()->compositionList()->front()->getEquationsAlgs()->getEquations()->front();
  }
  *r = -1;
  return NULL;
}

AST_Statement
parseStatement(string exp, int *r)
{
  fstream in;
  string tmpFile;
#ifndef _WIN32
  tmpFile = "/tmp/t";
#else
  tmpFile = "t";
#endif
  in.open(tmpFile.c_str(), fstream::out);
  in << "model A initial algorithm " << exp << "; end A;";
  in.close();
  int ret;
  AST_Class c = parseClass(tmpFile.c_str(), &ret);
  if(ret == 0)
  {
    *r = 0;
    return c->composition()->compositionList()->front()->getEquationsAlgs()->getAlgorithms()->front();
  }
  *r = -1;
  return NULL;
}

AST_Class
parseClass(string filename, int *r)
{
  AST_StoredDefinition sd = parseFile(filename, r);
  if(r[0] == 0)
  {
    return sd->models()->front();
  }
  return NULL;
}
