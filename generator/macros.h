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

#ifndef MMO_MACROS_H_
#define MMO_MACROS_H_

#include <fstream>
#include <string>
#include <list>

#include "../util/symbol_table.h"
#include "../ir/class.h"

namespace MicroModelica {
  namespace Generator {
    
    /**
     *
     */
    class Macros 
    {
      public:
        Macros(IR::Model& model, Util::Variable& variable); 
        ~Macros() {};
        friend std::ostream& operator<<(std::ostream& out, const Macros& m);
        std::string 
        print() const;
        std::string 
        parameters() const;
      private:
        IR::Model      _model;
        Util::Variable _variable;
    };
  }
}

#endif  /* MMO_MACROS_H_ */
