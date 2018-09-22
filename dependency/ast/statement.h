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


#ifndef AST_STATEMENT
#define AST_STATEMENT
#include <ast/equation.h>

namespace Modelica {
  namespace AST {
    
    struct StatementBase {

    };
    struct CallSt: public StatementBase {
      
      CallSt(){};
      CallSt(OptExpList out, Expression name, ExpList args);
      friend std::ostream& operator<<(std::ostream& out, const CallSt &a); // output

      bool operator==(const CallSt & other) const {
        return true;
      }
      bool operator!=(const CallSt & other) const {
        return false;
      }
      member_(OptExpList,out);
      member_(Expression,n);
      member_(ExpList,arg);
    };      
 
    struct Assign: public StatementBase {
     Assign(){};
     Assign(Expression l): left_(l), right_(), rl_(){}
     Assign(Expression l, OptExp r): left_(l), right_(r){ };
     Assign(Expression l, Option<ExpList> rl): left_(l), right_(), rl_(rl){ } ;
     printable(Assign);
     member_(Expression, left);
     member_(OptExp,right);
     member_(Option<ExpList>,rl);
   };      
   struct Break: public StatementBase {
      
      friend std::ostream& operator<<(std::ostream& out, const Break &) // output
      {
        out << "break";
        return out;
      }

      bool operator==(const Break & other) const {
        return true;
      }

      bool operator!=(const Break & other) const {
        return false;
      }

    };
 
    struct Return: public StatementBase {
      
      friend std::ostream& operator<<(std::ostream& out, const Return &) // output
      {
        out << "return";
        return out;
      }

      bool operator==(const Return & other) const {
        return true;
      }
      bool operator!=(const Return & other) const {
        return false;
      }
    };
 
    struct IfSt;
    struct ForSt;
    struct WhenSt;
    struct WhileSt;
    typedef variant<
      Assign,
      Break,
      Return,
      CallSt,
      recursive_wrapper<IfSt>,
      recursive_wrapper<ForSt>,
      recursive_wrapper<WhenSt>,
      recursive_wrapper<WhileSt>
    > Statement;

    typedef std::vector<Statement> StatementList;
    
 
    struct WhenSt: public When<Statement>, public StatementBase {
    
      WhenSt(): When() {}
      WhenSt(Expression cond, StatementList el, ElseList elsew): When(cond,el,elsew) {}
    };

    struct ForSt: public For<Statement>, public StatementBase {
      
      ForSt(): For() {}
      ForSt(Indexes range, StatementList el): For(range,el) {}
    };

    struct IfSt: public If<Statement>, public StatementBase {
      
      IfSt(): If() {}
      IfSt(Expression cond, StatementList el, ElseList elseif, StatementList elses): If(cond,el,elseif,elses) {}
    };

    struct StatementSection {
      StatementSection() {};
      StatementSection(bool ini, StatementList l): statements_(l),initial_(ini) {};
      member_(StatementList , statements);
      member_(bool, initial);
      void addStatement(Statement s) { statements_.push_back(s); }
      printable(StatementSection);
    };
    
    struct WhileSt: public StatementBase {
      WhileSt(){}
      WhileSt(Expression cond, StatementList el): cond_(cond), elements_(el) {}
      member_(Expression, cond);
      member_(StatementList , elements);
      printable(WhileSt);
    };


  }
}
#endif
