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

#ifndef MMO_CLASS_H_
#define MMO_CLASS_H_

#include <list>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../util/error.h"
#include "../util/util_types.h"
#include "mmo_base.h"
#include "mmo_types.h"

/**
 *
 */
typedef enum
{
  CL_MODEL,  //!< CL_MODEL
  CL_PACKAGE,  //!< CL_PACKAGE
  CL_FUNCTION  //!< CL_FUNCTION
} CL_Type;

/**
 *
 */
typedef enum
{
  DEC_PUBLIC,  //!< DEC_PUBLIC
  DEC_LOCAL  //!< DEC_LOCAL
} DEC_Type;

/**
 *
 */
class MMO_Class_ : public MMO_Base_
{
public:
  /**
   *
   */
  MMO_Class_ ();
  /**
   *
   */
  ~MMO_Class_ ();
  /**
   *
   * @return
   */
  MMO_Package
  getAsPackage ();
  /**
   *
   * @return
   */
  MMO_Model
  getAsModel ();
  /**
   *
   * @return
   */
  MMO_Function
  getAsFunction ();
  /**
   *
   * @param c
   */
  void
  setFather (MMO_Class c);
  /**
   *
   * @return
   */
  bool
  hasFather ();
  /**
   *
   * @return
   */
  MMO_Class
  father () const;
  /**
   *
   * @return
   */
  string
  print ();
  /**
   *
   * @return
   */
  virtual string
  name () const = 0;
  /**
   *
   * @return
   */
  virtual CL_Type
  classType () = 0;
  /**
   *
   * @param n
   */
  virtual void
  insert (string n) = 0;
  /**
   *
   * @param eq
   */
  virtual void
  insert (AST_Equation eq) = 0;
  /**
   *
   * @param stm
   * @param initial
   */
  virtual void
  insert (AST_Statement stm, bool initial) = 0;
  /**
   *
   * @param stm
   */
  virtual void
  insert (AST_Statement stm) = 0;
  /**
   *
   * @param f
   */
  virtual void
  insert (MMO_Function f) = 0;
  /**
   *
   * @param efc
   */
  virtual void
  insert (AST_External_Function_Call efc) = 0;
  /**
   *
   * @param n
   * @param vi
   * @param type
   */
  virtual void
  insert (VarName n, VarInfo vi, DEC_Type type) = 0;
  /**
   *
   * @param n
   * @param vi
   */
  virtual void
  insert (VarName n, VarInfo vi) = 0;
  /**
   *
   * @param x
   */
  virtual void
  insert (AST_Argument_Modification x) = 0;
  /**
   *
   * @return
   */
  virtual VarSymbolTable
  varTable () = 0;
  /**
   *
   * @return
   */
  virtual MMO_ImportTable
  imports () = 0;
private:
  MMO_Class _father;
};

/**
 *
 */
