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

#include "class.h"

#include <iterator>
#include <sstream>
#include <utility>

#include "../ast/ast_builder.h"
#include "../ast/composition.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../parser/parse.h"
#include "../util/ast_util.h"
#include "../util/dependencies.h"
#include "../util/md_index.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "../util/util.h"
#include "annotation.h"
#include "equation.h"
#include "event.h"
#include "expression.h"
#include "mmo_util.h"
#include "statement.h"

/* MMO_Class class. */

MMO_Class_::MMO_Class_ () :
        _father (NULL)
{
}

MMO_Class_::~MMO_Class_ ()
{
}

MMO_Package
MMO_Class_::getAsPackage ()
{
    return (dynamic_cast<MMO_Package> (this));
}

MMO_Model
MMO_Class_::getAsModel ()
{
    return (dynamic_cast<MMO_Model> (this));
}

MMO_Function
MMO_Class_::getAsFunction ()
{
    return (dynamic_cast<MMO_Function> (this));
}

void
MMO_Class_::setFather (MMO_Class c)
{
    _father = c;
}

bool
MMO_Class_::hasFather ()
{
    return (_father != NULL);
}

MMO_Class
MMO_Class_::father () const
{
    return (_father);
}

string
MMO_Class_::print ()
{
    string ret;
    return (ret);
}

/* MMO_Model class. */

MMO_Model_::MMO_Model_ (string name) :
        _initialCode (), _states (0), _discretes (0), _evs (0), _algs (0), _parameters (0), _funcs (), _inputs (0), _output (0), _stateEquations (0), _algebraicEquations (
                0), _externalFunctions (
        NULL), _name (name), _eqs (), _stms (), _outputIndexes (), _includeDirectories (), _libraryDirectories (), _linkLibraries (), _symbolicDerivatives (), _algebraicOffset (), _algebraicCurrentOffset ()
{
    _types = newTypeSymbolTable ();
    _declarations = newVarSymbolTable ();
    _declarations->initialize (_types);
    _derivatives = newMMO_EquationTable ();
    _algebraics = newMMO_EquationTable ();
    _outputs = newMMO_EquationTable ();
    _events = newMMO_EventTable ();
    _imports = newMMO_ImportTable ();
    _functions = newMMO_FunctionTable ();
    _calledFunctions = newMMO_SymbolRefTable ();
    _initialCode = newMMO_StatementTable ();
    _packages = newMMO_PackageTable ();
    _data = newMMO_ModelData ();
    _data->setSymbols (_declarations);
    _data->setPackages (_packages);
    _data->setAlgebraics (_algebraics);
    _data->setCalledFunctions (_calledFunctions);
    _data->setFunctions (_functions);
    _annotations = newMMO_ModelAnnotation (_data);
    _data->setAnnotation (_annotations);
}

MMO_Model_::~MMO_Model_ ()
{
}

CL_Type
MMO_Model_::classType ()
{
    return (CL_MODEL);
}

void
MMO_Model_::setExternalFunctions (MMO_FunctionTable ft)
{
    _externalFunctions = ft;
    _data->setExternalFunctions (ft);
    for (MMO_Function f = ft->begin (); !ft->end (); f = ft->next ())
    {
        _getFunctionInfo (f);
    }
}

string
MMO_Model_::printInitialAssignment (VarInfo vi, string indent, string localVar)
{
    Util::getInstance ()->setData (_data);
    return (Util::getInstance ()->printInitialAssignment (vi, indent));
}

void
MMO_Model_::insert (VarName n, VarInfo vi, DEC_Type type)
{
    insert (n, vi);
}

int
MMO_Model_::_evalExp (AST_Expression exp)
{
    MMO_EvalInitExp_ e (_declarations);
    return (e.foldTraverse (exp));
}

void
MMO_Model_::insert (VarName n, VarInfo vi)
{
    vi->setName (n);
    if (vi->typePrefix () & TP_CONSTANT)
    {
        vi->setValue (_evalExp (vi->exp ()));
    }
    else if (vi->typePrefix () & TP_DISCRETE)
    {
        Index idx;
        idx.setOffset (_discretes);
        if (vi->isArray ())
        {
            idx.setLow (1);
            idx.setHi (vi->size ());
        }
        _discretes += vi->size ();
        vi->setIndex (idx);
    }
    _declarations->insert (n, vi);
}

list<string>
MMO_Model_::includeDirectories ()
{
    return (_getFunctionDependencies (_includeDirectories));
}

list<string>
MMO_Model_::libraryDirectories ()
{
    return (_getFunctionDependencies (_libraryDirectories));
}

list<string>
MMO_Model_::linkLibraries ()
{
    return (_getFunctionDependencies (_linkLibraries));
}

string
MMO_Model_::_getComponentName (AST_Expression exp)
{
    string ret;
    if (exp->expressionType () == EXPCOMPREF)
    {
        AST_Expression_ComponentReference ecr = exp->getAsComponentReference ();
        ret = *AST_ListFirst (ecr->names ());
    }
    return (ret);
}

list<string>
MMO_Model_::_getFunctionDependencies (map<string, string> mdeps)
{
    list<string> ret;
    for (map<string, string>::iterator it = mdeps.begin (); it != mdeps.end (); it++)
    {
        ret.push_back (it->second);
    }
    return (ret);
}

Index
MMO_Model_::_getAlgebraicIndex (AST_Expression left, int begin, int end)
{
    AST_Expression_ComponentReference cr = left->getAsComponentReference ();
    string varName = _getComponentName (cr);
    VarInfo vi = _declarations->lookup (varName);
    Index lhs;
    if (vi == NULL)
    {
        Error::getInstance ()->add (left->lineNum (),
        EM_IR | EM_VARIABLE_NOT_FOUND,
                                    ER_Error, "%s", varName.c_str ());
        return (lhs);
    }
    if (cr->hasIndexes ())
    {
        AST_ExpressionList el = AST_ListFirst (cr->indexes ());
        ExpressionIndex_ ei (_declarations);
        lhs = ei.index (AST_ListFirst (el));
        if (lhs.factor () != 0)
        {
            lhs.setLow (begin);
            lhs.setHi (end);
        }
        lhs.setArray ();
    }
    lhs.setOffset (_algebraicOffset[varName]);
    Index viIndex = vi->index ();
    viIndex.setOffset (_algebraicOffset[varName]);
    vi->setIndex (viIndex);
    return (lhs);
}

void
MMO_Model_::_setAlgebraicOffset (AST_Expression left, int begin, int end)
{
    AST_Expression_ComponentReference cr = left->getAsComponentReference ();
    string varName = _getComponentName (cr);
    VarInfo vi = _declarations->lookup (varName);
    if (vi == NULL)
    {
        Error::getInstance ()->add (left->lineNum (),
        EM_IR | EM_VARIABLE_NOT_FOUND,
                                    ER_Error, "%s", varName.c_str ());
        return;
    }
    if (cr->hasIndexes ())
    {
        int range = end - begin + 1;
        if (vi->index ().range () == _algebraicCurrentOffset[varName] + range)
        {
            _algebraicOffset[varName] = _algebraicEquations;
            _algebraicEquations += _algebraicCurrentOffset[varName] + range;
        }
        else
        {
            _algebraicCurrentOffset[varName] += range;
        }
    }
    else
    {
        _algebraicOffset[varName] = _algebraicEquations;
        _algebraicEquations++;
    }
    return;
}

