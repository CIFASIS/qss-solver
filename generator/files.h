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

#include "model_instance.h"
#include "../ir/model.h"
#include "../util/compile_flags.h"


using namespace std;

/**
 *
 */
class MMO_Files
{
  public:
    /**
     *
     */
    MMO_Files();
    /**
     *
     * @param model
     * @param solver
     * @param flags
     */
    MMO_Files(MMO_Model* model, MMO_ModelInstance* modelInstance, MMO_CompileFlags flags);
    /**
     *
     * @param name
     * @param flags
     */
    MMO_Files(string name, MMO_CompileFlags flags);
    /**
     *
     */
    ~MMO_Files();
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
    settings(MMO_Annotation* annotation);
    void
    graph();
  private:
    void
    _printList(list<string> ann, string tag, MMO_Annotation* annotation);
    string             _fname;
    MMO_Model*         _model;
    MMO_ModelInstance* _modelInstance;
    MMO_Writer*        _writer;
    MMO_CompileFlags   _flags;
    ofstream           _file;
};

#endif  /* MMO_FILES_H_ */