class MMO_Model_ : public MMO_Class_
{
public:
  /**
   *
   * @param name
   */
  MMO_Model_ (string name);
  /**
   *
   */
  ~MMO_Model_ ();
  /**
   *
   * @return
   */
  string
  name () const;
  /**
   *
   * @return
   */
  CL_Type
  classType ();
  /**
   *
   * @param n
   */
  void
  insert (string n);
  /**
   *
   * @param n
   * @param vi
   * @param type
   */
  void
  insert (VarName n, VarInfo vi, DEC_Type type);
  /**
   *
   * @param n
   * @param vi
   */
  void
  insert (VarName n, VarInfo vi);
  /**
   *
   * @param eq
   */
  void
  insert (AST_Equation eq);
  /**
   *
   * @param stm
   * @param initial
   */
  void
  insert (AST_Statement stm, bool initial);
  /**
   *
   * @param stm
   */
  void
  insert (AST_Statement stm);
  /**
   *
   * @param f
   */
  void
  insert (MMO_Function f);
  /**
   *
   * @param efc
   */
  void
  insert (AST_External_Function_Call efc);
  /**
   *
   * @param x
   */
  void
  insert (AST_Argument_Modification x);
  /**
   *
   * @return
   */
  VarSymbolTable
  varTable ();
  /**
   *
   * @return
   */
  MMO_EquationTable
  derivatives ();
  /**
   *
   * @return
   */
  MMO_EquationTable
  algebraics ();
  /**
   *
   * @return
   */
  MMO_EquationTable
  outputs ();
  /**
   *
   * @return
   */
  MMO_Annotation
  annotation ();
  /**
   *
   * @return
   */
  MMO_StatementTable
  initialCode ();
  /**
   *
   * @return
   */
  MMO_EventTable
  events ();
  /**
   *
   * @return
   */
  MMO_ImportTable
  imports ();
  /**
   *
   * @return
   */
  MMO_FunctionTable
  functions ();
  /**
   *
   * @return
   */
  MMO_SymbolRefTable
  calledFunctions ();
  /**
   *
   * @return
   */
  bool
  hasExternalFunctions ();
  /**
   *
   */
  void
  setEquations ();
  /**
   *
   */
  void
  setEvents ();
  /**
   *
   */
  void
  initOutput ();
  /**
   *
   * @return
   */
  unsigned int
  states ();
  /**
   *
   * @return
   */
  unsigned int
  discretes ();
  /**
   *
   * @return
   */
  unsigned int
  algs ();
  /**
   *
   * @return
   */
  unsigned int
  evs ();
  /**
   *
   * @return
   */
  unsigned int
  funcs ();
  /**
   *
   * @return
   */
  unsigned int
  inputs ();
  /**
   *
   * @return
   */
  unsigned int
  outs ();
  /**
   *
   * @return
   */
  int
  imps ();
  /**
   *
   * @param idx
   * @return
   */
  int
  outputIndexes (Index idx);
  /**
   *
   * @return
   */
  list<string>
  includeDirectories ();
  /**
   *
   * @return
   */
  list<string>
  libraryDirectories ();
  /**
   *
   * @return
   */
  list<string>
  linkLibraries ();
  /**
   *
   * @param vi
   * @param indent
   * @param localVar
   * @return
   */
  string
  printInitialAssignment (VarInfo vi, string indent, string localVar = "i");
  /**
    *
    * @param ft
    */
   void
   setExternalFunctions (MMO_FunctionTable ft);
private:
  double
  _scalarValue (AST_Expression exp);
  AST_Expression_ComponentReference
  _getComponentReference (AST_Expression exp);
  VarInfo
  _variableLookup (string name, AST_Expression exp = NULL, ER_Type type =
		       ER_Error);
  bool
  _controlScalarExpression (AST_Expression exp);
  string
  _transformExpression (AST_Expression left, AST_Expression right);
  AST_Statement
  _transformStatement (AST_Statement st);
  AST_Equation
  _transformEquation (AST_Equation eq);
  int
  _evalExp (AST_Expression exp);
  Index
  _getAlgebraicIndex (AST_Expression left, int begin, int end);
  string
  _getComponentName (AST_Expression exp);
  bool
  _isArray (AST_Expression exp);
  list<string>
  _getFunctionDependencies (map<string, string> mdeps);
  void
  _getFunctionInfo (MMO_Function f);
  void
  _setAlgebraic (AST_Expression left, AST_Expression right, int begin,
		    int end, AST_Expression arguments = NULL);
  void
  _insertEvent (AST_Statement stm, int begin, int end);
  void
  _insertEquation (AST_Equation eq, int begin, int end);
  void
  _insertAlgebraicEquation (AST_Equation eq, int begin, int end);
  void
  _equationTraverse (void (MMO_Model_::*tr)(AST_Equation,int,int));
  void
  _controlEquation (AST_Equation eq);
  void
  _insertFunctionDependencies (list<string> deps, map<string, string> *mdeps);
  void
  _setRealVariables (AST_Equation eq, int begin, int end);
  void
  _setEvents ();
  AST_Statement_ElseList
  _transformStatementElse (AST_Statement_ElseList stel);
  AST_StatementList
  _transformStatementList (AST_StatementList sts);
  void
  _setIndex (AST_Expression derArg, VarInfo vi, Index *idx);
  void
  _controlDiscreteVariables (AST_Expression expEq);
  void
  _setRange (int begin, int end, Index *lhs);
  void
  _setAlgebraicOffset (AST_Expression left, int begin, int end);
  void
  _insertAlgebraic (AST_Equation eq, int begin, int end);
  MMO_EquationTable _derivatives;
  MMO_EquationTable _algebraics;
  MMO_EquationTable _outputs;
  MMO_EventTable _events;
  MMO_ModelAnnotation _annotations;
  MMO_StatementTable _initialCode;
  unsigned int _states;
  unsigned int _discretes;
  unsigned int _evs;
  unsigned int _algs;
  unsigned int _parameters;
  unsigned int _funcs;
  unsigned int _inputs;
  unsigned int _output;
  unsigned int _stateEquations;
  unsigned int _algebraicEquations;
  VarSymbolTable _declarations;
  TypeSymbolTable _types;
  MMO_ImportTable _imports;
  MMO_FunctionTable _functions;
  MMO_FunctionTable _externalFunctions;
  MMO_SymbolRefTable _calledFunctions;
  string _name;
  MMO_ModelData _data;
  list<AST_Equation> _eqs;
  list<AST_Statement> _stms;
  map<Index, int> _outputIndexes;
  MMO_PackageTable _packages;
  map<string, string> _includeDirectories;
  map<string, string> _libraryDirectories;
  map<string, string> _linkLibraries;
  map<string, string> _symbolicDerivatives;
  map<string, int> _algebraicOffset;
  map<string, int> _algebraicCurrentOffset;
};
/**
 *
 * @param name
 * @return
 */
MMO_Model
newMMO_Model (string name);
/**
 *
 * @param m
 */
void
deleteMMO_Model (MMO_Model m);

/**
 *
 */
