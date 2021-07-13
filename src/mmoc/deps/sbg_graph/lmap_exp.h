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

#pragma once

#include <ostream>
#include <vector>

namespace SB {
namespace Deps {

class CoeffContainer {
  public:
  CoeffContainer();
  CoeffContainer(std::vector<int> CoeffContainer);
  bool operator<(const CoeffContainer& other) const;
  bool operator==(const CoeffContainer& other) const;
  bool operator!=(const CoeffContainer& other) const;
  int operator[](const int& index) const;
  void insert(int value);
  bool isZeros();
  CoeffContainer operator-() const;
  CoeffContainer operator-(const CoeffContainer& other) const;
  CoeffContainer operator+(const CoeffContainer& other) const;
  CoeffContainer operator*(const CoeffContainer& other) const;
  CoeffContainer operator/(const CoeffContainer& other) const;
  typedef std::vector<int>::iterator iterator;
  typedef std::vector<int>::const_iterator const_iterator;
  const_iterator begin() const;
  iterator begin();
  iterator end();
  unsigned int size() const;
  friend std::ostream& operator<<(std::ostream& os, const CoeffContainer& CoeffContainer);

  protected:
  std::vector<int> _coeffs;
};

typedef CoeffContainer Constants;
typedef CoeffContainer Slopes;
typedef CoeffContainer InitValues;

class LMapExp {
  public:
  LMapExp();
  LMapExp(Constants constants, Slopes slopes, InitValues init_values);
  LMapExp& operator=(const LMapExp& other);

  std::vector<std::string> apply(std::vector<std::string> variable) const;
  std::vector<int> apply(std::vector<int> variable) const;
  Slopes slopes() const;
  Constants constants() const;
  InitValues initValues() const;
  LMapExp compose(const LMapExp& other);
  LMapExp solve(const LMapExp& other);
  LMapExp applyIndexShift(CoeffContainer index_shift);
  bool constantExp() const;
  friend std::ostream& operator<<(std::ostream& os, const LMapExp& map);

  protected:
  int apply(int value, int dim) const;

  Constants _constants;
  Slopes _slopes;
  InitValues _init_values;
};

} // Namspace Deps
} // Namespace SB