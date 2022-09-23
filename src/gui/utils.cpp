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

#include "utils.h"

QString Utils::appCommand(AppCmds c)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  QString cmd;
  switch (c) {
  case CMD_COMPILE:
    cmd = settings.value("Commands/comp", "Value not found in file qss-solver.ini").toString();
    break;
  case CMD_BUILD:
    cmd = settings.value("Commands/build", "Value not found in file qss-solver.ini").toString();
    break;
  case CMD_SIMULATE:
    cmd = settings.value("Commands/simulate", "Value not found in file qss-solver.ini").toString();
    break;
  case CMD_PLOT:
    cmd = settings.value("Commands/plot", "Value not found in file qss-solver.ini").toString();
    break;
  case CMD_SBML:
    cmd = settings.value("Commands/sbml", "Value not found in file qss-solver.ini").toString();
    break;
  case CMD_LOG:
    cmd = settings.value("Commands/log", "Value not found in file qss-solver.ini").toString();
    break;
  default:
    return QString();
  }
  return cmd;
}

QString Utils::appDir(AppDirs d)
{
  QDir dir(QCoreApplication::applicationDirPath());
  dir.cd(_getDir(d));
  return dir.absolutePath();
}

bool Utils::isSet(AppFlags f)
{
  QString _val;
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  switch (f) {
  case FLG_FLAGS:
    _val = settings.value("Flags/flags", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DEBUG:
    _val = settings.value("Flags/dbgflags", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_ALL:
    _val = settings.value("Flags/dbgAll", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_INIT_VALUES:
    _val = settings.value("Flags/dbgInitValues", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_EXTERNAL_EVENT:
    _val = settings.value("Flags/dbgExternalEvent", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_MEMORY:
    _val = settings.value("Flags/dbgMemory", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_SYNCHRONIZE:
    _val = settings.value("Flags/dbgSynchronize", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_STEP_INFO:
    _val = settings.value("Flags/dbgStepInfo", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_VAR_CHANGES:
    _val = settings.value("Flags/dbgVarChanges", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_WAIT_FOR:
    _val = settings.value("Flags/dbgWaitFor", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_WEIGHTS:
    _val = settings.value("Flags/dbgWeights", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  case FLG_DBG_DT:
    _val = settings.value("Flags/dbgDt", "Value not found in file qss-solver.ini").toString();
    return _val == "true";
  default:
    return _val.isEmpty();
  }
  return _val.isEmpty();
}

QString Utils::_getDebugValue()
{
  QString dbg;
  if (isSet(FLG_DBG_ALL)) {
    dbg += "-d SD_DBG_All ";
  }
  if (isSet(FLG_DBG_INIT_VALUES)) {
    dbg += "-d SD_DBG_InitValues ";
  }
  if (isSet(FLG_DBG_EXTERNAL_EVENT)) {
    dbg += "-d SD_DBG_ExternalEvent ";
  }
  if (isSet(FLG_DBG_MEMORY)) {
    dbg += "-d SD_DBG_Memory ";
  }
  if (isSet(FLG_DBG_SYNCHRONIZE)) {
    dbg += "-d SD_DBG_Synchronize ";
  }
  if (isSet(FLG_DBG_STEP_INFO)) {
    dbg += "-d SD_DBG_StepInfo ";
  }
  if (isSet(FLG_DBG_VAR_CHANGES)) {
    dbg += "-d SD_DBG_VarChanges ";
  }
  if (isSet(FLG_DBG_WAIT_FOR)) {
    dbg += "-d SD_DBG_WaitFor ";
  }
  if (isSet(FLG_DBG_WEIGHTS)) {
    dbg += "-d SD_DBG_Weights ";
  }
  if (isSet(FLG_DBG_DT)) {
    dbg += "-d SD_DBG_Dt ";
  }
  return dbg;
}

QString Utils::appFlag(AppFlags f)
{
  QString _val;
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  switch (f) {
  case FLG_FLAGS:
    _val = settings.value("Flags/flags", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DEBUG:
    _val = _getDebugValue();
    break;
  case FLG_DBG_ALL:
    _val = settings.value("Flags/dbgAll", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_INIT_VALUES:
    _val = settings.value("Flags/dbgInitValues", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_EXTERNAL_EVENT:
    _val = settings.value("Flags/dbgExternalEvent", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_MEMORY:
    _val = settings.value("Flags/dbgMemory", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_SYNCHRONIZE:
    _val = settings.value("Flags/dbgSynchronize", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_STEP_INFO:
    _val = settings.value("Flags/dbgStepInfo", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_VAR_CHANGES:
    _val = settings.value("Flags/dbgVarChanges", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_WAIT_FOR:
    _val = settings.value("Flags/dbgWaitFor", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_WEIGHTS:
    _val = settings.value("Flags/dbgWeights", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_DBG_DT:
    _val = settings.value("Flags/dbgDt", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_PLOT:
    _val = settings.value("Flags/plotflags", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_VERSION:
    _val = settings.value("Flags/version", "Value not found in file qss-solver.ini").toString();
    break;
  case FLG_BRANCH:
    _val = settings.value("Flags/branch", "Value not found in file qss-solver.ini").toString();
    break;
  default:
    break;
  }
  return _val;
}

QString Utils::relativePath(AppDirs d) { return _getDir(d); }

QString Utils::_getDir(AppDirs d)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  QString path;
  switch (d) {
  case MMOC_SOURCE:
    path = settings.value("src", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_MODELS:
    path = settings.value("models", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_OUTPUT:
    path = settings.value("output", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_BUILD:
    path = settings.value("build", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_ENGINE:
    path = settings.value("engine", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_LIBS:
    path = settings.value("libs", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_BIN:
    path = settings.value("bin", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_PACKAGES:
    path = settings.value("packages", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_LIBRARIES:
    path = settings.value("libraries", "Value not found in file qss-solver.ini").toString();
    break;
  case MMOC_INCLUDE:
    path = settings.value("include", "Value not found in file qss-solver.ini").toString();
    break;
  default:
    return QString();
  }
  return path;
}

void Utils::setCommand(AppCmds c, QString value)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  switch (c) {
  case CMD_COMPILE:
    settings.setValue("Commands/comp", value);
    break;
  case CMD_BUILD:
    settings.setValue("Commands/build", value);
    break;
  case CMD_SIMULATE:
    settings.setValue("Commands/simulate", value);
    break;
  case CMD_PLOT:
    settings.setValue("Commands/plot", value);
    break;
  case CMD_SBML:
    settings.setValue("Commands/sbml", value);
    break;
  case CMD_LOG:
    settings.setValue("Commands/log", value);
    break;
  default:
    break;
  }
}

void Utils::setDir(AppDirs d, QString value)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  switch (d) {
  case MMOC_SOURCE:
    settings.setValue("src", value);
    break;
  case MMOC_MODELS:
    settings.setValue("models", value);
    break;
  case MMOC_OUTPUT:
    settings.setValue("output", value);
    break;
  case MMOC_BUILD:
    settings.setValue("build", value);
    break;
  case MMOC_ENGINE:
    settings.setValue("engine", value);
    break;
  case MMOC_LIBS:
    settings.setValue("libs", value);
    break;
  case MMOC_BIN:
    settings.setValue("bin", value);
    break;
  case MMOC_PACKAGES:
    settings.setValue("packages", value);
    break;
  case MMOC_LIBRARIES:
    settings.setValue("libraries", value);
    break;
  case MMOC_INCLUDE:
    settings.setValue("include", value);
    break;
  default:
    break;
  }
}

void Utils::setFlag(AppFlags f, QString value)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  switch (f) {
  case FLG_FLAGS:
    settings.setValue("Flags/flags", value);
    break;
  case FLG_DBG_ALL:
    settings.setValue("Flags/dbgAll", value);
    break;
  case FLG_DBG_INIT_VALUES:
    settings.setValue("Flags/dbgInitValues", value);
    break;
  case FLG_DBG_EXTERNAL_EVENT:
    settings.setValue("Flags/dbgExternalEvent", value);
    break;
  case FLG_DBG_MEMORY:
    settings.setValue("Flags/dbgMemory", value);
    break;
  case FLG_DBG_SYNCHRONIZE:
    settings.setValue("Flags/dbgSynchronize", value);
    break;
  case FLG_DBG_STEP_INFO:
    settings.setValue("Flags/dbgStepInfo", value);
    break;
  case FLG_DBG_VAR_CHANGES:
    settings.setValue("Flags/dbgVarChanges", value);
    break;
  case FLG_DBG_WAIT_FOR:
    settings.setValue("Flags/dbgWaitFor", value);
    break;
  case FLG_DBG_WEIGHTS:
    settings.setValue("Flags/dbgWeights", value);
    break;
  case FLG_DBG_DT:
    settings.setValue("Flags/dbgDt", value);
    break;
  case FLG_PLOT:
    settings.setValue("Flags/plotflags", value);
    break;
  default:
    break;
  }
}