void
MMO_Model_::_setAlgebraic (AST_Expression left, AST_Expression right, int begin, int end, AST_Expression arguments)
{
    _data->clear ();
    AST_Expression_ComponentReference cr = left->getAsComponentReference ();
    string varName = _getComponentName (cr);
    Index lhs = _getAlgebraicIndex (left, begin, end);
    Index idx;
    idx.setConstant (_algebraicEquations);
    _setRange (begin, end, &lhs);
    _data->setLHS (lhs);
    _data->setBegin (begin);
    _data->setEnd (end);
    _data->setArguments (arguments);
    _data->setDisableSymDiff (true);
    MMO_Equation mse = newMMO_Equation (right, _data);
    _data->setDisableSymDiff (false);
    mse->controlAlgebraicDefinition ();
    _data->setArguments (NULL);
    _algebraics->insert (idx, mse);
    _algebraicEquations += end - begin + 1;
}

void
MMO_Model_::_setRange (int begin, int end, Index *lhs)
{
    if (begin > 0 && end > 0)
    {
        lhs->setRange ();
    }
}

/*! \brief Insert an equation defined in the equation section of the model.
 *
 * 	\param eq: AST_Equation to be evaluated.
 * 	\param begin: Begining of the equation range.
 * 	\param end: End of the equation range.
 *
 * 	Description:
 * 		Indentify the equation type which could be algebraic or derivative.
 * 		In the case of a derivative, we proceed as follows:
 * 			-# Generate the corresponding index needed by the equation table.
 * 			-# Update the model state variables count.
 * 			-# Generate the MMO_Equation and calculate the entries of the SD, DD and DS matrices.
 * 			-# Set the VarInfo entry for the variable to state and the corresponding index.
 * 			-# If the equation is not autonomous, update the inputs count of the model.
 *			-# Insert the MMO_Equation in the derivatives table.
 *		In the case of an algebraic equation:
 *			-# 
 */

void
MMO_Model_::_insertEquation (AST_Equation eq, int begin, int end)
{
    if (end < begin)
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong equation range.");
    }
    _controlEquation (eq);
    if (eq->equationType () == EQEQUALITY)
    {
        ExpressionIndex_ ei (_declarations);
        AST_Equation_Equality eqe = eq->getAsEquality ();
        if (eqe->left ()->expressionType () == EXPDERIVATIVE)
        {
            AST_Expression_Derivative ed = eqe->left ()->getAsDerivative ();
            string varName = _getComponentName (AST_ListFirst (ed->arguments ()));
            VarInfo vi = _declarations->lookup (varName);
            if (vi == NULL)
            {
                Error::getInstance ()->add (eqe->left ()->lineNum (),
                EM_IR | EM_VARIABLE_NOT_FOUND,
                                            ER_Error, "%s", varName.c_str ());
                return;
            }
            AST_Expression_ComponentReference edcr = AST_ListFirst (ed->arguments ())->getAsComponentReference ();
            Index idx, lhs;
            if (edcr->hasIndexes ())
            {
                lhs = ei.index (AST_ListFirst (edcr->firstIndex ()));
                if (lhs.factor () != 0)
                {
                    lhs.setLow (begin);
                    lhs.setHi (end);
                }
                lhs.setArray ();
                idx.setArray ();
                _setRange (begin, end, &lhs);
                _setRange (begin, end, &idx);
            }
            lhs.setOffset (vi->index ().offset ());
            idx.setLow (begin);
            idx.setHi (end);
            idx.setFactor (lhs.factor ());
            idx.setOffset (_stateEquations);
            _data->clear ();
            _data->setBegin (begin);
            _data->setEnd (end);
            _data->setLHS (lhs);
            _data->setCalculateAlgegraics (true);
            _data->setDisableSymDiff (true);
            MMO_Equation mse = newMMO_Equation (eqe->right (), _data);
            _data->setCalculateAlgegraics (false);
            _data->setDisableSymDiff (false);
            if (!mse->exp ()->deps ()->autonomous ())
            {
                _inputs += end - begin + 1;
            }
            _derivatives->insert (idx, mse);
            _stateEquations += end - begin + 1;
        }
        else if (eqe->left ()->expressionType () == EXPCOMPREF)
        {
            return;
        }
        else if (eqe->left ()->expressionType () == EXPOUTPUT)
        {
            return;
        }
        else
        {
            Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
        }
    }
    else
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
    }
}

void
MMO_Model_::_insertAlgebraicEquation (AST_Equation eq, int begin, int end)
{
    if (end < begin)
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong equation range.");
    }
    _controlEquation (eq);
    if (eq->equationType () == EQEQUALITY)
    {
        ExpressionIndex_ ei (_declarations);
        AST_Equation_Equality eqe = eq->getAsEquality ();
        if (eqe->left ()->expressionType () == EXPDERIVATIVE)
        {
            return;
        }
        else if (eqe->left ()->expressionType () == EXPCOMPREF)
        {
            _setAlgebraic (eqe->left (), eqe->right (), begin, end);
        }
        else if (eqe->left ()->expressionType () == EXPOUTPUT)
        {
            if (eqe->right ()->expressionType () != EXPCALL)
            {
                Error::getInstance ()->add (eqe->lineNum (),
                EM_IR | EM_UNKNOWN_ODE,
                                            ER_Error, "Insert model equation.");
            }
            AST_Expression_Output eout = eqe->left ()->getAsOutput ();
            AST_ExpressionList el = eout->expressionList ();
            AST_ExpressionListIterator it;
            foreach(it,el)
            {
                _setAlgebraic (current_element(it), eqe->right (), begin, end, eqe->left ());
            }
        }
        else
        {
            Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
        }
    }
    else
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
    }
}

void
MMO_Model_::_insertFunctionDependencies (list<string> deps, map<string, string> *mdeps)
{
    for (list<string>::iterator it = deps.begin (); it != deps.end (); it++)
    {
        mdeps->insert (pair<string, string> (*it, *it));
    }
}

int
MMO_Model_::outputIndexes (Index idx)
{
    return (_outputIndexes[idx]);
}

AST_Statement_ElseList
MMO_Model_::_transformStatementElse (AST_Statement_ElseList stel)
{
    AST_Statement_ElseList stelt = newAST_Statement_ElseList ();
    AST_Statement_ElseListIterator stelit;
    foreach(stelit,stel)
    {
        AST_Statement_Else ceif = current_element(stelit);
        AST_StatementList newElseIfSts = newAST_StatementList ();
        AST_StatementList elseIfSts = ceif->statements ();
        AST_StatementListIterator elseIfStIt;
        foreach(elseIfStIt,elseIfSts)
        {
            AST_ListAppend (newElseIfSts, _transformStatement (current_element(elseIfStIt)));
        }
        AST_Statement_Else newStElse = newAST_Statement_Else (ceif->condition (), newElseIfSts);
        AST_ListAppend (stelt, newStElse);
    }
    return (stelt);
}

