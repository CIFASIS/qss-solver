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

#include <QString>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>

typedef enum {
  MMOC_MODELS,
  MMOC_OUTPUT,
  MMOC_BUILD,
  MMOC_ENGINE,
  MMOC_LIBS,
  MMOC_BIN,
  MMOC_SOURCE,
  MMOC_PACKAGES,
  MMOC_LIBRARIES,
  MMOC_INCLUDE
} AppDirs;

typedef enum {
  FLG_DEBUG,
  FLG_DBG_ALL,
  FLG_DBG_INIT_VALUES,
  FLG_DBG_EXTERNAL_EVENT,
  FLG_DBG_MEMORY,
  FLG_DBG_SYNCHRONIZE,
  FLG_DBG_STEP_INFO,
  FLG_DBG_VAR_CHANGES,
  FLG_DBG_WAIT_FOR,
  FLG_DBG_WEIGHTS,
  FLG_DBG_DT,
  FLG_FLAGS,
  FLG_PLOT,
  FLG_VERSION,
  FLG_BRANCH
} AppFlags;

typedef enum { CMD_COMPILE, CMD_BUILD, CMD_SIMULATE, CMD_PLOT, CMD_SBML, CMD_LOG } AppCmds;

class Utils {
  public:
  ~Utils() = default;
  QString appCommand(AppCmds c);
  QString appDir(AppDirs d);
  QString appFlag(AppFlags f);
  QString defaultValue(QString tok);
  bool isSet(AppFlags f);
  QString relativePath(AppDirs d);
  void setCommand(AppCmds c, QString value);
  void setDir(AppDirs d, QString value);
  void setFlag(AppFlags f, QString value);

  private:
  QString _getDir(AppDirs d);
  QString _getDebugValue();
};
