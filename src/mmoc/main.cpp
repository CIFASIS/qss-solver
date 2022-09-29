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

#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>

#include "ast/ast_types.h"
#include "ast/stored_definition.h"
#include "generator/files.h"
#include "generator/generator.h"
#include <ir/class.h>
#include "ir/mmo_ir.h"
#include "ir/mmo_model_checker.h"
#include "ir/mmo_settings.h"
#include <ast/parser/parse.h>
#include "util/compile_flags.h"
#include "util/error.h"
#include "util/logger.h"
#include "util/symbol_table.h"
#include "util/util.h"
#include "util/util_types.h"

using namespace std;
using namespace MicroModelica::Generator;
using namespace MicroModelica::IR;
using namespace MicroModelica::Util;

void usage()
{
  cout << "Usage mmoc [options] file" << endl;
  cout << "Compile MicroModelica files and generate a C file that implements a model suitable for the Stand--Alone QSS solver." << endl;
  cout << endl;
  cout << "-d <flag>, --debug <flag>" << endl;
  cout << "                Sets the simulation debug <flag>, the debug information is written in the log file generated " << endl;
  cout << "                by the Stand--Alone QSS solver and is an OR combination of the following possibilities: " << endl;
  cout << "                    SD_DBG_All: Enable all the debug flags" << endl;
  cout << "                    SD_DBG_Dt: Log dt parameter changes in parallel simulations" << endl;
  cout << "                    SD_DBG_ExternalEvent: Output step information for each external event in parallel simulations" << endl;
  cout
      << "                    SD_DBG_InitValues: Output the simulation initialization, including initial state and state derivatives values"
      << endl;
  cout << "                    SD_DBG_Memory: Output memory footprint" << endl;
  cout << "                    SD_DBG_Synchronize: Output synchronization information in parallel simulations" << endl;
  cout << "                    SD_DBG_StepInfo: Output for each simulation step:" << endl;
  cout << "                       ** The step time" << endl;
  cout << "                       ** The type and index of the variable that changes" << endl;
  cout << "                    SD_DBG_VarChanges: Output the model's state variable changes and the number of handler executions" << endl;
  cout << "                                       for each event defined in the model" << endl;
  cout << "                    SD_DBG_WaitFor: Output synchronization information in parallel simulations" << endl;
  cout << "                    SD_DBG_Weights: Save the output values generated by the SD_DBG_VarChanges into a binary file" << endl;
  cout << "-e, --external-structure-file <file>" << endl;
  cout << "                Read model incidence matrices from <file>." << endl;
  cout << "-f, --force " << endl;
  cout << "                Force external package compilation." << endl;
  cout << "-h, --help      Display this information and exit" << endl;
  cout << "-i <path>, --include <path>" << endl;
  cout << "                Include <path> in the library path search.          " << endl;
  cout << "-o <file>, --output <file>" << endl;
  cout << "                Sets the output to <file>" << endl;
  cout << "-s, --settings-only" << endl;
  cout << "                Generate only the settings (.ini) file." << endl;
  cout << "-t, --test" << endl;
  cout << "                Don't generate GiNaC expressions for testing code structure." << endl;
  cout << "-v, --version   Display version information and exit" << endl;
  cout << endl;
  cout << "Report bugs to: joaquin.f.fernandez@gmail.com" << endl;
  cout << "MicroModelica C Compiler home page: https://github.com/CIFASIS/qss-solver-mmoc " << endl;
}

void version()
{
  cout << "MicroModelica C Compiler 3.3.0" << endl;
  cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
  cout << "This is free software: you are free to change and redistribute it." << endl;
  cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
}

