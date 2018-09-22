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

#include <mmo/mmo_class.h>
#include <boost/variant/get.hpp>
#include <util/debug.h>
#include <util/ast_visitors/eval_expression.h>

#include <iostream>

namespace Modelica {
  MMO_Class::MMO_Class(){father_ = NULL;}
  MMO_Class::MMO_Class(Class &c): variables_() {
    using namespace boost;
    
    equations_.set_initial(false);
    statements_.set_initial(false);
    initial_eqs_.set_initial(true);
    initial_sts_.set_initial(true);
	father_ = NULL;
	annotation_=c.composition().annotation();
    name_=c.name();
    prefixes_=c.prefixes();
	  if (c.composition().external()) {
      external_ = c.composition().call();
      external_annot_ = c.composition().ext_annot();
      language_ = c.composition().language();
    }
    foreach_(Element ce, c.composition().elements())
		  insertElement(ce);
    foreach_(Element ce, c.composition().elements()) { 
      if (is<Component>(ce)) {
        Component comp = boost::get<Component>(ce);
        foreach_ (Declaration d, comp.declarations()) {
          if (d.conditional()) {
            EvalExpression ev(syms_);
            Real res = Apply(ev,d.conditional().get());
            if (res==0) {
              Name name = d.name();
              variables_.erase(std::find(variables_.begin(),variables_.end(),name));
              syms_.remove(name);
              //WARNING ("Removing conditional declaration of %s", name.c_str());
            }
          }
        }
      }
      

    }
		
    foreach_(CompElement ce, c.composition().comp_elem()) {
      if (is<EquationSection>(ce)) {
        EquationSection eqs = boost::get<EquationSection>(ce);
        foreach_(Equation e, eqs.equations()) {
          if (eqs.initial())
            initial_eqs_.addEquation(e);
          else
            equations_.addEquation(e);
        }
      } else if (is<StatementSection>(ce)) {
        StatementSection sts = boost::get<StatementSection>(ce);
        foreach_(Statement s, sts.statements()) {
          if (sts.initial())
            initial_sts_.addStatement(s);
          else
            statements_.addStatement(s);
        }
      } else if (is<Public>(ce)) {
		Public pub = boost::get<Public>(ce);
        foreach_(Element e, pub) 
			insertElement(e);
	  } else if (is<Protected>(ce)) {
		Protected piv = boost::get<Protected>(ce);
        foreach_(Element e, piv) 
			insertElement(e);
	  }
    }   
     
  }

  std::ostream& operator<<(std::ostream& out, const MMO_Class &c) {
    VarSymbolTable table = c.syms();
    TypeSymbolTable tipos = c.tyTable();
    foreach_(Option<ClassPrefix> cp, c.prefixes())
      out << classPrefix(cp);
    out << c.name() <<  "\n";
    BEGIN_BLOCK;
    
    foreach_(Import n, c.imports()) 
		out << INDENT << n << ";\n";
		
    foreach_(Extends n, c.extends()) 
		out << INDENT << n << ";\n";
		
    foreach_(Name n, c.types()) {
		Option<Type::Type> t = tipos[n];
		if (t) 
			out << INDENT << (t.get()) << std::endl ;
	}
	
    foreach_(Name n, c.variables()) {
      VarInfo vinfo = table[n].get();
      out << INDENT;
      foreach_(Option<TypePrefix> tp, vinfo.prefixes()) 
        out << typePrefix(tp);
      out <<  vinfo.type() << " " << n;
      if (vinfo.indices()) {
        ExpList el = vinfo.indices().get();
        int l=el.size(),i=0;
        out << "[" ;
        foreach_(Expression e, el)
          out << e << (++i<l ? ", " : "");
        out << "]" ;
      }
      if (vinfo.modification())
        out << vinfo.modification().get();
      out <<  ";\n";
    }
    
    out << c.initial_eqs();
    out << c.initial_sts();
    out << c.equations();
    out << c.statements();
    END_BLOCK;
    if (c.external()) {
      out << INDENT << " external";
      if (c.language())
        out << " " << c.language().get() << " ";
      if (c.external()) {
        External e = c.external().get();
        if (e.comp_ref()) 
          out << e.comp_ref().get() << " =";
        out << " " << e.fun() << "(";
        unsigned int i=0;
        foreach_(OptExp oe, e.args()) {
          if (oe)
            out << oe.get() << (++i<e.args().size() ? "," : "");
        }
        out << ")";
        if (c.external_annot())
          out << c.external_annot().get() ;
      }
      out << ";\n";
    }
    if (c.annotation())  
      out << c.annotation().get() << ";\n";
    out << INDENT<<  "end " << c.name() << ";";
    return out;
  }
  
