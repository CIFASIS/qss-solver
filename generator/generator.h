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

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include "../util/compile_flags.h"
#include "../ir/stored_definition.h"
#include "../ir/function.h"
#include "../ir/model.h"
#include "../ir/package.h"
#include "solver.h"
#include "files.h"
#include "writer.h"

using namespace std;

/**
 *
 */
class MMO_Generator
{
  public:
    /**
     *
     * @param std
     * @param flags
     */
    MMO_Generator(MMO_StoredDefinition std, MMO_CompileFlags &flags);
    /**
     *
     */
    ~MMO_Generator();
    /**
     *
     * @return
     */
    int
    generate();
  private:
    void
    _generateFunctionCode(MMO_Function f);
    void
    _generateHeader(string name);
    void
    _printList(list<string> l);
    void
    _variablesInitCode();
    void
    _variables();
    void
    _generateModel();
    void
    _generateFunction(MMO_Function f, string fileName);
    void
    _generateFunction(MMO_FunctionDefinition f, string fileName);
    void
    _generateFunctionHeader(string fileName);
    void
    _generatePackage(MMO_Package p);
    void
    _header();
    MMO_StoredDefinition _std;
    MMO_Model _model;
    MMO_Function _function;
    MMO_Package _package;
    MMO_CompileFlags _flags;
    MMO_Solver _solver;
    MMO_Files _files;
    MMO_Writer _writer;
    ofstream _file;
    map<string, string> _includes;
    list<string> _fheader;
};

#endif  /* GENERATOR_H_ */
