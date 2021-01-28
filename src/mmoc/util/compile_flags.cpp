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

#include "compile_flags.h"

#include <utility>

#include "util.h"

namespace MicroModelica {
namespace Util {

CompileFlags::CompileFlags()
    : _store(true),
      _parallel(false),
      _externalStructureFiles(false),
      _output(true),
      _debug(0),
      _outputFile(),
      _path(),
      _libraryPaths(),
      _objects(),
      _optimizeQSS(false),
      _incidenceMatrices(false),
      _externalFunctions(false),
      _lps(0),
      _debugOptions(),
      _graph(false)
{
  _debugOptions["SD_DBG_VarChanges"] = 1 << 0;
  _debugOptions["SD_DBG_InitValues"] = 1 << 1;
  _debugOptions["SD_DBG_StepInfo"] = 1 << 2;
  _debugOptions["SD_DBG_Weights"] = 1 << 3;
  _debugOptions["SD_DBG_Memory"] = 1 << 4;
  _debugOptions["SD_DBG_ExternalEvent"] = 1 << 5;
  _debugOptions["SD_DBG_Synchronize"] = 1 << 6;
  _debugOptions["SD_DBG_WaitFor"] = 1 << 7;
  _debugOptions["SD_DBG_Dt"] = 1 << 8;
}

bool CompileFlags::store() { return _store; }

void CompileFlags::setStore(bool s) { _store = s; }

void CompileFlags::setIncidenceMatrices(bool im) { _incidenceMatrices = im; }

void CompileFlags::setOptimizeQSS(bool s) { _optimizeQSS = s; }

bool CompileFlags::incidenceMatrices() { return _incidenceMatrices; }

bool CompileFlags::parallel() { return _parallel; }

void CompileFlags::setParallel(bool s)
{
  _parallel = s;
  setGraph(true);
}

bool CompileFlags::output() { return _output; }

void CompileFlags::setOutput(bool s) { _output = s; }

bool CompileFlags::externalFunctions() { return _externalFunctions; }

void CompileFlags::setExternalFunctions(bool s) { _externalFunctions = s; }

int CompileFlags::debug() { return _debug; }

void CompileFlags::setDebug(int s) { _debug |= s; }

void CompileFlags::setOutputFile(string outputFile) { _outputFile = outputFile; }

string CompileFlags::outputFileName() { return Utils::instance().getFileName(_outputFile); }

string CompileFlags::outputFilePath() { return Utils::instance().getFilePath(_outputFile); }

string CompileFlags::outputFile() { return _outputFile; }

bool CompileFlags::hasOutputFile() { return !_outputFile.empty(); }

void CompileFlags::setPath(string p) { _path = p; }

string CompileFlags::path() { return _path; }

list<string> CompileFlags::libraryPaths() { return _libraryPaths; }

void CompileFlags::addLibraryPath(string l) { _libraryPaths.push_back(l); }

void CompileFlags::addObject(string o) { _objects.insert(pair<string, string>(o, o)); }

list<string> CompileFlags::objects()
{
  list<string> ret;
  for (map<string, string>::iterator it = _objects.begin(); it != _objects.end(); it++) {
    ret.push_back(it->second);
  }
  return ret;
}

bool CompileFlags::optimizeQSS() { return _optimizeQSS; }

bool CompileFlags::hasObjects() { return !_objects.empty(); }

bool CompileFlags::externalStructureFile() { return _externalStructureFiles; }

void CompileFlags::setExternalStructureFile(bool s) { _externalStructureFiles = s; }

void CompileFlags::setDebug(string s)
{
  if (_debugOptions.find(s) != _debugOptions.end()) {
    _debug |= _debugOptions[s];
    if (!s.compare("SD_DBG_Weights")) {
      setGraph(true);
      _debug |= _debugOptions["SD_DBG_VarChanges"];
    }
  }
}

void CompileFlags::setGraph(bool g) { _graph = g; }

bool CompileFlags::graph() { return _graph; }

void CompileFlags::setTesting(bool testing) { _testing = testing; }

bool CompileFlags::testing() { return _testing; }

}  // namespace Util
}  // namespace MicroModelica
