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

#ifndef INDEX_H_
#define INDEX_H_

#include <string>
#include <map>

#include "../ast/ast_types.h"
#include "util_types.h"
#include "../ir/expression.h"

using namespace std;

class IndexDefinition
{
  public:
    IndexDefinition();
    ~IndexDefinition();
  private:
    string _variable;
    MMO_expression _exp;
};

class RangeDefinition
{
  public:
    RangeDefinition();
    ~RangeDefinition();
  private:
    int _init;
    int _end;
    int _step;
};

class Range
{
  public:
    Range();
    ~Range();
    int
    begin(int dim);
    int
    end(int dim);
    void
    setBegin(int v, int d);
    void
    setEnd(int v, int d);
    int
    dimension();
    int
    size(int dim);
    int
    size();
    void
    setIndex(Index *lhs);
    void
    clear();
    bool
    check();
    void
    get(AST_Equation_For eqf, VarSymbolTable vt);
    void
    get(AST_Statement_For stf, VarSymbolTable vt);
    bool
    empty();
    void
    setVariable(string v, int d);
    string
    variable(int dim);
    void
    setLocalVariable(string v, int dim);
    string
    localVariable(string v);
    private:
    map<string,RangeDefinition> _ranges;
}

class Index
{
  public:
    /**
     *
     */
    Index();
    ~Index();
    string
    print() const;
    bool
    hasRange() const;
    Range
    range() const;
    void
    setRange(Range r);
    void
    setMap(MMO_expresion exp);
    bool
    hasMap() const;
    bool
    operator==(const Index &other) const;
    void
    addIndex(IndexDefinition id);
    int
    dimension();
    private:
    map<int,IndexDefinition> _indexes;
};

#endif /* INDEX_H_ */
