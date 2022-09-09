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

#include <QtGui>

class ModelInfo {
  public:
  ModelInfo(QString fullname = QString());
  ~ModelInfo() = default;
  QString baseName();
  bool dirty() { return _dirty; };
  QString fullname() { return _fullname; };
  bool init() { return _init; };
  QString name() const;
  QString path();
  void setDirty(bool d) { _dirty = d; };
  void setFullname(QString fn) { _fullname = fn; };
  void setInit(bool d) { _init = d; };
  bool isLogFile() const;

  private:
  bool _dirty;
  QString _fullname;
  bool _init;
};
