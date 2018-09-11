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

#include "index.h"

#include <sstream>

#include "helpers.h"
#include "../ast/expression.h"
#include "../ast/equation.h"
#include "../ast/statement.h"
#include "../util/ast_util.h"
#include "../util/error.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {

    IndexDefinition::IndexDefinition()
    {
    }
     
    std::ostream& operator<<(std::ostream& out, const IndexDefinition& id)
    {
      return out;
    }

    Index::Index() :
      _indexes(),
      _dim(0)
    {
    }

    Index::Index(IndexDefinition id) :
      _indexes(),
      _dim()
    {
      _indexes[_dim++] = id;
    }
     
    string
    Index::print() const 
    {
      return "";
    }
     
    void
    Index::setMap(Expression exp)
    {
    }
     
    bool
    Index::hasMap() const 
    {
      return false;
    }
     
    bool
    Index::operator==(const Index &other) const 
    {
      return false;
    }
     
    void
    Index::add(IndexDefinition id)
    {
    }

    std::ostream& operator<<(std::ostream& out, const Index& i)
    {
      return out;
    }

    RangeDefinition::RangeDefinition(int begin, int end, int step) :
      _begin(begin),
      _end(end),
      _step(step)
    {
    }

    std::ostream& operator<<(std::ostream& out, const RangeDefinition& rd)
    {
      return out;
    }

    Range::Range() :
      _ranges()
    {
    }
    
    Range::Range(AST_Equation_For eqf, VarSymbolTable symbols, RANGE::Type type) :
      _ranges(),
      _size(0),
      _type(type)
    {
      AST_ForIndexList fil = eqf->forIndexList();
      setRangeDefinition(fil, symbols);
    }

    Range::Range(AST_Statement_For stf, VarSymbolTable symbols, RANGE::Type type) :
      _ranges(),
      _size(0),
      _type(type)
    {
      AST_ForIndexList fil = stf->forIndexList();
      setRangeDefinition(fil, symbols);
    }
    void 
    Range::setRangeDefinition(AST_ForIndexList fil, VarSymbolTable symbols)
    {
      AST_ForIndexListIterator filit;
      foreach (filit, fil)
      {
        AST_ForIndex fi = current_element(filit);
        AST_Expression in = fi->in_exp();
        AST_ExpressionList el = in->getAsRange()->expressionList();
        AST_ExpressionListIterator eli;
        EvalInitExp eval(symbols);
        int size = el->size();
        int begin = eval.apply(AST_ListFirst(el));
        int end = eval.apply(AST_ListAt(el, size - 1));
        if(end < begin)
        {
          Error::instance().add(AST_ListFirst(el)->lineNum(), EM_IR | EM_UNKNOWN_ODE, ER_Error, "Wrong equation range.");
        }
        string index = fi->variable()->c_str();
        cout << "Agrega " << index;
        _ranges.insert(index,(size == 2 ? RangeDefinition(begin,end) : RangeDefinition(begin, end, eval.apply(AST_ListAt(el, 1)))));
        Option<RangeDefinition> range = _ranges[index];
        if(range) { _size += range->size(); }
      }
    }

    string 
    Range::end() const 
    {
      stringstream buffer;
      int size = _ranges.size();
      for(int i = 0; i < size; i++)
      {
        buffer << "}" << endl;
      }
      return buffer.str();
    }

    string 
    Range::print() const
    {
      stringstream buffer;
      string block = "";
      RangeDefinitionTable ranges = _ranges;
      RangeDefinitionTable::iterator it;
      for(RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it))
      {
        if(_type == RANGE::For)
        {
          string idx = ranges.key(it);
          buffer << block << "for(" << idx << " = " << r.begin() << "; ";
          buffer << idx << "<=" << r.end() << "; ";
          buffer << idx << "+=" << r.step() << ")" << endl;
          buffer << "{" << endl;
          block += TAB;
        }
      }
      return buffer.str();
    }

    std::ostream& operator<<(std::ostream& out, const Range& r)
    {
      return out << r.print();
    }
  }
}
