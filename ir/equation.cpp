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

#include "equation.h"

#include <iterator>
#include <map>
#include <sstream>

#include "../ast/ast_builder.h"
#include "../ast/expression.h"
#include "../util/ast_util.h"
#include "../util/dependencies.h"
#include "../util/derivative.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/util.h"
#include "annotation.h"
#include "expression.h"
#include "mmo_util.h"

MMO_Equation_::MMO_Equation_ (AST_Expression exp, MMO_ModelData data) :
        _data (data), _end (data->end ()), _init (data->begin ()), _lhs (_data->lhs ()), _variables (), _arguments (NULL), _code (), _equation (), _algebraics ()
{
    _exp[0] = newMMO_Expression (exp, data);
    _coeffs = data->annotation ()->polyCoeffs ();
    if (_data->hasArguments ())
    {
        _arguments = _data->arguments ();
        AST_ExpressionList el = _arguments->getAsOutput ()->expressionList ();
        AST_ExpressionListReverseIterator it;
        foreachReverse(it,el)
        {
            MMO_Expression argExp = newMMO_Expression (current_element(it), _data);
            Index *algIndex = argExp->deps ()->begin (DEP_ALGEBRAIC_DEF);
            if (algIndex != NULL)
            {
                _algebraicArguments.insert (*algIndex);
            }
            deleteMMO_Expression (argExp);
        }
    }
    if (_data->annotation ()->symDiff ())
    {
        _initDerivatives ();
    }
    if (_data->annotation()->classic ())
    {
        _generateJacobianExps ();
    }
}

MMO_Equation_::MMO_Equation_ (MMO_Expression exp, MMO_ModelData data) :
        _data (data), _end (data->end ()), _init (data->begin ()), _lhs (data->lhs ()), _variables (), _arguments (NULL), _code (), _equation (), _algebraics ()
{
    _exp[0] = exp;
    _coeffs = data->annotation ()->order () + 1;
    if (_data->hasArguments ())
    {
        _arguments = _data->arguments ();
    }
    if (_data->annotation ()->symDiff ())
    {
        _initDerivatives ();
    }
}

MMO_Equation_::~MMO_Equation_ ()
{
}

void
MMO_Equation_::_initDerivatives ()
{
    ExpressionDerivator ed;
    ReplaceDer_ rd (_data->symbols ());
    AST_Expression exp1 = ed.derivate (_exp[0]->exp (), _data->symbols (), _exp[0]);
    _exp[1] = newMMO_Expression (exp1, _data);
    AST_Expression exp2 = ed.derivate (exp1, _data->symbols (), _exp[0]);
    _exp[2] = newMMO_Expression (rd.foldTraverse (exp2), _data);
    AST_Expression exp3 = ed.derivate (exp2, _data->symbols (), _exp[0]);
    _exp[3] = newMMO_Expression (rd.foldTraverse (exp3), _data);
}

void
MMO_Equation_::_generateJacobianExps ()
{
    ExpressionDerivator ed;
    _jacobianExps = ed.generateJacobianExps (_exp[0]->exp (), _data);
}

MMO_Equation
MMO_Equation_::jacobianExp (Index idx, DEP_Type type)
{
    MMO_Expression chExp = _generateChainRule (idx);
    map<string, MMO_Expression>::iterator f = _jacobianExps.find (_exp[0]->deps ()->identifier (idx, type));

    if (f == _jacobianExps.end () && chExp == NULL)
    {
        return (NULL);
    }
    else if (f != _jacobianExps.end () && chExp == NULL)
    {
        return (newMMO_Equation (f->second, _data));
    }
    else if (f == _jacobianExps.end () && chExp != NULL)
    {
        return (newMMO_Equation (chExp, _data));
    }
    else
    {
        AST_Expression derExpAST = newAST_Expression_BinOp (f->second->exp (), chExp->exp (), BINOPADD);
        MMO_Expression derExpMMO = newMMO_Expression (derExpAST, _data);
        return (newMMO_Equation (derExpMMO, _data));
    }
    return (NULL);
}

