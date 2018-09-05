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
      buffer << " ";
      return buffer.str();
    }

    string 
    Macros::print() const 
    {
      stringstream buffer, arguments;
      buffer << "#define " << _variable;
      int dim = _variable.dimensions();
      buffer << parameters();
      if(_variable.isState()) { buffer << "x"; }
      if(_variable.isAlgebraic()) { buffer << "a"; }
      if(_variable.isDiscrete()) { buffer << "d"; } 
      if(_variable.isParameter()) { buffer << "__PAR__" << _variable.name(); } 
      stringstream end;
      if(!_model.annotations().classic() &&
        (_variable.isAlgebraic() || _variable.isState())) 
      { 
        end << "*" << _model.annotations().polyCoeffs(); 
      }
      end << "]"; 
      if(dim)
      {
        arguments << "[("; 
        for(int i = 0; i < dim; i++)
        {
          stringstream variable;
          variable << "*" << _variable.size(i) << "+";
          arguments << "(d" << i+1 <<  "-1)" << (i == dim-1 ? ")"+end.str() : variable.str()); 
        }
      }
      else if(_variable.isDiscrete() || _variable.isState() || _variable.isAlgebraic())
      {
        arguments << "[" << _variable.offset() << end.str();
      }
      arguments << endl;
      buffer << arguments.str();
      return buffer.str();
    }

    std::ostream& operator<<(std::ostream& out, const Macros& m)
    {
      return out << m.print();
    }
  }
}