class MMO_Function_ : public MMO_Class_
{
public:
  /**
   *
   * @param name
   */
  MMO_Function_ (string name);
  /**
   *
   */
  ~MMO_Function_ ();
  /**
   *
   * @return
   */
  string
  name () const;
  /**
   *
   * @param efc
   */
  void
  insert (AST_External_Function_Call efc);
  /**
   *
   * @param n
   * @param vi
   * @param type
   */
  void
  insert (VarName n, VarInfo vi, DEC_Type type);
  /**
   *
   * @param n
   * @param vi
   */
  void
  insert (VarName n, VarInfo vi);
  /**
   *
   * @param eq
   */
  void
  insert (AST_Equation eq);
  /**
   *
   * @param stm
   * @param initial
   */
  void
  insert (AST_Statement stm, bool initial);
  /**
   *
   * @param stm
   */
  void
  insert (AST_Statement stm);
  /**
   *
   * @param f
   */
  void
  insert (MMO_Function f);
  /**
   *
   * @param n
   */
  void
  insert (string n);
  /**
   *
   * @param x
   */
  void
  insert (AST_Argument_Modification x);
  /**
   *
   * @return
   */
  VarSymbolTable
  varTable ();
  /**
   *
   * @return
   */
  CL_Type
  classType ();
  /**
   *
   * @return
   */
  string
  prototype ();
  /**
   *
   * @return
   */
  list<string>
  localDeclarations ();
  /**
   *
   * @return
   */
  MMO_StatementTable
  statements ();
  /**
   *
   * @return
   */
  string
  returnStatement ();
  /**
   *
   * @return
   */
  MMO_ArgumentsTable
  externalFunctionCalls ();
  /**
   *
   * @return
   */
  MMO_ImportTable
  imports ();
  /**
   *
   * @param it
   */
  void
  setImports (MMO_ImportTable it);
  /**
   *
   * @param prefix
   */
  void
  setPrefix (string prefix);
  /**
   *
   * @return
   */
  MMO_Annotation
  annotation ();
  void
  setFunctions (MMO_FunctionTable functions, MMO_FunctionTable externalFunctions, MMO_SymbolRefTable calledFunctions);
private:
  string _name;
  VarSymbolTable _declarations;
  VarSymbolTable _localDeclarations;
  MMO_StatementTable _statements;
  MMO_FunctionAnnotation _annotations;
  MMO_ArgumentsTable _externalFunctionCalls;
  MMO_SymbolRefTable _calledFunctions;
  MMO_ImportTable _imports;
  unsigned int _externalFuncs;
  int _outputs;
  string _outputName;
  MMO_ModelData _data;
  MMO_PackageTable _packages;
  MMO_FunctionTable _functions;
  MMO_FunctionTable _externalFunctions;
  string _prefix;
  list<VarInfo> _arguments;
  TypeSymbolTable _types;
};
/**
 *
 * @param name
 * @return
 */
MMO_Function
newMMO_Function (string name);
/**
 *
 * @param m
 */
void
deleteMMO_Function (MMO_Function m);

/**
 *
 */
class MMO_Package_ : public MMO_Class_
{
public:
  /**
   *
   * @param name
   */
  MMO_Package_ (string name);
  /**
   *
   */
  ~MMO_Package_ ();
  /**
   *
   * @return
   */
  VarSymbolTable
  varTable ();
  /**
   *
   * @return
   */
  CL_Type
  classType ();
  /**
   *
   * @return
   */
  string
  name () const;
  /**
   *
   * @return
   */
  string
  fileName () const;
  /**
   *
   * @param n
   */
  void
  insert (string n);
  /**
   *
   * @param eq
   */
  void
  insert (AST_Equation eq);
  /**
   *
   * @param stm
   * @param initial
   */
  void
  insert (AST_Statement stm, bool initial);
  /**
   *
   * @param stm
   */
  void
  insert (AST_Statement stm);
  /**
   *
   * @param f
   */
  void
  insert (MMO_Function f);
  /**
   *
   * @param efc
   */
  void
  insert (AST_External_Function_Call efc);
  /**
   *
   * @param n
   * @param vi
   * @param type
   */
  void
  insert (VarName n, VarInfo vi, DEC_Type type);
  /**
   *
   * @param n
   * @param vi
   */
  void
  insert (VarName n, VarInfo vi);
  /**
   *
   * @param x
   */
  void
  insert (AST_Argument_Modification x);
  /**
   *
   * @return
   */
  MMO_FunctionTable
  functions ();
  /**
   *
   * @return
   */
  MMO_ImportTable
  imports ();
  /**
   *
   * @return
   */
  string
  prefix ();
private:
  MMO_FunctionTable _functions;
  string _name;
  unsigned int _funcs;
  MMO_ImportTable _imports;
  MMO_PackageTable _packages;
};
/**
 *
 * @param name
 * @return
 */
MMO_Package
newMMO_Package (string name);
/**
 *
 * @param m
 */
void
deleteMMO_Package (MMO_Package m);

#endif  /* MMO_CLASS_H_ */
