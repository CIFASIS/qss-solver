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

#ifndef VARIABLE_LOOKUP_H_
#define VARIABLE_LOOKUP_H_

#include "../ast_util.h"

namespace MicroModelica {
  namespace Util {
    /**
     * @breif Helper class that looks for a variable 
     *        definition in more than one symbol table.
     */
    class VariableLookup: public AST_Expression_Fold<bool>
    {
      public:
        /**
         *
         * @param st
         * @param lst
         */
        VariableLookup(MicroModelica::Util::VarSymbolTable st, MicroModelica::Util::VarSymbolTable lst);
        /**
         *
         */
        ~VariableLookup() {};
      private:
        bool
        foldTraverseElement(AST_Expression);
        bool
        foldTraverseElement(bool, bool, BinOpType);
        bool
        foldTraverseElementUMinus(AST_Expression);
        VarSymbolTable _st;
        VarSymbolTable _lst;
    };
  }
}
#endif  /* VARIABLE_LOOKUP_H_ */
