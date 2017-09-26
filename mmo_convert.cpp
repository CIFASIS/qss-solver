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

#include "mmo_convert.h"

#include <sbml/AssignmentRule.h>
#include <sbml/Constraint.h>
#include <sbml/ModifierSpeciesReference.h>
#include <sbml/Priority.h>
#include <sbml/RateRule.h>
#include <sbml/Reaction.h>
#include <sbml/SpeciesReference.h>
#include <iostream>
#include <utility>

#include "mmo_section.h"

MMOConvert::MMOConvert(string prefix, bool replace, WR_Type type) :
    _reaction_def(), _events(), _in_reaction(false), _type(type), _reaction_name()
{
  _model = MMOModel(prefix, replace, type);
}

MMOConvert::~MMOConvert()
{
}

void
MMOConvert::visit(const SBMLDocument &x)
{
  cout << "Visit SBMLDocument" << endl;
}

void
MMOConvert::visit(const Model &x)
{
  cout << "Visit Model" << endl;
}

void
MMOConvert::visit(const KineticLaw &x)
{
  cout << "Visit KineticLaw" << endl;
}

void
MMOConvert::visit(const ListOf &x, int type)
{
  cout << "Visit ListOf" << endl;
}

bool
MMOConvert::visit(const SBase &x)
{
  cout << "Visit SBase" << endl;
  return true;
}

bool
MMOConvert::visit(const FunctionDefinition &x)
{
  cout << "Visit FunctionDefinition " << endl;
  _model.add(x);
  return true;
}

bool
MMOConvert::visit(const UnitDefinition &x)
{
  cout << "Visit UnitDefinition" << endl;
  return true;
}

bool
MMOConvert::visit(const Unit &x)
{
  cout << "Visit Unit" << endl;
  return true;
}

bool
MMOConvert::visit(const CompartmentType &x)
{
  cout << "Visit CompartmentType" << endl;
  return true;
}

bool
MMOConvert::visit(const SpeciesType &x)
{
  cout << "Visit SpeciesType" << endl;
  return true;
}

bool
MMOConvert::visit(const Compartment &x)
{
  cout << "Visit Compartment " << x.getId() << endl;
  if(x.isSetSize())
  {
    _model.add(x.getId(), x.isSetConstant(), x.getSize(), SEC_DECLARATIONS);
  }
  else
  {
    _model.add(x.getId(), x.isSetConstant(), SEC_DECLARATIONS);
  }
  return true;
}

bool
MMOConvert::visit(const Species &x)
{
  if(x.isSetInitialAmount())
  {
    _model.add(x.getId(), x.getConstant(), x.getInitialAmount(),
        SEC_DECLARATIONS);
  }
  else if(x.isSetInitialConcentration())
  {
    _model.add(x.getId(), x.getConstant(), x.getInitialConcentration(),
        SEC_DECLARATIONS);
  }
  else
  {
    _model.add(x.getId(), x.getConstant(), SEC_DECLARATIONS);
  }
  cout << "Visit Species " << x.getId() << endl;
  return true;
}

bool
MMOConvert::visit(const Parameter &x)
{
  cout << "Visit Parameter " << x.getId() << endl;
  if(x.isSetValue())
  {
    _model.add(x.getId(), x.getConstant(), x.getValue(), SEC_DECLARATIONS);
  }
  else
  {
    _model.add(x.getId(), x.getConstant(), SEC_DECLARATIONS);
  }
  return true;
}

bool
MMOConvert::visit(const InitialAssignment &x)
{
  cout << "Visit InitialAssignment" << endl;
  _model.add(x.getId(), false, x.getMath(), SEC_INITIAL_ALGORITHM);
  return true;
}

bool
MMOConvert::visit(const Rule &x)
{
  cout << "Visit Rule" << x.getId() << endl;
  return true;
}

bool
MMOConvert::visit(const AlgebraicRule &x)
{
  cout << "Visit AlgebraicRule " << x.getId() << endl;
  _model.add(x.getVariable(), false, x.getMath(), SEC_EQUATION);
  return true;
}

bool
MMOConvert::visit(const AssignmentRule &x)
{
  cout << "Visit AssignmentRule " << x.getId() << endl;
  _model.add(x.getVariable(), false, x.getMath(), SEC_EQUATION);
  return true;
}

bool
MMOConvert::visit(const RateRule &x)
{
  cout << "Visit RateRule " << x.getId() << endl;
  _model.add(x.getVariable(), true, x.getMath(), SEC_EQUATION);
  return true;
}

bool
MMOConvert::visit(const Constraint &x)
{
  return true;
}

