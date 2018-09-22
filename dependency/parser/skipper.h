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


#ifndef SKIPPER_PARSER
#define SKIPPER_PARSER

namespace Modelica { namespace Parser {
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
template <typename Iterator>
struct Skipper : qi::grammar<Iterator> {
  Skipper() : Skipper::base_type(start) {
    using qi::char_;
    ascii::space_type space;

    start = space                               // tab/space/cr/lf
          |   "//" >> *(char_ - "\n") >> "\n"     // C++-style comments
          |   "/*" >> *(char_ - "*/") >> "*/"     // C-style comments
          ;
    }
    qi::rule<Iterator> start;
};
 template <typename T>
  std::vector<T> consume_one(T t) {
    std::vector<T> l(1,t);
    return l;
  }
 

  template <typename T>
  std::vector<T> append_list(std::vector<T> lt, std::vector<T> lt2) {
    lt.insert(lt.end(),lt2.begin(), lt2.end());
    return lt;
  }


}
}
#endif
