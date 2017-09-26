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

#include "../ir/mmo_types.h"
#include "../util/compile_flags.h"
#include "../util/util_types.h"
#include "generator_types.h"

using namespace std;

/*

 #include <map>

 #include <generator/generator_types.h>
 #include <ir/mmo_types.h>
 #include <util/compile_flags.h>
 */
/**
 *
 */
class MMO_Generator_
{
  public:
    /**
     *
     * @param std
     * @param flags
     */
    MMO_Generator_(MMO_StoredDefinition std, MMO_CompileFlags flags);
    /**
     *
     */
    ~MMO_Generator_();
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
    void
    _parameters();
    void
    _modelDef();
    void
    _modelDeps();
    void
    _zeroCrossing();
    void
    _handlerPositive();
    void
    _handlerNegative();
    void
    _output();
    void
    _initMatrices();
    void
    _functions();
    void
    _initSections();
    void
    _initEvents();
    void
    _initTime();
    void
    _initOutput();
    void
    _initModel();
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
/**
 *
 * @param std
 * @param flags
 * @return
 */
MMO_Generator
newMMO_Generator(MMO_StoredDefinition std, MMO_CompileFlags flags);
/**
 *
 * @param m
 */
void
deleteMMO_Generator(MMO_Generator m);

#endif  /* GENERATOR_H_ */
