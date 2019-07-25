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

#include "error.h"

#include <stdarg.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>

using namespace std;

namespace MicroModelica {
namespace Util {
Error::Error() : _errors(), _warnings(), _numErrors(0), _numWarnings(0), _stdDef() {}

Error::~Error() {}

void Error::setFile(string s) { _stdDef = s; }

string Error::file() { return _stdDef; }

void Error::setClassName(string className) { _className = className; }

string Error::className() { return _className; }

void Error::add(int pos, unsigned int code, ER_Type t, const string message, ...)
{
  va_list ap;
  ostringstream msg;
  string local;
  int size = 100;
  msg << "Line: " << pos << " Class: " << _className << endl;
  msg << _typeString(t) << "(" << hex << uppercase << code << ")" << endl;
  va_start(ap, message);
  vsnprintf((char *)local.c_str(), size, message.c_str(), ap);
  va_end(ap);
  msg << _printCode(code) << endl;
  msg << local.c_str() << endl;
  if (t == ER_Error) {
    _errors[_numErrors++] = msg.str();
  } else {
    _warnings[_numWarnings++] = msg.str();
  }
  if (t == ER_Fatal) {
    show();
    exit(-1);
  }
}

void Error::show()
{
  if (!_errors.empty() || !_warnings.empty()) {
    cout << "File: " << _stdDef << endl;
  }
  map<unsigned int, string>::iterator it;
  for (it = _warnings.begin(); it != _warnings.end(); it++) {
    cout << it->second;
  }
  for (it = _errors.begin(); it != _errors.end(); it++) {
    cout << it->second;
  }
}

string Error::_typeString(ER_Type t)
{
  switch (t) {
  case ER_Warning:
    return "WARNING: ";
  case ER_Error:
    return "ERROR: ";
  case ER_Fatal:
    return "FATAL ERROR: ";
  }
  return "";
}

string Error::_printCode(int code)
{
  int module_code = code & EM_MODULE;
  int message_code = code & EM_MESSAGE;
  string message;
  switch (module_code) {
  case EM_AST:
    message.append("Abstract Syntax Tree: ");
    break;
  case EM_IR:
    message.append("Intermediate Representation: ");
    break;
  case EM_CG:
    message.append("Code Generator: ");
    break;
  case EM_PP:
    message.append("Pretty Printer: ");
    break;
  case EM_ERROR:
    message.append("Error: ");
    break;
  case EM_GRM:
    message.append("Parser: ");
    break;
  }
  switch (message_code) {
  case EM_VARIABLE_NOT_FOUND:
    message.append("Variable not found.\n");
    break;
  case EM_VARIABLE_NOT_DEFINED:
    message.append("Variable not defined.\n");
    break;
  case EM_CANT_OPEN_FILE:
    message.append("Can not open file.\n");
    break;
  case EM_PARSE_FILE:
    message.append("Can not parse file.\n");
    break;
  case EM_WRONG_TYPE:
    message.append("Wrong variable type.\n");
    break;
  case EM_WRONG_EXP:
    message.append("Wrong expression type.\n");
    break;
  case EM_WRONG_SCANNER:
    message.append("Unknown scanner type.\n");
    break;
  case EM_UNKNOWN_OPER:
    message.append("Unknown operator type.\n");
    break;
  case EM_UNKNOWN_OPERATION:
    message.append("Unknown operation.\n");
    break;
  case EM_UNKNOWN_EXP:
    message.append("Unknown expression.\n");
    break;
  case EM_UNKNOWN_TYPE:
    message.append("Unknown type.\n");
    break;
  case EM_ARRAY_INIT:
    message.append("Array initialization not implemented.\n");
    break;
  case EM_ARRAY_INIT_VAL:
    message.append("Array wrong number of init values.\n");
    break;
  case EM_ARRAY_CTE:
    message.append("Array of type CONSTANT not allowed.\n");
    break;
  case EM_ARRAY_SIZE:
    message.append("Array size expected.\n");
    break;
  case EM_ALG_INIT:
    message.append("Can not assign initial values to algebraic variables.\n");
    break;
  case EM_ALG_INDEX:
    message.append("Algebraic variables definitions must use previously defined variables.\n");
    break;
  case EM_OUTPUT_INIT:
    message.append("Can not assign initial values to output variables.\n");
    break;
  case EM_FOR_VAR:
    message.append("Can not assign a simple variable inside a for loop.\n");
    break;
  case EM_FOR_DEF:
    message.append("Can not define a new variable inside a for loop.\n");
    break;
  case EM_CALL_EXP:
    message.append("Can not use a function call in an index defintion.\n");
    break;
  case EM_RANGE_EXP:
    message.append("Can not use a range expression in an index defintion.\n");
    break;
  case EM_INDEX_RANGE:
    message.append("Index out of range.\n");
    break;
  case EM_SIZE_ZERO:
    message.append("Algebraic variable size is zero.\n");
    break;
  case EM_GEN_DEF:
    message.append("Expecting generic event definition.\n");
    break;
  case EM_EVENT_DEF:
    message.append("Event not defined.\n");
    break;
  case EM_SYMBOL:
    message.append("Symbol not recognized.\n");
    break;
  case EM_SYM_DER:
    message.append("Symbolic derivative can not be generated (External functions?).\nTry using numerical integration.\n");
    break;
  case EM_EQ_DEF:
    message.append("Equation not defined.\n");
    break;
  case EM_NO_EQ:
    message.append("No equation defined in generic definition.\n");
    break;
  case EM_UNKNOWN_ODE:
    message.append("Unkown equation type.\n");
    break;
  case EM_EVENT_FOUND:
    message.append("Event not found.\n");
    break;
  case EM_UNKNOWN_ERROR:
    message.append("Unknown error.\n");
    break;
  case EM_OUTPUT_DEF:
    message.append("Missing output variable definition.\n");
    break;
  case EM_SAMPLED_DEF:
    message.append("Missing sample period definition.\n");
    break;
  case EM_ARGUMENTS:
    message.append("Function call, wrong number of arguments.\n");
    break;
  case EM_SAMPLED_OUT:
    message.append("Sampled output, can not output generic expressions, only state or dicrete variables are allowed.\n");
    break;
  case EM_HANDLER:
    message.append("Handler redefinition.\n");
    break;
  case EM_CONSTANT_FOR:
    message.append("Constant variable definition inside for loop.\n");
    break;
  case EM_DEFINITION_NOT_ALLOWED:
    message.append("Definition not allowed in MicroModelica.\n");
    break;
  case EM_CLASS_DEFINITION:
    message.append("Class definition.\n");
    break;
  case EM_ANNOTATION_NOT_FOUND:
    message.append("Annotation not found.\n");
    break;
  case EM_FUNCTION_NOT_FOUND:
    message.append("Function definition not found.\n");
    break;
  case EM_WRONG_VARIABLE_TYPE:
    message.append("Wrong variable type.\n");
    break;
  default:
    break;
  }
  return message;
}

int Error::errors() { return _numErrors; }
}  // namespace Util
}  // namespace MicroModelica
