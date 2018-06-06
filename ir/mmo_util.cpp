#include "mmo_util.h"

#include <sstream>

#include "../ast/expression.h"
#include "../util/symbol_table.h"
#include "annotation.h"
#include "class.h"
#include "expression.h"

/* MMO_ModelConfig class */

MMO_ModelConfig::MMO_ModelConfig() :
    _symbols(NULL), 
    _algebraics(NULL), 
    _functions(NULL), 
    _externalFunctions(NULL), 
    _packages(NULL), 
    _initialCode(false), 
    _generateDerivatives(true), 
    _range(), 
    _lhs(), 
    _whenStatement(false), 
    _weight(-1), 
    _calledFunctions(), 
    _calculateAlgebraics(false), 
    _arguments(NULL), 
    _annotation(NULL), 
    _disableSymDiff(false)
{
}

MMO_ModelConfig::~MMO_ModelConfig()
{
}

bool
MMO_ModelConfig::hasExternalFunctions()
{
  return _externalFunctions != NULL;
}

void
MMO_ModelConfig::setCalledFunctions(MMO_SymbolRefTable f)
{
  _calledFunctions = f;
}

void
MMO_ModelConfig::setCalculateAlgegraics(bool ca)
{
  _calculateAlgebraics = ca;
}

bool
MMO_ModelConfig::calculateAlgebraics()
{
  return _calculateAlgebraics;
}

MMO_SymbolRefTable
MMO_ModelConfig::calledFunctions()
{
  return _calledFunctions;
}

void
MMO_ModelConfig::setFunctions(MMO_FunctionTable f)
{
  _functions = f;
}

MMO_FunctionTable
MMO_ModelConfig::functions()
{
  return _functions;
}

void
MMO_ModelConfig::setPackages(MMO_PackageTable p)
{
  _packages = p;
}

MMO_PackageTable
MMO_ModelConfig::packages()
{
  return _packages;
}

void
MMO_ModelConfig::setExternalFunctions(MMO_FunctionTable ef)
{
  _externalFunctions = ef;
}

MMO_FunctionTable
MMO_ModelConfig::externalFunctions()
{
  return _externalFunctions;
}

void
MMO_ModelConfig::setAlgebraics(MMO_EquationTable algs)
{
  _algebraics = algs;
}

MMO_EquationTable
MMO_ModelConfig::algebraics()
{
  return _algebraics;
}

void
MMO_ModelConfig::setSymbols(VarSymbolTable symbols)
{
  _symbols = symbols;
}

VarSymbolTable
MMO_ModelConfig::symbols()
{
  return _symbols;
}

void
MMO_ModelConfig::setInitialCode(bool s)
{
  _initialCode = s;
}

bool
MMO_ModelConfig::initialCode()
{
  return _initialCode;
}

bool
MMO_ModelConfig::generateDerivatives()
{
  return _generateDerivatives;
}

void
MMO_ModelConfig::setGenerateDerivatives(bool gd)
{
  _generateDerivatives = gd;
}

int
MMO_ModelConfig::begin()
{
  return _range.begin(0);
}

void
MMO_ModelConfig::setBegin(int begin, int dim)
{
  _range.setBegin(begin, dim);
}

int
MMO_ModelConfig::end()
{
  return _range.end(0);
}

void
MMO_ModelConfig::setRange(Range range)
{
  _range = range;
}

Range
MMO_ModelConfig::range()
{
  return _range;
}

void
MMO_ModelConfig::setEnd(int end, int dim)
{
  _range.setEnd(end, dim);
}

void
MMO_ModelConfig::setLHS(Index idx)
{
  _lhs = idx;
}

Index
MMO_ModelConfig::lhs()
{
  return _lhs;
}

void
MMO_ModelConfig::clear()
{
  _initialCode = false;
  _range.clear();
  _lhs.clear();
  _generateDerivatives = true;
  _whenStatement = false;
}

void
MMO_ModelConfig::setWhenStatement(bool ws)
{
  _whenStatement = ws;
}

bool
MMO_ModelConfig::whenStatement()
{
  return _whenStatement;
}

bool
MMO_ModelConfig::hasWeight()
{
  return _weight >= 0;
}

void
MMO_ModelConfig::setWeight(double w)
{
  _weight = w;
}

double
MMO_ModelConfig::weight()
{
  return _weight;
}

MMO_FunctionConfig::MMO_FunctionConfig(string var, string name, AST_ExpressionList args, MMO_ModelConfig cfg) :
    _var(var), 
    _name(name), 
    _args(args), 
    _config(cfg)
{
}

MMO_FunctionConfig::~MMO_FunctionConfig()
{
}

list<string>
MMO_FunctionConfig::print(string indent)
{
  list<string> ret;
  stringstream buffer;
  if(!_var.empty())
  {
    buffer << _var << " = ";
  }
  buffer << _name << "(";
  if(_args != NULL)
  {
    VarSymbolTable vt = _data->symbols();
    vt->setPrintEnvironment(VST_FUNCTION);
    AST_ExpressionListIterator it;
    unsigned int count = 0;
    foreach(it,_args)
    {
      MMO_Expression e = newMMO_Expression(current_element(it), _data);
      buffer << e->print(indent);
      if(++count < _args->size())
      {
        buffer << ",";
      }
      list<string> code = e->getCode();
      ret.insert(ret.end(), code.begin(), code.end());
    }
  }
  buffer << ");";
  ret.push_back(buffer.str());
  return ret;
}

