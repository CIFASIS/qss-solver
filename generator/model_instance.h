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

#ifndef MMO_MODEL_INSTANCE_H_
#define MMO_MODEL_INSTANCE_H_

#include <boost/variant/variant.hpp>

#include "../ir/class.h"
#include "../util/compile_flags.h"
#include "../util/graph.h"
#include "../util/symbol_table.h"
#include "../util/util_types.h"
#include "writer.h"

namespace MicroModelica {
namespace Generator {

namespace MODEL_INSTANCE {
/**
 *
 */
typedef enum {
  Model_Settings,
  Model,
  Deps,
  Zero_Crossing,
  Handler_Pos,
  Handler_Neg,
  Output,
  Jacobian,
  CLC_Init,
  QSS_Init
} Component;

typedef enum { SD, SZ, HD, HZ, DD } NodeType;
} // namespace MODEL_INSTANCE
  /**
   *
   */
class ModelInstance {
public:
  ModelInstance(){};
  ModelInstance(IR::Model &model, Util::CompileFlags &flags, WriterPtr writer);
  virtual ~ModelInstance(){};
  void include();
  virtual void initializeDataStructures() = 0;
  void zeroCrossing();
  void handler();
  void settings();
  void inputs();
  void output();
  virtual Graph computationalGraph() { return Graph(0, 0); };
  void initialCode();
  virtual void header();
  virtual void generate() = 0;

protected:
  virtual void definition() = 0;
  std::string componentDefinition(MODEL_INSTANCE::Component c);
  void allocateOutput();
  void configOutput();
  void initializeMatrix(Deps::VariableDependencyMatrix vdm,
                        WRITER::Section alloc, WRITER::Section init, int size);
  string algebraics(Deps::EquationDependencyMatrix eqdm, Deps::depId key);
  void configEvents();
  void allocateVectors() const;
  void freeVectors() const;
  std::string allocateModel();
  void allocateVector(std::string name, int size) const;
  void freeVector(std::string name, int size) const;

private:
  IR::Model _model;
  Util::CompileFlags _flags;
  WriterPtr _writer;
};

/**
 *
 */
class QSSModelInstance : public ModelInstance {
public:
  QSSModelInstance(){};
  QSSModelInstance(IR::Model &model, Util::CompileFlags &flags,
                   WriterPtr writer);
  ~QSSModelInstance(){};
  void initializeDataStructures();
  Graph computationalGraph();
  void generate();
  void header();

protected:
  void definition();
  void dependencies();

private:
  void allocateSolver();
  std::string allocateModel();
  IR::Model _model;
  Util::CompileFlags _flags;
  WriterPtr _writer;
};

/**
 *
 */
class ClassicModelInstance : public ModelInstance {
public:
  ClassicModelInstance(IR::Model &model, Util::CompileFlags &flags,
                       WriterPtr writer);
  ~ClassicModelInstance(){};
  void initializeDataStructures();
  void generate();
  void header();

protected:
  void definition();

private:
  void allocateSolver();
  std::string allocateModel();
  IR::Model _model;
  Util::CompileFlags _flags;
  WriterPtr _writer;
};

typedef std::shared_ptr<ModelInstance> ModelInstancePtr;
} // namespace Generator
} // namespace MicroModelica
#endif /* MMO_MODEL_INSTANCE_H */
