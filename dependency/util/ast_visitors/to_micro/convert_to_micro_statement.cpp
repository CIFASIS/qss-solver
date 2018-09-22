/****************************************************************************

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

#include <util/ast_visitors/to_micro/convert_to_micro_statement.h>
#include <util/ast_visitors/to_micro/convert_to_micro_expression.h>
#include <ast/queries.h>
#include <stdio.h>
#include <sstream>

namespace Modelica {

    using namespace boost;
    ConvertToMicroStatement::ConvertToMicroStatement(MMO_Class &cl, unsigned int &discont): mmo_class(cl), disc_count(discont) {
    };
    Statement ConvertToMicroStatement::operator()(Assign v) const { 
      Expression l=v.left(); 
      Expression r=v.left(); 
      OptExp opt_r=v.right(); 
      ConvertToMicroExpression tom(mmo_class, disc_count, false,true);
      l = Apply(tom, l);
      if (opt_r) 
        opt_r = Apply(tom, opt_r.get());
      v.left_ref() = l; 
      v.right_ref() = opt_r; 
      return v;
    }

    Statement ConvertToMicroStatement::operator()(Break v) const { 
      return v;
    }

    Statement ConvertToMicroStatement::operator()(Return v) const { 
      return v;
    }

    Statement ConvertToMicroStatement::operator()(CallSt v) const { 
      ERROR("Call statement not supported yet");
      return v;
    }

    Statement ConvertToMicroStatement::operator()(IfSt v) const { 
      //ConvertToMicroExpression tom(mmo_class, i, false,true);
      //v.cond_ref() = boost::ApplyThis_visitor(tom, v.cond_ref());
      if (is<Reference>(v.cond())) {
        v.cond_ref() = BinOp(v.cond(), Greater, 0.5);
      } 
      foreach_(Statement &s, v.elements_ref()) 
        s=ApplyThis(s);
      foreach_(Statement &s, v.ifnot_ref()) 
        s=ApplyThis(s);
      foreach_(IfSt::Else &es, v.elseif_ref()) {
        foreach_(Statement &s, get<1>(es)) 
          s=ApplyThis(s);
        es=IfSt::Else(get<0>(es),get<1>(es));
      }
      return v;
    }

    Statement ConvertToMicroStatement::operator()(ForSt v) const { 
      ERROR("For statement not supported yet");
      return v;
    }

    Statement ConvertToMicroStatement::operator()(WhenSt v) const { 
      if (is<Call>(v.cond())) {
        Call c = get<Call>(v.cond());
        if ("sample"==c.name()) {
            Expression d=newDiscrete(c.args().front());
            v.cond_ref() = BinOp(Reference("time"),Greater,Call("pre",ExpList(1,d)));
            v.elements_ref().insert(v.elements_ref().begin(), Assign(d,OptExp(BinOp(Reference("time"),Add, c.args().at(1)))));
        }
      } else if (is<Reference>(v.cond())) {
        v.cond_ref() = BinOp(v.cond(), Greater, 0.5);
      }
      if (!isRelation(v.cond())) {
        std::cerr << v.cond();
        WARNING("Condition in when is not a relational");
      }
      foreach_(Statement &s, v.elements_ref()) 
        s=ApplyThis(s);
      return v;
    }

    Statement ConvertToMicroStatement::operator()(WhileSt v) const { 
      ERROR("While statement not supported yet");
      return v;
    }

    StatementList ConvertToMicroStatement::statements() const { return statements_; }

    Expression ConvertToMicroStatement::newDiscrete(Option<Expression> oe) const {
        std::stringstream disc_name;
        disc_name << "d" << disc_count++;
        Name name(disc_name.str());
        VarSymbolTable &syms=mmo_class.syms_ref();     
        if (oe) {
          ClassModification cm;
          cm.push_back(ElMod("start",ModEq(oe.get())));
          ModClass mc(cm);
          Modification m = mc;
          syms.insert(name,VarInfo(TypePrefixes(1,discrete) , "Real", Option<Comment>(), m));
        }  else
          syms.insert(name,VarInfo(TypePrefixes(1,discrete) , "Real"));
        mmo_class.variables_ref().push_back(name);
        return Reference(Ref(1,RefTuple(name,ExpList())));
    }
 
}
