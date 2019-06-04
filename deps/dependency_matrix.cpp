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

#include "dependency_matrix.h"

#include "../util/visitors/replace_index.h"
#include "../util/util.h"

namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Deps {

    VariableDependencyMatrix::VariableDependencyMatrix(MatrixConfig cfg) : 
      _cfg(cfg), 
      _mode(VDM::Normal), 
      _method(VDM::Alloc) 
    {

    }

    void
    VariableDependency::setRange(Usage ifrUsg, Usage ifeUsg)
    {
       _range.generate(_ran);
       ReplaceIndex riIfr(_range, ifrUsg);
       _ifr = Expression(riIfr.apply(_ifr.expression()), Utils::instance().symbols());
       ReplaceIndex riIfe(_range, ifeUsg);
       _ife = Expression(riIfe.apply(_ife.expression()), Utils::instance().symbols());
    }

    string 
    VariableDependencyMatrix::print() const 
    {
      cout << "Imprime la matriz de salida " << _cfg.container << " " << size() << endl;
      stringstream buffer;
      VariableDependencyMatrix::const_iterator it;
      string matrix = _cfg.names[_method + _mode];
      string access = _cfg.access[_mode];
      for( it = begin(); it != end(); it++)
      {  
        cout << "Set node " << endl;
        VariableDependencies vds = it->second;
        cout << "DEPS " << vds.size() << endl;
        for (auto vd : vds) {
          Index ifr = vd.ifr();
          Index ife = vd.ife();
          if (_mode == VDM::Transpose) {
            ifr = vd.ife();
            ife = vd.ifr();
          }
          Range range = vd.ifce.range();
          buffer << range;
          if (_method == VDM::Alloc) {
            buffer << range.block() << _cfg.container << matrix << "[" << ifr << "]++;" << endl;   
          } else {
            buffer << range.block() << _cfg.container << matrix << "[" << ifr << "][" << access << "[" << ifr << "]++] = " << ife << endl;   
          }
          if (!range.empty()) {
            buffer << range.end() << endl;  
          }
        }
      }
      cout << buffer.str() << endl;
      return buffer.str();
    }
    
    std::ostream& operator<<(std::ostream& out, const VariableDependencyMatrix& d) 
    {
      return out << d.print();
    }
  }
}