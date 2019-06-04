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

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include "stored_definition.h"

namespace MicroModelica {
  namespace IR {

    StoredDefinition::StoredDefinition() :
      _def(),
      _functions()
    {
    }

    void 
    StoredDefinition::setModel(string name)
    {
      _def = Model(name);
    }

    void 
    StoredDefinition::setPackage(string name)
    {
      _def = Package(name);
    }

    void 
    StoredDefinition::addFunction(Function& f)
    {
      _functions.insert(f.name(), f);
    }

    Option<Function> 
    StoredDefinition::function(string name) 
    {
      Option<Function> f = _functions[name];
      return f;
    }

    Model& 
    StoredDefinition::model() 
    {
      return boost::get<Model>(_def);
    }

    Package&
    StoredDefinition::package()
    {
      return boost::get<Package>(_def);
    }

    bool 
    StoredDefinition::isModel()
    {
      return _def.type() == typeid(Model);
    }
    
    bool 
    StoredDefinition::isPackage()
    {
      return _def.type() == typeid(Package);
    }

    void 
    StoredDefinition::postProcess()
    {
      if(isModel())
      {
        boost::get<Model>(_def).setEquations();
        boost::get<Model>(_def).setEvents();
        boost::get<Model>(_def).setOutputs();
        boost::get<Model>(_def).setCalledFunctions(_functions);
        boost::get<Model>(_def).setInputs();
        boost::get<Model>(_def).computeDependencies();
      }
      else 
      {
        boost::get<Package>(_def).setFunctions(_functions);
      }
    }

  }
}