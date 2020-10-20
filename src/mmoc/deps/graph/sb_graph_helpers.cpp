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

#include "sb_graph_helpers.h"

#include <boost/math/common_factor.hpp>
#include <iostream>
#include <stdarg.h>

namespace MicroModelica {
using namespace IR;
namespace Deps {
namespace SBG {

/* SBGraphs Interval interface */

Interval::Interval() : _interval(), _step(1){};

Interval::Interval(int a, int b, int step) : _interval(), _step(step)
{
  _interval = ICL::discrete_interval<int>(a, b, ICL::interval_bounds::closed());
};

Interval::Interval(DiscreteInterval interval, int step) : _interval(interval), _step(step){};

Interval::Interval(const Interval& other)
{
  _step = other._step;
  _interval = other._interval;
}

int Interval::getStep(const Interval& other) const { return boost::math::lcm(_step, other._step); }

int Interval::getLowerBound(const Interval& other, int new_step) const
{
  int lower_a = _interval.lower();
  int lower_b = other._interval.lower();
  if (lower_a == lower_b) {
    return lower_a;
  }
  int lower = lower_a;
  int lower_step = _step;
  int bigger = lower_b;
  int bigger_step = other._step;
  if (lower_b < lower) {
    lower = lower_b;
    lower_step = other._step;
    bigger = lower_a;
    bigger_step = _step;
  }
  int end_search = bigger + new_step;
  int N = (bigger - lower) / lower_step;
  int current_lower = lower + lower_step * N;
  int current_upper = bigger;
  while (current_lower < end_search) {
    if (current_lower == current_upper) {
      return current_lower;
    }
    // Advance one step in lower.
    current_lower += lower_step;
    // Advance one or more step in bigger if needed.
    while (current_lower > current_upper) {
      current_upper += bigger_step;
    }
  }
  return -1;
}

int Interval::getUpperBound(int new_begin, int new_step, int upper) const
{
  int N = (upper - new_begin) / new_step;
  return new_begin + N * new_step;
}

bool Interval::checkStepIntersection(const Interval& other) const
{
  int lower_a = _interval.lower();
  int lower_b = other._interval.lower();
  bool no_common_div = boost::math::gcd(_step, other._step) == 1;
  bool lower_even = isEven(lower_a) || isEven(lower_b);
  bool lower_odd = isOdd(lower_a) || isOdd(lower_b);
  bool new_step_even = isEven(getStep(other));
  return !(no_common_div && lower_even && lower_odd && new_step_even);
}

Interval Interval::operator&(const Interval& other) const
{
  DiscreteInterval intersection = _interval & other._interval;
  // Intersection without steps.
  if (_step == 1 && other._step == 1) {
    return Interval(intersection);
  }
  if (!checkStepIntersection(other)) {
    return Interval();
  }
  int new_step = getStep(other);
  assert(new_step > 0);
  int lower_bound = getLowerBound(other, new_step);
  if (lower_bound == -1) {
    return Interval();
  }
  if (lower_bound + new_step > intersection.upper()) {
    return Interval(lower_bound, lower_bound);
  }
  int upper_bound = getUpperBound(lower_bound, new_step, intersection.upper());
  return Interval(lower_bound, upper_bound, new_step);
}

bool Interval::operator<(const Interval& other) const { return _interval < other._interval; }

bool Interval::operator==(const Interval& other) const { return this->_interval == other._interval; };

bool Interval::operator!=(const Interval& other) const { return this->_interval != other._interval; };

int Interval::lower() const { return _interval.lower(); }

int Interval::upper() const { return _interval.upper(); }

int Interval::size() const { return ICL::size(_interval); }

bool Interval::contains(const Interval& other) const { return ICL::contains(_interval, other._interval); }

bool Interval::intersects(const Interval& other) const { return ICL::intersects(_interval, other._interval); }

bool Interval::isEmpty() const { return ICL::is_empty(_interval); }

int Interval::step() const { return _step; };

bool Interval::isEven(int n) const { return (n % 2) == 0; }

bool Interval::isOdd(int n) const { return !isEven(n); }

/* SBGraph Usage interface */

Usage::Usage() : _usage(){};

Usage::Usage(int size) : _usage(std::vector<int>(size)) {}

Usage::Usage(int size, int value) : _usage(std::vector<int>(size, value)) {}

int& Usage::operator[](int index) { return _usage[index]; }

const int& Usage::operator[](int index) const { return _usage[index]; }

void Usage::push_back(const int i) { _usage.push_back(i); }

bool Usage::operator==(const Usage& other) const { return this->_usage == other._usage; };

bool Usage::operator!=(const Usage& other) const { return this->_usage != other._usage; };

int Usage::size() { return _usage.size(); }

bool Usage::isUnused()
{
  for (int i : _usage) {
    if (i != -1) return false;
  }
  return true;
}

bool Usage::isUsed(int i)
{
  if (_usage.size() > 0) {
    return _usage[i] != -1;
  } else {
    return false;
  }
  return false;
}

void Usage::join(Usage usage)
{
  iterator it;
  for (it = usage.begin(); it != usage.end(); it++) {
    _usage.push_back(*it);
  }
}

Usage::const_iterator Usage::begin() const { return _usage.begin(); }

Usage::iterator Usage::begin() { return _usage.begin(); }

Usage::iterator Usage::end() { return _usage.end(); }

/* SBGraph Offset interface */

Offset::Offset(std::vector<int> offset) : _offset(offset){};

Offset::Offset() : _offset(){};

bool Offset::operator<(const Offset& other) const { return this->_offset < other._offset; };

bool Offset::operator==(const Offset& other) const { return this->_offset == other._offset; };

bool Offset::operator!=(const Offset& other) const { return this->_offset != other._offset; };

int Offset::operator[](const int& index) const { return _offset[index]; };

bool Offset::isZeros()
{
  for (int i : _offset) {
    if (i != 0) return false;
  }
  return true;
}

Offset Offset::operator-() const
{
  std::vector<int> ret(_offset.size());
  for (int i = 0; i < (int)_offset.size(); i++) {
    ret[i] = -_offset[i];
  }
  return ret;
};

Offset Offset::operator-(const Offset& other) const
{
  // cout << size() << " " << other.size() << endl;
  assert(size() == other.size());
  std::vector<int> ret(_offset.size());
  for (int i = 0; i < (int)_offset.size(); i++) {
    ret[i] = _offset[i] - other._offset[i];
  }
  return ret;
};

Offset Offset::operator+(const Offset& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_offset.size());
  for (int i = 0; i < (int)_offset.size(); i++) {
    ret[i] = _offset[i] + other._offset[i];
  }
  return ret;
};

Offset Offset::operator*(const Offset& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_offset.size());
  for (int i = 0; i < (int)_offset.size(); i++) {
    ret[i] = _offset[i] * other._offset[i];
  }
  return ret;
};

Offset Offset::operator/(const Offset& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_offset.size());
  for (int i = 0; i < (int)_offset.size(); i++) {
    if (other._offset[i] == 0) {
      ret[i] = 0;
    } else {
      ret[i] = _offset[i] / other._offset[i];
    }
  }
  return ret;
};

