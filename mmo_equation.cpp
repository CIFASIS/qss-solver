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

#include "mmo_equation.h"

#include "mmo_utils.h"

MMOEquation::MMOEquation () :
        _equation (NULL), _hasEquation (false)
{
}

MMOEquation::MMOEquation (const ASTNode *node) :
        _hasEquation (true)
{
    _equation = new ASTNode (*node);
    _parseEquation (_equation);
}

MMOEquation::~MMOEquation ()
{
    delete _equation;
}

ASTNode *
MMOEquation::getEquation ()
{
    if (_hasEquation)
    {

        return (_equation);
    }
    return (NULL);
}

bool
MMOEquation::hasZeroCrossing ()
{
    return (_events.size () > 0);
}

pair<ASTNode *, list<pair<string, ASTNode *> > >
MMOEquation::first ()
{
    _it = _events.begin ();
    pair<ASTNode *, list<pair<string, ASTNode *> > > ret = _make_pair ();
    return (ret);
}

pair<ASTNode *, list<pair<string, ASTNode *> > >
MMOEquation::next ()
{
    _it++;
    pair<ASTNode *, list<pair<string, ASTNode *> > > ret;
    if (!end ())
    {
        ret = _make_pair ();
    }
    return (ret);
}

bool
MMOEquation::end ()
{
    return (_it == _events.end ());
}

bool
MMOEquation::hasEquation ()
{
    return (_hasEquation);
}

ASTNode*
MMOEquation::ASTExpression ()
{
    return (_equation);
}

void
MMOEquation::joinEvents (MMOEquation *b)
{
    if (b->hasZeroCrossing ())
    {
        pair<ASTNode *, list<pair<string, ASTNode *> > > it;
        for (it = b->first (); !b->end (); it = b->next ())
        {
            _events.push_back (it);
        }
    }
}

pair<ASTNode *, list<pair<string, ASTNode *> > >
MMOEquation::_make_pair ()
{
    pair<ASTNode *, list<pair<string, ASTNode *> > > ret;
    list<pair<string, ASTNode *> > asgs;
    for (list<pair<string, ASTNode *> >::iterator i = _it->second.begin (); i != _it->second.end (); i++)
    {
        pair<string, ASTNode *> asg;
        asg = make_pair (i->first, i->second);
        asgs.push_back (asg);
    }
    ret = make_pair (_it->first, asgs);
    return (ret);
}

