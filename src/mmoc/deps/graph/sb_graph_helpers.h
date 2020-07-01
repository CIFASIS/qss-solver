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

#ifndef DEPS_SB_GRAPH_HELPERS_H_
#define DEPS_SB_GRAPH_HELPERS_H_

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
namespace SBG {

class Interval {
  public:
  typedef ICL::discrete_interval<int> DiscreteInterval;
  Interval();
  Interval(int a, int b, int step = 1);
  Interval(DiscreteInterval interval, int step = 1);
  Interval(const Interval& other);
  bool operator==(const Interval& other) const;
  bool operator!=(const Interval& other) const;
  int lower() const;
  int upper() const;
  int size() const;
  bool operator<(const Interval& other) const;
  Interval operator&(const Interval& other) const;
  bool contains(const Interval& other) const;
  bool intersects(const Interval& other) const;
  bool isEmpty() const;
  int step() const;

  protected:
  int getStep(const Interval& other) const;
  int getLowerBound(const Interval& other, int new_step) const;
  int getUpperBound(int new_begin, int new_step, int upper) const;
  bool checkStepIntersection(const Interval& other) const;
  bool isEven(int n) const;
  bool isOdd(int n) const;

  private:
  DiscreteInterval _interval;
  int _step;
};

typedef std::list<Interval> IntervalList;
typedef std::vector<Interval> IntervalVector;

class Usage {
  public:
  Usage();
  Usage(int size);
  Usage(int size, int value);
  int& operator[](int index);
  const int& operator[](int index) const;
  void push_back(const int i);
  bool operator==(const Usage& other) const;
  bool operator!=(const Usage& other) const;
  int size();
  bool isUnused();
  bool isUsed(int i);
  void join(Usage usage);
  typedef std::vector<int>::iterator iterator;
  typedef std::vector<int>::const_iterator const_iterator;
  const_iterator begin() const;
  iterator begin();
  iterator end();

  private:
  std::vector<int> _usage;
};

class Offset {
  public:
  Offset();
  Offset(std::vector<int> offset);
  bool operator<(const Offset& other) const;
  bool operator==(const Offset& other) const;
  bool operator!=(const Offset& other) const;
  int operator[](const int& index) const;
  bool isZeros();
  Offset operator-() const;
  Offset operator-(const Offset& other) const;
  Offset operator+(const Offset& other) const;
  Offset operator*(const Offset& other) const;
  typedef std::vector<int>::iterator iterator;
  typedef std::vector<int>::const_iterator const_iterator;
  const_iterator begin() const;
  iterator begin();
  iterator end();
  unsigned int size() const;
  friend std::ostream& operator<<(std::ostream& os, const Offset& offset);

  protected:
  std::vector<int> _offset;
};

class MDI {
  public:
  MDI(int d, ...);
  MDI(IntervalList intervals);
  MDI();
  MDI(IntervalVector intervals);
  int dimension() const;
  int size() const;
  MDI applyOffset(Offset offset) const;
  MDI revertOffset(Offset offset) const;
  MDI applyUsage(Usage usage, MDI ran) const;
  MDI revertUsage(Usage usage, MDI dom) const;
  MDI applyStep(MDI other);
  MDI revertStep(MDI other);
  bool operator<(const MDI& other) const;
  Option<MDI> operator&(const MDI& other) const;
  Option<MDI> intersection(MDI& other);
  bool operator==(const MDI& other) const;
  bool operator!=(const MDI& other) const;
  IntervalVector intervals();
  bool unique() const;
  bool isEmpty() const;
  bool reduction(MDI other);
  friend std::ostream& operator<<(std::ostream& os, const MDI mdi);

  private:
  IntervalVector _intervals;
};

namespace PAIR {
typedef enum { RN_N, RN_1, R1_N, R1_1 } Rel;
}

class LinearFunction {
  public:
  LinearFunction();
  LinearFunction(Offset constant, Offset factor);
  std::vector<std::string> apply(std::vector<std::string> variable) const;
  std::vector<int> apply(std::vector<int> variable) const;
  Offset factor() const;
  Offset constant() const;

  protected:
  Offset _constant;
  Offset _factor;
};

typedef std::map<int, Offset> IndexShift;

class Map {
  public:
  Map();
  Map(IR::Expression exp);
  Map(Usage usage, Offset offset, IR::Expression exp, LinearFunction linear_function);
  ~Map() = default;
  Map& operator=(const Map& other);
  MDI apply(MDI dom, MDI ran) const;
  MDI revert(MDI dom, MDI ran) const;
  Map compose(const Map& other);
  Map applyIndexShift(Offset index_shift);
  IR::Expression exp() const;
  std::vector<std::string> exps(std::vector<std::string> variables) const;
  bool operator<(const Map& other) const;
  bool operator==(const Map& other) const;
  bool operator!=(const Map& other) const;

  protected:
  Usage _usage;
  Offset _offset;
  IR::Expression _exp;
  LinearFunction _linear_function;
};

typedef std::map<int, Map> Maps;

class Pair {
  public:
  Pair();
  Pair(MDI dom, MDI ran, Map map, IR::Expression exp = IR::Expression());
  MDI dom() const;
  MDI ran() const;
  Map map() const;
  bool operator<(const Pair& other) const;
  PAIR::Rel type() const;
  friend std::ostream& operator<<(std::ostream& os, const Pair& ip);

  private:
  MDI _dom;
  MDI _ran;
  Map _map;
};

typedef std::set<Pair> PairSet;

}  // namespace SBG
}  // namespace Deps
}  // namespace MicroModelica

#endif /* DEPS_GRAPH_HELPERS_H_ */
