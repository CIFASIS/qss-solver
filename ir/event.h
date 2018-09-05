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
          Positive = 0, //!< HND_POSITIVE
          Negative = 1, //!< HND_NEGATIVE
          Zero = 2      //!< HND_ZERO
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
        Event(AST_Expression cond, Util::VarSymbolTable& symbols);
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
        void 
        add(AST_Statement);
        bool 
        compare(AST_Expression zc);
        friend std::ostream& operator<<(std::ostream& out, const Event& e);
        std::string 
        print() const;
      private:
        AST_Expression
        getExpression(AST_Expression zc);
        Equation              _zeroCrossing;
        StatementTable        _positiveHandler;
        StatementTable        _negativeHandler;
        EVENT::Type           _type;
        EVENT::Type           _current;
        EVENT::Relation       _zcRelation;
        Util::VarSymbolTable  _symbols;
        int                   _positiveHandlerId;
        int                   _negativeHandlerId;
    };

    typedef ModelTable<int,Event> EventTable;
  }
}

#endif  /*  MMO_EVENT_H_ */