MMO_FunctionInfo::MMO_FunctionInfo(MMO_Function f) :
    _f(f), _fd(NULL)
{
}

MMO_FunctionInfo::MMO_FunctionInfo(MMO_FunctionDefinition fd) :
    _f(NULL), _fd(fd)
{
}

MMO_FunctionInfo::~MMO_FunctionInfo()
{
}

bool
MMO_FunctionInfo::isFunction()
{
  return _f != NULL;
}

bool
MMO_FunctionInfo::isFunctionDefinition()
{
  return _fd != NULL;
}

MMO_Function
MMO_FunctionInfo::function()
{
  return _f;
}

MMO_FunctionDefinition
MMO_FunctionInfo::functionDefinition()
{
  return _fd;
}

MMO_PackageConfig::MMO_PackageConfig(string name, MMO_FunctionDefinitionTable fit,
    MMO_ImportTable objects) :
    _name(name), _fit(fit), _objects(objects)
{
}

MMO_PackageConfig::~MMO_PackageConfig()
{
}

string
MMO_PackageConfig::name()
{
  return _name;
}

string
MMO_PackageConfig::prefix()
{
  return "__" + _name + "__";
}

MMO_FunctionDefinitionTable
MMO_PackageConfig::functions()
{
  return _fit;
}

MMO_ImportTable
MMO_PackageConfig::objects()
{
  return _objects;
}

list<string>
MMO_PackageConfig::linkLibraries()
{
  list<string> ret;
  for(MMO_FunctionDefinition fd = _fit->begin(); !_fit->end(); fd =
      _fit->next())
  {
    if(fd->hasLibraries())
    {
      list<string> libs = fd->libraries();
      ret.insert(ret.end(), libs.begin(), libs.end());
    }
  }
  return ret;
}

list<string>
MMO_PackageConfig::libraryDirectories()
{
  list<string> ret;
  for(MMO_FunctionDefinition fd = _fit->begin(); !_fit->end(); fd =
      _fit->next())
  {
    if(fd->hasLibraryDirectory())
    {
      ret.push_back(fd->libraryDirectory());
    }
  }
  return ret;
}

list<string>
MMO_PackageConfig::includeDirectories()
{
  list<string> ret;
  for(MMO_FunctionDefinition fd = _fit->begin(); !_fit->end(); fd =
      _fit->next())
  {
    if(fd->hasIncludeDirectory())
    {
      ret.push_back(fd->includeDirectory());
    }
  }
  return ret;
}

MMO_PackageData
newMMO_PackageData(string name, MMO_FunctionDefinitionTable fit,
    MMO_ImportTable objects)
{
  return new MMO_PackageConfig(name, fit, objects);
}

void
deleteMMO_PackageData(MMO_PackageData m)
{
  delete m;
}

MMO_FunctionDefinition::MMO_FunctionDefinition(string name, string includeDir, string libraryDir, list<string> libraries) :
  _def(), 
  _name(name), 
  _prototype(), 
  _includeDirectory(includeDir), 
  _libraryDirectory(libraryDir), 
  _libraries(libraries)
{
}

MMO_FunctionDefinition::~MMO_FunctionDefinition()
{
}

bool
MMO_FunctionDefinition::hasIncludeDirectory()
{
  return !_includeDirectory.empty();
}

bool
MMO_FunctionDefinition::hasLibraryDirectory()
{
  return !_libraryDirectory.empty();
}

bool
MMO_FunctionDefinition::hasLibraries()
{
  return _libraries.size() > 0;
}

string
MMO_FunctionDefinition::includeDirectory()
{
  return _includeDirectory;
}

string
MMO_FunctionDefinition::libraryDirectory()
{
  return _libraryDirectory;
}

list<string>
MMO_FunctionDefinition::libraries()
{
  return _libraries;
}

string
MMO_FunctionDefinition::name()
{
  return _name;
}

list<string>
MMO_FunctionDefinition::def()
{
  return _def;
}

string
MMO_FunctionDefinition::prototype()
{
  return _prototype;
}

void
MMO_ModelConfig::setArguments(AST_Expression arguments)
{
  _arguments = arguments;
}

bool
MMO_ModelConfig::hasArguments()
{
  return _arguments != NULL;
}

AST_Expression
MMO_ModelConfig::arguments()
{
  return _arguments;
}

void
MMO_ModelConfig::setAnnotation(MMO_Annotation annotation)
{
  _annotation = annotation;
}

MMO_Annotation
MMO_ModelConfig::annotation()
{
  return _annotation;
}

bool
MMO_ModelConfig::hasAnnotation()
{
  return _annotation != NULL;
}

void
MMO_ModelConfig::setDisableSymDiff(bool d)
{
  _disableSymDiff = d;
}

bool
MMO_ModelConfig::disableSymDiff()
{
  return _disableSymDiff;
}