AST_StatementList
MMO_Model_::_transformStatementList (AST_StatementList sts)
{
    AST_StatementList stt = newAST_StatementList ();
    AST_StatementListIterator stit;
    foreach(stit,sts)
    {
        AST_ListAppend (stt, _transformStatement (current_element(stit)));
    }
    return (stt);
}

AST_Statement
MMO_Model_::_transformStatement (AST_Statement st)
{
    if (st->statementType () == STASSING)
    {
        MMO_ReplaceInnerProduct rip (_declarations);
        AST_Expression l = st->getAsAssign ()->lhs ();
        AST_Expression r = rip.foldTraverse (st->getAsAssign ()->exp ());
        string transform = _transformExpression (l, r);
        if (transform.empty ())
        {
            AST_Statement retSt = newAST_Statement_Assign (st->getAsAssign ()->lhs (), r);
            return (retSt);
        }
        int rValue;
        AST_Statement retSt = parseStatement (transform, &rValue);
        return (retSt);
    }
    else if (st->statementType () == STFOR)
    {
        AST_Statement_For stf = st->getAsFor ();
        AST_ForIndexList fil = stf->forIndexList ();
        AST_StatementList sts = stf->statements ();
        AST_StatementList stt = _transformStatementList (sts);
        AST_Statement retFor = newAST_Statement_For (fil, stt);
        return (retFor);
    }
    else if (st->statementType () == STIF)
    {
        AST_Statement_If sti = st->getAsIf ();
        AST_StatementList stt = _transformStatementList (sti->statements ());
        AST_StatementList stet = _transformStatementList (sti->else_statements ());
        AST_Statement_ElseList stel = sti->else_if ();
        AST_Statement_ElseList stelt = _transformStatementElse (stel);
        AST_Statement retIf = newAST_Statement_If (sti->condition (), stt, stelt, stet);
        return (retIf);
    }
    else if (st->statementType () == STWHEN)
    {
        AST_Statement_When stWhen = st->getAsWhen ();
        AST_StatementList stList = _transformStatementList (stWhen->statements ());
        AST_Statement_ElseList stElseList = _transformStatementElse (stWhen->else_when ());
        AST_Statement retWhen = newAST_Statement_When (stWhen->condition (), stList, stElseList, stWhen->comment ());
        return (retWhen);
    }
    return (st);
}

string
MMO_Model_::_transformExpression (AST_Expression l, AST_Expression r)
{
    AST_Expression_ComponentReference eleft = _getComponentReference (l);
    if (eleft == NULL)
    {
        return ("");
    }
    string prefix = "";
    string postfix = "";
    if (l->expressionType () == EXPDERIVATIVE)
    {
        prefix = "der(";
        postfix = ")";
    }
    if (r->expressionType () == EXPBINOP)
    {
        AST_Expression_BinOp bo = r->getAsBinOp ();
        if (bo->binopType () == BINOPELADD || bo->binopType () == BINOPELSUB || bo->binopType () == BINOPELDIV || bo->binopType () == BINOPELMULT
                || bo->binopType () == BINOPELEXP)
        {
            AST_Expression left = bo->left ();
            AST_Expression right = bo->right ();
            if (right->expressionType () == EXPCOMPREF && left->expressionType () == EXPCOMPREF)
            {
                AST_Expression_ComponentReference cleft = left->getAsComponentReference ();
                AST_Expression_ComponentReference cright = right->getAsComponentReference ();
                if (!cleft->hasIndexes () && !cright->hasIndexes ())
                {
                    VarInfo lvi = _variableLookup (cleft->name (), cleft, ER_Fatal);
                    VarInfo rvi = _variableLookup (cright->name (), cright, ER_Fatal);
                    if (rvi->size () != lvi->size () && rvi->isArray () && lvi->isArray ())
                    {
                        Error::getInstance ()->add (cright->lineNum (),
                        EM_IR | EM_VARIABLE_NOT_FOUND,
                                                    ER_Fatal, "Different array size in element-wise operation");
                    }
                    VarInfo elvi = _variableLookup (eleft->name (), eleft, ER_Fatal);
                    if (lvi->isArray () > 1 && rvi->isArray ())
                    {
                        if (rvi->size () != elvi->size ())
                        {
                            Error::getInstance ()->add (cright->lineNum (),
                            EM_IR | EM_VARIABLE_NOT_FOUND,
                                                        ER_Fatal, "Different array size in element-wise operation");
                        }
                        _declarations->insert ("i", newVarInfo (newType_Integer (), TP_FOR, NULL, NULL, vector<int> (1,1), false));
                        stringstream buffer;
                        buffer << "for i in 1:" << elvi->size () << " loop" << endl;
                        buffer << prefix << eleft->name () << "[i]" << postfix << " = " << cleft->name () << "[i] "
                                << Util::getInstance ()->opString (bo->binopType ());
                        buffer << cright->name () << "[i];" << endl;
                        buffer << "end for" << endl;
                        return (buffer.str ());
                    }
                    else if (lvi->size () == 1 && rvi->size () > 1)
                    {
                        if (rvi->size () != elvi->size ())
                        {
                            Error::getInstance ()->add (cright->lineNum (),
                            EM_IR | EM_VARIABLE_NOT_FOUND,
                                                        ER_Fatal, "Different array size in element-wise operation");
                        }
                        _declarations->insert ("i", newVarInfo (newType_Integer (), TP_FOR, NULL, NULL, vector<int> (1,1), false));
                        stringstream buffer;
                        buffer << "for i in 1:" << elvi->size () << " loop" << endl;
                        buffer << prefix << eleft->name () << "[i]" << postfix << " = " << cleft->name ()
                                << Util::getInstance ()->opString (bo->binopType ());
                        buffer << cright->name () << "[i];" << endl;
                        buffer << "end for" << endl;
                        return (buffer.str ());
                    }
                    else if (lvi->size () > 1 && rvi->size () == 1)
                    {
                        if (lvi->size () != elvi->size ())
                        {
                            Error::getInstance ()->add (cleft->lineNum (),
                            EM_IR | EM_VARIABLE_NOT_FOUND,
                                                        ER_Error, "Different array size in element-wise operation");
                        }
                        _declarations->insert ("i", newVarInfo (newType_Integer (), TP_FOR, NULL, NULL, vector<int> (1,1), false));
                        stringstream buffer;
                        buffer << "for i in 1:" << elvi->size () << " loop" << endl;
                        buffer << prefix << eleft->name () << "[i]" << postfix << " = " << cleft->name () << "[i]"
                                << Util::getInstance ()->opString (bo->binopType ());
                        buffer << cright->name () << ";" << endl;
                        buffer << "end for" << endl;
                        return (buffer.str ());
                    }
                }
            }
            else if (left->expressionType () == EXPCOMPREF && _controlScalarExpression (right))
            {
                AST_Expression_ComponentReference cleft = left->getAsComponentReference ();
                if (!cleft->hasIndexes ())
                {
                    VarInfo lvi = _variableLookup (cleft->name (), cleft, ER_Fatal);
                    VarInfo elvi = _variableLookup (eleft->name (), eleft, ER_Fatal);
                    if (lvi->size () != elvi->size ())
                    {
                        Error::getInstance ()->add (cleft->lineNum (),
                        EM_IR | EM_VARIABLE_NOT_FOUND,
                                                    ER_Error, "Different array size in element-wise operation");
                    }
                    _declarations->insert ("i", newVarInfo (newType_Integer (), TP_FOR, NULL, NULL, vector<int> (1,1), false));
                    stringstream buffer;
                    buffer << "for i in 1:" << elvi->size () << " loop" << endl;
                    buffer << prefix << eleft->name () << "[i]" << postfix << " = " << cleft->name () << "[i]"
                            << Util::getInstance ()->opString (bo->binopType ());
                    buffer << _scalarValue (right) << ";" << endl;
                    buffer << "end for" << endl;
                    return (buffer.str ());

                }
            }
            else if (right->expressionType () == EXPCOMPREF && _controlScalarExpression (left))
            {
                AST_Expression_ComponentReference cright = right->getAsComponentReference ();
                if (!cright->hasIndexes ())
                {
                    VarInfo lvi = _variableLookup (cright->name (), cright, ER_Fatal);
                    VarInfo elvi = _variableLookup (eleft->name (), eleft, ER_Fatal);
                    if (lvi->size () != elvi->size ())
                    {
                        Error::getInstance ()->add (cright->lineNum (),
                        EM_IR | EM_VARIABLE_NOT_FOUND,
                                                    ER_Error, "Different array size in element-wise operation");
                    }
                    _declarations->insert ("i", newVarInfo (newType_Integer (), TP_FOR, NULL, NULL, vector<int> (1,1), false));
                    stringstream buffer;
                    buffer << "for i in 1:" << elvi->size () << " loop" << endl;
                    buffer << prefix << eleft->name () << "[i]" << postfix << " = " << _scalarValue (left)
                            << Util::getInstance ()->opString (bo->binopType ());
                    buffer << cright->name () << "[i]" << ";" << endl;
                    buffer << "end for" << endl;
                    return (buffer.str ());
                }
            }
        }
    }
    return ("");
}

