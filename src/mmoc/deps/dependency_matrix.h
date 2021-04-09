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

typedef int equation_id;

typedef std::string variable_id;

class VariableDependency {
  public:
  VariableDependency();
  ~VariableDependency() = default;
  bool operator==(const VariableDependency& other) const;
  bool operator<(const VariableDependency& other) const;
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
  inline void setAlgUsage(IR::Expression alg_usage) { _alg_usage = alg_usage; }
  inline IR::Expression usage() const { return _alg_usage; };
  inline void setIfrRange(MDI ifr_range) { _ifr_range = ifr_range; }
  IR::Range ifrRange();
  void setRange(int equation_id = 0);
  MDI dom() { return _dom; };
  MDI ran() { return _ran; };
  std::string variable() { return _variable; };
  MDI getImage(MDI sub_dom);
  inline int equationId() const { return _equationId; };
  inline IndexPair ifrPair() const { return _ifr; };
  inline IndexPair ifePair() const { return _ife; };
  inline void setReduction(bool is_reduction) { _is_reduction = is_reduction; };
  inline bool isReduction() const { return _is_reduction; };
  void setEquationRange(IR::Range equation_range);
  IR::Range equationRange();

  // Temp hack until graph migration is complete.
  void setSwap(bool swap_usage);

  private:
  MDI _dom;
  MDI _ran;
  IndexPair _ifr;
  IndexPair _ife;
  IR::Expression _ifr_exp;
  IR::Expression _ife_exp;
  IR::Expression _alg_usage;
  IR::Range _range;
  int _id;
  int _equationId;
  std::string _variable;
  bool _is_reduction;
  MDI _ifr_range;
  // Delte this when the graphs are migrated.
  bool _swap_usage;
  IR::Range _equation_range;
};

// AlgebraicPath
typedef list<VariableDependency> AlgebraicPath;

typedef struct {
  AlgebraicPath algs;
  VariableDependency ifce;
  inline const IR::Index ifr() { return ifce.ifr(); };
  inline const IR::Index ife() { return ifce.ife(); };
} Path;

std::ostream& operator<<(std::ostream& out, const Path& d);

typedef list<Path> Paths;

struct MatrixConfig {
  std::string container;
  std::string names[4];
  std::string access[2];
  std::string component[2];
};

static MatrixConfig EmptyCfg = {"", {}, {}, {}};

template <typename ID>
class DependencyMatrix : public ModelTable<ID, Paths> {
  public:
  DependencyMatrix(VDM::Key key) : _cfg(EmptyCfg), _mode(VDM::Normal), _method(VDM::Alloc), _key(key){};
  DependencyMatrix(MatrixConfig cfg, VDM::Key key) : _cfg(cfg), _mode(VDM::Normal), _method(VDM::Alloc), _key(key){};
  ~DependencyMatrix() = default;

  inline const DependencyMatrix& alloc()
  {
    _method = VDM::Alloc;
    return *this;
  };

  inline const DependencyMatrix& init()
  {
    _method = VDM::Init;
    return *this;
  };

  inline void setMode(VDM::Mode mode) { _mode = mode; };
  
  std::string print() const
  {
    stringstream buffer;
    typename ModelTable<ID, Paths>::const_iterator it;
    string matrix = _cfg.names[_method + _mode];
    string access = _cfg.access[_mode];
    for (it = ModelTable<ID, Paths>::begin(); it != ModelTable<ID, Paths>::end(); it++) {
      Paths vds = it->second;
      for (auto vd : vds) {
        IR::Index ifr = vd.ifr().replace();
        IR::Index ife = vd.ife().replace();
        if (_mode == VDM::Transpose) {
          ifr = vd.ife().replace();
          ife = vd.ifr().replace();
        }
        IR::Range range = vd.ifce.range();
        // 1 -> N 
        if (IR::Index(vd.ifce.usage()).isConstant() && !vd.ifce.equationRange().isEmpty()) {
          range = vd.ifce.equationRange();
          if (_mode == VDM::Transpose) {
            ife = IR::Index(vd.ifce.ifrPair().exp()).replace();
          } else {
            ifr = IR::Index(vd.ifce.ifrPair().exp()).replace();
          }
        }
        buffer << range;
        if (_method == VDM::Alloc) {
          buffer << range.block() << _cfg.container << matrix << "[" << ifr << "]" << component() << "++;" << endl;
        } else {
          buffer << range.block() << _cfg.container << matrix << "[" << ifr << "]" << component() << "[" << access << "[" << ifr
                 << "]++] = " << ife << ";" << endl;
        }
        if (!range.isEmpty()) {
          buffer << range.end() << endl;
        }
      }
    }
    return buffer.str();
  }

  inline std::string accessVector() const { return _cfg.access[_mode]; };

  string component() const
  {
    stringstream buffer;
    string component = _cfg.component[0];
    if (_method == VDM::Init) {
      component = _cfg.component[1];
    }
    if (!component.empty()) {
      buffer << "." << component;
    }
    return buffer.str();
  }

  template <typename U>
  friend std::ostream& operator<<(std::ostream& out, const DependencyMatrix<U>& d);

  private:
  MatrixConfig _cfg;
  VDM::Mode _mode;
  VDM::Method _method;
  VDM::Key _key;
};

template <typename ID>
ostream& operator<<(std::ostream& out, const DependencyMatrix<ID>& d)
{
  return out << d.print();
};

class EquationDependencyMatrix : public DependencyMatrix<equation_id> {
  public:
  EquationDependencyMatrix() : DependencyMatrix(VDM::Int_Key){};
  EquationDependencyMatrix(MatrixConfig cfg) : DependencyMatrix(cfg, VDM::Int_Key){};
  ~EquationDependencyMatrix() = default;

  inline VDM::Key keyType() const { return VDM::Int_Key; };
  void insert(variable_id id, Paths paths){};
  void insert(equation_id id, Paths paths) { DependencyMatrix::insert(id, paths); };
};

class VariableDependencyMatrix : public DependencyMatrix<variable_id> {
  public:
  VariableDependencyMatrix() : DependencyMatrix(VDM::String_Key){};
  VariableDependencyMatrix(MatrixConfig cfg) : DependencyMatrix(cfg, VDM::String_Key){};
  ~VariableDependencyMatrix() = default;

  inline VDM::Key keyType() const { return VDM::String_Key; };
  void insert(equation_id id, Paths paths){};
  void insert(variable_id id, Paths paths) { DependencyMatrix::insert(id, paths); };
};

}  // namespace Deps
}  // namespace MicroModelica

#endif /* DEPENDENCY_MATRIX_H_ */