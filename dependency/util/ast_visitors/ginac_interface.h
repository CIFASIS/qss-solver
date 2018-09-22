
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

#ifndef GINAC_INTERFACE
#define GINAC_INTERFACE
#include <map>
#include <vector>
#include <string>
#include <ginac/ginac.h>
#include <ginac/flags.h>
#include <boost/variant/static_visitor.hpp>
#include <ast/expression.h>
#include <util/table.h>

DECLARE_FUNCTION_2P(var)
DECLARE_FUNCTION_1P(der)
DECLARE_FUNCTION_1P(pre)

namespace Modelica {
  using namespace Modelica::AST;
  Expression ConvertToExp(GiNaC::ex e); 
  class ConvertToGiNaC: public boost::static_visitor<GiNaC::ex> {
  public:
  ConvertToGiNaC(VarSymbolTable  &varEnv,bool forDerivation=false);
  ::GiNaC::ex operator()(Integer v) const;
  ::GiNaC::ex operator()(Boolean v) const;
  ::GiNaC::ex operator()(String v) const;
  ::GiNaC::ex operator()(Name v) const;
  ::GiNaC::ex operator()(Real v) const;
  ::GiNaC::ex operator()(SubEnd v) const;
  ::GiNaC::ex operator()(SubAll v) const;
  ::GiNaC::ex operator()(BinOp) const;
  ::GiNaC::ex operator()(UnaryOp) const;
  ::GiNaC::ex operator()(Brace) const;
  ::GiNaC::ex operator()(Bracket) const;
  ::GiNaC::ex operator()(Call) const;
  ::GiNaC::ex operator()(FunctionExp) const;
  ::GiNaC::ex operator()(ForExp) const;
  ::GiNaC::ex operator()(IfExp) const;
  ::GiNaC::ex operator()(Named) const;
  ::GiNaC::ex operator()(Output) const;
  ::GiNaC::ex operator()(Reference) const;
  ::GiNaC::ex operator()(Range) const;

  ::GiNaC::symbol& getSymbol(Modelica::AST::Name) const;
  ::GiNaC::symbol& getSymbol(Modelica::AST::Call);
  ::GiNaC::symbol& getTime() const;
  private:
    mutable std::map<std::string, ::GiNaC::symbol> directory;
    VarSymbolTable  &varEnv;
    bool _forDerivation;
  };
}
#endif