Offset::const_iterator Offset::begin() const { return _offset.begin(); }

Offset::iterator Offset::begin() { return _offset.begin(); }

Offset::iterator Offset::end() { return _offset.end(); }

unsigned int Offset::size() const { return _offset.size(); }

void Offset::add(int offset) { _offset.push_back(offset); }

std::ostream& operator<<(std::ostream& os, const Offset& offset)
{
  list<string> offsets;
  int size = offset.size();
  for (int i = 0; i < size; i++) {
    stringstream buffer;
    buffer << offset[i];
    offsets.push_back(buffer.str());
  }
  os << boost::algorithm::join(offsets, ",");
  return os;
}

/* SBGraph LinearFunction interface */

LinearFunction::LinearFunction() : _constant(), _factor() {}

LinearFunction::LinearFunction(Offset constant, Offset factor) : _constant(constant), _factor(factor)
{
  assert(_constant.size() == _factor.size());
}

LinearFunction& LinearFunction::operator=(const LinearFunction& other)
{
  _constant = other._constant;
  _factor = other._factor;

  return *this;
}

vector<string> LinearFunction::apply(vector<string> variable) const
{
  assert(_factor.size() == variable.size());
  vector<string> exps;
  int size = (int)_factor.size();
  for (int i = 0; i < size; i++) {
    stringstream code;
    if (_factor[i] != 0) {
      if (_factor[i] != 1) {
        code << _factor[i] << " * ";
      }
      code << variable[i];
      if (_constant[i] > 0) {
        code << " + ";
      }
    }
    if (_constant[i]) {
      code << _constant[i];
    }
    exps.push_back(code.str());
  }
  return exps;
}

