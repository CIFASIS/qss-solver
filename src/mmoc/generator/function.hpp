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

#pragma once

#include <boost/variant/variant.hpp>

#include <generator/writer.hpp>
#include <ir/class.hpp>
#include <util/compile_flags.hpp>
#include <util/util.hpp>

namespace MicroModelica {
namespace Generator {
class Function {
  public:
  Function(IR::Function& function, Util::CompileFlags& flags, WriterPtr writer);
  ~Function();
  void definition();
  std::string header();
  void setPrefix(std::string prefix);
  void addInclude(std::string include);
  void addInclude(Util::SymbolTable includes);
  Util::SymbolTable functionIncludes() const;

  private:
  void includes();
  void body();
  void localSymbols();
  std::string prototype();
  IR::Function _function;
  Util::CompileFlags _flags;
  WriterPtr _writer;
  std::string _prefix;
  Util::SymbolTable _include;
  std::string _return_variable;
  Util::VarSymbolTable _symbols;
  bool _void_function;
};
}  // namespace Generator
}  // namespace MicroModelica