  member_imp(MMO_Class,std::vector<Name>, variables);
  member_imp(MMO_Class,std::vector<Name>,  types);
  member_imp(MMO_Class,EquationSection, equations);
  member_imp(MMO_Class,EquationSection, initial_eqs);

  member_imp(MMO_Class,StatementSection, initial_sts);
  member_imp(MMO_Class,StatementSection, statements);

  member_imp(MMO_Class,Name,name);
  member_imp(MMO_Class,Option<Annotation>,annotation);
  member_imp(MMO_Class,Option<External>,external);
  member_imp(MMO_Class,Option<Annotation>,external_annot);
  member_imp(MMO_Class,Option<String>,language);
 
  member_imp(MMO_Class,VarSymbolTable,syms);
  member_imp(MMO_Class,TypeSymbolTable,tyTable);
  member_imp(MMO_Class,ClassPrefixes,prefixes);
  member_imp(MMO_Class,ExtendList,extends);
  member_imp(MMO_Class,ImportList,imports);
  member_imp(MMO_Class,MMO_Class *,father);
  
  bool MMO_Class::isConnector() 
  {
	  foreach_(Option<ClassPrefix> cp, prefixes()) 
		  if (cp && (cp.get() == connector)) return true;
	  return false;
  }
  
  void MMO_Class::addEquation(Equation e) { equations_.addEquation(e); }
  void MMO_Class::addInitEquation(Equation e) {initial_eqs_.addEquation(e);}
  void MMO_Class::addStatement(Statement e) {statements_.addStatement(e); }
  void MMO_Class::addInitStatement(Statement e) { initial_sts_.addStatement(e); }
  
  void MMO_Class::insertElement(Element e)
  {
	  if (is<Component>(e)) {
		Component comp = boost::get<Component>(e);
		foreach_ (Declaration d, comp.declarations()) {
         variables_.push_back(d.name());
          ExpList exp;
          Option<ExpList> ind;
          if (comp.indices()) exp += comp.indices().get();
          if (d.indices()) exp += d.indices().get();
          if (exp.size() > 0) 
			    ind = exp;		  
          syms_.insert(d.name(),VarInfo(comp.prefixes(),comp.type(),d.comment(),d.modification(),ind,false));
        }
      } else if (is<Extends>(e)) {
		extends_.push_back(boost::get<Extends>(e));		
	  } else if (is<Import>(e)) {
		imports_.push_back(boost::get<Import>(e));	
	  } else if (is<ElemClass>(e)) {		  
		  ElemClass c = boost::get<ElemClass>(e);
		  ClassType ct = c.class_element().get().cl();
		  if (is<Class>(ct)) {
			  Class c = boost::get<Class>(ct);
			  MMO_Class * cl = new MMO_Class(c);
			  cl->set_father(this);
			  Type::Class tt = Type::Class(cl->name(),cl);
		   	  tyTable_.insert(c.name(),tt);
		   	  types_.push_back(c.name());
		  } else if (is<DefClass>(ct)) {
			  DefClass c = boost::get<DefClass>(ct);
			  Type::TypeDef tt = Type::TypeDef(c.prefixes(),c.name(),c.type_prefixes(),c.definition(),c.indices(),c.modification());
			  tyTable_.insert(c.name(),tt);
			  types_.push_back(c.name());
		  } else if (is<EnumClass>(ct)) {
			  EnumClass c = boost::get<EnumClass>(ct);
			  Type::EnumType tt = Type::EnumType(c.prefixes(),c.name(),c.enum_spec());
			  tyTable_.insert(c.name(),tt);
			  types_.push_back(c.name());
		  }
	  }
  }
  
  void MMO_Class::addVar(Name n , VarInfo var)
  {
	  syms_.insert(n,var);
  }
  
  Option<VarInfo> MMO_Class::getVar(Name n)
  {
	  Option<VarInfo>  v = syms_[n];
	  if (v) return v;
	  else if (father_) return father_->getVar(n);
	  else return Option<VarInfo>();
  }
  
  bool MMO_Class::isLocal(Name n)
  {
	  foreach_(Name m, variables()) {
		  if (m == n) return true;
	  }
	  return false;
  }
  
};


