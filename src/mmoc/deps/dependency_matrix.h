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

#ifndef DEPENDENCY_MATRIX_H_
#define DEPENDENCY_MATRIX_H_

#include <map>

#include "graph/graph_helpers.h"
#include "../ir/index.h"
#include "../util/table.h"

namespace MicroModelica {
namespace Deps {

namespace VDM {
typedef enum { Normal, Transpose } Mode;

typedef enum { Alloc = 0, Init = 2 } Method;

typedef enum { String_Key, Int_Key } Key;
}  // namespace VDM

typedef int depId;

typedef std::string varId;

class VariableDependency {
  public:
  VariableDependency(){};
  ~VariableDependency() = default;
  inline void setVariable(std::string var) { _variable = var; };
  inline void setDom(MDI dom) { _dom = dom; };
  inline void setRan(MDI ran) { _ran = ran; };
  inline void setId(int id) { _id = id; };
  inline void setEquationId(int eqid) { _equationId = eqid; };
  inline const IR::Index ifr() { return IR::Index(_ifr_exp); };
  inline const IR::Index ife() { return IR::Index(_ife_exp); };
  IR::Range range() { return _range; };
  inline void setIfr(IndexPair ifr) { _ifr = ifr; };
  inline void setIfe(IndexPair ife) { _ife = ife; };
  void setRange();
  MDI dom() { return _dom; };
  MDI ran() { return _ran; };
  std::string variable() { return _variable; };
  MDI getImage(MDI sub_dom);
  inline int equationId() const { return _equationId; };
  inline IndexPair ifrPair() const { return _ifr; };
  inline IndexPair ifePair() const { return _ife; };
  inline void setReduction(bool is_reduction) { _is_reduction = is_reduction; };
  inline bool isReduction() const { return _is_reduction; };

  private:
  MDI _dom;
  MDI _ran;
  IndexPair _ifr;
  IndexPair _ife;
  IR::Expression _ifr_exp;
  IR::Expression _ife_exp;
  IR::Range _range;
  int _id;
  int _equationId;
  std::string _variable;
  bool _is_reduction;
};

// AlgebraicDependencies
typedef list<VariableDependency> AlgebraicDependencies;

typedef struct {
  AlgebraicDependencies algs;
  VariableDependency ifce;
  inline const IR::Index ifr() { return ifce.ifr(); };
  inline const IR::Index ife() { return ifce.ife(); };
} Influences;

std::ostream& operator<<(std::ostream& out, const Influences& d);

typedef list<Influences> VariableDependencies;

struct MatrixConfig {
  std::string container;
  std::string names[4];
  std::string access[2];
  std::string component[2];
};

class VariableDependencyMatrix : public ModelTable<varId, VariableDependencies> {
  public:
  VariableDependencyMatrix(MatrixConfig cfg);
  ~VariableDependencyMatrix(){};
  void insert(varId id, VariableDependencies deps) { ModelTable::insert(id, deps); };
  void insert(depId id, VariableDependencies deps){};

  inline const VariableDependencyMatrix& alloc()
  {
    _method = VDM::Alloc;
    return *this;
  };
  inline const VariableDependencyMatrix& init()
  {
    _method = VDM::Init;
    return *this;
  };
  inline void setMode(VDM::Mode mode) { _mode = mode; };
  std::string print() const;
  inline std::string accessVector() const { return _cfg.access[_mode]; };
  inline VDM::Key keyType() const { return VDM::String_Key; };
  friend std::ostream& operator<<(std::ostream& out, const VariableDependencyMatrix& d);

  private:
  MatrixConfig _cfg;
  VDM::Mode _mode;
  VDM::Method _method;

  std::string component() const;
};

class EquationDependencyMatrix : public ModelTable<depId, VariableDependencies> {
  public:
  inline VDM::Key keyType() const { return VDM::Int_Key; };
  void insert(varId id, VariableDependencies deps){};
  void insert(depId id, VariableDependencies deps) { ModelTable::insert(id, deps); };
};

}  // namespace Deps
}  // namespace MicroModelica

#endif /* DEPENDENCY_MATRIX_H_ */