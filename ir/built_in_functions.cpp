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

#include <sstream>

#include "built_in_functions.h"
#include "../util/error.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {

    list<string>
    BIF::generateCode(string variableMap, string variableIndex, IndexTable indexes, int expOrder)
    {
     /* stringstream buffer;
      list<string> code;
      bool states = hasStates(variableInterval);
      Index idx = index(variableInterval);
      if(states)
      {
        buffer << "for(" << variableIndex << " = 0;" << variableIndex << " < "
            << expOrder << "; " << variableIndex << "++)";
        code.push_back(buffer.str());
        buffer.str("");
        code.push_back("{");
        code.push_back(init(variableMap, variableIndex, variableInterval, states));
        code.push_back("}");
        buffer << "for(" << variableIndex << " = " << idx.begin() << "; "
            << variableIndex << " <= " << idx.end() << "; " << variableIndex
            << "++)";
        code.push_back(buffer.str());
        buffer.str("");
        code.push_back("{");
        for(int j = 0; j < expOrder; j++)
        {
          code.push_back(reduce(variableMap, variableIndex, j, variableInterval, states));
        }
        code.push_back("}");

      }
      else
      {
        code.push_back(_init(variableMap, variableIndex, variableInterval, states));
        buffer << "for(" << variableIndex << " = " << idx.begin() << "; "
            << variableIndex << " <= " << idx.end() << "; " << variableIndex
            << "++)";
        code.push_back(buffer.str());
        buffer.str("");
        code.push_back("{");
        code.push_back(
            _reduce(variableMap, variableIndex, 0, variableInterval, states));
        code.push_back("}");
      }*/
      list<string> code;
      return code;
    }

    bool
    BIF::hasStates(IndexTable variables)
    {
     /* IndexTable::iterator it;
      for(it = variables.begin(); it != variables.end(); it++)
      {
        Variable vi = _symbols->lookup(it->name());
        if(vi == NULL)
        {
          Error::instance().add(0,
          EM_IR | EM_VARIABLE_NOT_FOUND,
              ER_Fatal, "%s", it->name().c_str());
          return false;
        }
        if(vi->isState() || vi->isAlgebraic())
        {
          return true;
        }
      }*/
      return false;
    }

    void
    BIF::setSymbolTable(VarSymbolTable symbols)
    {
      _symbols = symbols;
    }

    Index
    BIF::index(IndexTable indexes)
    {
      //IndexTable vi = variables.front();
      return Index();
    }

    string
    BIF::variableName(IndexTable indexes)
    {
     /* Variable vi = _variableInfo(vin);
      string varStr = _symbols->getTypePrefix();
      if(vi->isState())
      {
        varStr += "x";
      }
      else if(vi->isAlgebraic())
      {
        varStr += "alg";
      }
      else if(vi->isDiscrete())
      {
        varStr += "d";
      }
      else
      {
        varStr = "__PAR_" + vi->name();
      }*/
      return "";
    }

    string
    BIF::expressionOrderStr(int order, IndexTable vin)
    {
    /*  Variable vi = _variableInfo(vin);
      stringstream expOrder;
      if(_symbols->printEnvironment() != VST_CLASSIC_MODEL_FUNCTIONS)
      {
        expOrder << " * " << _expressionOrder << " + " << order;
      }
      string varStr = _symbols->getTypePrefix() + "x";
      if(vi->isAlgebraic())
      {
        if(_symbols->printEnvironment() == VST_CLASSIC_MODEL_FUNCTIONS)
        {
          expOrder << " * " << _expressionOrder << " + " << order;
        }
        varStr = _symbols->getTypePrefix() + "alg";
      }
      return expOrder.str();*/
      return "";
    }

    VarSymbolTable
    BIF::symbolTable(VarSymbolTable vt)
    {
      return _symbols;
    }

    string
    BIF::print(Index idx, string variableIndex)
    {
      //return idx.print(variableIndex, 0, false);
      return "";
    }

    BuiltInFunction::~BuiltInFunction()
    {
    }

    string
    BuiltInFunction::reduce(string variableMap, string variableIndex, int variableOrder, IndexTable indexes, bool hasStates)
    {
      return "";
    }

    string
    BuiltInFunction::init(string variableMap, string variableIndex, IndexTable indexes, bool hasStates)
    {
      return "";
    }

    BuiltInSumFunction::~BuiltInSumFunction()
    {
    }

    string
    BuiltInSumFunction::reduce(string variableMap, string variableIndex, int variableOrder, IndexTable indexes, bool hasStates)
    {
     /* IndexTable vin = variableInterval.front();
      string variablePrefix = BIF::variableName(vin);
      stringstream buffer;
      Index idx = vin.index();
      string expOrderStr = BIF::expressionOrderStr(variableOrder, vin);
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableOrder << "] += "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrderStr << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] += " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInSumFunction::init(string variableMap, string variableIndex, IndexTable indexes, bool hasStates)
    {
      stringstream buffer;
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = 0;";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = 0;";
      }
      return buffer.str();
    }

    BuiltInProductFunction::~BuiltInProductFunction()
    {
    }

    string
    BuiltInProductFunction::reduce(string variableMap, string variableIndex, int variableOrder, IndexTable indexes, bool hasStates)
    {
    /*  IndexTable vin = variableInterval.front();
      string variablePrefix = BIF::variableName(vin);
      stringstream buffer;
      Index idx = vin.index();
      string expOrderStr = BIF::expressionOrderStr(variableOrder, vin);
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableOrder << "] *= "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrderStr << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] *= " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInProductFunction::init(string variableMap, string variableIndex, IndexTable indexes, bool hasStates)
    {
      stringstream buffer;
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = 0;";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = 0;";
      }
      return buffer.str();
    }

    BuiltInInnerProductFunction::~BuiltInInnerProductFunction()
    {
    }

    string
    BuiltInInnerProductFunction::reduce(string variableMap, string variableIndex, int variableOrder, IndexTable indexes, bool hasStates)
    {
     /* if(variableInterval.size() != 2)
      {
        Error::instance().add(0,
        EM_IR | EM_ARGUMENTS,
            ER_Fatal, "");
      }
      IndexTable vin[2];
      IndexTable::iterator it;
      int i = 0;
      for(it = variableInterval.begin(); it != variableInterval.end(); it++)
      {
        vin[i++] = *it;
      }
      Index idx0 = vin[0].index();
      Index idx1 = vin[1].index();
      stringstream buffer;
      string variable0Prefix = BIF::variableName(vin[0]);
      string variable1Prefix = BIF::variableName(vin[1]);
      if(hasStates)
      {
        if(BIF::isState(vin[0]) && BIF::isState(vin[1]))
        {
          if(variableOrder == 0)
          {
            string expOrderStr = BIF::expressionOrderStr(variableOrder, vin[0]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrderStr
                << "] * " << variable1Prefix << "[(" << print(idx1, variableIndex)
                << ")" << expOrderStr << "];";
          }
          else if(variableOrder == 1)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(variableOrder, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(variableOrder, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << " ]* " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << " ] + "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder00Str << " ] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "];";
          }
          else if(variableOrder == 2)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(1, vin[0]);
            string expOrder02Str = BIF::expressionOrderStr(2, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(1, vin[1]);
            string expOrder12Str = BIF::expressionOrderStr(2, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder02Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << "] + 2 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << " ] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "] + "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder00Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder12Str << "];";
          }
          else if(variableOrder == 3)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(1, vin[0]);
            string expOrder02Str = BIF::expressionOrderStr(2, vin[0]);
            string expOrder03Str = BIF::expressionOrderStr(3, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(1, vin[1]);
            string expOrder12Str = BIF::expressionOrderStr(2, vin[1]);
            string expOrder13Str = BIF::expressionOrderStr(3, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder03Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << "] + 3 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder02Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "] + 3 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder12Str << "] + "
                << variable0Prefix
                << "[(" << print(idx0, variableIndex) << ")" << expOrder00Str
                << "] * " << variable1Prefix << "[(" << print(idx1, variableIndex)
                << ")"
                << expOrder13Str << "];";
          }
          else if(variableOrder == 4)
          {
            string expOrder00Str = BIF::expressionOrderStr(0, vin[0]);
            string expOrder01Str = BIF::expressionOrderStr(1, vin[0]);
            string expOrder02Str = BIF::expressionOrderStr(2, vin[0]);
            string expOrder03Str = BIF::expressionOrderStr(3, vin[0]);
            string expOrder04Str = BIF::expressionOrderStr(4, vin[0]);
            string expOrder10Str = BIF::expressionOrderStr(0, vin[1]);
            string expOrder11Str = BIF::expressionOrderStr(1, vin[1]);
            string expOrder12Str = BIF::expressionOrderStr(2, vin[1]);
            string expOrder13Str = BIF::expressionOrderStr(3, vin[1]);
            string expOrder14Str = BIF::expressionOrderStr(4, vin[1]);
            buffer << "\t" << variableMap << "[" << variableOrder << "] += "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder04Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder10Str << "] + 4 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder03Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder11Str << "] + 6 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder02Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder12Str << "] + 4 * "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder01Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder13Str << "] + "
                << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
                << expOrder00Str << "] * " << variable1Prefix << "[("
                << print(idx1, variableIndex) << ")" << expOrder14Str << "];";
          }
        }
        else if(BIF::isState(vin[0]) && !BIF::isState(vin[1]))
        {
          string expOrderStr = BIF::expressionOrderStr(variableOrder, vin[0]);
          buffer << "\t" << variableMap << "[" << variableOrder << "] += "
              << variable0Prefix << "[(" << print(idx0, variableIndex) << ")"
              << expOrderStr
              << "] * " << variable1Prefix << "[" << print(idx1, variableIndex)
              << "];";

        }
        else if(!BIF::isState(vin[0]) && BIF::isState(vin[1]))
        {
          string expOrderStr = BIF::expressionOrderStr(variableOrder, vin[1]);
          buffer << "\t" << variableMap << "[" << variableOrder << "] += "
              << variable0Prefix << "[" << print(idx0, variableIndex) << "]" << "*"
              << variable1Prefix << "[(" << print(idx1, variableIndex) << ")"
              << expOrderStr << "];";
        }
      }
      else
      {
        buffer << "\t" << variableMap << "[0] += " << variable0Prefix << "["
            << print(idx0, variableIndex) << "] * " << variable1Prefix << "["
            << print(idx1, variableIndex) << "];";
      }
      return buffer.str();*/
        return "";
    }

    string
    BuiltInInnerProductFunction::init(string variableMap, string variableIndex, IndexTable indexes, bool hasStates)
    {
      stringstream buffer;
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = 0;";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = 0;";
      }
      return buffer.str();
    }

    BuiltInMinFunction::~BuiltInMinFunction()
    {
    }

    string
    BuiltInMinFunction::reduce(string variableMap, string variableIndex, int variableOrder, IndexTable indexes, bool hasStates)
    {
      /*stringstream buffer;
      IndexTable vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t if (" << variableMap << "[" << variableOrder << "] > "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "])"
            << endl;
        buffer << "\t \t" << variableMap << "[" << variableOrder << "] = "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "];";
      }
      else
      {
        buffer << "\t if (" << variableMap << "[0] > " << variablePrefix << "["
            << print(idx, variableIndex) << "])" << endl;
        buffer << "\t \t" << variableMap << "[0] = " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInMinFunction::init(string variableMap, string variableIndex, IndexTable indexes, bool hasStates)
    {
      /*stringstream buffer;
      IndexTable vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = "
            << variablePrefix << "[" << idx.offset() << expOrder << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = " << variablePrefix << "[0];";
      }
      return buffer.str();*/
      return "";
    }

    BuiltInMaxFunction::~BuiltInMaxFunction()
    {
    }

    string
    BuiltInMaxFunction::reduce(string variableMap, string variableIndex, int variableOrder, IndexTable indexes, bool hasStates)
    {
      /*stringstream buffer;
      IndexTable vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t if (" << variableMap << "[" << variableOrder << "] < "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "])"
            << endl;
        buffer << "\t \t" << variableMap << "[" << variableOrder << "] = "
            << variablePrefix << "[(" << print(idx, variableIndex) << ")"
            << expOrder << "];";
      }
      else
      {
        buffer << "\t if (" << variableMap << "[0] < " << variablePrefix << "["
            << print(idx, variableIndex) << "])" << endl;
        buffer << "\t \t" << variableMap << "[0] = " << variablePrefix << "["
            << print(idx, variableIndex) << "];";
      }
      return buffer.str();*/
      return "";
    }

    string
    BuiltInMaxFunction::init(string variableMap, string variableIndex, IndexTable indexes, bool hasStates)
    {
     /* stringstream buffer;
      IndexTable vin = variableInterval.front();
      string expOrder = BIF::expressionOrderStr(0, vin);
      string variablePrefix = BIF::variableName(vin);
      Index idx = vin.index();
      if(hasStates)
      {
        buffer << "\t" << variableMap << "[" << variableIndex << "] = "
            << variablePrefix << "[" << idx.offset() << expOrder << "];";
      }
      else
      {
        buffer << "\t" << variableMap << "[0] = " << variablePrefix << "[0];";
      }
      return buffer.str();
      */
      return "";
    }

    void
    BIF::setExpressionOrder(int expressionOrder)
    {
      _expressionOrder = expressionOrder;
    }

    int
    BIF::expressionOrder()
    {
      return _expressionOrder;
    }

    Option<Variable>
    BIF::variableInfo(IndexTable vin)
    {
      Option<Variable> vi = _symbols[""];
      if(!vi)
      {
        Error::instance().add(0, EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "%s", "Fix Variable interval name");
      }
      return vi;
    }

    bool
    BIF::isState(IndexTable vin)
    {
      Option<Variable> vi = variableInfo(vin);
      return vi->isState() || vi->isAlgebraic();
    }

  }
}