MMO_Expression
MMO_Equation_::exp ()
{
    return (_exp[0]);
}

bool
MMO_Equation_::hasRange ()
{
    return (_init != _end);
}

int
MMO_Equation_::init ()
{
    return (_init);
}

int
MMO_Equation_::end ()
{
    return (_end);
}

string
MMO_Equation_::print ()
{
    return ("");
}

string
MMO_Equation_::printRange (string variable, string idx, string indent, Index lhs, bool variableChange)
{
    stringstream buffer;
    if (variableChange)
    {
        buffer << indent << idx << " = " << lhs.definition (variable) << ";" << endl;
        buffer << indent << "if (" << variable << " >= " << lhs.begin () << " && " << variable << " <= " << lhs.end () << ")" << endl;
        buffer << indent << "{";
    }
    else if (lhs.factor () == 0)
    {
        int c = lhs.constant ();
        buffer << indent << idx << " = " << variable << ";" << endl;
        buffer << indent << "if (" << idx << " == " << c << ")" << endl;
        buffer << indent << "{";
    }
    else
    {
        buffer << indent << idx << " = " << lhs.printReverseDefinition (variable) << ";" << endl;
        buffer << indent << "if (" << idx << " >= " << lhs.begin () << " && " << idx << " <= " << lhs.end () << ")" << endl;
        buffer << indent << "{";
    }
    return (buffer.str ());
}

