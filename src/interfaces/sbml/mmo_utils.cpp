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

#include "mmo_utils.h"

#include <sbml/math/FormulaFormatter.h>
#include <sstream>
#include <utility>

using namespace std;

bool MMOUtils::_instance = false;

MMOUtils *MMOUtils::_utils = NULL;

MMOUtils::MMOUtils() :
    _bioricaLanguage(false)
{
  map<string, string> mathFunctions;
  mathFunctions["rand"] = "rand";
  mathFunctions["getRandomValue"] = "getRandomValue";
  mathFunctions["exponential"] = "exponential";
  mathFunctions["uniform"] = "uniform";
  mathFunctions["normal"] = "normal";
  mathFunctions["pow"] = "pow";
  mathFunctions["root"] = "root";
  _predefinedFunctions.insert(
      pair<string, map<string, string> >("math", mathFunctions));
  _varnum = 0;
}
;

MMOUtils *
MMOUtils::getInstance()
{
  if(_instance)
  {
    return _utils;
  }
  else
  {
    _utils = new MMOUtils();
    _instance = true;
    return _utils;
  }
}

const char*
MMOUtils::getVar(string str)
{
  string ret = str;
  stringstream cvr;
  cvr << _varnum++;
  ret.append(cvr.str());
  return ret.c_str();
}

string
MMOUtils::indent(int n)
{
  int i;
  string ret = "";
  for(i = 0; i < n; i++)
  {
    ret += "\t";
  }
  return ret;
}

string
MMOUtils::_applyList(ASTNode *node, string oper)
{
  int numChilds = node->getNumChildren(), i;
  stringstream buffer;
  if(_controlOperation(node->getChild(0)->getType()))
  {
    buffer << "(" << getExp(node->getChild(0)) << ")";
  }
  else
  {
    buffer << getExp(node->getChild(0));
  }
  for(i = 1; i < numChilds; i++)
  {
    buffer << " " << oper << " ";
    ASTNodeType_t type = node->getChild(i)->getType();
    if(_controlOperation(type))
    {

      buffer << "(" << getExp(node->getChild(i)) << ")";
    }
    else
    {
      buffer << getExp(node->getChild(i));
    }
  }
  return buffer.str();
}
string
MMOUtils::getExp(ASTNode *node, string asgVariable)
{
  if(node->getType() == AST_RELATIONAL_GT)
  {
    return getExp(node->getChild(0)) + " > " + getExp(node->getChild(1));
  }
  else if(node->getType() == AST_POWER)
  {
    return "(" + getExp(node->getChild(0)) + ") ^ (" + getExp(node->getChild(1))
        + ")";
  }
  else if(node->getType() == AST_RELATIONAL_GEQ)
  {
    return getExp(node->getChild(0)) + " >= " + getExp(node->getChild(1));
  }
  else if(node->getType() == AST_RELATIONAL_LT)
  {
    return getExp(node->getChild(0)) + " < " + getExp(node->getChild(1));
  }
  else if(node->getType() == AST_RELATIONAL_LEQ)
  {
    return getExp(node->getChild(0)) + " <= " + getExp(node->getChild(1));
  }
  else if(node->getType() == AST_RELATIONAL_EQ)
  {
    string eq = " == ";
    if(_bioricaLanguage)
    {
      eq = " = ";
    }
    return getExp(node->getChild(0)) + eq + getExp(node->getChild(1));
  }
  else if(node->getType() == AST_RELATIONAL_NEQ)
  {
    return getExp(node->getChild(0)) + " != " + getExp(node->getChild(1));
  }
  else if(node->getType() == AST_LOGICAL_AND)
  {
    return _applyList(node, "and");
  }
  else if(node->getType() == AST_LOGICAL_OR)
  {
    return _applyList(node, "or");
  }
  else if(node->getType() == AST_LOGICAL_NOT)
  {
    return "not (" + getExp(node->getChild(0)) + ")";
  }
  else if(node->getType() == AST_NAME_TIME)
  {
    return "time";
  }
  else if(node->getType() == AST_PLUS)
  {
    return _applyList(node, "+");
  }
  else if(node->getType() == AST_DIVIDE)
  {
    return _applyList(node, "/");
  }
  else if(node->getType() == AST_MINUS)
  {
    if(node->getNumChildren() == 1)
    {
      return "-" + getExp(node->getChild(0));
    }
    else
    {
      return _applyList(node, "-");
    }
  }
  else if(node->getType() == AST_TIMES)
  {
    return _applyList(node, "*");
  }
  else if(node->getType() == AST_INTEGER)
  {
    if(node->getInteger() < 0)
    {
      string ret = "(";
      ret.append(SBML_formulaToString(node)).append(")");
      return ret;
    }
  }
  else if(node->getType() == AST_FUNCTION_PIECEWISE)
  {
    stringstream ret;
    int numchilds = node->getNumChildren();
    bool otherwise = false;
    if(numchilds % 2)
    {
      otherwise = true;
      numchilds--;
    }
    int i;
    for(i = 0; i < numchilds; i = i + 2)
    {
      if(i > 0)
      {
        ret << "elseif ";
      }
      else
      {
        ret << "if ";
      }
      ret << getExp(node->getChild(i + 1), asgVariable) << " then" << endl;
      ret << asgVariable << " := " << getExp(node->getChild(i), asgVariable)
          << ";" << endl;
    }
    if(otherwise)
    {
      ret << "else " << endl;
      if(node->getChild(numchilds)->getType() == AST_FUNCTION_PIECEWISE)
      {
        ret << getExp(node->getChild(numchilds), asgVariable) << ";" << endl;
      }
      else
      {
        ret << asgVariable << " := "
            << getExp(node->getChild(numchilds), asgVariable) << ";" << endl;
      }
    }
    ret << "end if";
    return ret.str();
  }
  return SBML_formulaToString(node);
}

string
MMOUtils::checkPredefinedFunctions(ASTNode* node)
{
  if(node->isFunction())
  {
    map<string, map<string, string> >::iterator it;
    string compare = node->getName();
    for(it = _predefinedFunctions.begin(); it != _predefinedFunctions.end();
        it++)
    {
      map<string, string>::iterator itFunctions = it->second.find(compare);
      if(itFunctions != it->second.end())
      {
        return it->first;
      }
    }
  }
  return "";
}

void
MMOUtils::bioricaLanguage(bool f)
{
  _bioricaLanguage = f;
}

bool
MMOUtils::_controlOperation(ASTNodeType_t type)
{
  if(type == AST_RELATIONAL_EQ || type == AST_RELATIONAL_GEQ
      || type == AST_RELATIONAL_GT || type == AST_RELATIONAL_LEQ
      || type == AST_RELATIONAL_LT || type == AST_RELATIONAL_NEQ
      || type == AST_PLUS || type == AST_TIMES || type == AST_MINUS
      || type == AST_DIVIDE || type == AST_POWER || type == AST_FUNCTION_POWER)
  {
    return true;
  }
  return false;
}
