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

#include "lmap_exp.h"

#include <assert.h>
#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <list>
#include <sstream>

namespace SB{
namespace Deps {

CoeffContainer::CoeffContainer(std::vector<int> coeffs) : _coeffs(coeffs){};

CoeffContainer::CoeffContainer() : _coeffs(){};

bool CoeffContainer::operator<(const CoeffContainer& other) const { return this->_coeffs < other._coeffs; };

bool CoeffContainer::operator==(const CoeffContainer& other) const { return this->_coeffs == other._coeffs; };

bool CoeffContainer::operator!=(const CoeffContainer& other) const { return this->_coeffs != other._coeffs; };

int CoeffContainer::operator[](const int& index) const { return _coeffs[index]; };

void CoeffContainer::insert(int value) { return _coeffs.push_back(value); };

bool CoeffContainer::isZeros()
{
  for (int i : _coeffs) {
    if (i != 0) return false;
  }
  return true;
}

CoeffContainer CoeffContainer::operator-() const
{
  std::vector<int> ret(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = -_coeffs[i];
  }
  return ret;
};

CoeffContainer CoeffContainer::operator-(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = _coeffs[i] - other._coeffs[i];
  }
  return ret;
};

CoeffContainer CoeffContainer::operator+(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = _coeffs[i] + other._coeffs[i];
  }
  return ret;
};

CoeffContainer CoeffContainer::operator*(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = _coeffs[i] * other._coeffs[i];
  }
  return ret;
};

CoeffContainer CoeffContainer::operator/(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    if (other._coeffs[i] == 0) {
      ret[i] = 0;
    } else {
      ret[i] = _coeffs[i] / other._coeffs[i];
    }
  }
  return ret;
};

CoeffContainer::const_iterator CoeffContainer::begin() const { return _coeffs.begin(); }

CoeffContainer::iterator CoeffContainer::begin() { return _coeffs.begin(); }

CoeffContainer::iterator CoeffContainer::end() { return _coeffs.end(); }

CoeffContainer::const_iterator CoeffContainer::end() const { return _coeffs.end(); }

unsigned int CoeffContainer::size() const { return _coeffs.size(); }

std::ostream& operator<<(std::ostream& os, const CoeffContainer& coeffs)
{
  std::list<std::string> coeffs_str;
  int size = coeffs.size();
  for (int i = 0; i < size; i++) {
    std::stringstream buffer;
    buffer << coeffs[i];
    coeffs_str.push_back(buffer.str());
  }
  os << boost::algorithm::join(coeffs_str, ",");
  return os;
}

LMapExp::LMapExp() : _constants(), _slopes(), _init_values() {}

LMapExp::LMapExp(Constants constants, Slopes slopes, InitValues init_values) : _constants(constants), _slopes(slopes), _init_values(init_values)
{
  assert(_constants.size() == _slopes.size());
}

LMapExp& LMapExp::operator=(const LMapExp& other)
{
  _constants = other._constants;
  _slopes = other._slopes;
  _init_values = other._init_values;

  return *this;
}

bool LMapExp::operator<(const LMapExp& other) const
{
  assert(_init_values.size() == other._init_values.size());
  int size = (int)_init_values.size();
  for (int i = 0; i < size; i++) {
    if (_init_values[i] < other._init_values[i]) {
      return true;
    }
  }
  return false;
}

bool LMapExp::constantExp() const
{
  int size = (int)_slopes.size();
  for (int i = 0; i < size; i++) {
    if (_slopes[i] != 0) {
      return false;
    }
  }
  return true;
}

int LMapExp::nonZeroSlopes() const
{
  int nzl = 0;
  for (int i : _slopes) {
    if (i != 0) {
      nzl++;
    }
  }
  return nzl;
}

std::vector<std::string> LMapExp::apply(std::vector<std::string> variable) const
{
  assert(_slopes.size() == variable.size() || nonZeroSlopes() == 0);
  
  std::vector<std::string> exps;
  int size = (int)_slopes.size();
  if (size == 0) {
    return exps;
  }
  for (int i = 0; i < size; i++) {
    std::stringstream code;
    if (_slopes[i] != 0) {
      if (_slopes[i] != 1) {
        code << _slopes[i] << " * ";
      }
      code << variable[i];
      if (_constants[i] > 0) {
        code << " + ";
      }
    }
    if (_constants[i]) {
      code << _constants[i];
    }
    exps.push_back(code.str());
  }
  return exps;
}

std::vector<int> LMapExp::apply(std::vector<int> variable) const
{
  assert(_slopes.size() == variable.size());
  std::vector<int> exps;
  int size = (int)_slopes.size();
  for (int i = 0; i < size; i++) {
    exps.push_back(_slopes[i] * variable[i] + _constants[i]);
  }
  return exps;
}

int LMapExp::apply(int value, int dim) const { return _slopes[dim] * value + _constants[dim]; }

Slopes LMapExp::slopes() const { return _slopes; }

Constants LMapExp::constants() const { return _constants; }

InitValues LMapExp::initValues() const { return _init_values; }

LMapExp LMapExp::compose(const LMapExp& other)
{
  Slopes new_slopes = other.slopes() * slopes();
  Constants new_constants = slopes() * other.constants() + constants();
  return LMapExp(new_constants, new_slopes, initValues());
}

LMapExp LMapExp::solve(const LMapExp& other)
{
  Slopes new_slopes = other.slopes() / slopes();
  Constants new_constants = (other.constants() - constants()) / slopes();
  return LMapExp(new_constants, new_slopes, initValues());
}

LMapExp LMapExp::applyIndexShift(Constants index_shift)
{
  Constants new_constants = constants() - index_shift;
  return LMapExp(new_constants, slopes(), initValues());
}

std::ostream& operator<<(std::ostream& os, const LMapExp& function)
{
  os << "Constant: " << function._constants << std::endl;
  os << "Factor: " << function._slopes << std::endl;
  return os;
}

} // Namespace Deps
} // Namespace SB