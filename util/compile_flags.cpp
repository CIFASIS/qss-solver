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

MMO_CompileFlags_::MMO_CompileFlags_ () :
    _store (true), _parallel (false), _externalStructureFiles (false), _output (
	true), _debug (0), _outputFile (), _path (), _libraryPaths (), _objects (), _optimizeQSS (
	false), _incidenceMatrices (false), _externalFunctions (false), _lps(0), _debugOptions(), _graph (false)
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

MMO_CompileFlags_::~MMO_CompileFlags_ ()
{
}

bool
MMO_CompileFlags_::store ()
{
  return (_store);
}

void
MMO_CompileFlags_::setStore (bool s)
{
  _store = s;
}

void
MMO_CompileFlags_::setIncidenceMatrices (bool im)
{
  _incidenceMatrices = im;
}

void
MMO_CompileFlags_::setOptimizeQSS (bool s)
{
  _optimizeQSS = s;
}

bool
MMO_CompileFlags_::incidenceMatrices ()
{
  return (_incidenceMatrices);
}

bool
MMO_CompileFlags_::parallel ()
{
  return (_parallel);
}

void
MMO_CompileFlags_::setParallel (bool s)
{
  _parallel = s;
  setGraph(true);
}

bool
MMO_CompileFlags_::output ()
{
  return (_output);
}

void
MMO_CompileFlags_::setOutput (bool s)
{
  _output = s;
}

bool
MMO_CompileFlags_::externalFunctions ()
{
  return (_externalFunctions);
}

void
MMO_CompileFlags_::setExternalFunctions (bool s)
{
  _externalFunctions = s;
}

int
MMO_CompileFlags_::debug ()
{
  return (_debug);
}

void
MMO_CompileFlags_::setDebug (int s)
{
  _debug |= s;
}

void
MMO_CompileFlags_::setOutputFile (string outputFile)
{
  _outputFile = outputFile;
}

string
MMO_CompileFlags_::outputFileName ()
{
  return (Util::getInstance ()->getFileName (_outputFile));
}

string
MMO_CompileFlags_::outputFilePath ()
{
  return (Util::getInstance ()->getFilePath (_outputFile));
}

string
MMO_CompileFlags_::outputFile ()
{
  return (_outputFile);
}

bool
MMO_CompileFlags_::hasOutputFile ()
{
  return (!_outputFile.empty ());
}

void
MMO_CompileFlags_::setPath (string p)
{
  _path = p;
}

string
MMO_CompileFlags_::path ()
{
  return (_path);
}
MMO_CompileFlags
newMMO_CompileFlags ()
{
  return (new MMO_CompileFlags_ ());
}

void
deleteMMO_CompileFlags (MMO_CompileFlags m)
{
  delete m;
}

list<string>
MMO_CompileFlags_::libraryPaths ()
{
  return (_libraryPaths);
}

void
MMO_CompileFlags_::addLibraryPath (string l)
{
  _libraryPaths.push_back (l);
}

void
MMO_CompileFlags_::addObject (string o)
{
  _objects.insert (pair<string, string> (o, o));
}

list<string>
MMO_CompileFlags_::objects ()
{
  list<string> ret;
  for (map<string, string>::iterator it = _objects.begin ();
      it != _objects.end (); it++)
    {
      ret.push_back (it->second);
    }
  return (ret);
}

bool
MMO_CompileFlags_::optimizeQSS ()
{
  return (_optimizeQSS);
}

bool
MMO_CompileFlags_::hasObjects ()
{
  return (!_objects.empty ());
}

bool
MMO_CompileFlags_::externalStructureFile ()
{
  return (_externalStructureFiles);
}

void
MMO_CompileFlags_::setExternalStructureFile (bool s)
{
  _externalStructureFiles = s;
}

void
MMO_CompileFlags_::setDebug (string s)
{
  if (_debugOptions.find( s ) != _debugOptions.end())
    {
       _debug |= _debugOptions[s];
       if (!s.compare("SD_DBG_Weights"))
	 {
	   setGraph(true);
	   _debug |= _debugOptions["SD_DBG_VarChanges"];
	 }
    }
}

void
MMO_CompileFlags_::setGraph (bool g)
{
  _graph = g;
}

bool
MMO_CompileFlags_::graph ()
{
  return (_graph);
}
