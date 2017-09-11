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

#include "mmo_zerocrossing.h"

#include <iostream>

#include "mmo_utils.h"

MMOZeroCrossing::MMOZeroCrossing () :
        _zero_crossing (NULL)
{
}

MMOZeroCrossing::MMOZeroCrossing (ASTNode *math)
{
    _parseLogicalOperators (math, NULL);
}

MMOZeroCrossing::~MMOZeroCrossing ()
{
}

ASTNode *
MMOZeroCrossing::_addCond (ASTNode *cond, ASTNode *node)
{
    ASTNode *ret = NULL;
    if (cond == NULL)
    {
        cond = new ASTNode (AST_LOGICAL_AND);
    }
    cond->addChild (node);
    return (ret);
}

void
MMOZeroCrossing::_negateNode (ASTNode *node, ASTNode *cond)
{
    if (node->getType () == AST_RELATIONAL_EQ)
    {
        node->setType (AST_RELATIONAL_NEQ);
    }
    else if (node->getType () == AST_RELATIONAL_NEQ)
    {
        node->setType (AST_RELATIONAL_EQ);
    }
    else if (node->getType () == AST_RELATIONAL_GT)
    {
        node->setType (AST_RELATIONAL_LEQ);
    }
    else if (node->getType () == AST_RELATIONAL_GEQ)
    {
        node->setType (AST_RELATIONAL_LT);
    }
    else if (node->getType () == AST_RELATIONAL_LT)
    {
        node->setType (AST_RELATIONAL_GEQ);
    }
    else if (node->getType () == AST_RELATIONAL_LEQ)
    {
        node->setType (AST_RELATIONAL_GT);
    }
    else if (node->getType () == AST_LOGICAL_AND)
    {
        ASTNode *root = new ASTNode (AST_LOGICAL_OR);
        int childs = node->getNumChildren (), i;
        for (i = 0; i < childs; i++)
        {
            ASTNode *l = new ASTNode (AST_LOGICAL_NOT);
            l->addChild (node->getChild (i));
            root->addChild (l);
        }
        _parseZeroCrossing (root, cond);
        return;
    }
    else if (node->getType () == AST_LOGICAL_OR)
    {
        ASTNode *root = new ASTNode (AST_LOGICAL_AND);
        int childs = node->getNumChildren (), i;
        for (i = 0; i < childs; i++)
        {
            ASTNode *l = new ASTNode (AST_LOGICAL_NOT);
            l->addChild (node->getChild (i));
            root->addChild (l);
        }
        _parseZeroCrossing (root, cond);
        return;
    }
    else if (node->getType () == AST_LOGICAL_XOR)
    {
        ASTNode *root = new ASTNode (AST_LOGICAL_XOR);
        int childs = node->getNumChildren (), i;
        childs--;
        for (i = 0; i < childs; i++)
        {
            ASTNode *l = new ASTNode (AST_LOGICAL_NOT);
            l->addChild (node->getChild (i));
            root->addChild (l);
        }
        root->addChild (node->getChild (childs));
        _parseZeroCrossing (root, cond);
        return;
    }
    else if (node->getType () == AST_CONSTANT_FALSE)
    {
        node->setType (AST_CONSTANT_TRUE);
    }
    else if (node->getType () == AST_CONSTANT_TRUE)
    {
        node->setType (AST_CONSTANT_FALSE);
    }
    _parseZeroCrossing (node, cond);
}

