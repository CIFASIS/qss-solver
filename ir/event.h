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
        Event(AST_Expression cond, int id, int offset, Util::VarSymbolTable& symbols, Option<Range> range);
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
        std::string 
        handler(EVENT::Type type) const;
        void 
        add(AST_Statement);
        bool 
        compare(AST_Expression zc);
        inline bool 
        hasRange() { return _range.is_initialized(); }; 
        std::string 
        macro() const;
        int 
        id() const { return _id; };
        /**
         * @brief      This method returns the range expression of the event.
         *              
         *             The range is computed from the LHS of the zero crossing
         *             function associated with the event. 
         *            
         * @return     Expression with the LHS used in graph builders.
         */
        Expression 
        exp();
        bool
        isValid() const { return _zeroCrossing.isValid(); };
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
        Option<Range>         _range;
        int                   _positiveHandlerId;
        int                   _negativeHandlerId;
        int                   _id;
        int                   _offset;
    };

    typedef ModelTable<int,Event> EventTable;
  }
}

#endif  /*  MMO_EVENT_H_ */
