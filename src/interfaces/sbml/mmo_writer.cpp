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

#include "mmo_writer.h"

#include <iostream>
#include <iterator>

#include "mmo_decl.h"
#include "mmo_utils.h"

MMOWriter::MMOWriter(string file) :
    _indent(1), _modelName(), _out(file.c_str(),
        ios::out | ios::in | ios::trunc),
        _outputs(), _condition(false), _initialAsignment(false)
{
  if(!_out.good())
  {
    cout << "Can not open file: " << file << endl;
  }
  _modelName = file;
  unsigned int found = _modelName.rfind("/");
  if(found != std::string::npos)
  {
    _modelName.erase(_modelName.begin(), _modelName.begin() + found + 1);
  }
  found = _modelName.rfind(".mo");
  if(found != std::string::npos)
  {
    _modelName.erase(_modelName.begin() + found, _modelName.end());
  }
  _out << "model " << _modelName << endl;
}

MMOWriter::~MMOWriter()
{
  _out << "\tannotation(\n\t experiment(\n\t\t MMO_Description=\"" << _modelName
      << " automatic model translation.\"";
  _out
      << ",\n\t\t MMO_Solver=LIQSS2,\n\t\t StartTime=0,\n\t\t StopTime=1,\n\t\t Tolerance= 1e-3,\n\t\t AbsTolerance= 1e-6,\n\t\t";
  _out << "MMO_SymDiff=FALSE\n\t));" << endl;
  _out << "end " << _modelName << ";" << endl;
  _out.close();
}

void
MMOWriter::visit(MMOExp *x)
{
  x->accept(this);
}

void
MMOWriter::leave(MMODecl *x)
{
}

void
MMOWriter::visit(MMODecl *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  if(x->isDerivative())
  {
    _outputs.push_back(x->getId());
    _out << utils->indent(_indent) << "der(" << x->getId() << ") = "
        << x->getExp() << ";" << endl;
  }
  if(x->isImplicit())
  {
    _out << utils->indent(_indent) << "0 = " << x->getExp() << ";" << endl;
  }
  if(x->isAlgebraicEquation())
  {
    _out << utils->indent(_indent) << x->getId() << " = " << x->getExp() << ";"
        << endl;
  }
  if(x->isInitialAssignment() || x->isAssignment())
  {
    _out << utils->indent(_indent) << x->getId() << " := " << x->getExp() << ";"
        << endl;
  }
  if(x->isConditionalAssignment())
  {
    _out << utils->indent(_indent) << x->getExp() << ";" << endl;
  }
  if(x->isReinit())
  {
    _out << utils->indent(_indent) << "reinit(" << x->getId() << ","
        << x->getExp() << ");" << endl;
  }
  if(x->isZeroCrossing())
  {
    _out << x->getExp() << " then" << endl;
    _indent++;
  }
  if(x->isOppositeZeroCrossing())
  {
    if(_initialAsignment)
    {
      _out << "else if " << x->getExp() << " then" << endl;
    }
    else
    {
      _out << "elsewhen " << x->getExp() << " then" << endl;
    }
    _indent++;
  }
  if(x->isCondition())
  {
    _condition = true;
    _out << utils->indent(_indent) << "if " << x->getExp() << " then" << endl;
    _indent++;
  }
  if(x->isState())
  {
    if(x->hasValue())
    {
      _out << utils->indent(_indent) << "Real " << x->getId() << "(start = "
          << x->getValue() << ");" << endl;
    }
    else
    {
      _out << utils->indent(_indent) << "Real " << x->getId() << ";" << endl;
    }
  }
  if(x->isAlgebraic())
  {
    _outputs.push_back(x->getId());
    _out << utils->indent(_indent) << "Real " << x->getId() << ";" << endl;
  }
  if(x->isParameter())
  {
    if(x->hasValue())
    {
      _out << utils->indent(_indent) << "parameter Real " << x->getId() << " = "
          << x->getValue() << ";" << endl;
    }
    else
    {
      _out << utils->indent(_indent) << "parameter Real " << x->getId() << ";"
          << endl;
    }
  }
  if(x->isConstant())
  {
    if(x->hasValue())
    {
      _out << utils->indent(_indent) << "constant Integer " << x->getId()
          << " = " << x->getValue() << ";" << endl;
    }
  }
  if(x->isDiscrete())
  {
    _outputs.push_back(x->getId());
    if(x->hasValue())
    {
      _out << utils->indent(_indent) << "discrete Real " << x->getId()
          << "(start = " << x->getValue() << ");" << endl;
    }
    else
    {
      _out << utils->indent(_indent) << "discrete Real " << x->getId() << ";"
          << endl;
    }
  }
  if(x->isFunctionInput())
  {
    _out << utils->indent(_indent) << "input Real " << x->getId() << ";"
        << endl;
  }
  if(x->isFunctionOutput())
  {
    _out << utils->indent(_indent) << "output Real " << x->getId() << ";"
        << endl;
    _out << utils->indent(_indent) << "algorithm" << endl;
  }
  if(x->isFunctionDefinition())
  {
    _out << utils->indent(_indent) << x->getId() << " := " << x->getExp() << ";"
        << endl;
  }
  if(x->isImport())
  {
    _out << utils->indent(_indent) << "import " << x->getId() << ";" << endl;
  }
}

void
MMOWriter::visit(MMODecl x)
{
  visit(&x);
}

void
MMOWriter::visit(MMOEvent *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  _initialAsignment = x->initialAlgorithm();
  if(_initialAsignment)
  {
    _out << utils->indent(_indent) << "if ";
  }
  else
  {
    _out << utils->indent(_indent) << "when ";
  }
}

void
MMOWriter::visit(MMOEvent x)
{
  visit(&x);
}

void
MMOWriter::leave(MMOEvent *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  if(_condition)
  {
    _out << utils->indent(--_indent) << "end if;" << endl;
    _condition = false;
  }
  if(_initialAsignment)
  {
    _out << utils->indent(--_indent) << "end if;" << endl;
  }
  else
  {
    _out << utils->indent(--_indent) << "end when;" << endl;
  }
  _initialAsignment = false;
}

void
MMOWriter::visit(MMOSection *x)
{
  if(!x->isEmpty())
  {
    static MMOUtils *utils = MMOUtils::getInstance();
    _out << utils->indent(_indent) << x->getId() << endl;
    _indent++;
  }
}

void
MMOWriter::leave(MMOSection *x)
{
  if(!x->isEmpty())
  {
    _indent--;
  }
}

void
MMOWriter::visit(MMOSection x)
{
  visit(&x);
}

void
MMOWriter::visit(MMOFunction *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  _out << utils->indent(_indent) << "function " << x->getId() << endl;
  _indent++;
}

void
MMOWriter::visit(MMOFunction x)
{
  visit(&x);
}

void
MMOWriter::leave(MMOFunction *x)
{
  static MMOUtils *utils = MMOUtils::getInstance();
  _out << utils->indent(_indent) << "return;" << endl;
  _indent--;
  _out << utils->indent(_indent) << "end " << x->getId() << ";" << endl;
}