list<string>
MMO_Equation_::print (string indent, string lhs, string idx, bool palgs, MMO_EquationTable algs, EQ_Type type, int order, bool constant, int offset, bool dereq,
                      int forOffset, int constantOffset)
{
    stringstream buffer;
    list<string> ret;
    if (palgs)
    {
        set<Index> algebraicArguments;
        string aux1, aux2;
        Dependencies d = _exp[0]->deps ();
        list<string> code;
        string algPrefix = _data->symbols ()->getTypePrefix () + "alg";
        for (Index *dIdx = d->begin (DEP_ALGEBRAIC); !d->end (DEP_ALGEBRAIC); dIdx = d->next (DEP_ALGEBRAIC))
        {
            if (d->isVector (dIdx))
            {
                string vn;
                Index pIdx (*dIdx);
                list<MMO_Equation> eqs = algs->equation (*dIdx);
                list<MMO_Equation>::iterator eq;
                for (eq = eqs.begin (); eq != eqs.end (); eq++)
                {
                    if (controlAlgebraicArguments (&algebraicArguments, (*eq)->algebraicArguments ()))
                    {
                        Index lhsEq = (*eq)->lhs ();
                        stringstream aLhs;
                        string iter;
                        if (lhsEq.hasRange ())
                        {
                            iter = Util::getInstance ()->newVarName ("j", _data->symbols ());
                            _variables.push_back ("int " + iter + "= 0;");

                            buffer << indent << "for(" << iter << " = " << lhsEq.begin () << "; " << iter << " <= " << lhsEq.end () << "; " << iter << "++)";
                            ret.push_back (buffer.str ());
                            ret.push_back (indent + "{");
                            buffer.str ("");
                            aLhs << algPrefix << "[(" << pIdx.print (iter, offset, false) << ")";
                            aLhs << " * " << _coeffs;
                        }
                        else
                        {
                            int cte = dIdx->mappedValue ((*eq)->lhs ().constant ());
                            aLhs << algPrefix << "[" << cte * _coeffs;
                        }
                        list<string> a = (*eq)->print (indent + indent, aLhs.str (), Util::getInstance ()->getVarName (iter), false, algs, EQ_ALGEBRAIC, order, false, offset, false, forOffset,
                                                       -1);
                        _algebraics.insert (_algebraics.end (), a.begin (), a.end ());
                        ret.insert (ret.end (), a.begin (), a.end ());
                        if (lhsEq.hasRange ())
                        {
                            ret.push_back (indent + "}");
                        }
                        list<string> vars = (*eq)->getVariables ();
                        _variables.insert (_variables.end (), vars.begin (), vars.end ());
                    }
                }
            }
            else
            {
                string vn, vm;
                list<MMO_Equation> eqs = algs->equation (*dIdx);
                list<MMO_Equation>::iterator eq;
                for (eq = eqs.begin (); eq != eqs.end (); eq++)
                {
                    if (controlAlgebraicArguments (&algebraicArguments, (*eq)->algebraicArguments ()))
                    {
                        bool range = ((*eq)->lhs ().hasRange () && (*eq)->lhs ().range () != _lhs.range ()) && (dIdx->factor () != 0);
                        if (range)
                        {
                            if (aux1.empty ())
                            {
                                aux1 = Util::getInstance ()->newVarName ("j", _data->symbols ());
                                _variables.push_back ("int " + aux1 + " = 0;");
                            }
                            vn = aux1;
                        }
                        else
                        {
                            vn = idx + "0";
                        }
                        bool variableMod = false;
                        stringstream aLhs;
                        int cte = constantOffset;
                        bool constantValue = false;
                        string strIdx = idx;
                        if ((*eq)->lhs ().factor () == 0)
                        {
                            cte = (*eq)->lhs ().mappedConstant ();
                            aLhs << algPrefix << "[" << cte * _coeffs;
                            constantValue = true;
                        }
                        else if (dIdx->factor () == 0)
                        {
                            if (dIdx->isArray ())
                            {
                                cte = dIdx->mappedValue ((*eq)->lhs ().constant ());
                                aLhs << algPrefix << "[" << cte * _coeffs;
                                cte = dIdx->constant ();
                                constantValue = true;
                            }
                            else
                            {
                                aLhs << algPrefix << "[(" << dIdx->print (idx) << ")";
                            }
                        }
                        else
                        {
                            if (dIdx->factor () != 1 || dIdx->operConstant () != 0)
                            {
                                if (aux2.empty ())
                                {
                                    aux2 = Util::getInstance ()->newVarName ("j", _data->symbols ());
                                    _variables.push_back ("int " + aux2 + " = 0;");
                                }
                                vm = aux2;
                                variableMod = true;
                                aLhs << algPrefix << "[(" << (*eq)->lhs ().print (dIdx->definition (vn), -(*eq)->lhs ().operConstant () + offset, false) << ")";
                                strIdx = Util::getInstance ()->getVarName (vm);
                            }
                            else
                            {
                                int off = offset;
                                if (range)
                                {
                                    off -= (*eq)->lhs ().operConstant ();
                                }
                                aLhs << algPrefix << "[(" << (*eq)->lhs ().print (idx, off) << ")";
                                strIdx = Util::getInstance ()->getVarName (vn);
                            }
                        }
                        if (!constantValue)
                        {
                            aLhs << " * " << _coeffs;
                        }
                        if (range)
                        {
                            ret.push_back (printRange (idx + "0", vn, indent, (*eq)->lhs ()));
                        }
                        if (variableMod)
                        {
                            ret.push_back (printRange (vn, vm, indent, *dIdx, true));
                        }
                        list<string> a = (*eq)->print (indent, aLhs.str (), strIdx, false, algs, EQ_ALGEBRAIC, order, constantValue, offset, false, forOffset,
                                                       cte);
                        list<string> vars = (*eq)->getVariables ();
                        _variables.insert (_variables.end (), vars.begin (), vars.end ());
                        _algebraics.insert (_algebraics.end (), a.begin (), a.end ());
                        ret.insert (ret.end (), a.begin (), a.end ());
                        if (variableMod)
                        {
                            ret.push_back (indent + "}");
                        }
                        if (range)
                        {
                            ret.push_back (indent + "}");
                        }
                    }
                }
            }
        }
        ret.insert (ret.end (), code.begin (), code.end ());
    }
    int coeff = 1;
    int derCoeff = 0;
    if (type == EQ_DERIVATIVE || type == EQ_DEPENDENCIES)
    {
        derCoeff = 1;
    }
    for (int i = 0; i < order; i++)
    {
        int cte = -1;
        if (constant)
        {
            cte = constantOffset;
        }
        coeff *= i + derCoeff;
        if (coeff == 0)
        {
            coeff = 1;
        }
        stringstream lhsOrder;
        if (type == EQ_HANDLER_IF)
        {
            buffer << lhs << "(" << _exp[i]->print (idx, offset, 0, cte, order, forOffset) << ")";
        }
        else
        {
            if (_arguments != NULL && i == 0)
            {
                buffer << _printArguments (i, idx, offset, cte, order, forOffset);
            }
            else
            {
                if (type != EQ_CLASSIC && type != EQ_HANDLER)
                {
                    if (type == EQ_DERIVATIVE)
                    {
                        lhsOrder << "[" << i + 1 << "]";
                    }
                    else if (type == EQ_ALGEBRAIC)
                    {
                        if (i > 0)
                        {
                            lhsOrder << " + " << i;
                        }
                        lhsOrder << "]";
                    }
                    else if (type == EQ_DEPENDENCIES)
                    {
                        lhsOrder << " + " << i + 1 << "]";
                    }
                    else if (type == EQ_ZC || type == EQ_OUTPUT)
                    {
                        lhsOrder << "[" << i << "]";
                    }
                }
                if (type == EQ_JACOBIAN)
                {
                    lhsOrder.str ("");
                }
                if (coeff > 1 && dereq)
                {
                    buffer << indent << lhs << lhsOrder.str () << " = (" << _exp[i]->print (idx, offset, 0, cte, order, forOffset) << ")/" << coeff << ";";
                }
                else
                {
                    buffer << indent << lhs << lhsOrder.str () << " = " << _exp[i]->print (idx, offset, 0, cte, order, forOffset) << ";";
                }
            }
        }
        _code = _exp[i]->getCode ();
        list<string> vars = _exp[i]->getVariables ();
        _variables.insert (_variables.end (), vars.begin (), vars.end ());
        ret.insert (ret.end (), _code.begin (), _code.end ());
        _equation.push_back (buffer.str ());
        ret.push_back (buffer.str ());
        buffer.str ("");
    }
    return (ret);
}

