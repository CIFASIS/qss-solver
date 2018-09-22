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

/*
 * collect_unknowns.cpp
 *
 *  Created on: 29/04/2013
 *      Author: fede
 */

#include <ast/queries.h>
#include <causalize/unknowns_collector.h>
#include <util/ast_visitors/eval_expression.h>
#include <util/table.h>
#include <util/type.h>
#include <util/debug.h>
#include <iostream>

UnknownsCollector::UnknownsCollector(MMO_Class &c): _c(c), _finder(c) {

}


ExpList UnknownsCollector::collectUnknowns() {
  ExpList _unknowns;
  _finder.findStateVariables();
  foreach_(VarSymbolTable::table_type::value_type val, _c.syms_ref()) {
    VarInfo varInfo = val.second;
    Name name = val.first;
		if (!varInfo.builtin() && !isConstant(name,_c.syms_ref()) && !isDiscrete(name, _c.syms_ref()) && !isParameter(name,_c.syms_ref())) {
      Option<Type::Type> opt_type =  _c.tyTable_ref()[varInfo.type()];
      ERROR_UNLESS((bool)opt_type, "No %s type found", varInfo.type().c_str());
      Type::Type type = opt_type.get();
      if (is<Type::Real>(type)) {
        if (varInfo.state()) {
            if (!varInfo.indices())
              _unknowns.push_back(Call("der",ExpList(1,Reference(Ref(1,RefTuple(name,ExpList(0)))))));
            else if (varInfo.indices().get().size()==1) {
              EvalExpression ev(_c.syms_ref());
              Expression lim=varInfo.indices().get().front();
              const int limit=Apply(ev,lim);
              for (int i=1;i<=limit;i++)
                _unknowns.push_back(Call("der",ExpList(1,Reference(Ref(1,RefTuple(name,ExpList(1,Integer(i))))))));
            } else {
              ERROR("Multidimensional variables not supported yet");
            } 
        } else {
            if (!varInfo.indices())
              _unknowns.push_back(Reference(Ref(1,RefTuple(name,ExpList(0)))));
            else if (varInfo.indices().get().size()==1) {
              EvalExpression ev(_c.syms_ref());
              Expression lim=varInfo.indices().get().front();
              const int limit=Apply(ev,lim);
              for (int i=1;i<=limit;i++) 
                _unknowns.push_back(Reference(Ref(1,RefTuple(name,ExpList(1,Integer(i))))));
            } else {
              ERROR("Multidimensional variables not supported yet");
            }
        }

      } else if (is<Type::Array>(type)) {
        ERROR("No vectorial support yet!");
      }
    } 
  }
	return _unknowns;
}


