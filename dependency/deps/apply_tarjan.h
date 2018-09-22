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
 * apply_tarjan.h
 *
 *  Created on: 21/12/2014
 *      Author: fede
 */

#ifndef APPLY_TARJAN_H_
#define APPLY_TARJAN_H_

#include <causalize/graph/graph_definition.h>
#include <utility>
#include <list>
#include <map>

namespace Causalize {
  struct Component {
    std::list<Vertex> *uVertices;
    std::list<Vertex> *eqVertices;
  };
  typedef Component *ComponentPtr;
  int apply_tarjan(CausalizationGraph &graph, std::map<int, ComponentPtr> &components);
}


#endif /* APPLY_TARJAN_H_ */