list<string>
MMO_Equation_::getVariables ()
{
    return (_variables);
}

Index
MMO_Equation_::lhs ()
{
    return (_lhs);
}

MMO_Equation
newMMO_Equation (AST_Expression exp, MMO_ModelData data)
{
    return (new MMO_Equation_ (exp, data));
}

MMO_Equation
newMMO_Equation (MMO_Expression exp, MMO_ModelData data)
{
    return (new MMO_Equation_ (exp, data));
}

void
deleteMMO_Equation (MMO_Equation m)
{
    delete m;
}

void
MMO_Equation_::controlAlgebraicDefinition ()
{
    Dependencies deps = _exp[0]->deps ();
    list<MMO_Equation> eqs;
    for (Index *idx = deps->begin (DEP_ALGEBRAIC_DEF); !deps->end (DEP_ALGEBRAIC_DEF); idx = deps->next (DEP_ALGEBRAIC_DEF))
    {
        eqs = _data->algebraics ()->equation (*idx);
        if (eqs.empty ())
        {
            Error::getInstance ()->add (_exp[0]->exp ()->lineNum (), EM_IR | EM_ALG_INDEX, ER_Error, "%s and %s.", _lhs.print ("i").c_str (),
                                        idx->print ("i").c_str ());
        }
    }
    for (Index *idx = deps->begin (DEP_ALGEBRAIC_VECTOR_DEF); !deps->end (DEP_ALGEBRAIC_DEF); idx = deps->next (DEP_ALGEBRAIC_DEF))
    {
        if (_lhs < *idx)
        {
            Error::getInstance ()->add (_exp[0]->exp ()->lineNum (), EM_IR | EM_ALG_INDEX, ER_Error, "%s and %s.", _lhs.print ("i").c_str (),
                                        idx->print ("i").c_str ());
        }
    }
}

