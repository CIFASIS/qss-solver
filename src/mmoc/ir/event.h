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

namespace EVENT {

typedef enum {
  Zero = 0,      //!< HND_ZERO
  Positive = 1,  //!< HND_POSITIVE
  Negative = 2   //!< HND_NEGATIVE
} Type;

/**
 * Define the original relation type of the zero-crossing function
 * needed by the initial algorithm.
 */
typedef enum {
  LT,  //!< ZC_LT
  LE,  //!< ZC_LE
  GT,  //!< ZC_GT
  GE   //!< ZC_GE
} Relation;
}  // namespace EVENT

class Event {
  public:
  Event();
  Event(AST_Expression cond, int id, int offset, Option<Range> range);
  ~Event() = default;

  inline Equation zeroCrossing() { return _zero_crossing; };
  inline StatementTable positiveHandler() { return _positive_handler; };
  inline StatementTable negativeHandler() { return _negative_handler; };
  std::string handler(EVENT::Type type) const;
  void add(AST_Statement);
  bool compare(AST_Expression zc);
  inline bool hasRange() { return _range.is_initialized(); };
  std::string macro() const;
  int id() const { return _id; };
  /**
   * @brief      This method returns the range expression of the event.
   *
   *             The range is computed from the LHS of the zero crossing
   *             function associated with the event.
   *
   * @return     Expression with the LHS used in graph builders.
   */
  Expression exp();
  bool isValid() const { return _zero_crossing.isValid(); };
  std::string config() const;
  inline Option<Range> range() const { return _range; };

  private:
  AST_Expression getExpression(AST_Expression zc);
  Equation _zero_crossing;
  StatementTable _positive_handler;
  StatementTable _negative_handler;
  EVENT::Type _type;
  EVENT::Type _current;
  EVENT::Relation _zc_relation;
  Option<Range> _range;
  int _positive_handler_id;
  int _negative_handler_id;
  int _id;
  int _offset;
};

typedef ModelTable<int, Event> EventTable;
}  // namespace IR
}  // namespace MicroModelica

#endif /*  MMO_EVENT_H_ */
