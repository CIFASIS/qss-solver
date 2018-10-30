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

#ifndef CONVERT_CONDITION_H_
#define CONVERT_CONDITION_H_

#include "../ast_util.h"

namespace MicroModelica {
  namespace Util {

    class ConvertExpression 
    {
      public:
        ConvertExpression(AST_Expression left, AST_Expression right, MicroModelica::Util::VarSymbolTable& symbols);
        /**
         *
         */
        ~ConvertExpression() {};
        inline std::string 
        get() { return _convert; };
      private:
        AST_Expression_ComponentReference 
        componentReference(AST_Expression exp);
        MicroModelica::Util::Variable 
        variable(AST_Expression_ComponentReference exp);
        bool 
        scalarExpression(AST_Expression exp);
        double 
        scalarValue(AST_Expression exp);
        void 
        convert();
        AST_Expression                       _left;
        AST_Expression                       _right;
        MicroModelica::Util::VarSymbolTable  _symbols;
        std::string                          _convert;
    };

    class ConvertEquation
    {
      public:
        ConvertEquation(AST_Equation equation, MicroModelica::Util::VarSymbolTable& symbols);
        ~ConvertEquation() {};
        inline AST_Equation 
        get() { return _equation; };
      private:
        AST_Equation 
        convert(AST_Equation eq);
        AST_Equation                        _equation;
        MicroModelica::Util::VarSymbolTable _symbols;

    };

    class ConvertStatement 
    {
      public:
        ConvertStatement(AST_Statement statement, MicroModelica::Util::VarSymbolTable& symbols);
        ~ConvertStatement() {};
        inline AST_Statement 
        get() { return _statement; };
      private:
        AST_Statement 
        convert(AST_Statement st);
        AST_StatementList 
        convert(AST_StatementList sts);  
        AST_Statement_ElseList 
        convert(AST_Statement_ElseList stel);
        AST_Statement                       _statement;
        MicroModelica::Util::VarSymbolTable _symbols;
    };

    /**
     *
     */
    class ConvertCondition: public AST_Expression_Visitor<AST_Expression>
    {
      public:
        /**
         *
         */
        ConvertCondition();
        /**
         *
         */
        ~ConvertCondition() {};
        /**
         *
         * @return
         */
        inline MicroModelica::IR::EVENT::Type 
        zeroCrossing() { return _zc; };;
        inline MicroModelica::IR::EVENT::Relation
        zeroCrossingRelation() { return _zcRelation; };
      private:
        AST_Expression
        foldTraverseElement(AST_Expression exp) { return exp; };
        AST_Expression
        foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
        AST_Expression
        foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); };
        MicroModelica::IR::EVENT::Type _zc;
        MicroModelica::IR::EVENT::Relation _zcRelation;
    };
  }
}
#endif  /* CONVERT_CONDITION_H_ */
