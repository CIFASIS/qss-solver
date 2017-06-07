#include "mmo_util.h"

#include <sstream>

#include "../ast/expression.h"
#include "../util/symbol_table.h"
#include "annotation.h"
#include "class.h"
#include "expression.h"

/* MMO_ModelData class */

MMO_ModelData_::MMO_ModelData_ () :
        _symbols (NULL), _algebraics (NULL), _functions (NULL), _externalFunctions (
        NULL), _packages (NULL), _initialCode (false), _generateDerivatives (true), _begin (0), _end (0), _lhs (), _whenStatement (false), _weight (
                -1), _calledFunctions (), _calculateAlgebraics (false), _arguments (NULL), _annotation (NULL), _disableSymDiff (false)
{
}

MMO_ModelData_::~MMO_ModelData_ ()
{
}

bool
MMO_ModelData_::hasExternalFunctions ()
{
    return (_externalFunctions != NULL);
}

void
MMO_ModelData_::setCalledFunctions (MMO_SymbolRefTable f)
{
    _calledFunctions = f;
}

void
MMO_ModelData_::setCalculateAlgegraics (bool ca)
{
    _calculateAlgebraics = ca;
}

bool
MMO_ModelData_::calculateAlgebraics ()
{
    return (_calculateAlgebraics);
}

MMO_SymbolRefTable
MMO_ModelData_::calledFunctions ()
{
    return (_calledFunctions);
}

void
MMO_ModelData_::setFunctions (MMO_FunctionTable f)
{
    _functions = f;
}

MMO_FunctionTable
MMO_ModelData_::functions ()
{
    return (_functions);
}

void
MMO_ModelData_::setPackages (MMO_PackageTable p)
{
    _packages = p;
}

MMO_PackageTable
MMO_ModelData_::packages ()
{
    return (_packages);
}

void
MMO_ModelData_::setExternalFunctions (MMO_FunctionTable ef)
{
    _externalFunctions = ef;
}

MMO_FunctionTable
MMO_ModelData_::externalFunctions ()
{
    return (_externalFunctions);
}

void
MMO_ModelData_::setAlgebraics (MMO_EquationTable algs)
{
    _algebraics = algs;
}

MMO_EquationTable
MMO_ModelData_::algebraics ()
{
    return (_algebraics);
}

void
MMO_ModelData_::setSymbols (VarSymbolTable symbols)
{
    _symbols = symbols;
}

VarSymbolTable
MMO_ModelData_::symbols ()
{
    return (_symbols);
}

void
MMO_ModelData_::setInitialCode (bool s)
{
    _initialCode = s;
}

bool
MMO_ModelData_::initialCode ()
{
    return (_initialCode);
}

bool
MMO_ModelData_::generateDerivatives ()
{
    return (_generateDerivatives);
}

void
MMO_ModelData_::setGenerateDerivatives (bool gd)
{
    _generateDerivatives = gd;
}

int
MMO_ModelData_::begin ()
{
    return (_begin);
}

void
MMO_ModelData_::setBegin (int begin)
{
    _begin = begin;
}

int
MMO_ModelData_::end ()
{
    return (_end);
}

void
MMO_ModelData_::setEnd (int end)
{
    _end = end;
}

void
MMO_ModelData_::setLHS (Index idx)
{
    _lhs = idx;
}

Index
MMO_ModelData_::lhs ()
{
    return (_lhs);
}

void
MMO_ModelData_::clear ()
{
    _initialCode = false;
    _begin = 0;
    _end = 0;
    _lhs.clear ();
    _generateDerivatives = true;
    _whenStatement = false;
}

void
MMO_ModelData_::setWhenStatement (bool ws)
{
    _whenStatement = ws;
}

bool
MMO_ModelData_::whenStatement ()
{
    return (_whenStatement);
}

bool
MMO_ModelData_::hasWeight ()
{
    return (_weight >= 0);
}

void
MMO_ModelData_::setWeight (double w)
{
    _weight = w;
}

double
MMO_ModelData_::weight ()
{
    return (_weight);
}

MMO_ModelData
newMMO_ModelData ()
{
    return (new MMO_ModelData_ ());
}

void
deleteMMO_ModelData (MMO_ModelData m)
{
    delete m;
}

MMO_FunctionData_::MMO_FunctionData_ (string var, string name, AST_ExpressionList args, MMO_ModelData data) :
        _var (var), _name (name), _args (args), _data (data)
{
}

MMO_FunctionData_::~MMO_FunctionData_ ()
{
}

list<string>
MMO_FunctionData_::print (string indent)
{
    list<string> ret;
    stringstream buffer;
    if (!_var.empty ())
    {
        buffer << _var << " = ";
    }
    buffer << _name << "(";
    if (_args != NULL)
    {
        VarSymbolTable vt = _data->symbols ();
        vt->setPrintEnvironment (VST_FUNCTION);
        AST_ExpressionListIterator it;
        unsigned int count = 0;
        foreach(it,_args)
        {
            MMO_Expression e = newMMO_Expression (current_element(it), _data);
            buffer << e->print (indent);
            if (++count < _args->size ())
            {
                buffer << ",";
            }
            list<string> code = e->getCode ();
            ret.insert (ret.end (), code.begin (), code.end ());
        }
    }
    buffer << ");";
    ret.push_back (buffer.str ());
    return (ret);
}

MMO_FunctionData
newMMO_FunctionData (string var, string name, AST_ExpressionList args, MMO_ModelData data)
{
    return (new MMO_FunctionData_ (var, name, args, data));
}