vector<int> LinearFunction::apply(vector<int> variable) const
{
  assert(_factor.size() == variable.size());
  vector<int> exps;
  int size = (int)_factor.size();
  for (int i = 0; i < size; i++) {
    exps.push_back(_factor[i] * variable[i] + _constant[i]);
  }
  return exps;
}

int LinearFunction::apply(int value, int dim) const { return _factor[dim] * value + _constant[dim]; }

MDI LinearFunction::apply(MDI dom) const
{
  assert(dom.intervals().size() == _factor.size());
  int size = (int)_factor.size();
  IntervalVector new_ran(size);
  IntervalVector doms = dom.intervals();
  for (int i = 0; i < size; i++) {
    new_ran[i] = Interval(apply(doms[i].lower(), i), apply(doms[i].upper(), i), doms[i].step());
  }
  return MDI(new_ran);
}

Offset LinearFunction::factor() const { return _factor; }

Offset LinearFunction::constant() const { return _constant; }

std::ostream& operator<<(std::ostream& os, const LinearFunction& function)
{
  os << "Constant: " << function._constant << endl;
  os << "Factor: " << function._factor << endl;
  return os;
}

/* SBGraph Map interface */

Map::Map() : _usage(), _offset(), _exp() {}

Map::Map(Expression exp) : _usage(), _offset(), _exp(exp) {}

Map::Map(Usage usage, Offset offset, Expression exp, LinearFunction linear_function)
    : _usage(usage), _offset(offset), _exp(exp), _linear_function(linear_function)
{
}

Map& Map::operator=(const Map& other)
{
  _usage = other._usage;
  _offset = other._offset;
  _exp = other._exp;
  _linear_function = other._linear_function;
  return *this;
}

MDI Map::apply(MDI dom, MDI ran) const
{
  MDI new_dom = dom.applyUsage(_usage, ran);
  new_dom = new_dom.applyStep(ran);
  return _linear_function.apply(new_dom);
}

MDI Map::revert(MDI dom, MDI ran) const
{
  MDI mdi = ran.revertOffset(_offset);
  mdi = mdi.revertUsage(_usage, dom);
  return mdi.revertStep(dom);
}

Map Map::compose(const Map& other)
{
  Offset new_factor = other._linear_function.factor() * _linear_function.factor();
  Offset new_constant = _linear_function.factor() * other._linear_function.constant() + _linear_function.constant();
  LinearFunction new_linear_function(new_constant, new_factor);
  return Map(_usage, _offset, _exp, new_linear_function);
}

Map Map::solve(const Map& other)
{
  Offset new_factor = other._linear_function.factor() / _linear_function.factor();
  Offset new_constant = (other._linear_function.constant() - _linear_function.constant()) / _linear_function.factor();
  LinearFunction new_linear_function(new_constant, new_factor);
  return Map(_usage, _offset, _exp, new_linear_function);
}

Map Map::applyIndexShift(Offset index_shift)
{
  Offset new_constant = _linear_function.constant() - index_shift;
  LinearFunction new_linear_function(new_constant, _linear_function.factor());
  return Map(_usage, _offset, _exp, new_linear_function);
}

vector<string> Map::exps(vector<string> variables) const { return _linear_function.apply(variables); }

Expression Map::exp() const { return _exp; }

bool Map::operator==(const Map& other) const { return this->_offset == other._offset && this->_usage == other._usage; }

bool Map::operator!=(const Map& other) const { return this->_offset != other._offset || this->_usage != other._usage; }

