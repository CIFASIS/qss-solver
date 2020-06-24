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

#ifndef DEPS_GRAPH_HELPERS_H_
#define DEPS_GRAPH_HELPERS_H_

#include <list>
#include <set>
#include <vector>
#include <boost/icl/discrete_interval.hpp>
#include <boost/algorithm/string/join.hpp>

#include "../../ir/expression.h"
#include "../../util/util_types.h"

namespace ICL = boost::icl;

namespace MicroModelica {
namespace Deps {

class Interval {
  public:
  typedef ICL::discrete_interval<int> DiscreteInterval;
  Interval() : _interval(), _step(1){};
  Interval(int a, int b, int step = 1) : _interval(), _step(step)
  {
    _interval = ICL::discrete_interval<int>(a, b, ICL::interval_bounds::closed());
  };
  Interval(DiscreteInterval interval, int step = 1) : _interval(interval), _step(step){};
  Interval(const Interval& other);
  inline bool operator==(const Interval& other) const { return this->_interval == other._interval; };
  inline bool operator!=(const Interval& other) const { return this->_interval != other._interval; };
  inline int lower() { return _interval.lower(); }
  inline int upper() { return _interval.upper(); }
  inline int size() { return ICL::size(_interval); }
  bool operator<(const Interval& other) const;
  Interval operator&(const Interval& other) const;
  inline bool contains(const Interval& other) const { return ICL::contains(_interval, other._interval); }
  inline bool intersects(const Interval& other) const { return ICL::intersects(_interval, other._interval); }
  inline bool isEmpty() const { return ICL::is_empty(_interval); }
  inline int step() const { return _step; };

  protected:
  int getStep(const Interval& other) const;
  int getLowerBound(const Interval& other, int new_step) const;
  int getUpperBound(int new_begin, int new_step, int upper) const;
  bool checkStepIntersection(const Interval& other) const;
  inline bool isEven(int n) const { return (n % 2) == 0; }
  inline bool isOdd(int n) const { return !isEven(n); }

  private:
  DiscreteInterval _interval;
  int _step;
};

typedef std::list<Interval> IntervalList;
typedef std::vector<Interval> IntervalVector;

class Usage {
  public:
  inline Usage() : _usage(){};
  inline Usage(int size) : _usage(std::vector<int>(size)) {}
  inline Usage(int size, int value) : _usage(std::vector<int>(size, value)) {}
  inline int& operator[](int index) { return _usage[index]; }
  inline const int& operator[](int index) const { return _usage[index]; }
  inline void push_back(const int i) { _usage.push_back(i); }
  inline bool operator==(const Usage& other) const { return this->_usage == other._usage; };
  inline bool operator!=(const Usage& other) const { return this->_usage != other._usage; };
  inline int Size() { return _usage.size(); }
  inline bool isUnused()
  {
    for (int i : _usage) {
      if (i != -1) return false;
    }
    return true;
  }
  bool isUsed(int i);
  void join(Usage usage);
  typedef std::vector<int>::iterator iterator;
  typedef std::vector<int>::const_iterator const_iterator;
  inline const_iterator begin() const { return _usage.begin(); }
  inline iterator begin() { return _usage.begin(); }
  inline iterator end() { return _usage.end(); }

  private:
  std::vector<int> _usage;
};

class Offset {
  public:
  inline Offset(std::vector<int> offset) : offset(offset){};
  inline Offset() : offset(){};
  inline bool operator<(const Offset& other) const { return this->offset < other.offset; };
  inline bool operator==(const Offset& other) const { return this->offset == other.offset; };
  inline bool operator!=(const Offset& other) const { return this->offset != other.offset; };
  inline int operator[](const int& index) const { return offset[index]; };
  inline bool isZeros()
  {
    for (int i : offset) {
      if (i != 0) return false;
    }
    return true;
  }
  Offset operator-() const;
  typedef std::vector<int>::iterator iterator;
  typedef std::vector<int>::const_iterator const_iterator;
  inline const_iterator begin() const { return offset.begin(); }
  inline iterator begin() { return offset.begin(); }
  inline iterator end() { return offset.end(); }
  inline unsigned int Size() const { return offset.size(); }

