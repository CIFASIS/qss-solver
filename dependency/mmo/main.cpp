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

#include <iostream>
#include <parser/parser.h>
#include <boost/variant/get.hpp>
#include <mmo/mmo_class.h>
#include <util/table.h>
#include <util/debug.h>
#include <util/ast_visitors/to_micro/convert_to_micro.h>
#include <cstdlib>


int main(int argc, char ** argv)
{
  using namespace std;
  using namespace Modelica::AST;
  using namespace Modelica::Parser;
  using namespace Modelica;
  using namespace boost;
  bool ret;
  int opt;
  StoredDef sd;

  while ((opt = getopt(argc, argv, "d")) != -1) {
    switch (opt) {
     case 'd':
       if (optarg != NULL && isDebugParam(optarg)) {
         debugInit(optarg);
       } else {
         ERROR("command-line option d has no arguments\n");
       }
       break;
    }
  }


  if (argv[optind]!=NULL) 
    sd=ParseFile(argv[optind],ret);
  else
    sd=ParseFile("",ret);
 
  if (!ret) 
    return -1;


  if (ret) {
	  MMO_Class mmo(get<Class>(sd.classes().front()));
    ConvertToMicro tm(mmo);
    tm.convert();
	  std::cout << mmo << std::endl;
  } else std::cout << "Error parser" <<  std::endl;
  return 0;
}
