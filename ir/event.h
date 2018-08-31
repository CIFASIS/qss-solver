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

#ifndef MMO_EVENT_H_
#define MMO_EVENT_H_

#include "../ast/ast_types.h"
#include "../util/table.h"
#include "equation.h"
#include "statement.h"

namespace MicroModelica {
  namespace IR {
    
    namespace EVENT 
    {
        /**
         *
         */
        typedef enum
        {
          POSITIVE, //!< HND_POSITIVE
          NEGATIVE, //!< HND_NEGATIVE
          ZERO      //!< HND_ZERO
        } Type;

        /**
         * Define the original relation type of the zero-crossing function
         * needed by the initial algorithm.
         */
        typedef enum
        {
          LT,    //!< ZC_LT
          LE,    //!< ZC_LE
          GT,    //!< ZC_GT
          GE     //!< ZC_GE
        } Relation;
    }

    /**
     *
     */
    class Event
    {
      public:
        /**
         *
         * @param cond
         * @param data
         */
        Event() {};
        Event(AST_Equation cond);
        /**
         *
         */
        ~Event() {};
            inline Equation 
        zeroCrossing() { return _zeroCrossing; };
        inline StatementTable 
        positiveHandler() { return _positiveHandler; };
        inline StatementTable 
        negativeHandler() { return _negativeHandler; };
        friend std::ostream& operator<<(std::ostream& out, const Event& e);
      private:
        Equation _zeroCrossing;
        StatementTable _positiveHandler;
        StatementTable _negativeHandler;
    };

    typedef ModelTable<int,Event> EventTable;
  }
}

#endif  /*  MMO_EVENT_H_ */