void
MMOZeroCrossing::_parseZeroCrossing (ASTNode *node, ASTNode *cond)
{
    if (node->getType () == AST_LOGICAL_AND)
    {
        int childs = node->getNumChildren (), i;
        for (i = 0; i < childs; i++)
        {
            ASTNode *c = node->getChild (i);
            if (cond == NULL)
            {
                cond = new ASTNode (AST_LOGICAL_AND);
            }
            cond->addChild (c);
            _parseLogicalOperators (c, cond);
        }
    }
    else if (node->getType () == AST_LOGICAL_OR)
    {
        int childs = node->getNumChildren (), i;
        for (i = 0; i < childs; i++)
        {
            ASTNode *c = node->getChild (i);
            _parseLogicalOperators (c, cond);
        }
    }
    else if (node->getType () == AST_LOGICAL_XOR)
    {
        int childs = node->getNumChildren (), i;
        for (i = 0; i < childs; i++)
        {
            ASTNode *c = node->getChild (i);
            ASTNode *cl = new ASTNode (AST_LOGICAL_NOT);
            if (cond == NULL)
            {
                cond = new ASTNode (AST_LOGICAL_OR);
            }
            cl->addChild (c);
            cond->addChild (cl);
            _parseLogicalOperators (c, cond);
        }
    }
    else if (node->getType () == AST_LOGICAL_NOT)
    {
        ASTNode *negateNode = new ASTNode (*(node->getChild (0)));
        _negateNode (negateNode, cond);
    }
    else if (node->getType () == AST_RELATIONAL_GT || node->getType () == AST_RELATIONAL_GEQ)
    {
        ASTNode *zc = new ASTNode (*node);
        pair<ASTNode*, ASTNode*> p (zc, cond);
        _zero_crossings.push_back (p);
        _types.push_back (true);
    }
    else if (node->getType () == AST_RELATIONAL_LT || node->getType () == AST_RELATIONAL_LEQ)
    {
        ASTNode *zc = new ASTNode (*node);
        pair<ASTNode*, ASTNode*> p (zc, cond);
        _zero_crossings.push_back (p);
        _types.push_back (false);
    }
    else if (node->getType () == AST_RELATIONAL_EQ)
    {
        ASTNode *zc_geq = new ASTNode (AST_RELATIONAL_GEQ);
        zc_geq->addChild (node->getChild (0));
        zc_geq->addChild (node->getChild (1));
        pair<ASTNode*, ASTNode*> p (zc_geq, cond);
        _zero_crossings.push_back (p);
        _types.push_back (true);
        ASTNode *zc_lt = new ASTNode (AST_RELATIONAL_LEQ);
        zc_lt->addChild (node->getChild (0));
        zc_lt->addChild (node->getChild (1));
        pair<ASTNode*, ASTNode*> p_lt (zc_lt, cond);
        _zero_crossings.push_back (p_lt);
        _types.push_back (false);
    }
    else if (node->getType () == AST_RELATIONAL_NEQ)
    {
        ASTNode *zc_gt = new ASTNode (AST_RELATIONAL_GT);
        zc_gt->addChild (node->getChild (0));
        zc_gt->addChild (node->getChild (1));
        pair<ASTNode*, ASTNode*> p (zc_gt, cond);
        _zero_crossings.push_back (p);
        _types.push_back (true);
        ASTNode *zc_lt = new ASTNode (AST_RELATIONAL_LT);
        zc_lt->addChild (node->getChild (0));
        zc_lt->addChild (node->getChild (1));
        pair<ASTNode*, ASTNode*> p_lt (zc_lt, cond);
        _zero_crossings.push_back (p_lt);
        _types.push_back (false);
    }
    else if (node->getType () == AST_CONSTANT_TRUE)
    {
        ASTNode *zcPos = new ASTNode (AST_RELATIONAL_GEQ);
        ASTNode *constant = new ASTNode (AST_INTEGER);
        constant->setValue (0);
        ASTNode *variable = new ASTNode (AST_NAME);
        variable->setName ("time");
        zcPos->addChild (variable);
        zcPos->addChild (constant);
        pair<ASTNode*, ASTNode*> p (zcPos, cond);
        _zero_crossings.push_back (p);
        _types.push_back (true);
    }
    else if (node->getType () == AST_CONSTANT_FALSE)
    {
        ASTNode *zcPos = new ASTNode (AST_RELATIONAL_GEQ);
        ASTNode *constant = new ASTNode (AST_INTEGER);
        constant->setValue (1);
        ASTNode *variable = new ASTNode (AST_INTEGER);
        variable->setValue (0);
        zcPos->addChild (variable);
        zcPos->addChild (constant);
        pair<ASTNode*, ASTNode*> p (zcPos, cond);
        _zero_crossings.push_back (p);
        _types.push_back (true);
    }
}