bool
MMO_Model_::_controlScalarExpression (AST_Expression exp)
{
    if (exp->expressionType () == EXPREAL || exp->expressionType () == EXPINTEGER)
    {
        return (true);
    }
    return (false);
}

VarInfo
MMO_Model_::_variableLookup (string name, AST_Expression exp, ER_Type type)
{
    VarInfo rvi = _declarations->lookup (name);
    int lineNum = 0;
    if (exp != NULL)
    {
        lineNum = exp->lineNum ();
    }
    if (rvi == NULL)
    {
        Error::getInstance ()->add (lineNum,
        EM_IR | EM_VARIABLE_NOT_FOUND,
                                    type, "%s", name.c_str ());
    }
    return (rvi);
}

AST_Expression_ComponentReference
MMO_Model_::_getComponentReference (AST_Expression exp)
{
    if (exp->expressionType () == EXPDERIVATIVE)
    {
        AST_Expression_Derivative ed = exp->getAsDerivative ();
        return (AST_ListFirst (ed->arguments ())->getAsComponentReference ());
    }
    else if (exp->expressionType () == EXPCOMPREF)
    {
        return (exp->getAsComponentReference ());
    }
    return (NULL);
}

double
MMO_Model_::_scalarValue (AST_Expression exp)
{
    if (exp->expressionType () == EXPINTEGER)
    {
        return (exp->getAsInteger ()->val ());
    }
    else if (exp->expressionType () == EXPREAL)
    {
        return (exp->getAsReal ()->val ());
    }
    return (0);
}

void
MMO_Model_::_setEvents ()
{
    for (list<AST_Statement>::iterator st = _stms.begin (); st != _stms.end (); st++)
    {
        AST_Statement stm = *st;
        if (stm->statementType () == STWHEN)
        {
            _insertEvent (stm, 0, 0);
        }
        else if (stm->statementType () == STFOR)
        {
            int range[2], i = 0;
            AST_Statement_For stf = stm->getAsFor ();
            AST_ForIndex fi = AST_ListFirst (stf->forIndexList ());
            AST_Expression in = fi->in_exp ();
            AST_ExpressionList el = in->getAsRange ()->expressionList ();
            AST_ExpressionListIterator eli;
            MMO_EvalInitExp_ eie (_declarations);
            foreach(eli,el)
            {
                range[i++] = eie.foldTraverse (current_element(eli));
            }
            AST_StatementList sts = stf->statements ();
            AST_StatementListIterator stit;
            foreach(stit,sts)
            {
                _insertEvent (current_element(stit), range[0], range[1]);
            }
        }
    }
}

void
MMO_Model_::_insertAlgebraic (AST_Equation eq, int begin, int end)
{
    if (eq->equationType () == EQEQUALITY)
    {
        ExpressionIndex_ ei (_declarations);
        AST_Equation_Equality eqe = eq->getAsEquality ();
        if (eqe->left ()->expressionType () == EXPDERIVATIVE)
        {
            return;
        }
        if (eqe->left ()->expressionType () == EXPCOMPREF)
        {
            _setAlgebraicOffset (eqe->left (), begin, end);
        }
        else if (eqe->left ()->expressionType () == EXPOUTPUT)
        {
            AST_Expression_Output eout = eqe->left ()->getAsOutput ();
            AST_ExpressionList el = eout->expressionList ();
            AST_ExpressionListIterator it;
            list<Index> lidx;
            foreach(it,el)
            {
                _setAlgebraicOffset (current_element(it), begin, end);
            }
        }
        else
        {
            Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
        }
    }
    else
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
    }
}

void
MMO_Model_::_equationTraverse (void
(MMO_Model_::*tr) (AST_Equation, int, int))
{
    list<AST_Equation>::iterator it;
    for (it = _eqs.begin (); it != _eqs.end (); it++)
    {
        AST_Equation eq = current_element(it);
        if (eq->equationType () == EQEQUALITY)
        {
            (this->*tr) (eq, 0, 0);
        }
        else if (eq->equationType () == EQFOR)
        {
            int range[2], i = 0;
            AST_Equation_For eqf = eq->getAsFor ();
            AST_ForIndex fi = AST_ListFirst (eqf->forIndexList ());
            AST_Expression in = fi->in_exp ();
            AST_ExpressionList el = in->getAsRange ()->expressionList ();
            AST_ExpressionListIterator eli;
            MMO_EvalInitExp_ eie (_declarations);
            foreach(eli,el)
            {
                range[i++] = eie.foldTraverse (current_element(eli));
            }
            AST_EquationList eqs = eqf->equationList ();
            AST_EquationListIterator it;
            foreach(it,eqs)
            {
                (this->*tr) (current_element(it), range[0], range[1]);
            }
        }
    }
}

/*! \brief Set all the equations defined in the model.
 *
 * 	Description:
 * 		Once the type of all the variables in the model has been set
 * 		insert the equations and calculate the corresponding
 * 		dependencies.
 */

void
MMO_Model_::setEquations ()
{
    _equationTraverse (&MMO_Model_::_insertAlgebraic);
    _algebraicEquations = 1;
    _equationTraverse (&MMO_Model_::_insertAlgebraicEquation);
    _equationTraverse (&MMO_Model_::_insertEquation);
}

MMO_SymbolRefTable
MMO_Model_::calledFunctions ()
{
    return (_calledFunctions);
}

