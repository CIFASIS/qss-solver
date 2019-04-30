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

#include "macros.h"

#include <sstream>

using namespace std;

namespace MicroModelica {
  using namespace Util;
  using namespace IR;
  namespace Generator {
    
    Macros::Macros(IR::Model& model, Util::Variable& variable) : 
          _model(model),
          _variable(variable)
    {
    }

    string 
    Macros::parameters() const
    {
      stringstream buffer;
      int dim = _variable.dimensions();
      if(dim) { buffer << "("; }
      for(int i = 0; i < dim; i++)
      {
        buffer << "d" << i+1 << (i == dim-1 ? ")" : ","); 
      }
      return buffer.str();
    }

    string 
    Macros::arguments() const 
    {
      stringstream arguments;
      if(_variable.isEqType()) {
        arguments << _variable << endl;
        return arguments.str();
      }
      int dim = _variable.dimensions();
      stringstream end;
      if(!_model.annotations().classic() &&
        (_variable.isAlgebraic() || _variable.isState())) 
      { 
        end << "*" << _model.annotations().polyCoeffs(); 
      }
      if(dim)
      {
        arguments << "("; 
        for(int i = 0; i < dim; i++)
        {
          stringstream variable;
          variable << "*" << _variable.rowSize(i) << "+";
          if(_variable.offset()) { arguments << _variable.offset() << "+"; }
          arguments << "(d" << i+1 <<  "-1)" << (i == dim-1 ? ")"+end.str() : variable.str()); 
        }
      }
      else if(_variable.isDiscrete() || _variable.isState() || _variable.isAlgebraic())
      {
        arguments << _variable.offset() << end.str();
      }
      arguments << endl;
      return arguments.str();
    }

    string 
    Macros::print() const 
    {
      stringstream buffer, index;
      int dim = _variable.dimensions();
      bool idx = !_variable.isParameter() || dim;
      if(idx) 
      { 
        index << "_idx" << _variable << parameters();  
        buffer << "#define " << index.str() << " " << arguments(); 
      }
      if(!_variable.isEqType()) {
        buffer << "#define " << _variable << parameters() << " ";
        if(_variable.isState()) { buffer << "x"; }
        if(_variable.isAlgebraic()) { buffer << "a"; }
        if(_variable.isDiscrete()) { buffer << "d"; } 
        if(_variable.isParameter()) { buffer << "__PAR__" << _variable.name(); } 
        if(idx) { buffer << "["; }
        buffer << index.str(); 
        if(idx) { buffer << "]"; }
        buffer << endl;
      }
      return buffer.str();
    }

    string 
    Macros::usage(string token, Option<Range> range, int id) const
    {
      stringstream buffer, index;
      if(range) { index << "(" << range->indexes() << ")"; }
      buffer << token << index.str();
      return buffer.str();
    }

    string 
    Macros::indexMacro(string token, Option<Range> range, int id) const 
    {
      stringstream buffer, index, arguments;
      if(range) { index << "(" << range->indexes() << ")"; }
      buffer << "#define " << token << index.str() << " ";
      if(range)
      {
        int dim = range->definition().size();
        stringstream end;
        RangeDefinitionTable rdt = range->definition();
        RangeDefinitionTable::iterator it;
        arguments << "("; 
        int i = 0;
        for(RangeDefinition rd = rdt.begin(it); !rdt.end(it); rd = rdt.next(it), i++)
        {
          stringstream variable;
          variable << "*" << range->rowSize(i) << "+";
          arguments << "(" << rdt.key(it) << "-1)" << (i == dim-1 ? ")" : variable.str()); 
        }
        buffer << arguments.str();
      }
      else 
      {
        buffer << id - 1 << endl;
      }
      return buffer.str();
    }

    std::ostream& operator<<(std::ostream& out, const Macros& m)
    {
      return out << m.print();
    }
  }
}
