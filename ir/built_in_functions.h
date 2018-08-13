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

#ifndef MMO_BUILT_IN_FUNCTIONS_H
#define MMO_BUILT_IN_FUNCTIONS_H 

#include "../util/ast_util.h"
#include "../util/symbol_table.h"
#include "../util/util.h"

namespace MicroModelica {
  namespace IR {

    class BIF
    {
      public:
        /**
         *
         */
        virtual
        ~BIF() {};
        /**
         *
         */
        typedef enum
        {
          NONE,  //!< BIF_NONE
          REINIT,  //!< BIF_REINIT
          TERMINATE, //!< BIF_TERMINATE
          SUM,   //!< BIF_SUM
          PRODUCT,   //!< BIF_PRODUCT
          INNER_PRODUCT,   //!< BIF_INNER_PRODUCT
          MIN,   //!< BIF_MIN
          MAX,   //!< BIF_MAX
          ABS,   //!< BIF_ABS
          SIGN,  //!< BIF_SIGN
          SQRT,  //!< BIF_SQRT
          CEIL,  //!< BIF_CEIL
          FLOOR, //!< BIF_FLOOR
          SIN,   //!< BIF_SIN
          COS,   //!< BIF_COS
          TAN,   //!< BIF_TAN
          ASIN,  //!< BIF_ASIN
          ACOS,  //!< BIF_ACOS
          ATAN,  //!< BIF_ATAN
          ATAN2, //!< BIF_ATAN2
          SINH,  //!< BIF_SINH
          COSH,  //!< BIF_COSH
          TANH,  //!< BIF_TANH
          EXP,   //!< BIF_EXP
          LOG,   //!< BIF_LOG
          LOG10, //!< BIF_LOG10
          PRE,    //!< BIF_PRE
          GQLINK  //!< BIF_GQLINK
        } Function;
        
        /**
         *
         */
        typedef enum
        {
          None, //!< BIV_NONE
          Time, //!< BIV_TIME
          Sum,   //!< BIV_SUM
          Product,   //!< BIV_PRODUCT
          Inner_Product,   //!< BIV_INNER_PRODUCT
          Min,   //!< BIV_MIN
          Max   //!< BIV_MAX
        } Variable;
        list<string>
        generateCode(string variableMap, string variableIndex, list<VariableInterval> variableInterval, int expOrder);
        void
        setSymbolTable(Util::VarSymbolTable vt);
        Util::VarSymbolTable
        symbolTable(Util::VarSymbolTable vt);
        string
        variableName(VariableInterval vin);
        string
        expressionOrderStr(int order, VariableInterval vin);
        bool
        isState(VariableInterval vin);
        void
        setExpressionOrder(int expressionOrder);
        int
        expressionOrder();
        string
        print(Index idx, string variableIndex);
      private:
        bool
        hasStates(list<VariableInterval> variables);
        Index
        index(list<VariableInterval> variables);
        Option<Variable>
        variableInfo(VariableInterval vin);
        virtual string
        reduce(string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates) = 0;
        virtual string
        init(string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates) = 0;
        Util::VarSymbolTable _vt;
        int _expressionOrder;
    };

    class BuiltInFunction: public BIF
    {
      public:
        /**
         *
         */
        ~BuiltInFunction();
      private:
        string
        reduce(string variableMap, string variableIndex, int variableOrder,
            list<VariableInterval> variableInterval, bool hasStates);
        string
        init(string variableMap, string variableIndex,
            list<VariableInterval> variableInterval, bool hasStates);
    };

    class BuiltInSumFunction: public BIF
    {
      public:
        /**
         *
         */
        ~BuiltInSumFunction();
      private:
        string
        reduce(string variableMap, string variableIndex, int variableOrder,
            list<VariableInterval> variableInterval, bool hasStates);
        string
        init(string variableMap, string variableIndex,
            list<VariableInterval> variableInterval, bool hasStates);
    };

    class BuiltInProductFunction: public BIF
    {
      public:
        /**
         *
         */
        ~BuiltInProductFunction();
      private:
        string
        reduce(string variableMap, string variableIndex, int variableOrder,
            list<VariableInterval> variableInterval, bool hasStates);
        string
        init(string variableMap, string variableIndex,
            list<VariableInterval> variableInterval, bool hasStates);
    };

    class BuiltInInnerProductFunction: public BIF
    {
      public:
        /**
         *
         */
        ~BuiltInInnerProductFunction();
      private:
        string
        reduce(string variableMap, string variableIndex, int variableOrder,
            list<VariableInterval> variableInterval, bool hasStates);
        string
        init(string variableMap, string variableIndex,
            list<VariableInterval> variableInterval, bool hasStates);
    };

    class BuiltInMinFunction: public BIF
    {
      public:
        /**
         *
         */
        ~BuiltInMinFunction();
      private:
        string
        reduce(string variableMap, string variableIndex, int variableOrder,
            list<VariableInterval> variableInterval, bool hasStates);
        string
        init(string variableMap, string variableIndex,
            list<VariableInterval> variableInterval, bool hasStates);
    };

    class BuiltInMaxFunction: public BIF
    {
      public:
        /**
         *
         */
        ~BuiltInMaxFunction();
      private:
        string
        reduce(string variableMap, string variableIndex, int variableOrder,
            list<VariableInterval> variableInterval, bool hasStates);
        string
        init(string variableMap, string variableIndex,
            list<VariableInterval> variableInterval, bool hasStates);
    };

  }
}

#endif /* MMO_BUILT_IN_FUNCTIONS_H */