void
MMOEquation::_parseEquation (ASTNode *node)
{
    if (node->getType () == AST_FUNCTION_POWER)
    {
        node->setType (AST_POWER);
    }
    else if (node->getType () == AST_FUNCTION_PIECEWISE)
    {
        _hasEquation = false;
        ASTNode *otherwise = NULL;
        ASTNode *otherwiseChilds = NULL;
        list<ASTNode*> elseIf;
        int numchilds = node->getNumChildren ();
        if (numchilds % 2)
        {
            otherwise = new ASTNode (AST_LOGICAL_NOT);
            if (numchilds > 3)
            {
                otherwiseChilds = new ASTNode (AST_LOGICAL_AND);
            }
            numchilds--;
        }
        int i;
        for (i = 0; i < numchilds; i = i + 2)
        {
            MMOEquation *tmpEq = new MMOEquation (node->getChild (i + 1));
            joinEvents (tmpEq);
            ASTNode *zc;
            if (i > 0)
            {
                zc = new ASTNode (AST_LOGICAL_AND);
                ASTNode *originalCond = new ASTNode (*(tmpEq->ASTExpression ()));
                ASTNode *notElseIf = new ASTNode (AST_LOGICAL_NOT);
                ASTNode *andElseIf;
                if (elseIf.size () == 1)
                {
                    andElseIf = new ASTNode (*(elseIf.front ()));
                }
                else
                {
                    andElseIf = new ASTNode (AST_LOGICAL_AND);
                    list<ASTNode*>::iterator it;
                    for (it = elseIf.begin (); it != elseIf.end (); it++)
                    {
                        andElseIf->addChild (*it);
                    }
                }
                notElseIf->addChild (andElseIf);
                zc->addChild (originalCond);
                zc->addChild (notElseIf);
            }
            else
            {
                zc = new ASTNode (*(tmpEq->ASTExpression ()));
            }
            list<pair<string, ASTNode*> > asgs;
            pair<string, ASTNode*> as;
            ASTNode *evAsg = new ASTNode (*node->getChild (i));
            as = make_pair ("", evAsg);
            asgs.push_back (as);
            pair<ASTNode *, list<pair<string, ASTNode*> > > ev;
            ev = make_pair (zc, asgs);
            _events.push_back (ev);
            if (otherwise != NULL)
            {
                if (otherwiseChilds == NULL)
                {
                    otherwise->addChild (zc);
                }
                else
                {
                    otherwiseChilds->addChild (zc);
                }
            }
            elseIf.push_front (new ASTNode (*(tmpEq->ASTExpression ())));
        }
        if (otherwise != NULL)
        {
            if (otherwiseChilds != NULL)
            {
                otherwise->addChild (otherwiseChilds);
            }
            list<pair<string, ASTNode*> > asgs;
            pair<string, ASTNode*> as;
            ASTNode *evAsg = new ASTNode (*node->getChild (numchilds));
            as = make_pair ("", evAsg);
            asgs.push_back (as);
            pair<ASTNode *, list<pair<string, ASTNode*> > > ev;
            ev = make_pair (otherwise, asgs);
            _events.push_back (ev);
        }
        return;
    }
    else if (node->getType () == AST_FUNCTION_FLOOR)
    {
        string var = MMOUtils::getInstance ()->getVar ();
        ASTNode *zc = new ASTNode (AST_RELATIONAL_GEQ);
        ASTNode *minus = new ASTNode (AST_MINUS);
        ASTNode *dvar = new ASTNode (AST_NAME);
        ASTNode *cond = new ASTNode (AST_INTEGER);
        ASTNode *fminus = new ASTNode (*node->getChild (0));
        dvar->setName (var.c_str ());
        minus->addChild (fminus);
        minus->addChild (dvar);
        cond->setValue (1);
        zc->addChild (minus);
        zc->addChild (cond);
        ASTNode *inc = new ASTNode (AST_PLUS);
        inc->addChild (dvar);
        ASTNode *cte = new ASTNode (AST_INTEGER);
        cte->setValue (1);
        inc->addChild (cte);
        list<pair<string, ASTNode*> > asgs;
        pair<string, ASTNode*> as;
        as = make_pair (var, inc);
        asgs.push_back (as);
        pair<ASTNode *, list<pair<string, ASTNode*> > > ev;
        ev = make_pair (zc, asgs);
        _events.push_back (ev);
        ASTNode *zc_lt = new ASTNode (AST_RELATIONAL_LT);
        ASTNode *cond_lt = new ASTNode (AST_INTEGER);
        cond_lt->setValue (0);
        zc_lt->addChild (minus);
        zc_lt->addChild (cond_lt);
        ASTNode *dec = new ASTNode (AST_MINUS);
        dec->addChild (dvar);
        dec->addChild (cte);
        list<pair<string, ASTNode*> > asgs_lt;
        pair<string, ASTNode*> as_lt;
        as_lt = make_pair (var, dec);
        asgs_lt.push_back (as_lt);
        pair<ASTNode *, list<pair<string, ASTNode*> > > ev_lt;
        ev_lt = make_pair (zc_lt, asgs_lt);
        _events.push_back (ev_lt);
        node->setType (AST_NAME);
        node->setName (var.c_str ());
        node->removeChild (0);
    }
    else if (node->getType () == AST_FUNCTION_CEILING)
    {
        string var = MMOUtils::getInstance ()->getVar ();
        ASTNode *zc = new ASTNode (AST_RELATIONAL_LT);
        ASTNode *minus = new ASTNode (AST_MINUS);
        ASTNode *dvar = new ASTNode (AST_NAME);
        ASTNode *cond = new ASTNode (AST_INTEGER);
        ASTNode *fminus = new ASTNode (*node->getChild (0));
        dvar->setName (var.c_str ());
        minus->addChild (dvar);
        minus->addChild (fminus);
        cond->setValue (0);
        zc->addChild (minus);
        zc->addChild (cond);
        ASTNode *inc = new ASTNode (AST_PLUS);
        inc->addChild (dvar);
        ASTNode *cte = new ASTNode (AST_INTEGER);
        cte->setValue (1);
        inc->addChild (cte);
        list<pair<string, ASTNode*> > asgs;
        pair<string, ASTNode*> as;
        as = make_pair (var, inc);
        asgs.push_back (as);
        pair<ASTNode *, list<pair<string, ASTNode*> > > ev;
        ev = make_pair (zc, asgs);
        _events.push_back (ev);
        ASTNode *zc_lt = new ASTNode (AST_RELATIONAL_GEQ);
        ASTNode *cond_lt = new ASTNode (AST_INTEGER);
        cond_lt->setValue (1);
        zc_lt->addChild (minus);
        zc_lt->addChild (cond_lt);
        ASTNode *dec = new ASTNode (AST_MINUS);
        dec->addChild (dvar);
        dec->addChild (cte);
        list<pair<string, ASTNode*> > asgs_lt;
        pair<string, ASTNode*> as_lt;
        as_lt = make_pair (var, dec);
        asgs_lt.push_back (as_lt);
        pair<ASTNode *, list<pair<string, ASTNode*> > > ev_lt;
        ev_lt = make_pair (zc_lt, asgs_lt);
        _events.push_back (ev_lt);
        node->setType (AST_NAME);
        node->setName (var.c_str ());
        node->removeChild (0);
    }
    else if (node->getType () == AST_FUNCTION_ABS)
    {
        string var = MMOUtils::getInstance ()->getVar ();
        ASTNode *zc = new ASTNode (AST_RELATIONAL_GEQ);
        ASTNode *cond = new ASTNode (AST_INTEGER);
        ASTNode *farg = new ASTNode (*node->getChild (0));
        cond->setValue (0);
        zc->addChild (farg);
        zc->addChild (cond);
        list<pair<string, ASTNode*> > asgs;
        pair<string, ASTNode*> as;
        as = make_pair (var, farg);
        asgs.push_back (as);
        pair<ASTNode *, list<pair<string, ASTNode*> > > ev;
        ev = make_pair (zc, asgs);
        _events.push_back (ev);
        ASTNode *zc_lt = new ASTNode (AST_RELATIONAL_LT);
        zc_lt->addChild (farg);
        zc_lt->addChild (cond);
        ASTNode *asg_lt = new ASTNode (AST_TIMES);
        ASTNode *condMinus = new ASTNode (AST_INTEGER);
        condMinus->setValue (-1);
        asg_lt->addChild (condMinus);
        asg_lt->addChild (farg);
        list<pair<string, ASTNode*> > asgs_lt;
        pair<string, ASTNode*> as_lt;
        as_lt = make_pair (var, asg_lt);
        asgs_lt.push_back (as_lt);
        pair<ASTNode *, list<pair<string, ASTNode*> > > ev_lt;
        ev_lt = make_pair (zc_lt, asgs_lt);
        _events.push_back (ev_lt);
        node->setType (AST_NAME);
        node->setName (var.c_str ());
        node->removeChild (0);
    }
    int childs = node->getNumChildren ();
    int i;
    for (i = 0; i < childs; i++)
    {
        _parseEquation (node->getChild (i));
    }
}
