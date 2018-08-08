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

#ifndef MMO_GEN_PACKAGE_H_
#define MMO_GEN_PACKAGE_H_

#include <boost/variant/variant.hpp>

#include "../ir/class.h"
#include "../util/compile_flags.h"
#include "writer.h"

namespace MicroModelica 
{
  namespace Generator 
  {
    class Package 
    {
      public:
        Package(IR::Package& package, Util::CompileFlags& flags, WriterPtr writer);
        ~Package();
        void 
        generate();
      private:
        IR::Package            _package;
        Util::CompileFlags     _flags;
        WriterPtr              _writer;
        std::list<std::string> _includes;
    };
  }
}
#endif /* MMO_GEN_PACKAGE_H */
