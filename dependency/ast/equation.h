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


#ifndef AST_EQUATION
#define AST_EQUATION
#include <ast/modification.h>

namespace Modelica {
  namespace AST {
    
    struct EquationBase {
      member_(Option<Comment>,comment);
    };
    struct Equality: public EquationBase {
      Equality(){};
      Equality(Expression l, Expression r);
      member_(Expression,left);
      member_(Expression,right);
      printable(Equality);
      comparable(Equality);

   };

   struct Connect: public EquationBase {
      Connect(){};
      Connect(Reference l, Reference r): left_(l), right_(r){};
      Connect(Expression l, Expression r): left_(l), right_(r){};
      friend std::ostream& operator<<(std::ostream& out, const Connect &e) // output
      {
        out << INDENT << "connect(" << e.left() << " , " << e.right() << ")";
        return out;
      }
      member_(Expression,left);
      member_(Expression,right);
      comparable(Connect);
    };

    struct CallEq: public EquationBase {
      CallEq(){};
      CallEq(Name name, ExpList args): name_(name), args_(args) {}
      friend std::ostream& operator<<(std::ostream& out, const CallEq &e);// output
      member_(Name,name);
      member_(ExpList,args);
      comparable(CallEq);
    };
    struct ForEq;
    struct WhenEq;
    struct IfEq;

    typedef variant<
      Connect,
      Equality,
      CallEq,
      recursive_wrapper<ForEq>,
      recursive_wrapper<IfEq>,
      recursive_wrapper<WhenEq>
    > Equation;
    template<typename T> 
    bool is(Equation e) {
      return e.type()==typeid(T);
    }
 

    typedef std::vector<Equation> EquationList;


    template<typename E>
    struct For {
      For() {};
      For(Indexes range, std::vector<E> els): r(range), els(els) {};

      friend std::ostream& operator<<(std::ostream& out, const For &f) // output
      {
        out << "for " << f.range() << " loop" << std::endl;
        BEGIN_BLOCK;
        foreach_(E e, f.elements()) 
          out << INDENT << e << ";" << std::endl;
        END_BLOCK;
        out << INDENT <<"end for";
        return out;
      }
      bool operator==(const For & other) const {
        return other.range()==range() && other.elements()==elements();
      }
      bool operator!=(const For & other) const {
        return !(other.range()==range() && other.elements()==elements());
      }
      Indexes range() const { return r; }
      Indexes &range_ref() { return r; }
      void set_range(Indexes i) { r=i; }

      std::vector<E>    elements() const { return els; }
      std::vector<E> & elements_ref()  { return els; }
      Indexes r;
      std::vector<E> els;

    };

    template<typename E>
    struct If {
      typedef tuple<Expression,std::vector<E> >  Else;
      typedef std::vector<Else> ElseList;
      
      If() {};
      If(Expression cond, std::vector<E> then, ElseList elseif, std::vector<E> elses): c(cond), els(then), elses(elses), elsesif(elseif){};
      If(Expression cond, std::vector<E> then, ElseList elseif): c(cond), els(then), elsesif(elseif){};
      friend std::ostream& operator<<(std::ostream& out, const If &i) // output
      {
        out << "if " << i.cond() << " then " << std::endl;
        BEGIN_BLOCK;
        foreach_(E e, i.elements())
          out << INDENT << e << ";" << std::endl;
        END_BLOCK;
        foreach_(Else e, i.elseif()) {
          out << INDENT << "elseif " << get<0>(e) << " then " << std::endl;
          BEGIN_BLOCK;
          foreach_(E el, get<1>(e))
            out << INDENT << el << ";" << std::endl;
          END_BLOCK;
        }
        if (i.ifnot().size()) {
          out << INDENT << "else " << std::endl;
          BEGIN_BLOCK;
          foreach_(E e, i.ifnot())
            out << INDENT << e << ";" << std::endl;
          END_BLOCK;
        }
        out << INDENT << "end if";
        return out;
      }


      bool operator==(const If & other) const {
        return (other.cond()==cond() && other.elements()==elements() && other.ifnot()==ifnot() && other.elseif()==elseif());
      }
      bool operator!=(const If & other) const {
        return (other.cond()==cond() && other.elements()==elements() && other.ifnot()==ifnot() && other.elseif()==elseif());
      }
      Expression cond() const { return c; }
      Expression & cond_ref() { return c; }

      std::vector<E> elements() const { return els; }
      std::vector<E> & elements_ref() { return els; }

      std::vector<E> ifnot() const { return elses; }
      std::vector<E> & ifnot_ref() { return elses; }

      ElseList elseif() const { return elsesif; }
      ElseList & elseif_ref() { return elsesif; }

      
  
      Expression c;
      std::vector<E> els;
      std::vector<E> elses;
      ElseList elsesif;

 
    };
    template<typename E>
    struct When {
      typedef tuple<Expression,std::vector<E> >  Else;
      typedef std::vector<Else> ElseList;
      When(){};
      When(Expression cond, std::vector<E> els): c(cond), els(els) {};
      When(Expression cond, std::vector<E> els, ElseList elsew): c(cond), els(els), elsew(elsew) {};

      friend std::ostream& operator<<(std::ostream& out, const When  &w) // output
      {
        out << "when " << w.cond() << " then " << std::endl;
        BEGIN_BLOCK;
        foreach_(E e, w.elements()) 
          out << INDENT <<e << ";" << std::endl;
        END_BLOCK;
        foreach_(Else e, w.elsewhen()) {
          out << INDENT << "elsewhen " << get<0>(e) << " then " << std::endl;
          BEGIN_BLOCK;
          foreach_(E ee, get<1>(e))
            out << INDENT << ee << ";" << std::endl;
          END_BLOCK;
        }
        out << INDENT<< "end when";
        return out;
      }

      bool operator==(const When & other) const {
        return other.cond()==cond() && other.elements()==elements() && other.elsewhen()==elsewhen();
      }

      bool operator!=(const When & other) const {
        return !(other.cond()==cond() && other.elements()==elements() && other.elsewhen()==elsewhen());
      }
      
      Expression cond() const { return c; }
      Expression & cond_ref() { return c; }

      std::vector<E> elements() const { return els; }
      std::vector<E> & elements_ref() { return els; }

      ElseList elsewhen() const { return elsew; }
      ElseList & elsewhen_ref() { return elsew; }
      
  
      Expression c;
      std::vector<E> els;
      ElseList elsew;

    };



    struct ForEq: public For<Equation>, public EquationBase {
      ForEq() {}
      ForEq(Indexes range, EquationList el): For(range,el) {}
    };

    struct WhenEq: public When<Equation> , public EquationBase {
      
      WhenEq(): When() {}
      WhenEq(Expression cond, EquationList el/*, std::vector<tuple<Expression, EquationList> > elsew*/): When(cond,el) {}
      WhenEq(Expression cond, EquationList el, std::vector<tuple<Expression, EquationList> > elsew): When(cond,el,elsew) {}
    };

    struct IfEq: public If<Equation> , public EquationBase {
      IfEq() {}
      IfEq(Expression cond, EquationList el, ElseList elseif, EquationList elses): If(cond,el,elseif,elses) {}
      IfEq(Expression cond, EquationList el, ElseList elseif): If(cond,el,elseif) {}
    };
  
    struct EquationSection {
      EquationSection() {};
      EquationSection(bool ini, EquationList l): equations_(l),initial_(ini) {};
      member_(EquationList, equations);
      member_(bool, initial);
      printable(EquationSection);
      void addEquation(Equation e) { equations_.push_back(e); }
    };
  }
}
#endif
