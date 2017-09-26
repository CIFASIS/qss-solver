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

#ifndef MMO_FILES_H_
#define MMO_FILES_H_

#include <fstream>
#include <string>
#include <list>

#include "../ir/mmo_types.h"
#include "../util/util_types.h"
#include "generator_types.h"

using namespace std;

/**
 *
 */
class MMO_Files_
{
  public:
    /**
     *
     * @param model
     * @param solver
     * @param flags
     */
    MMO_Files_(MMO_Model model, MMO_Solver solver, MMO_CompileFlags flags);
    /**
     *
     * @param name
     * @param flags
     */
    MMO_Files_(string name, MMO_CompileFlags flags);
    /**
     *
     */
    ~MMO_Files_();
    /**
     *
     */
    void
    makefile();
    /**
     *
     */
    void
    run();
    /**
     *
     */
    void
    plot();
    /**
     *
     * @param annotation
     */
    void
    settings(MMO_Annotation annotation);
    void
    graph();
    private:
    string
    _variableSettings(Dependencies deps, string varName);
    void
    _printList(list<string> ann, string tag, MMO_Annotation annotation);
    string _fname;
    MMO_Model _model;
    MMO_Solver _solver;
    MMO_Writer _writer;
    MMO_CompileFlags _flags;
    ofstream _file;
};

/**
 *
 * @param model
 * @param solver
 * @param flags
 * @return
 */
MMO_Files
newMMO_Files(MMO_Model model, MMO_Solver solver, MMO_CompileFlags flags);

/**
 *
 * @param name
 * @param flags
 * @return
 */
MMO_Files
newMMO_Files(string name, MMO_CompileFlags flags);

/**
 *
 * @param m
 */
void
deleteMMO_Files(MMO_Files m);

#endif  /* MMO_FILES_H_ */