bool Map::operator<(const Map& other) const { return this->_offset < other._offset; }

std::ostream& operator<<(std::ostream& os, const Map& map)
{
  os << "Expression: " << map._exp << endl;
  os << "Offset: " << map._offset << endl;
  os << "Linear Function: " << map._linear_function << endl;
  return os;
}

/* SBGraph Pair interface */

Pair::Pair(){};

Pair::Pair(MDI dom, MDI ran, Map map, IR::Expression exp) : _dom(dom), _ran(ran), _map(map){};

MDI Pair::dom() const { return _dom; }

MDI Pair::ran() const { return _ran; }

Map Pair::map() const { return _map; }

bool Pair::operator<(const Pair& other) const
{
  return this->dom() < other.dom() || this->ran() < other.ran() || this->map() < other.map();
}

PAIR::Rel Pair::type() const
{
  if (_dom.size() == _ran.size() && !_dom.isEmpty() && !_ran.isEmpty() && _dom.unique()) {
    return PAIR::R1_1;
  } else if (_dom.size() == _ran.size() && !_dom.isEmpty() && !_ran.isEmpty()) {
    return PAIR::RN_N;
  } else if (_dom.isEmpty() && _ran.isEmpty()) {
    return PAIR::R1_1;
  } else if (_dom.unique() && _ran.isEmpty()) {
    return PAIR::R1_1;
  } else if (_dom.isEmpty() && _ran.unique()) {
    return PAIR::R1_1;
  } else if (_dom.unique() && _ran.unique()) {
    return PAIR::R1_1;
  } else if ((_dom.size() > _ran.size()) && _ran.unique()) {
    return PAIR::RN_1;
  } else if (_dom.unique() && _ran.size() > 1) {
    return PAIR::R1_N;
  } else if (_dom.size() < _ran.size()) {
    return PAIR::RN_N;
  }
  assert(false);
}

std::ostream& operator<<(std::ostream& os, const Pair& pair)
{
  os << "(" << pair.dom() << ", " << pair.ran() << ")";
  return os;
}

/* SBGraph MDI interface */

MDI::MDI() { _intervals.resize(0); }

MDI::MDI(IntervalVector intervals) : _intervals(intervals){};

MDI::MDI(int d, ...)
{
  _intervals.resize(d);
  va_list vl;
  va_start(vl, d);
  for (int i = 0; i < d; i++) {
    int lower = va_arg(vl, int);
    int upper = va_arg(vl, int);
    _intervals[i] = Interval(lower, upper);
  }
  va_end(vl);
}

MDI::MDI(IntervalList intervals) : _intervals(IntervalVector(intervals.begin(), intervals.end())) {}

int MDI::size() const
{
  int size = 1;
  foreach_(Interval i, _intervals) { size *= i.size(); }
  return size;
}

bool MDI::operator<(const MDI& other) const { return (this->_intervals) < (other._intervals); }

bool MDI::operator==(const MDI& other) const { return this->_intervals == other._intervals; };

bool MDI::operator!=(const MDI& other) const { return !((*this) == other); };

IntervalVector MDI::intervals() { return _intervals; };

bool MDI::isEmpty() const { return _intervals.size() == 0; };

Option<MDI> MDI::intersection(MDI& other)
{
  if (_intervals.size() != other._intervals.size()) {
    return Option<MDI>();
  } else if (_intervals.size() == other._intervals.size() && _intervals.size() == 0) {
    return other;
  }
  return other & (*this);
}

MDI MDI::applyOffset(Offset offset) const
{
  if (dimension() == 0 || offset.size() == 0) {
    return *this;
  }
  IntervalVector copy_intervals = _intervals;
  for (int i = 0; i < (int)copy_intervals.size(); i++) {
    copy_intervals[i] = Interval(copy_intervals[i].lower() + offset[i], copy_intervals[i].upper() + offset[i]);
  }
  return MDI(copy_intervals);
}