MMO_FunctionTable
MMO_Model_::functions ()
{
    return (_functions);
}

MMO_ImportTable
MMO_Model_::imports ()
{
    return (_imports);
}

bool
MMO_Model_::hasExternalFunctions ()
{
    return (_calledFunctions->count () > 0);
}

void
MMO_Model_::_controlDiscreteVariables (AST_Expression expEq)
{
    AST_Expression_ComponentReference cr = expEq->getAsComponentReference ();
    string varName = _getComponentName (cr);
    VarInfo vi = _declarations->lookup (varName);
    if (vi == NULL)
    {
        Error::getInstance ()->add (expEq->lineNum (),
        EM_IR | EM_VARIABLE_NOT_FOUND,
                                    ER_Error, "%s", varName.c_str ());
    }
    else if (vi->isDiscrete ())
    {
        Error::getInstance ()->add (expEq->lineNum (),
        EM_IR | EM_WRONG_VARIABLE_TYPE,
                                    ER_Error, "Discrete variable %s assigned in equation section", varName.c_str ());
    }
}

void
MMO_Model_::_controlEquation (AST_Equation eq)
{
    if (eq->equationType () == EQEQUALITY)
    {
        AST_Equation_Equality eqe = eq->getAsEquality ();
        if (eqe->left ()->expressionType () == EXPDERIVATIVE)
        {
            AST_Expression_Derivative ed = eqe->left ()->getAsDerivative ();
            AST_Expression expEq = AST_ListFirst (ed->arguments ());
            _controlDiscreteVariables (expEq);
        }
        else if (eqe->left ()->expressionType () == EXPCOMPREF)
        {
            AST_Expression expEq = eqe->left ();
            _controlDiscreteVariables (expEq);
        }
        else if (eqe->left ()->expressionType () == EXPOUTPUT)
        {
            AST_Expression_Output eout = eqe->left ()->getAsOutput ();
            AST_ExpressionList el = eout->expressionList ();
            AST_ExpressionListIterator it;
            list<Index> lidx;
            foreach(it,el)
            {
                _controlDiscreteVariables (current_element(it));
            }
        }
        else
        {
            Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
        }
    }
    else
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
    }
}

void
MMO_Model_::_setIndex (AST_Expression derArg, VarInfo vi, Index *idx)
{
    if (_isArray (derArg))
    {
        idx->setLow (1);
        idx->setHi (vi->size (0));
        idx->setArray (0);
        for (int i = 1; i < vi->dimensions(); i++)
        {
            idx->setLow (1,i);
            idx->setHi (vi->size (i),i);
            idx->setArray (i);
        }
    }
}

/*! \brief Set the type of the Real variables defined in the model.
 *
 * 	\param eq: AST_Equation to be evaluated.
 * 	\param begin: Begining of the equation interval.
 * 	\param end: End of the equation interval.
 *
 * 	\note{As a side effect, the equation is stored in a local AST_Equation list
 * 	in order to be able to insert the model equations in the proper table
 * 	once the type of all variables defined is set.}
 *
 */

void
MMO_Model_::_setRealVariables (AST_Equation eq, int begin, int end)
{
    if (end < begin)
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong equation range.");
    }
    AST_Equation_Equality eqe = eq->getAsEquality ();
    if (eqe->left ()->expressionType () == EXPDERIVATIVE)
    {
        AST_Expression_Derivative ed = eqe->left ()->getAsDerivative ();
        AST_Expression derArg = AST_ListFirst (ed->arguments ());
        string varName = _getComponentName (derArg);
        VarInfo vi = _declarations->lookup (varName);
        if (vi == NULL)
        {
            Error::getInstance ()->add (eqe->left ()->lineNum (),
            EM_IR | EM_VARIABLE_NOT_FOUND,
                                        ER_Error, "%s", varName.c_str ());
            return;
        }
        if (!vi->hasIndex ())
        {
            Index idx;
            idx.setOffset (_states);
            _setIndex (derArg, vi, &idx);
            vi->setIndex (idx);
            vi->setState ();
            _states += vi->size ();
        }
    }
    else if (eqe->left ()->expressionType () == EXPCOMPREF)
    {
        string varName = _getComponentName (eqe->left ());
        VarInfo vi = _declarations->lookup (varName);
        if (vi == NULL)
        {
            Error::getInstance ()->add (eqe->left ()->lineNum (),
            EM_IR | EM_VARIABLE_NOT_FOUND,
                                        ER_Error, "%s", varName.c_str ());
            return;
        }
        if (!vi->hasIndex ())
        {
            Index idx;
            idx.setOffset (_algs);
            _setIndex (eqe->left (), vi, &idx);
            vi->setIndex (idx);
            vi->setAlgebraic ();
            _algs += vi->size ();
        }
    }
    else if (eqe->left ()->expressionType () == EXPOUTPUT)
    {
        AST_Expression_Output eout = eqe->left ()->getAsOutput ();
        AST_ExpressionList el = eout->expressionList ();
        AST_ExpressionListIterator it;
        list<string> lvars;
        list<Index> lidx;
        foreach(it,el)
        {
            string varName = _getComponentName (current_element(it));
            VarInfo vi = _declarations->lookup (varName);
            if (vi == NULL)
            {
                Error::getInstance ()->add (eqe->left ()->lineNum (),
                EM_IR | EM_VARIABLE_NOT_FOUND,
                                            ER_Error, "%s", varName.c_str ());
                return;
            }
            if (!vi->hasIndex ())
            {
                Index idx;
                idx.setOffset (_algs);
                _setIndex (current_element(it), vi, &idx);
                vi->setIndex (idx);
            }
            vi->setAlgebraic ();
            _algs += vi->size ();
        }
    }
    else
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Insert model equation.");
    }
}

AST_Equation
MMO_Model_::_transformEquation (AST_Equation eq)
{
    if (eq->equationType () == EQEQUALITY)
    {
        MMO_ReplaceInnerProduct rip (_declarations);
        AST_Expression l = eq->getAsEquality ()->left ();
        AST_Expression r = rip.foldTraverse (eq->getAsEquality ()->right ());
        string transform = _transformExpression (l, r);
        if (transform.empty ())
        {
            AST_Equation ret = newAST_Equation_Equality (l, r);
            return (ret);
        }
        int rValue;
        AST_Equation retEq = parseEquation (transform, &rValue);
        return (retEq);
    }
    else if (eq->equationType () == EQFOR)
    {
        AST_Equation_For eqf = eq->getAsFor ();
        AST_ForIndexList fil = eqf->forIndexList ();
        AST_EquationList eqs = eqf->equationList ();
        AST_EquationListIterator it;
        AST_EquationList tel = newAST_EquationList ();
        foreach(it,eqs)
        {
            AST_ListAppend (tel, _transformEquation (current_element(it)));
        }
        AST_Equation eqFor = newAST_Equation_For (fil, tel);
        return (eqFor);
    }
    return (eq);
}

