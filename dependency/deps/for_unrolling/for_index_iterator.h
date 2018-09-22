/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <ast/ast_types.h>
#include <ast/expression.h>
#include <util/table.h>

namespace Causalize {
class ForIndexIterator {
public:
  virtual bool hasNext() = 0;
  virtual Real next() = 0;
  virtual ~ForIndexIterator() {};
};


class RangeIterator : public ForIndexIterator {
public:
  RangeIterator(Range range, VarSymbolTable symbolTable);
  bool hasNext();
  Real next();
  Real begin(){return _rangeBegin;};
  Real end(){return _rangeEnd;}
private:
  Real eval(Expression exp, VarSymbolTable symbolTable);
  ExpList _rangeElements;
  Real _rangeBegin;
  Real _rangeStep;
  Real _rangeEnd;
  Real _current;
};

class BraceIterator : public ForIndexIterator {
public:
  BraceIterator(Brace braceExp, VarSymbolTable &);
  bool hasNext();
  Real next();
private:
  ExpList _braceExpElements;
  ExpList::iterator _braceExpElementsIter;
  VarSymbolTable &vtable;
};
}
