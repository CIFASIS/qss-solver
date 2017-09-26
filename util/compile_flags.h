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

#ifndef MMO_COMPILE_FLAGS_H_
#define MMO_COMPILE_FLAGS_H_

#include <list>
#include <map>
#include <string>

#include "util_types.h"

/**
 *
 */
class MMO_CompileFlags_
{
  public:
    /**
     *
     */
    MMO_CompileFlags_();
    /**
     *
     */
    ~MMO_CompileFlags_();
    /**
     *
     * @param l
     */
    void
    addLibraryPath(string l);
    /**
     *
     * @param o
     */
    void
    addObject(string o);
    /**
     *
     * @return
     */
    int
    debug();
    /**
     *
     * @return
     */
    string
    enginePath();
    /**
     *
     * @return
     */
    string
    engineLibs();
    /**
     *
     * @return
     */
    bool
    externalFunctions();
    /**
     *
     * @return
     */
    bool
    hasObjects();
    /**
     *
     * @return
     */
    bool
    hasOutputFile();
    /**
     *
     * @return
     */
    bool
    incidenceMatrices();
    /**
     *
     * @return
     */
    list<string>
    libraryPaths();
    /**
     *
     * @return
     */
    list<string>
    objects();
    /**
     *
     * @return
     */
    bool
    optimizeQSS();
    /**
     *
     * @return
     */
    bool
    externalStructureFile();
    /**
     *
     * @return
     */
    bool
    output();
    /**
     *
     * @return
     */
    string
    outputFile();
    /**
     *
     * @return
     */
    string
    outputFileName();
    /**
     *
     * @return
     */
    string
    outputFilePath();
    /**
     *
     * @return
     */
    bool
    parallel();
    /**
     *
     * @return
     */
    string
    path();
    /**
     *
     * @return
     */
    bool
    store();
    /**
     *
     * @param s
     */
    void
    setDebug(int s);
    /**
     *
     * @param s
     */
    void
    setDebug(string s);
    /**
     *
     * @param p
     */
    void
    setEngineLibs(string p);
    /**
     *
     * @param p
     */
    void
    setEnginePath(string p);
    /**
     *
     * @param s
     */
    void
    setExternalFunctions(bool s);
    /**
     *
     * @param p
     */
    void
    setExternalLibs(string p);
    /**
     *
     * @param im
     */
    void
    setIncidenceMatrices(bool im);
    /**
     *
     * @param s
     */
    void
    setOptimizeQSS(bool s);
    /**
     *
     * @param s
     */
    void
    setOutput(bool s);
    /**
     *
     * @param outputFile
     */
    void
    setOutputFile(string outputFile);
    /**
     *
     * @param s
     */
    void
    setParallel(bool s);
    /**
     *
     * @param s
     */
    void
    setExternalStructureFile(bool s);
    /**
     *
     * @param p
     */
    void
    setPath(string p);
    /**
     *
     * @param s
     */
    void
    setStore(bool s);
    /**
     *
     * @return
     */
    string
    externalLibs();
    /**
     *
     * @return
     */
    int
    lps() const
    {
      return _lps;
    }
    /**
     *
     * @param lps
     */
    void
    setLps(int lps)
    {
      _lps = lps;
    }
    void
    setGraph(bool g);
    bool
    graph();
    private:
    bool _store;
    bool _parallel;
    bool _externalStructureFiles;
    bool _output;
    int _debug;
    string _outputFile;
    string _path;
    list<string> _libraryPaths;
    map<string, string> _objects;
    bool _optimizeQSS;
    bool _incidenceMatrices;
    bool _externalFunctions;
    int _lps;
    map<string, int> _debugOptions;
    bool _graph;
};
/**
 *
 * @return
 */
MMO_CompileFlags
newMMO_CompileFlags();
/**
 *
 * @param m
 */
void
deleteMMO_CompileFlags(MMO_CompileFlags m);

#endif  /* MMO_COMPILE_FLAGS_H_ */
