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

#include "files.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <utility>

#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "../util/compile_flags.h"
#include "../util/symbol_table.h"
#include "../util/util.h"
#include "writer.h"
#include "model_instance.h"

#ifdef __linux__
#include <sys/stat.h>
#endif

using namespace std;

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Generator {

Files::Files() : _fname(), _model(), _writer(), _flags() {}

Files::Files(ModelInstancePtr modelInstance, Model& model, CompileFlags& flags)
    : _fname(model.name()), _model(model), _modelInstance(modelInstance), _writer(new FileWriter()), _flags(flags)
{
  if (_flags.hasOutputFile()) {
    _fname = _flags.outputFile();
  }
}

Files::Files(string name, CompileFlags& flags) : _fname(name), _model(), _writer(new FileWriter()), _flags(flags)
{
  if (_flags.hasOutputFile()) {
    _fname = _flags.outputFile();
  }
}

Files::~Files() {}

void Files::makefile()
{
  stringstream buffer;
  stringstream includes;
  string fname = _fname;
  SymbolTable include;
  fname.append(".makefile");
  _writer->setFile(fname);
  _writer->print("#Compiler and Linker");
  _writer->print("CC    := gcc");
  _writer->print("");
  _writer->print("#The Target Binary Program ");
  _writer->print("TARGET    := " + _fname);
  _writer->print("");
  _writer->print("#Flags, Libraries and Includes");
  includes << "LDFLAGS    :=-L " << Utils::instance().environmentVariable("MMOC_LIBS");
  SymbolTable tmp = _model.libraryDirectories();
  SymbolTable::iterator it;
  for (string l = tmp.begin(it); !tmp.end(it); l = tmp.next(it)) {
    includes << " -L" << l;
  }
  includes << " -L " << Utils::instance().environmentVariable("MMOC_PATH") << "/usr/lib";
  _writer->print(includes);
  buffer << "LIBS   :=" << (_flags.debug() ? " -lqssd -ltimestepd" : " -lqss -ltimestep");
  _writer->print(buffer);
  buffer << "INC    := -I" + Utils::instance().environmentVariable("MMOC_ENGINE");
  tmp = _model.includeDirectories();
  for (string i = tmp.begin(it); !tmp.end(it); i = tmp.next(it)) {
    include[i] = i;
  }
  if (_flags.hasObjects()) {
    list<string> objects = _flags.objects();
    for (list<string>::iterator it = objects.begin(); it != objects.end(); it++) {
      string inc = *it;
      unsigned int f = inc.rfind("/");
      inc.erase(inc.begin() + f, inc.end());
      include[inc] = inc;
    }
  }
  string pinclude = Utils::instance().environmentVariable("MMOC_INCLUDE");
  include[pinclude] = pinclude;
  if (_flags.hasObjects()) {
    pinclude = Utils::instance().environmentVariable("MMOC_PACKAGES");
    include[pinclude] = pinclude;
  }
  for (string i = include.begin(it); !include.end(it); i = include.next(it)) {
    buffer << " -I" << i;
  }
  _writer->print(buffer);
  if (_flags.debug()) {
    _writer->print("CFLAGS    := -Wall -g -msse2 -mfpmath=sse $(LDFLAGS) $(LIBS)");
  } else {
    _writer->print("CFLAGS    := -Wall -msse2 -mfpmath=sse -O2 $(LDFLAGS) $(LIBS)");
  }
  tmp = _model.linkLibraries();
  for (string i = tmp.begin(it); !tmp.end(it); i = tmp.next(it)) {
    includes << " -l" << i;
  }
  _writer->print("RMS    := rm -rf");
  _writer->print("");
  _writer->print("#Source Files");
  buffer << "TARGET_SRC    := " << _fname << ".c";
  if (!_model.calledFunctions().empty()) {
    buffer << " " << _fname << "_functions.c";
  }
  _writer->print(buffer);
  if (_flags.hasObjects()) {
    buffer << "SRC    := ";
    list<string> objects = _flags.objects();
    for (list<string>::iterator it = objects.begin(); it != objects.end(); it++) {
      buffer << *it << " ";
    }
    _writer->print(buffer);
  }
  _writer->print("");
  if (_flags.hasObjects()) {
    _writer->print("#Objects");
    _writer->print("OBJ = $(SRC:.c=.o)");
    _writer->print("\%.o: \%.c");
    _writer->print(_writer->indent(1) + "$(CC) $(INC) -c $< -o $@ $(CFLAGS)");
    _writer->print("");
  }
  _writer->print("default: $(TARGET)");
  _writer->print("");
  buffer << "$(TARGET):";
  if (_flags.hasObjects()) {
    buffer << " $(OBJ)";
  }
  _writer->print(buffer);
  buffer << _writer->indent(1) << "$(CC) $(INC)";
  if (_flags.hasObjects()) {
    buffer << " $(OBJ)";
  }
  buffer << " $(TARGET_SRC) $(CFLAGS) -o $@ -lm -lgsl -lconfig -lgfortran";
#ifdef __linux__
  buffer << " -lpthread -lmetis -lscotch -lscotcherr -lpatoh -lrt -lsundials_cvode -lsundials_ida -lsundials_nvecserial -llapack -latlas "
            "-lf77blas -lklu";
#endif
  buffer << " -lgslcblas" << includes.str();
  if (_flags.parallel()) {
    buffer << " -DQSS_PARALLEL";
  }
  _writer->print(buffer);
  _writer->print("");
  _writer->print(".PHONY: clean");
  _writer->print("");
  _writer->print("clean:");
  _writer->print(_writer->indent(1) + "$(RMS) $(TARGET) *.dat *.log");
  _writer->clearFile();
}

void Files::run()
{
  string fname = _fname;
  fname.append(".sh");
  stringstream buffer;
  _writer->setFile(fname);
  _writer->print("cwd=$(pwd)");
  _writer->print("cd " + Utils::instance().getFilePath(_fname));
  _writer->print("make -f " + _fname + ".makefile clean");
  buffer << "make -f " + _fname + ".makefile";
  _writer->print(buffer);
  _writer->print("./" + Utils::instance().getFileName(_fname));
  if (_model.outputNbr()) {
    _writer->print("echo");
    _writer->print("gnuplot " + _fname + ".plt");
  }
  _writer->print("cd $cwd");
  _writer->clearFile();
#ifdef __linux__
  chmod(fname.c_str(), S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif
}

void Files::plot()
{
  /*if(!_model->outs())
  {
    return;
  }
  stringstream buffer;
  string fname = _fname;
  fname.append(".plt");
  _writer->setFile(fname);
  _writer->print("set terminal wxt");
  _writer->print("set title \"" + _fname + "\"");
  _writer->print("set ylabel \"State Variables\"");
  _writer->print("set xlabel \"Time\"");
  _writer->print("set grid");
  buffer << "plot ";
  MMO_EquationTable outputs = _model->outputs();
  int outs = 0, total = _model->outs();
  VarSymbolTable vt = _model.symbols();
  for(MMO_Equation eq = outputs->begin(); !outputs->end(); eq = outputs->next())
  {
    Index index = outputs->key();
    string varName;
    Dependencies deps = eq->exp()->deps();
    if(index.hasRange())
    {
      for(int i = index.begin(); i <= index.end(); i++)
      {
        varName = eq->exp()->print();
        buffer << "\"" << varName << ".dat\""
            << _variableSettings(deps, varName);
        if(i + 1 <= index.end())
        {
          buffer << ",";
        }
      }
    }
    else
    {
      varName = eq->exp()->print("");
      buffer << "\"" << varName << ".dat\"" << _variableSettings(deps, varName);
    }
    outs += index.range();
    if(outs < total)
    {
      buffer << ",";
    }
  }
  _writer->print(&buffer);
  _writer->print("pause mouse close");
  _writer->clearFile();*/
}

void Files::settings(ModelAnnotation annotation)
{
  stringstream buffer;
  string fname = _fname;
  fname.append(".ini");
  _writer->setFile(fname);
  buffer.precision(5);
  buffer << "minstep=" << scientific << annotation.minStep() << ";";
  _writer->print(buffer);
  buffer << "zchyst=" << annotation.ZCHyst() << ";";
  _writer->print(buffer);
  buffer << "derdelta=" << annotation.derDelta() << ";";
  _writer->print(buffer);
  buffer << "symdiff=" << annotation.symDiff() << ";";
  _writer->print(buffer);
  if (annotation.parallel()) {
    buffer << "lps=" << annotation.lps() << ";";
    _writer->print(buffer);
    buffer << "partitionMethod=\"" << annotation.partitionMethodString() << "\";";
    _writer->print(buffer);
    buffer << "dt=" << annotation.DT() << ";";
    _writer->print(buffer);
    buffer << "dtSynch=\"" << annotation.dtSynchString() << "\";";
    _writer->print(buffer);
  } else {
    _writer->print("lps=0;");
  }
  buffer << "nodesize=" << annotation.nodeSize() << ";";
  _writer->print(buffer);
  buffer << "jacobian=" << (annotation.jacobian()) << ";";
  _writer->print(buffer);
  buffer << "it=" << annotation.initialTime() << ";";
  _writer->print(buffer);
  buffer << "ft=" << annotation.finalTime() << ";";
  _writer->print(buffer);
  _writer->print("sol=\"" + annotation.solverString() + "\";");
  list<double> dq = annotation.dqmin();
  buffer << "dqmin=(";
  int count = 0, size = dq.size();
  for (list<double>::iterator it = dq.begin(); it != dq.end(); it++) {
    buffer << *it;
    if (++count < size) {
      buffer << ",";
    }
  }
  buffer << ");";
  _writer->print(buffer);
  dq = annotation.dqrel();
  buffer << "dqrel=(";
  count = 0;
  size = dq.size();
  for (list<double>::iterator it = dq.begin(); it != dq.end(); it++) {
    buffer << *it;
    if (++count < size) {
      buffer << ",";
    }
  }
  buffer << ");";
  _writer->print(buffer);
  printList(annotation.patohSettings(), "patohOptions");
  printList(annotation.scotchSettings(), "scotchOptions");
  printList(annotation.metisSettings(), "metisOptions");
  _writer->clearFile();
}

void Files::printList(list<string> ann, string tag)
{
  stringstream buffer;
  if (ann.empty()) {
    return;
  }
  buffer << tag << "=(";
  int count = 0, size = ann.size();
  for (list<string>::iterator it = ann.begin(); it != ann.end(); it++) {
    buffer << *it;
    if (++count < size) {
      buffer << ",";
    }
  }
  buffer << ");";
  _writer->print(buffer);
}

void Files::graph()
{
  /*  Graph g = _modelInstance->graph();
    if(g.empty())
    {
      return;
    }
    string fileName = _fname + ".graph";
    string hFileName = _fname + ".hgraph";
    string tmp1FileName = _fname + ".tmp1";
    string tmp2FileName = _fname + ".tmp2";
    string tmp3FileName = _fname + ".tmp3";
    string wFileName = _fname + ".eweights";
    string hwFileName = _fname + ".heweights";
    string nwFileName = _fname + ".vweights";
    ofstream matrix(fileName.c_str(), ios::out | ios::binary);
    matrix.seekp(0);
    ofstream wMatrix(wFileName.c_str(), ios::out | ios::binary);
    wMatrix.seekp(0);
    ofstream hwMatrix(hwFileName.c_str(), ios::out | ios::binary);
    hwMatrix.seekp(0);
    ofstream nwMatrix(nwFileName.c_str(), ios::out | ios::binary);
    nwMatrix.seekp(0);
    ofstream tmp1(tmp1FileName.c_str(), ios::out | ios::binary);
    tmp1.seekp(0);
    ofstream tmp2(tmp2FileName.c_str(), ios::out | ios::binary);
    tmp2.seekp(0);
    ofstream tmp3(tmp3FileName.c_str(), ios::out | ios::binary);
    tmp3.seekp(0);
    grp_t nvtxs = _model->evs() + _model->states();
    grp_t w, size = 0;
    g.connectGraphs();
    grp_t i;
    for(i = 0; i < nvtxs; i++)
    {
      size += g.graphNodeEdges(i);
      matrix.write((char*) &size, sizeof(grp_t));
    }
    grp_t hedges = g.hyperGraphEdges();
    map<int, set<int> > graph = g.graph();
    map<int, set<int> > hGraph = g.hyperGraph();
    for(i = 0; i < nvtxs; i++)
    {
      set<int>::iterator it;
      for(it = graph[i].begin(); it != graph[i].end(); it++)
      {
        grp_t inf = *it;
        matrix.write((char*) &inf, sizeof(grp_t));
        w = g.graphEdgeWeight(i, *it);
        wMatrix.write((char*) &w, sizeof(grp_t));
      }
      w = g.nodeWeight(i);
      nwMatrix.write((char*) &w, sizeof(grp_t));
    }
    size = 0;
    tmp1.write((char*) &hedges, sizeof(grp_t));
    for(i = 0; i < nvtxs; i++)
    {
      set<int>::iterator it;
      for(it = hGraph[i].begin(); it != hGraph[i].end(); ++it)
      {
        grp_t inf = *it;
        tmp2.write((char*) &inf, sizeof(grp_t));
      }
      if(!hGraph[i].empty())
      {
        w = g.hyperGraphEdgeWeight(i);
        hwMatrix.write((char*) &w, sizeof(grp_t));
        size += hGraph[i].size();
        tmp1.write((char*) &size, sizeof(grp_t));
        tmp3.write((char*) &i, sizeof(grp_t));
      }
    }
    matrix.close();
    wMatrix.close();
    hwMatrix.close();
    tmp1.close();
    tmp2.close();
    tmp3.close();
    string command = "cat " + tmp1FileName + " " + tmp2FileName + " " + tmp3FileName + " > " + hFileName;
    system(command.c_str());
    remove(tmp1FileName.c_str());
    remove(tmp2FileName.c_str());
    remove(tmp3FileName.c_str());*/
}
}  // namespace Generator
}  // namespace MicroModelica