bool
MMOConvert::visit(const Reaction &x)
{
  KineticLaw *kl = new KineticLaw(*x.getKineticLaw());
  // TODO: Parse local parameters, find examples.
  const ListOfSpeciesReferences *ref = x.getListOfReactants();
  int s = ref->size();
  for(int i = 0; i < s; i++)
  {
    const SimpleSpeciesReference *sp = ref->get(i);
    string sName = sp->getSpecies();
    const Model *model = x.getModel();
    const Species *spe = model->getSpecies(sName);
    if(spe->getBoundaryCondition() == false && spe->getConstant() == false)
    {
      map<string, ASTNode*>::iterator reac = _reaction_def.find(sName);
      if(reac == _reaction_def.end())
      {
        ASTNode *min = new ASTNode(AST_TIMES);
        ASTNode *ft = new ASTNode(AST_MINUS);
        ASTNode *constant = new ASTNode(AST_INTEGER);
        constant->setValue(1);
        ft->addChild(constant);
        min->addChild(ft);
        min->addChild(new ASTNode(*kl->getMath()));
        _reaction_def[sName] = min;
      }
      else
      {
        ASTNode *min = new ASTNode(AST_PLUS);
        min->addChild(reac->second);
        ASTNode *pt = new ASTNode(AST_TIMES);
        ASTNode *ft = new ASTNode(AST_MINUS);
        ASTNode *constant = new ASTNode(AST_INTEGER);
        constant->setValue(1);
        ft->addChild(constant);
        pt->addChild(ft);
        pt->addChild(new ASTNode(*kl->getMath()));
        min->addChild(pt);
        _reaction_def[sName] = min;
      }
    }
  }
  ref = x.getListOfProducts();
  s = ref->size();
  for(int i = 0; i < s; i++)
  {
    const SimpleSpeciesReference *sp = ref->get(i);
    string sName = sp->getSpecies();
    const Model *model = x.getModel();
    const Species *spe = model->getSpecies(sName);
    if(spe->getBoundaryCondition() == false && spe->getConstant() == false)
    {
      map<string, ASTNode*>::iterator reac = _reaction_def.find(sName);
      if(reac == _reaction_def.end())
      {
        _reaction_def[sName] = new ASTNode(*kl->getMath());
      }
      else
      {
        ASTNode *min = new ASTNode(AST_PLUS);
        min->addChild(reac->second);
        min->addChild(new ASTNode(*kl->getMath()));
        _reaction_def[sName] = min;
      }
    }
  }
  return true;
}

bool
MMOConvert::visit(const SimpleSpeciesReference &x)
{
  cout << "Visit SimpleSpeciesReference" << endl;
  return true;
}

bool
MMOConvert::visit(const SpeciesReference &x)
{
  cout << "Visit SpeciesReference" << endl;
  return true;
}

bool
MMOConvert::visit(const ModifierSpeciesReference &x)
{
  cout << "Visit ModifierSpeciesReference" << endl;
  return true;
}

bool
MMOConvert::visit(const Event &x)
{
  cout << "Visit Event" << endl;
  _events.push_back(x);
  return true;
}

bool
MMOConvert::visit(const EventAssignment &x)
{
  cout << "Visit EventAssignment" << endl;
  return true;
}

bool
MMOConvert::visit(const Trigger &x)
{
  cout << "Visit Trigger" << endl;
  return true;
}

bool
MMOConvert::visit(const Delay &x)
{
  cout << "Visit Delay" << endl;
  return true;
}

void
MMOConvert::visit(const Priority &x)
{
  cout << "Visit Priority" << endl;
}

void
MMOConvert::leave(const SBMLDocument &x)
{
  cout << "Leave SBMLDocument" << endl;
}

void
MMOConvert::leave(const Model &x)
{
  map<string, ASTNode*>::iterator it;
  for(it = _reaction_def.begin(); it != _reaction_def.end(); it++)
  {
    _model.add(it->first, true, it->second, SEC_EQUATION);
  }
  list<Event>::iterator i;
  for(i = _events.begin(); i != _events.end(); i++)
  {
    _model.add(*i);
  }
  cout << "Leave Model" << endl;
}

void
MMOConvert::leave(const KineticLaw &x)
{
  cout << "Leave KineticLaw" << endl;
}

void
MMOConvert::leave(const Priority &x)
{
  cout << "Leave Priority" << endl;
}

void
MMOConvert::leave(const Reaction &x)
{
  cout << "Leave Reaction" << endl;
}

void
MMOConvert::leave(const SBase &x)
{
  cout << "Leave SBase" << endl;
}

void
MMOConvert::leave(const ListOf &x, int type)
{
  cout << "Leave ListOf " << type << endl;
}
