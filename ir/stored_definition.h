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

#ifndef MMO_STORED_DEFINITION_H_
#define MMO_STORED_DEFINITION_H_

#include <string>

#include "../ir/class.h"

namespace MicroModelica {
  namespace IR {

    /**
     *
     */
    class StoredDefinition
    {
      public:
        /**
         *
         */
        StoredDefinition();
        /**
         *
         */
        ~StoredDefinition() {};
        bool 
        isModel();
        bool 
        isPackage();
        Model& 
        model();
        Package&
        package();
        void 
        setModel(string name);
        void 
        setPackage(string name);
        void 
        addFunction(Function& f);
        Option<Function> 
        function(string name);
        void 
        postProcess();
      private:
        ClassType      _def;
        FunctionTable  _functions;
    };
  }
}

#endif  /* MMO_STORED_DEFINITION_H_ */