void
deleteMMO_FunctionData (MMO_FunctionData m)
{
    delete m;
}

MMO_FunctionInfo::MMO_FunctionInfo (MMO_Function f) :
        _f (f), _fd (NULL)
{
}

MMO_FunctionInfo::MMO_FunctionInfo (MMO_FunctionDefinition fd) :
        _f (NULL), _fd (fd)
{
}

MMO_FunctionInfo::~MMO_FunctionInfo ()
{
}

bool
MMO_FunctionInfo::isFunction ()
{
    return (_f != NULL);
}

bool
MMO_FunctionInfo::isFunctionDefinition ()
{
    return (_fd != NULL);
}

MMO_Function
MMO_FunctionInfo::function ()
{
    return (_f);
}

MMO_FunctionDefinition
MMO_FunctionInfo::functionDefinition ()
{
    return (_fd);
}

MMO_PackageData_::MMO_PackageData_ (string name, MMO_FunctionDefinitionTable fit, MMO_ImportTable objects) :
        _name (name), _fit (fit), _objects (objects)
{
}

MMO_PackageData_::~MMO_PackageData_ ()
{
}

string
MMO_PackageData_::name ()
{
    return (_name);
}

string
MMO_PackageData_::prefix ()
{
    return ("__" + _name + "__");
}

MMO_FunctionDefinitionTable
MMO_PackageData_::functions ()
{
    return (_fit);
}

MMO_ImportTable
MMO_PackageData_::objects ()
{
    return (_objects);
}

list<string>
MMO_PackageData_::linkLibraries ()
{
    list<string> ret;
    for (MMO_FunctionDefinition fd = _fit->begin (); !_fit->end (); fd = _fit->next ())
    {
        if (fd->hasLibraries ())
        {
            list<string> libs = fd->libraries ();
            ret.insert (ret.end (), libs.begin (), libs.end ());
        }
    }
    return (ret);
}

list<string>
MMO_PackageData_::libraryDirectories ()
{
    list<string> ret;
    for (MMO_FunctionDefinition fd = _fit->begin (); !_fit->end (); fd = _fit->next ())
    {
        if (fd->hasLibraryDirectory ())
        {
            ret.push_back (fd->libraryDirectory ());
        }
    }
    return (ret);
}

list<string>
MMO_PackageData_::includeDirectories ()
{
    list<string> ret;
    for (MMO_FunctionDefinition fd = _fit->begin (); !_fit->end (); fd = _fit->next ())
    {
        if (fd->hasIncludeDirectory ())
        {
            ret.push_back (fd->includeDirectory ());
        }
    }
    return (ret);
}

MMO_PackageData
newMMO_PackageData (string name, MMO_FunctionDefinitionTable fit, MMO_ImportTable objects)
{
    return (new MMO_PackageData_ (name, fit, objects));
}

void
deleteMMO_PackageData (MMO_PackageData m)
{
    delete m;
}

MMO_FunctionDefinition_::MMO_FunctionDefinition_ (string name, string includeDir, string libraryDir, list<string> libraries) :
        _def (), _name (name), _prototype (), _includeDirectory (includeDir), _libraryDirectory (libraryDir), _libraries (libraries)
{
}

MMO_FunctionDefinition_::~MMO_FunctionDefinition_ ()
{
}

bool
MMO_FunctionDefinition_::hasIncludeDirectory ()
{
    return (!_includeDirectory.empty ());
}

bool
MMO_FunctionDefinition_::hasLibraryDirectory ()
{
    return (!_libraryDirectory.empty ());
}

bool
MMO_FunctionDefinition_::hasLibraries ()
{
    return (_libraries.size () > 0);
}

string
MMO_FunctionDefinition_::includeDirectory ()
{
    return (_includeDirectory);
}

string
MMO_FunctionDefinition_::libraryDirectory ()
{
    return (_libraryDirectory);
}

list<string>
MMO_FunctionDefinition_::libraries ()
{
    return (_libraries);
}

string
MMO_FunctionDefinition_::name ()
{
    return (_name);
}

list<string>
MMO_FunctionDefinition_::def ()
{
    return (_def);
}

string
MMO_FunctionDefinition_::prototype ()
{
    return (_prototype);
}

MMO_FunctionDefinition
newMMO_FunctionDefinition (string name, string includeDir, string libraryDir, list<string> libraries)
{
    return (new MMO_FunctionDefinition_ (name, includeDir, libraryDir, libraries));
}

void
deleteMMO_FunctionDefinition (MMO_FunctionDefinition m)
{
    delete m;
}

void
MMO_ModelData_::setArguments (AST_Expression arguments)
{
    _arguments = arguments;
}

bool
MMO_ModelData_::hasArguments ()
{
    return (_arguments != NULL);
}

AST_Expression
MMO_ModelData_::arguments ()
{
    return (_arguments);
}

void
MMO_ModelData_::setAnnotation (MMO_Annotation annotation)
{
    _annotation = annotation;
}

MMO_Annotation
MMO_ModelData_::annotation ()
{
    return (_annotation);
}

bool
MMO_ModelData_::hasAnnotation ()
{
    return (_annotation != NULL);
}

void
MMO_ModelData_::setDisableSymDiff (bool d)
{
    _disableSymDiff = d;
}

bool
MMO_ModelData_::disableSymDiff ()
{
    return (_disableSymDiff);
}