/*! \brief Set the variable information for the equation.
 *
 * 	\param eq: AST_Equation with the state variable or algebraic variable definition.
 * 	
 * 	Description:
 * 		Before we can generate the MMO_Equation and insert the equation into the
 * 		algebraic or derivative tables, we need to know the type of the variable
 * 		defined in the left hand side in order to be able to calculate the 
 * 		dependencies. This is done in two separate steps, in this procedure, the
 * 		type of the variable and the index are set. Then, when all the equations of
 * 		the model are inserted, \f$ setEquations() \f$ has to be called in order to
 * 		generate and insert the equations with their corresponding dependencies.
 *
 * 		\note{The indexes of the array variables are not used when the expression
 * 		is printed but they are set anyway, the index expression of array variables
 * 		in the VarSymbolTable shouldn't be used. The index expression of the variable
 * 		is only important for simple variables.}
 *
 */

void
MMO_Model_::insert (AST_Equation eq)
{
    AST_Equation teq = _transformEquation (eq);
    _eqs.push_back (teq);
    if (teq->equationType () == EQEQUALITY)
    {
        _setRealVariables (teq, 0, 0);
    }
    else if (teq->equationType () == EQFOR)
    {
        int range[2], i = 0;
        AST_Equation_For eqf = teq->getAsFor ();
        AST_ForIndex fi = AST_ListFirst (eqf->forIndexList ());
        AST_Expression in = fi->in_exp ();
        AST_ExpressionList el = in->getAsRange ()->expressionList ();
        AST_ExpressionListIterator eli;
        MMO_EvalInitExp_ eie (_declarations);
        foreach(eli,el)
        {
            range[i++] = eie.foldTraverse (current_element(eli));
        }
        AST_EquationList eqs = eqf->equationList ();
        AST_EquationListIterator it;
        foreach(it,eqs)
        {
            if (current_element(it)->equationType () == EQFOR)
            {
                insert (current_element(it));
            }
            else
            {
                _setRealVariables (current_element(it), range[0], range[1]);
            }
        }
    }
    else
    {
        Error::getInstance ()->add (eq->lineNum (), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Equation type not recognized.");
    }
}

/*! \brief Given a when statement, generate the appropriate model event.
 *	
 *	\param stm: When statement to be evaluated.
 *	\param begin: Beginning of the event range.
 *	\param end: End of the event range.
 *
 * 	Description:
 * 		
 */

void
MMO_Model_::_insertEvent (AST_Statement stm, int begin, int end)
{
    if (stm->statementType () == STWHEN)
    {
        int factor = 0;
        if (begin != end)
        {
            factor = 1;
        }
        Index idx (0, factor, begin, end);
        idx.setOffset (_evs);
        _setRange (begin, end, &idx);
        _evs += end - begin + 1;
        AST_Statement_When sw = stm->getAsWhen ();
        _data->setLHS (idx);
        if (sw->hasComment ())
        {
            _annotations->eventComment (sw->comment ());
        }
        _data->setWeight (_annotations->weight ());
        _data->setDisableSymDiff (true);
        MMO_Event ev = newMMO_Event (sw->condition (), _data);
        _data->setDisableSymDiff (false);
        AST_StatementList stl = sw->statements ();
        AST_StatementListIterator it;
        foreach(it,stl)
        {
            ev->insert (current_element(it));
        }
        if (sw->hasElsewhen ())
        {
            int newEvent = false;
            AST_Statement_ElseList ewl = sw->else_when ();
            AST_Statement_ElseListIterator ewit;
            foreach(ewit,ewl)
            {
                AST_Statement_Else se = current_element(ewit);
                MMO_Event eve;
                Index idxe (0, factor, begin, end);
                _setRange (begin, end, &idxe);
                idxe.setOffset (_evs);
                if (ev->compareCondition (se->condition ()))
                {
                    eve = ev;
                }
                else
                {
                    _data->setLHS (idxe);
                    eve = newMMO_Event (se->condition (), _data);
                    _evs += end - begin + 1;
                    newEvent = true;
                    eve->setIndex (idxe);
                }
                AST_StatementList stel = se->statements ();
                AST_StatementListIterator steit;
                foreach(steit,stel)
                {
                    eve->insert (current_element(steit));
                }
                if (newEvent)
                {
                    _events->insert (idxe, eve);
                }
            }
        }
        _events->insert (idx, ev);
    }
}

void
MMO_Model_::insert (AST_Statement stm)
{
    insert (stm, false);
}

void
MMO_Model_::insert (AST_External_Function_Call efc)
{
    return;
}

void
MMO_Model_::insert (AST_Statement stm, bool initial)
{
    AST_Statement st = _transformStatement (stm);
    if (initial)
    {
        _data->setInitialCode (true);
        _initialCode->insert (newMMO_Statement (st, _data));
        _data->setInitialCode (false);
    }
    else
    {
        _stms.push_back (st);
    }
}

void
MMO_Model_::_getFunctionInfo (MMO_Function f)
{
    MMO_Annotation fa = f->annotation ();
    if (fa->hasIncludeDirectory ())
    {
        string in = fa->includeDirectory ();
        _includeDirectories.insert (pair<string, string> (in, in));
    }
    if (fa->hasLibraryDirectory ())
    {
        string in = fa->libraryDirectory ();
        _libraryDirectories.insert (pair<string, string> (in, in));
    }
    if (fa->hasLibraries ())
    {
        list<string> in = fa->libraries ();
        _insertFunctionDependencies (in, &_linkLibraries);
    }
    MMO_ImportTable imt = f->imports ();
    for (string name = imt->begin (); !imt->end (); name = imt->next ())
    {
        MMO_PackageData pd = Util::getInstance ()->readPackage (name);
        if (pd == NULL)
        {
            Error::getInstance ()->add (0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", name.c_str ());
            return;
        }
        list<string> tmp = pd->includeDirectories ();
        _insertFunctionDependencies (tmp, &_includeDirectories);
        tmp = pd->libraryDirectories ();
        _insertFunctionDependencies (tmp, &_libraryDirectories);
        tmp = pd->linkLibraries ();
        _insertFunctionDependencies (tmp, &_linkLibraries);
    }
}

void
MMO_Model_::insert (MMO_Function f)
{
    Index i;
    i.setOffset (_funcs++);
    _functions->insert (i, f);
    _getFunctionInfo (f);
}

void
MMO_Model_::insert (AST_Argument_Modification x)
{
    if (!_annotations->insert (x))
    {
        Error::getInstance ()->add (x->lineNum (),
        EM_IR | EM_ANNOTATION_NOT_FOUND,
                                    ER_Error, "%s", x->name ()->c_str ());
    }
}

VarSymbolTable
MMO_Model_::varTable ()
{
    return (_declarations);
}

void
MMO_Model_::insert (string n)
{
    _imports->insert (n);
    if (!Util::getInstance ()->readPackage (n, _packages))
    {
        Error::getInstance ()->add (0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str ());
        return;
    }
    MMO_PackageData pd = _packages->last ();
    list<string> tmp = pd->includeDirectories ();
    _insertFunctionDependencies (tmp, &_includeDirectories);
    tmp = pd->libraryDirectories ();
    _insertFunctionDependencies (tmp, &_libraryDirectories);
    tmp = pd->linkLibraries ();
    _insertFunctionDependencies (tmp, &_linkLibraries);
}

MMO_Annotation
MMO_Model_::annotation ()
{
    return (_annotations);
}

string
MMO_Model_::name () const
{
    return (_name);
}

MMO_EquationTable
MMO_Model_::algebraics ()
{
    return (_algebraics);
}

MMO_EquationTable
MMO_Model_::derivatives ()
{
    return (_derivatives);
}

MMO_EventTable
MMO_Model_::events ()
{
    return (_events);
}

unsigned int
MMO_Model_::states ()
{
    return (_states);
}

unsigned int
MMO_Model_::discretes ()
{
    return (_discretes);
}

unsigned int
MMO_Model_::algs ()
{
    return (_algs);
}

unsigned int
MMO_Model_::evs ()
{
    return (_evs);
}

unsigned int
MMO_Model_::funcs ()
{
    return (_funcs);
}

unsigned int
MMO_Model_::inputs ()
{
    return (_inputs);
}

int
MMO_Model_::imps ()
{
    return (_imports->count ());
}

void
MMO_Model_::initOutput ()
{
    list<AST_Expression> outputs = _annotations->output ();
    for (list<AST_Expression>::iterator it = outputs.begin (); it != outputs.end (); it++)
    {
        MMO_ReplaceInterval_ oe (_declarations);
        AST_Expression exp = oe.foldTraverse (current_element(it));
        VariableInterval vi = oe.first ();
        Index idx (vi.index ());
        idx.setOffset (_output);
        if (idx.hasRange ())
        {
            idx.setFactor (1);
            idx.setConstant (0);
            idx.setArray ();
        }
        if (!vi.isEmpty ())
        {
            VarInfo v = _declarations->lookup (vi.name ());
            if (v != NULL)
            {
                if (v->isAlgebraic ())
                {
                    idx.setRangeOp (true);
                }
            }
        }
        _data->setLHS (idx);
        _data->setGenerateDerivatives (false);
        _data->setCalculateAlgegraics (true);
        MMO_Equation mse = newMMO_Equation (exp, _data);
        _data->setCalculateAlgegraics (false);
        _data->clear ();
        _outputs->insert (idx, mse);
        _outputIndexes[idx] = oe.indexes ();
        _output += idx.range ();
    }
}

unsigned int
MMO_Model_::outs ()
{
    return (_output);
}

MMO_EquationTable
MMO_Model_::outputs ()
{
    return (_outputs);
}

MMO_StatementTable
MMO_Model_::initialCode ()
{
    return (_initialCode);
}

MMO_Model
newMMO_Model (string name)
{
    return (new MMO_Model_ (name));
}

void
deleteMMO_Model (MMO_Model m)
{
    delete m;
}

/* MMO_Function class */

MMO_Function_::MMO_Function_ (string name) :
        _name (name), _externalFuncs (0), _outputs (0), _outputName (), _externalFunctions (
        NULL), _prefix ("__"), _arguments ()
{
    _types = newTypeSymbolTable ();
    _annotations = newMMO_FunctionAnnotation ();
    _externalFunctionCalls = newMMO_ArgumentsTable ();
    _calledFunctions = newMMO_SymbolRefTable ();
    _declarations = newVarSymbolTable ();
    _declarations->initialize (_types);
    _localDeclarations = newVarSymbolTable ();
    _statements = newMMO_StatementTable ();
    _packages = newMMO_PackageTable ();
    _imports = newMMO_ImportTable ();
    _data = newMMO_ModelData ();
    _functions = newMMO_FunctionTable ();
    _data->setAnnotation (_annotations);
    _data->setInitialCode (true);
    _data->setPackages (_packages);
    _data->setCalledFunctions (_calledFunctions);
    _data->setFunctions (_functions);
    _data->setSymbols (_declarations);
}

MMO_Function_::~MMO_Function_ ()
{
}

CL_Type
MMO_Function_::classType ()
{
    return (CL_FUNCTION);
}

MMO_ImportTable
MMO_Function_::imports ()
{
    return (_imports);
}

VarSymbolTable
MMO_Function_::varTable ()
{
    return (_declarations);
}

void
MMO_Function_::setImports (MMO_ImportTable it)
{
    for (string i = it->begin (); !it->end (); i = it->next ())
    {
        insert (i);
    }
}

MMO_Annotation
MMO_Function_::annotation ()
{
    return (_annotations);
}

void
MMO_Function_::insert (string n)
{
    _imports->insert (n);
    if (!Util::getInstance ()->readPackage (n, _packages))
    {
        Error::getInstance ()->add (0, EM_IR | EM_CANT_OPEN_FILE, ER_Error, "%s.moo", n.c_str ());
    }
}

void
MMO_Function_::insert (AST_Equation eq)
{
    return;
}

void
MMO_Function_::insert (AST_Statement stm)
{
    MMO_Statement s = newMMO_Statement (stm, _data);
    _statements->insert (s);
}

void
MMO_Function_::insert (AST_Statement stm, bool initial)
{
    insert (stm);
}

void
MMO_Function_::insert (MMO_Function f)
{
    return;
}

MMO_StatementTable
MMO_Function_::statements ()
{
    return (_statements);
}

void
MMO_Function_::insert (AST_External_Function_Call efc)
{
    string crName;
    if (efc->hasComponentReference ())
    {
        AST_Expression_ComponentReference cr = efc->componentReference ();
        VarInfo vi = _declarations->lookup (cr->name ());
        if (vi == NULL)
        {
            vi = _localDeclarations->lookup (cr->name ());
            if (vi == NULL)
            {
                Error::getInstance ()->add (efc->lineNum (),
                EM_IR | EM_VARIABLE_NOT_FOUND,
                                            ER_Error, "%s", cr->name ().c_str ());
                return;
            }
        }
        crName = cr->name ();
    }
    AST_ExpressionListIterator eli;
    ControlVars cv = newControlVars (_declarations, _localDeclarations);
    if (efc->args () != NULL)
    {
        foreach(eli,efc->args())
        {
            if (!cv->foldTraverse (current_element(eli)))
            {
                Error::getInstance ()->add (efc->lineNum (),
                EM_IR | EM_VARIABLE_NOT_FOUND,
                                            ER_Error, "External function call.");
                return;
            }
        }
    }
    deleteControlVars (cv);
    Index i (_externalFuncs++, 0);
    MMO_FunctionData fd = newMMO_FunctionData (crName, efc->name (), efc->args (), _data);
    _externalFunctionCalls->insert (i, fd);
}

MMO_ArgumentsTable
MMO_Function_::externalFunctionCalls ()
{
    return (_externalFunctionCalls);
}

void
MMO_Function_::insert (VarName n, VarInfo vi, DEC_Type type)
{
    Index idx;
    idx.setHi (vi->size ());
    MMO_EvalInitExp_ e (_declarations);
    vi->setName (n);
    vi->setIndex (idx);
    if (vi->typePrefix () & TP_CONSTANT)
    {
        vi->setValue (e.foldTraverse (vi->modification ()->getAsEqual ()->exp ()));
    }
    _declarations->insert (n, vi);
    if (type == DEC_PUBLIC)
    {
        if (vi->isOutput ())
        {
            _outputs++;
        }
        _arguments.push_back (vi);
    }
    else
    {
        _localDeclarations->insert (n, vi);
    }
}

void
MMO_Function_::insert (VarName n, VarInfo vi)
{
    insert (n, vi, DEC_PUBLIC);
}

void
MMO_Function_::insert (AST_Argument_Modification x)
{
    if (!_annotations->insert (x))
    {
        Error::getInstance ()->add (x->lineNum (),
        EM_IR | EM_ANNOTATION_NOT_FOUND,
                                    ER_Error, "%s", x->name ()->c_str ());
    }
}

string
MMO_Function_::name () const
{
    return (_name);
}

list<string>
MMO_Function_::localDeclarations ()
{
    list<string> ret;
    stringstream buffer;
    _localDeclarations->setPrintEnvironment (VST_FUNCTION);
    for (VarInfo vi = _localDeclarations->begin (); !_localDeclarations->end (); vi = _localDeclarations->next ())
    {
        if (vi->isConstant ())
        {
            continue;
        }
        Index idx = vi->index ();
        buffer << "double " << vi->name ();
        if (vi->isArray ())
        {
            buffer << "[" << vi->size () << "]";
        }
        buffer << ";";
        ret.push_back (buffer.str ());
        buffer.str ("");
        string indexVar;
        if (vi->hasEachModifier ())
        {
            indexVar = Util::getInstance ()->newVarName ("i", _declarations);
            ret.push_back ("int " + indexVar + ";");
        }
        if (vi->hasAssignment () || vi->hasStartModifier () || vi->hasEachModifier ())
        {
            _data->setSymbols (_localDeclarations);
            Util::getInstance ()->setData (_data);
            buffer << Util::getInstance ()->printInitialAssignment (vi, "", indexVar);
            ret.push_back (buffer.str ());
            buffer.str ("");
            _data->setSymbols (_declarations);
        }
    }
    if (_outputs == 1)
    {
        _declarations->setPrintEnvironment (VST_FUNCTION);
        for (VarInfo vi = _declarations->begin (); !_declarations->end (); vi = _declarations->next ())
        {
            if (!vi->isOutput ())
            {
                continue;
            }
            Index idx = vi->index ();
            buffer << "double " << vi->name ();
            if (vi->isArray ())
            {
                buffer << "[" << vi->size () << "]";
            }
            buffer << ";";
            ret.push_back (buffer.str ());
            buffer.str ("");
            if (vi->hasAssignment () || vi->hasStartModifier () || vi->hasEachModifier ())
            {
                _data->setSymbols (_localDeclarations);
                Util::getInstance ()->setData (_data);
                buffer << Util::getInstance ()->printInitialAssignment (vi, "");
                ret.push_back (buffer.str ());
                buffer.str ("");
                _data->setSymbols (_declarations);
            }
        }
    }
    return (ret);
}

void
MMO_Function_::setPrefix (string prefix)
{
    _prefix = prefix;
}

string
MMO_Function_::prototype ()
{
    stringstream input;
    stringstream output;
    stringstream func;
    list<VarInfo>::iterator it;
    for (it = _arguments.begin (); it != _arguments.end (); it++)
    {
        VarInfo vi = *it;
        if (vi->isInput ())
        {
            input << "double ";
            if (vi->isArray ())
            {
                input << "*";
            }
            input << vi->name () << ",";
        }
        else if (vi->isOutput ())
        {
            output << "double *" << vi->name () << ",";
            _outputName = vi->name ();
        }
    }
    if (_outputs == 0)
    {
        string in = input.str ();
        if (!in.empty ())
        {
            in.erase (in.end () - 1, in.end ());
        }
        func << "void " << _prefix << _name << "(" << in << ")";
    }
    else if (_outputs == 1)
    {
        string in = input.str ();
        if (!in.empty ())
        {
            in.erase (in.end () - 1, in.end ());
        }
        func << "double " << _prefix << _name << "(" << in << ")";
    }
    else
    {
        string out = output.str ();
        out.erase (out.end () - 1, out.end ());
        func << "void " << _prefix << _name << "(" << input.str () << out << ")";
    }
    return (func.str ());
}

string
MMO_Function_::returnStatement ()
{
    if (_outputs == 1)
    {
        return ("return " + _outputName + ";");
    }
    return ("");
}

MMO_Function
newMMO_Function (string name)
{
    return (new MMO_Function_ (name));
}

void
deleteMMO_Function (MMO_Function m)
{
    delete m;
}

/* MMO_Package class. */

MMO_Package_::MMO_Package_ (string name) :
        _name (name), _funcs (0), _packages ()
{
    _imports = newMMO_ImportTable ();
    _functions = newMMO_FunctionTable ();
}

MMO_Package_::~MMO_Package_ ()
{
}

CL_Type
MMO_Package_::classType ()
{
    return (CL_PACKAGE);
}

MMO_ImportTable
MMO_Package_::imports ()
{
    return (_imports);
}

VarSymbolTable
MMO_Package_::varTable ()
{
    return (NULL);
}

void
MMO_Package_::insert (string n)
{
    _imports->insert (n);
}

void
MMO_Package_::insert (AST_Equation eq)
{
    return;
}

void
MMO_Package_::insert (AST_Statement stm)
{
    return;
}

void
MMO_Package_::insert (AST_Statement stm, bool initial)
{
    return;
}

void
MMO_Package_::insert (MMO_Function f)
{
    Index i;
    i.setOffset (_funcs++);
    _functions->insert (i, f);
    return;
}

MMO_FunctionTable
MMO_Package_::functions ()
{
    return (_functions);
}

void
MMO_Package_::insert (AST_External_Function_Call efc)
{
    return;
}

void
MMO_Package_::insert (VarName n, VarInfo vi, DEC_Type type)
{
    return;
}

void
MMO_Package_::insert (VarName n, VarInfo vi)
{
    return;
}

void
MMO_Package_::insert (AST_Argument_Modification x)
{
}

string
MMO_Package_::fileName () const
{
    return (Util::getInstance ()->packageName (_name));
}

string
MMO_Package_::name () const
{
    return (_name);
}

string
MMO_Package_::prefix ()
{
    return ("__" + _name + "__");
}

MMO_Package
newMMO_Package (string name)
{
    return (new MMO_Package_ (name));
}

void
deleteMMO_Package (MMO_Package m)
{
    delete m;
}

bool
MMO_Model_::_isArray (AST_Expression exp)
{
    if (exp->expressionType () == EXPCOMPREF)
    {
        AST_Expression_ComponentReference ecr = exp->getAsComponentReference ();
        return (ecr->hasIndexes ());
    }
    return (false);
}

void
MMO_Model_::setEvents ()
{
    _setEvents ();
}

void
MMO_Function_::setFunctions (MMO_FunctionTable functions, MMO_FunctionTable externalFunctions, MMO_SymbolRefTable calledFunctions)
{
    _externalFunctions = externalFunctions;
    if (_functions != NULL)
    {
        deleteMMO_FunctionTable (_functions);
    }
    if (_calledFunctions != NULL)
    {
        deleteMMO_SymbolRefTable (_calledFunctions);
    }
    _calledFunctions = calledFunctions;
    _functions = functions;
    _data->setExternalFunctions (_externalFunctions);
    _data->setFunctions (_functions);
    _data->setCalledFunctions (_calledFunctions);
}
