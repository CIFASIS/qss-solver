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

#include <ir/class.hpp>
#include <util/compile_flags.hpp>
#include "writer.hpp"

namespace MicroModelica {
namespace Generator {
class Package {
  public:
  Package(IR::Package& package, Util::CompileFlags& flags, WriterPtr writer);
  ~Package(){};
  void generate();

  private:
  IR::Package _package;
  Util::CompileFlags _flags;
  WriterPtr _writer;
};
}  // namespace Generator
}  // namespace MicroModelica
