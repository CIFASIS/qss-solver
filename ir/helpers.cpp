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

#include "helpers.h"
#include "../util/error.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {
   

    /* EvalInitExp class */

    EvalInitExp::EvalInitExp(VarSymbolTable vt) :
        _vt(vt)
    {
    }

    EvalInitExp::~EvalInitExp()
    {
    }

    int
    EvalInitExp::foldTraverseElement(AST_Expression exp)
    {
      int ret = 0;
      switch(exp->expressionType())
      {
        case EXPCOMPREF:
          {
          AST_Expression_ComponentReference cr = exp->getAsComponentReference();
          Option<VarInfo> vi = _vt[cr->name()];
          if(!vi)
          {
            Error::getInstance()->add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", cr->name().c_str());
            return ret;
          }
          if(!vi->isConstant())
          {
            Error::getInstance()->add(exp->lineNum(), EM_IR | EM_INIT_EXP, ER_Error, "Only constants allowed inside initial expressions. %s", cr->name().c_str());
            return ret;
          }
          return vi->value();
        }
        case EXPBOOLEAN:
          {
          AST_Expression_Boolean eb = exp->getAsBoolean();
          if(eb->value())
          {
            return 1;
          }
          else
          {
            return 0;
          }
        }
        case EXPBOOLEANNOT:
          {
          AST_Expression_BooleanNot ebn = exp->getAsBooleanNot();
          int res = foldTraverse(ebn->exp());
          if(res == 0)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        }
        case EXPREAL:
          Error::getInstance()->add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Implicit conversion from Real to Integer, in initial expression.");
          return exp->getAsReal()->val();
        case EXPINTEGER:
          return exp->getAsInteger()->val();
        default:
          Error::getInstance()->add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Initial expression not recognized, returning zero as default value.");
          break;
      }
      return ret;
    }

    int
    EvalInitExp::foldTraverseElementUMinus(AST_Expression exp)
    {
      return -1 * foldTraverse(exp->getAsUMinus()->exp());
    }

    int
    EvalInitExp::foldTraverseElement(int l, int r, BinOpType bot)
    {
      switch(bot)
      {
        case BINOPOR:
          if(l != 0 || r != 0)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPAND:
          if(l != 0 && r != 0)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPLOWER:
          if(l < r)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPLOWEREQ:
          if(l <= r)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPGREATER:
          if(l > r)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPGREATEREQ:
          if(l >= r)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPCOMPNE:
          if(l != r)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPCOMPEQ:
          if(l == r)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        case BINOPADD:
          return l + r;
        case BINOPSUB:
          return l - r;
        case BINOPDIV:
          if(r != 0)
          {
            return l / r;
          }
          else
          {
            Error::getInstance()->add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Initial expression zero division, returning zero as default value.");
          }
          break;
        case BINOPMULT:
          return l * r;
        default:
          break;
      }
      return 0;
    }
    /* VariableLookup class */

    VariableLookup::VariableLookup(VarSymbolTable st, VarSymbolTable lst) :
        _st(st), 
        _lst(lst)
    {
    }

    VariableLookup::~VariableLookup()
    {
    }

    bool
    VariableLookup::foldTraverseElement(AST_Expression e)
    {
      if(e->expressionType() == EXPCOMPREF)
      {
        AST_Expression_ComponentReference cr = e->getAsComponentReference();
        Option<VarInfo> vi = _st[cr->name()];
        if(!vi)
        {
          vi = _lst[cr->name()];
          if(!vi)
          {
            return false;
          }
        }
      }
      return true;
    }

    bool
    VariableLookup::foldTraverseElement(bool e1, bool e2, BinOpType bot)
    {
      return e1 && e2;
    }

    bool
    VariableLookup::foldTraverseElementUMinus(AST_Expression e)
    {
      return foldTraverseElement(e);
    }

    /* ExternalFunction Class Implementation */

    ExternalFunction::ExternalFunction(string lvalue, string name, AST_ExpressionList args) :
      _lvalue(lvalue),
      _name(name),
      _args(args)
    {
    }
    
    ExternalFunction::~ExternalFunction()
    {
    }
    
    /* CompiledFunction Class Implementation */

    CompiledFunction::CompiledFunction() :
      _def(), 
      _name(), 
      _prototype(), 
      _includeDirectory(), 
      _libraryDirectory(), 
      _libraries()
    {
    }


    CompiledFunction::CompiledFunction(string name, string includeDir, string libraryDir, list<string> libraries) :
      _def(), 
      _name(name), 
      _prototype(), 
      _includeDirectory(includeDir), 
      _libraryDirectory(libraryDir), 
      _libraries(libraries)
    {
    }

    CompiledFunction::~CompiledFunction()
    {
    }

    bool
    CompiledFunction::hasIncludeDirectory()
    {
      return !_includeDirectory.empty();
    }

    bool
    CompiledFunction::hasLibraryDirectory()
    {
      return !_libraryDirectory.empty();
    }

    bool
    CompiledFunction::hasLibraries()
    {
      return _libraries.size() > 0;
    }

    string
    CompiledFunction::includeDirectory()
    {
      return _includeDirectory;
    }

    string
    CompiledFunction::libraryDirectory()
    {
      return _libraryDirectory;
    }

    list<string>
    CompiledFunction::libraries()
    {
      return _libraries;
    }

    string
    CompiledFunction::name()
    {
      return _name;
    }

    list<string>
    CompiledFunction::def()
    {
      return _def;
    }

    string
    CompiledFunction::prototype()
    {
      return _prototype;
    }

    /* CompiledPackage Class Implementation */
    
    CompiledPackage::CompiledPackage()
    {
    }

    CompiledPackage::CompiledPackage(string name, CompiledFunctionTable cft, ImportTable objects) :
        _name(name), 
        _cft(cft), 
        _objects(objects)
    {
    }

    CompiledPackage::~CompiledPackage()
    {
    }

    string
    CompiledPackage::name()
    {
      return _name;
    }

    string
    CompiledPackage::prefix()
    {
      return "__" + _name + "__";
    }

    CompiledFunctionTable 
    CompiledPackage::definitions()
    {
      return _cft;
    }

    ImportTable
    CompiledPackage::objects()
    {
      return _objects;
    }

    list<string>
    CompiledPackage::linkLibraries()
    {
      list<string> ret;
      CompiledFunctionTable::iterator it;
      for(CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it))
      {
        if(cf.hasLibraries())
        {
          list<string> libs = cf.libraries();
          ret.insert(ret.end(), libs.begin(), libs.end());
        }
      }
      return ret;
    }

    list<string>
    CompiledPackage::libraryDirectories()
    {
      list<string> ret;
      CompiledFunctionTable::iterator it;
      for(CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it))
      {
        if(cf.hasLibraryDirectory())
        {
          ret.push_back(cf.libraryDirectory());
        }
      }
      return ret;
    }

    list<string>
    CompiledPackage::includeDirectories()
    {
      list<string> ret;
      CompiledFunctionTable::iterator it;
      for(CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it))
      {
        if(cf.hasIncludeDirectory())
        {
          ret.push_back(cf.includeDirectory());
        }
      }
      return ret;
    }
  }
}
