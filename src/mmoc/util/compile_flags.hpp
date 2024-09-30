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

#include <list>
#include <map>
#include <string>

#include "util_types.h"

namespace MicroModelica {
namespace Util {

class CompileFlags {
  public:
  CompileFlags();
  ~CompileFlags() = default;
  void addLibraryPath(string l);
  void addObject(string o);
  int debug();
  string enginePath();
  string engineLibs();
  bool externalFunctions();
  bool hasObjects();
  bool hasOutputFile();
  bool incidenceMatrices();
  list<string> libraryPaths();
  list<string> objects();
  bool externalStructureFile();
  bool output();
  string outputFile();
  string outputFileName();
  string outputFilePath();
  string path();
  bool store();
  void setDebug(int s);
  void setDebug(string s);
  void setEngineLibs(string p);
  void setEnginePath(string p);
  void setExternalFunctions(bool s);
  void setExternalLibs(string p);
  void setIncidenceMatrices(bool im);
  void setOutput(bool s);
  void setOutputFile(string outputFile);
  void setExternalStructureFile(bool s);
  void setPath(string p);
  void setStore(bool s);
  string externalLibs();
  int lps() const { return _lps; }
  void setLps(int lps) { _lps = lps; }
  void setTesting(bool testing);
  bool testing();
  
  private:
  bool _store;
  bool _externalStructureFiles;
  bool _output;
  int _debug;
  string _outputFile;
  string _path;
  list<string> _libraryPaths;
  map<string, string> _objects;
  bool _incidenceMatrices;
  bool _externalFunctions;
  int _lps;
  map<string, int> _debugOptions;
  bool _testing;
};
}  // namespace Util
}  // namespace MicroModelica