int parsePackages(AST_StringList imports, CompileFlags& flags, bool recompile)
{
  int r = 0;
  CompileFlags flg;
  AST_StringListIterator it;
  Utils& utils = Utils::instance();
  foreach (it, imports) {
    string i = *current_element(it);
    string p = utils.packagePath(i, flags);
    flags.addObject(utils.generatePath(p, utils.packageName(i) + ".c"));
    if (!utils.searchCompiledPackage(i, flags) || recompile) {
      string file_name = utils.generatePath(p, i + ".mo");
      AST_StoredDefinition sd = nullptr;
      sd = parseFile(file_name, &r);
      Error::instance().setFile(file_name);
      if (r == 0) {
        ModelChecker mmo(file_name);
        r = mmo.apply(sd);
        if (r == 0) {
          flg.setOutputFile(utils.generatePath(p, i));
          flg.setPath(p);
          parsePackages(sd->imports(), flg, recompile);
          list<string> objects = flg.objects();
          for (list<string>::iterator it = objects.begin(); it != objects.end(); it++) {
            flags.addObject(*it);
          }
          Error::instance().setFile(file_name);
          utils.setCompileFlags(flg);
          MicroModelicaIR ir(file_name);
          r = ir.apply(sd);
          if (r == 0) {
            Package package = ir.definition().package();
            utils.setPackageFunctions(package.definitions());
            utils.setPackagePrefix(package.prefix());
            Generator gen(ir.definition(), flg);
            r = gen.generate();
            utils.setPackageFunctions(FunctionTable());
            utils.setPackagePrefix("");
            if (r == 0) {
              string compiled_file_name = utils.generatePath(p, "pkg_" + i + ".moo");
              const bool FULL_PATH = true;
              Option<CompiledPackage> cp = utils.readPackage(compiled_file_name, FULL_PATH, i);
              assert(cp);
              utils.addCompiledFunctions(cp->definitions());
            }
          }
        }
      } else {
        delete sd;
      }
      delete sd;
    } else {
      utils.setCompileFlags(flags);
      Option<CompiledPackage> pd = utils.readPackage(i);
      if (!pd) {
        Error::instance().add(0, EM_CANT_OPEN_FILE, ER_Error, "%s.moo", i.c_str());
        return -1;
      }
      ImportTable objects = pd->objects();

      ImportTable::iterator it;
      for (string import = objects.begin(it); !objects.end(it); import = objects.next(it)) {
        flags.addObject(import);
      }
    }
  }
  return r;
}

int main(int argc, char** argv)
{
  int r = 0;
  int opt;
  extern char* optarg;
  char str_arg[128];
  bool recompile = false;
  bool settings = false;
  CompileFlags flags;
  while (true) {
    static struct option long_options[] = {{"version", no_argument, 0, 'v'},       {"help", no_argument, 0, 'h'},
                                           {"include", required_argument, 0, 'i'}, {"external-structure-file", required_argument, 0, 'e'},
                                           {"force", no_argument, 0, 'f'},         {"settings-only", no_argument, 0, 's'},
                                           {"test", no_argument, 0, 't'},          {"debug", required_argument, 0, 'd'},
                                           {"output", required_argument, 0, 'o'},  {0, 0, 0, 0}};
    int option_index = 0;
    opt = getopt_long(argc, argv, "vhmfsti:e:d:o:", long_options, &option_index);
    if (opt == EOF) break;
    switch (opt) {
    case 'v':
      version();
      exit(0);
    case 'h':
      usage();
      exit(0);
    case 'm':
      flags.setIncidenceMatrices(true);
      break;
    case 'd':
      sscanf(optarg, "%s", str_arg);
      flags.setDebug(str_arg);
      break;
    case 'o':
      flags.setOutputFile(optarg);
      break;
    case 'i':
      flags.addLibraryPath(optarg);
      break;
    case 'e':
      flags.setExternalStructureFile(true);
      break;
    case 'f':
      recompile = true;
      break;
    case 's':
      settings = true;
      break;
    case 't':
      flags.setTesting(true);
      break;
    case '?':
      usage();
      exit(-1);
      break;
    default:
      abort();
    }
  }
  string pkg = Utils::instance().environmentVariable("MMOC_PACKAGES");
  if (!pkg.empty() && !flags.testing()) {
    flags.addLibraryPath(pkg);
  }
  AST_StoredDefinition sd;
  string file_name;
  if (argv[optind] != nullptr) {
    file_name = argv[optind];
    string path = Utils::instance().getFilePath(file_name);
    flags.setPath(path);
    sd = parseFile(file_name, &r);
    Error::instance().setFile(file_name);
    if (r == 0) {
      ModelChecker mmo(file_name);
      r = mmo.apply(sd);
      if (r == 0) {
        if (settings) {
          Settings set(file_name);
          set.apply(sd);
          Files sc(file_name, flags);
          sc.settings(set.annotations());
          return 0;
        }
        Logger::instance().setFile(file_name);
        int res = parsePackages(sd->imports(), flags, recompile);
        if (res != 0) {
          Error::instance().show();
          cout << "Exit code: " << res << endl;
          return res;
        }
        Utils::instance().setCompileFlags(flags);
        Error::instance().setFile(file_name);
        MicroModelicaIR ir(file_name);

        r = ir.apply(sd);
        if (r == 0) {
          Generator gen(ir.definition(), flags);
          r = gen.generate();
        }
      }
    }
  }
  Error::instance().show();
  cout << "Exit code: " << r << endl;
  return r;
}