MDI MDI::applyUsage(Usage usage, MDI mdi) const
{
  if (usage.size() == 0 || usage.isUnused() || dimension() == 0) {
    return mdi;
  }
  assert(usage.size() == mdi.dimension());
  IntervalVector new_intervals(usage.size());
  for (int i = 0; i < (int)usage.size(); i++) {
    if (usage[i] >= 0) {
      new_intervals[i] = _intervals[usage[i]];
    } else {
      new_intervals[i] = mdi._intervals[i];
    }
  }
  return MDI(new_intervals);
}

MDI MDI::revertOffset(Offset offset) const
{
  if (dimension() == 0 || offset.size() == 0) {
    return *this;
  }
  IntervalVector copy_intervals = _intervals;
  for (int i = 0; i < (int)copy_intervals.size(); i++) {
    copy_intervals[i] = Interval(copy_intervals[i].lower() - offset[i], copy_intervals[i].upper() - offset[i]);
  }
  return MDI(copy_intervals);
}

MDI MDI::revertUsage(Usage usage, MDI dom) const
{
  if (usage.size() == 0 || usage.isUnused() || dimension() == 0) {
    return dom;
  } else {
    IntervalVector new_intervals(usage.size());
    int usages = 0;
    for (int i = 0; i < usage.size(); i++) {
      if (usage[i] >= 0) {
        new_intervals[usage[i]] = _intervals[i];
        usages++;
      }
    }
    new_intervals.resize(usages);
    return MDI(new_intervals);
  }
}

MDI MDI::applyStep(MDI other)
{
  if (dimension() == 0) {
    return *this;
  }
  assert(dimension() == other.dimension());
  IntervalVector copy_intervals = _intervals;
  IntervalVector other_intervals = other.intervals();
  for (int i = 0; i < (int)copy_intervals.size(); i++) {
    int step = other_intervals[i].step();
    copy_intervals[i] = Interval(copy_intervals[i].lower() * step, copy_intervals[i].upper() * step);
  }
  return MDI(copy_intervals);
}

MDI MDI::revertStep(MDI other)
{
  if (dimension() == 0) {
    return *this;
  }
  assert(dimension() == other.dimension());
  IntervalVector copy_intervals = _intervals;
  IntervalVector other_intervals = other.intervals();
  for (int i = 0; i < (int)copy_intervals.size(); i++) {
    int step = other_intervals[i].step();
    copy_intervals[i] = Interval(copy_intervals[i].lower() / step, copy_intervals[i].upper() / step);
  }
  return MDI(copy_intervals);
}

Option<MDI> MDI::operator&(const MDI& other) const
{
  if (this->dimension() != other.dimension()) {
    // std::cout << *this << " " << other << std::endl;
    // std::cout << this->dimension() << " " << other.dimension() << std::endl;
    return Option<MDI>();
  }
  IntervalList intersection;
  for (int i = 0; i < dimension(); i++) {
    // If i-th interval does not intersect with its corresponding interval in
    // the other MDI: return an empty MDI
    if (!_intervals[i].intersects(other._intervals[i])) {
      return Option<MDI>();
    } else {
      Interval possible_intersection = (this->_intervals[i]) & (other._intervals[i]);
      if (possible_intersection.isEmpty()) {
        return Option<MDI>();
      }
      intersection.push_back(possible_intersection);
    }
  }
  // All intervals intersect with its corresponding interval in the other MDI:
  // return the resulting intersection MDI
  return MDI(intersection);
}

bool MDI::unique() const { return size() == 1; }

bool MDI::reduction(MDI other)
{
  if (unique()) {
    return false;
  }
  if (other.unique()) {
    return true;
  }
  return false;
}

int MDI::dimension() const { return _intervals.size(); }

std::ostream& operator<<(std::ostream& os, const MDI mdi)
{
  list<string> mdis;
  for (Interval x : mdi._intervals) {
    stringstream buffer;
    if (x.lower() == x.upper()) {
      buffer << "[" << x.lower() << "]";
    } else {
      buffer << "[" << x.lower() << ":" << x.upper() << "]";
    }
    mdis.push_back(buffer.str());
  }
  os << "<" << boost::algorithm::join(mdis, ",") << ">";
  return os;
}

}  // namespace SBG
}  // namespace Deps
}  // namespace MicroModelica