  private:
  std::vector<int> offset;
};

class IndexPair;

class MDI {  // Multi-Dimensional Interval
  public:
  MDI(int d, ...);
  MDI(IntervalList intervalList);
  inline MDI() { intervals.resize(0); }
  inline MDI(IntervalVector intervals) : intervals(intervals){};
  inline int Dimension() const { return intervals.size(); }
  int Size() const;
  std::list<MDI> operator-(const MDI& other);
  std::list<MDI> Difference(const MDI& other) { return (*this) - other; };
  MDI ApplyOffset(Offset) const;
  MDI RevertOffset(Offset offset, Usage usage, MDI ran) const;
  MDI ApplyUsage(Usage, MDI ran = MDI({})) const;
  MDI RevertUsage(Usage usage, MDI dom = MDI({})) const;
  MDI applyStep(MDI other);
  MDI revertStep(MDI other);
  // karupayun - Para moverse usando la info de la conexion entre Dom y Ran
  MDI DomToRan(IndexPair ip) const;
  MDI RanToDom(IndexPair ip) const;
  bool operator<(const MDI& other) const;
  Option<MDI> operator&(const MDI& other) const;
  Option<MDI> Intersection(MDI& other);
  friend std::ostream& operator<<(std::ostream& os, const MDI mdi);
  inline const IntervalVector& Intervals() const { return intervals; }
  bool Contains(const MDI& other) const;
  inline bool operator==(const MDI& other) const { return this->intervals == other.intervals; };
  inline bool operator!=(const MDI& other) const { return !((*this) == other); };
  inline IntervalVector mdi() { return intervals; };
  bool unique() const;
  inline bool isEmpty() const { return intervals.size() == 0; };
  MDI getImage(IndexPair p);
  MDI revertImage(IndexPair p);
  bool reduction(MDI other);

  private:
  IntervalVector intervals;
  typedef IntervalVector::iterator iterator;
  typedef IntervalVector::const_iterator const_iterator;
  inline const_iterator begin() const { return intervals.begin(); }
  inline iterator begin() { return intervals.begin(); }
  inline iterator end() { return intervals.end(); }

  IntervalList Partition(Interval iA, Interval iB);
  // MDI ApplyOffset(Offset offset);
  std::list<MDI> PutHead(Interval i, std::list<MDI> mdiList);
  std::list<MDI> Filter(std::list<MDI> mdiList, MDI mdi);
  std::list<MDI> CartProd(std::list<MDI> mdiList);
  std::list<MDI> PutLists(MDI mdi, std::list<MDI> mdiList);
};

namespace INDEX_PAIR {
typedef enum { RN_N, RN_1, R1_N, R1_1 } Rel;
}

class IndexPair {
  public:
  IndexPair(){};
  IndexPair(MDI dom_, MDI ran_, Offset os, Usage us, IR::Expression exp = IR::Expression())
      : dom(dom_), ran(ran_), offset(os), usage(us), _exp(exp){};
  inline MDI Dom() const { return dom; }
  inline MDI Ran() const { return ran; }
  inline Offset GetOffset() const { return offset; }
  inline Usage GetUsage() const { return usage; }
  std::list<IndexPair> operator-(const IndexPair& other) const;
  std::list<IndexPair> Difference(const IndexPair& other) { return (*this) - other; }
  std::set<IndexPair> RemoveUnknowns(MDI unk2remove);
  std::set<IndexPair> RemoveEquations(MDI eqs2remove);
  bool operator<(const IndexPair& other) const;
  Option<IndexPair> operator&(const IndexPair& other) const;
  friend std::ostream& operator<<(std::ostream& os, const IndexPair& ip);
  bool Contains(const IndexPair& other) const;
  INDEX_PAIR::Rel Type() const;
  IR::Expression exp() const { return _exp; };

  private:
  MDI dom, ran;
  Offset offset;
  Usage usage;
  IR::Expression _exp;
};

std::ostream& operator<<(std::ostream& os, const std::list<IndexPair>& ipList);

typedef std::set<IndexPair> IndexPairSet;
std::ostream& operator<<(std::ostream& os, const IndexPairSet& ips);

}  // namespace Deps
}  // namespace MicroModelica

#endif /* DEPS_GRAPH_HELPERS_H_ */
