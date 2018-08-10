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

#ifndef MMO_GEN_FUNCTION_H_
#define MMO_GEN_FUNCTION_H_

#include <boost/variant/variant.hpp>

#include "../ir/class.h"
#include "../util/compile_flags.h"
#include "writer.h"

namespace MicroModelica 
{
  namespace Generator 
  {
    class Function 
    {
      public:
        Function(IR::Function& function, Util::CompileFlags& flags, WriterPtr writer);
        ~Function();
        void 
        definition();
        std::string 
        header();
        void 
        setPrefix(std::string prefix);
        void 
        addInclude(std::string include);
        void 
        addInclude(std::list<std::string> includes);
      private:
        void 
        macros();
        void 
        includes();
        void 
        body();
        void 
        localSymbols();
        std::string 
        prototype();
        IR::Function        _function;
        Util::CompileFlags  _flags;
        WriterPtr           _writer;
        std::string         _prefix;
        Util::ImportTable   _include;
        std::string         _returnVariable;
    };
  }
}
#endif /* MMO_GEN_FUNCTION_H */
