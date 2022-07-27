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

namespace SB {
namespace Deps {

CoeffContainer::CoeffContainer(std::vector<int> coeffs) : _coeffs(coeffs), _valid_dims()
{
  for(size_t i = 0; i < _coeffs.size(); i++) {
    _valid_dims.push_back(true);
  }
}

CoeffContainer::CoeffContainer(std::vector<int> coeffs, std::vector<bool> valid_dims) : _coeffs(coeffs), _valid_dims(valid_dims) {}

CoeffContainer::CoeffContainer() : _coeffs(), _valid_dims() {}

CoeffContainer& CoeffContainer::operator=(const CoeffContainer&  other)
{
  _coeffs = other._coeffs;
  _valid_dims = other._valid_dims;
  return *this;
}

bool CoeffContainer::operator<(const CoeffContainer& other) const { return this->_coeffs < other._coeffs; }

bool CoeffContainer::operator==(const CoeffContainer& other) const { return this->_coeffs == other._coeffs; }

bool CoeffContainer::operator!=(const CoeffContainer& other) const { return this->_coeffs != other._coeffs; }

int CoeffContainer::operator[](const int& index) const { return _coeffs[index]; }

void CoeffContainer::insert(int value)
{
  _valid_dims.push_back(true);
  _coeffs.push_back(value);
}

void CoeffContainer::addDim()
{
  _valid_dims.push_back(false);
  _coeffs.push_back(0);
}

bool CoeffContainer::isValid(int i) const
{
  return _valid_dims[i];
}

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
  return CoeffContainer(ret, _valid_dims);
}

CoeffContainer CoeffContainer::id(int size)
{
  std::vector<int> ret(size);
  std::vector<bool> valid(size);
  for (int i = 0; i < size; i++) {
    ret[i] = 1;
    valid[i] = true;
  }
  return CoeffContainer(ret, valid);
}

CoeffContainer CoeffContainer::operator-(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  std::vector<bool> valid(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = _coeffs[i] - other._coeffs[i];
    valid[i] = _valid_dims[i] && other._valid_dims[i];
  }
  return CoeffContainer(ret, valid);
}

CoeffContainer CoeffContainer::operator+(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  std::vector<bool> valid(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = _coeffs[i] + other._coeffs[i];
    valid[i] = _valid_dims[i] && other._valid_dims[i];
  }
  return CoeffContainer(ret, valid);
}

CoeffContainer CoeffContainer::operator*(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  std::vector<bool> valid(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    ret[i] = _coeffs[i] * other._coeffs[i];
    valid[i] = _valid_dims[i] && other._valid_dims[i];
  }
  return CoeffContainer(ret, valid);
}

CoeffContainer CoeffContainer::operator/(const CoeffContainer& other) const
{
  assert(size() == other.size());
  std::vector<int> ret(_coeffs.size());
  std::vector<bool> valid(_coeffs.size());
  for (int i = 0; i < (int)_coeffs.size(); i++) {
    if (other._coeffs[i] == 0) {
      ret[i] = 0;
    } else {
      ret[i] = _coeffs[i] / other._coeffs[i];
    }
    valid[i] = _valid_dims[i] && other._valid_dims[i];
  }
  return CoeffContainer(ret, valid);
}

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
    buffer << coeffs[i] << " " << coeffs.isValid(i);
    coeffs_str.push_back(buffer.str());
  }
  os << boost::algorithm::join(coeffs_str, ",");
  return os;
}

LMapExp::LMapExp() : _constants(), _slopes(), _init_values() {}

LMapExp::LMapExp(Constants constants, Slopes slopes, InitValues init_values)
    : _constants(constants), _slopes(slopes), _init_values(init_values)
{
  assert(_constants.size() == _slopes.size());
}

bool LMapExp::isEmpty() const { return _init_values.size() == 0; }

LMapExp& LMapExp::operator=(const LMapExp& other)
{
  _constants = other._constants;
  _slopes = other._slopes;
  _init_values = other._init_values;

  return *this;
}

bool LMapExp::operator==(const LMapExp& other) const
{
  return (_constants == other._constants) && (_slopes == other._slopes) && (_init_values == other._init_values);
}

bool LMapExp::operator<(const LMapExp& other) const { return print() < other.print(); }

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

size_t LMapExp::nonZeroSlopes() const
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
  assert(_slopes.size() <= variable.size() || nonZeroSlopes() == 0 || nonZeroSlopes() == variable.size());

  std::vector<std::string> exps;
  int size = (int)_slopes.size();
  if (size == 0) {
    return exps;
  }
  for (int i = 0; i < size; i++) {
    if (!_slopes.isValid(i)) {
      continue;
    }
    std::stringstream code;
    if (_slopes[i] != 0) {
      if (_slopes[i] != 1) {
        code << _slopes[i] << " * ";
      }
      code << variable[i];
      if (_constants[i] > 0) {
        code << " + ";
      }
      if (_constants[i]) {
        code << _constants[i];
      }
    } else {
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
  if (other.isEmpty() || isEmpty()) {
    return *this;
  }
  Slopes new_slopes = other.slopes() * slopes();
  Constants new_constants = slopes() * other.constants() + constants();
  return LMapExp(new_constants, new_slopes, initValues());
}

LMapExp LMapExp::solve(const LMapExp& other)
{
  if (other.isEmpty()) {
    return *this;
  }
  Slopes new_slopes = other.slopes() / slopes();
  Constants new_constants = (slopes().isZeros()) ? other.constants() - constants() : (other.constants() - constants()) / slopes();
  return LMapExp(new_constants, new_slopes, initValues());
}

LMapExp LMapExp::revert()
{
  Slopes ids;
  Slopes new_slopes = ids.id(slopes().size()) / slopes();
  Constants new_constants = (slopes().isZeros()) ? -constants() : (-constants()) / slopes();
  return LMapExp(new_constants, new_slopes, initValues());
}

LMapExp LMapExp::applyIndexShift(Constants index_shift)
{
  Constants new_constants = constants() - index_shift;
  return LMapExp(new_constants, slopes(), initValues());
}

std::string LMapExp::print() const
{
  std::stringstream code;
  size_t size = _slopes.size();
  std::vector<std::string> variables;
  int dim = 1;
  for (size_t i = 0; i < size; i++) {
    std::stringstream var_name;
    var_name << "_d" << dim++;
    variables.push_back(var_name.str());
  }
  std::vector<std::string> exps = apply(variables);
  size = 0;
  for (std::string exp : exps) {
    code << exp << ((size++ == exps.size()) ? "" : " , ");
  }
  return code.str();
}

std::ostream& operator<<(std::ostream& os, const LMapExp& function)
{
  os << function.print();
  return os;
}

}  // Namespace Deps
}  // Namespace SB