ASTNode *
MMOZeroCrossing::_reverseRelation (ASTNode *node)
{
    ASTNode *ret = new ASTNode ();
    if (node->getType () == AST_RELATIONAL_EQ)
    {
        ret->setType (AST_RELATIONAL_NEQ);
    }
    else if (node->getType () == AST_RELATIONAL_NEQ)
    {
        ret->setType (AST_RELATIONAL_EQ);
    }
    else if (node->getType () == AST_RELATIONAL_GT)
    {
        ret->setType (AST_RELATIONAL_LEQ);
    }
    else if (node->getType () == AST_RELATIONAL_GEQ)
    {
        ret->setType (AST_RELATIONAL_LT);
    }
    else if (node->getType () == AST_RELATIONAL_LT)
    {
        ret->setType (AST_RELATIONAL_GT);
    }
    else if (node->getType () == AST_RELATIONAL_LEQ)
    {
        ret->setType (AST_RELATIONAL_GT);
    }
    ret->addChild (node->getChild (0));
    ret->addChild (node->getChild (1));
    return (ret);
}

pair<string, string>
MMOZeroCrossing::first ()
{
    static MMOUtils *utils = MMOUtils::getInstance ();
    pair<string, string> ret;
    _it = _zero_crossings.begin ();
    string cond = "";
    if (_it->second != NULL)
    {
        cond = utils->getExp (_it->second);
    }
    ret = make_pair (utils->getExp (_it->first), cond);
    return (ret);
}

pair<string, string>
MMOZeroCrossing::next ()
{
    static MMOUtils *utils = MMOUtils::getInstance ();
    pair<string, string> ret;
    _it++;
    if (_it == _zero_crossings.end ())
    {
        return (ret);
    }
    string cond = "";
    if (_it->second != NULL)
    {
        cond = utils->getExp (_it->second);
    }
    ret = make_pair (utils->getExp (_it->first), cond);
    return (ret);
}

bool
MMOZeroCrossing::end ()
{
    return (_it == _zero_crossings.end ());
}

string
MMOZeroCrossing::getReverseRelation ()
{
    string ret;
    static MMOUtils *utils = MMOUtils::getInstance ();
    ret = utils->getExp (_it->first);
    return (ret);
}

bool
MMOZeroCrossing::isPositive (int i)
{
    return (_types[i]);
}

void
MMOZeroCrossing::_parseLogicalOperators (ASTNode* node, ASTNode *cond)
{
    if (node->getType () == AST_LOGICAL_AND || node->getType () == AST_LOGICAL_XOR)
    {
        int childs = node->getNumChildren ();
        if (childs == 0)
        {
            node->setType (AST_CONSTANT_FALSE);
        }
        else if (childs == 1)
        {
            _parseZeroCrossing (node->getChild (0), cond);
            return;
        }
    }
    else if (node->getType () == AST_LOGICAL_OR)
    {
        int childs = node->getNumChildren ();
        if (childs == 0)
        {
            node->setType (AST_CONSTANT_TRUE);
        }
        else if (childs == 1)
        {
            _parseZeroCrossing (node->getChild (0), cond);
            return;
        }
    }
    else if (node->getType () == AST_LOGICAL_NOT)
    {
        ASTNode *c = node->getChild (0);
        if ((c->getType () == AST_LOGICAL_AND || c->getType () == AST_LOGICAL_XOR) && c->getNumChildren () == 0)
        {
            c->setType (AST_CONSTANT_FALSE);
        }
        else if (c->getType () == AST_LOGICAL_OR && c->getNumChildren () == 0)
        {
            c->setType (AST_CONSTANT_TRUE);
        }
    }
    _parseZeroCrossing (node, cond);
}