bool
MMO_Equation_::controlAlgebraicArguments (set<Index> *algs, set<Index> eqAlgs)
{
    if (eqAlgs.empty ())
    {
        return (true);
    }
    set<Index>::iterator it;
    for (it = eqAlgs.begin (); it != eqAlgs.end (); it++)
    {
        if (algs->find (*it) != algs->end ())
        {
            return (false);
        }
        algs->insert (*it);
    }
    return (true);
}

string
MMO_Equation_::_printArguments (int i, string idx, int offset, int cte, int order, int forOffset)
{
    stringstream buffer;
    stringstream arguments;
    AST_Expression_Output eout = _arguments->getAsOutput ();
    AST_ExpressionList el = eout->expressionList ();
    AST_ExpressionListReverseIterator it;
    int args = el->size ();
    int argCount = 0;
    foreachReverse(it,el)
    {
        if (argCount++ < args)
        {
            arguments << ",";
        }
        MMO_Expression argExp = newMMO_Expression (current_element(it), _data);
        arguments << "&(" << argExp->print (idx, offset, 0, cte, order, forOffset) << ")";
        deleteMMO_Expression (argExp);
    }
    arguments << ")";
    string exp = _exp[i]->print (idx, offset, 0, cte, order, forOffset);
    exp.erase (exp.size () - 1);
    buffer << exp << arguments.str () << ";";
    return (buffer.str ());
}

list<string>
MMO_Equation_::getCode ()
{
    return (_code);
}

list<string>
MMO_Equation_::getEquation ()
{
    return (_equation);
}

list<string>
MMO_Equation_::getAlgebraics ()
{
    return (_algebraics);
}

set<Index>
MMO_Equation_::algebraicArguments ()
{
    return (_algebraicArguments);
}

MMO_Expression
MMO_Equation_::_generateChainRule (Index idx)
{
    Dependencies deps = _exp[0]->deps ();
    MMO_Expression exp = NULL;
    for (Index *dIdx = deps->begin (DEP_ALGEBRAIC_DEF); !deps->end (DEP_ALGEBRAIC_DEF); dIdx = deps->next (DEP_ALGEBRAIC_DEF))
    {
        map<string, MMO_Expression>::iterator af = _jacobianExps.find (_exp[0]->deps ()->identifier (*dIdx, DEP_ALGEBRAIC_DEF));
        if (af == _jacobianExps.end ())
        {
            return (NULL);
        }
        list<MMO_Equation> eqs = _data->algebraics ()->equation (*dIdx);
        list<MMO_Equation>::iterator eq;
        MMO_Expression algExp = NULL;
        for (eq = eqs.begin (); eq != eqs.end (); eq++)
        {
            map<string, MMO_Expression>::iterator f = (*eq)->_jacobianExps.find ((*eq)->_exp[0]->deps ()->identifier (idx, DEP_STATE));
            MMO_Expression derExp = (f == (*eq)->_jacobianExps.end ()) ? NULL : f->second;
            if (derExp != NULL)
            {
                if (algExp == NULL)
                {
                    algExp = derExp;
                }
                else
                {
                    AST_Expression derAST = newAST_Expression_BinOp (algExp->exp (), derExp->exp (), BINOPADD);
                    algExp = newMMO_Expression (derAST, _data);
                }
            }
            MMO_Expression rExp = (*eq)->_generateChainRule (idx);
            if (rExp != NULL)
            {
                if (algExp == NULL)
                {
                    algExp = rExp;
                }
                else
                {
                    AST_Expression derAST = newAST_Expression_BinOp (algExp->exp (), rExp->exp (), BINOPADD);
                    algExp = newMMO_Expression (derAST, _data);
                }
            }
        }
        if (algExp != NULL)
        {
            AST_Expression chain = newAST_Expression_BinOp (af->second->exp (), algExp->exp (), BINOPADD);
            exp = newMMO_Expression (chain, _data);
        }
    }
    return (exp);
}
