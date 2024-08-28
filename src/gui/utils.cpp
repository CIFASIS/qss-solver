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

QString Utils::appCommand(AppCmds cmd)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  return settings.value(appCommandsName(cmd), "Value not found in file qss-solver.ini").toString();
}

QString Utils::appDir(AppDirs d)
{
  QDir dir(QCoreApplication::applicationDirPath());
  dir.cd(getDir(d));
  return dir.absolutePath();
}

bool Utils::isSet(AppFlags flag)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  QString val = settings.value(appFlagName(flag), "Value not found in file qss-solver.ini").toString();
  return val == "true";
}

QString Utils::getDebugValue()
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

QString Utils::appFlag(AppFlags flag)
{
  QString val;
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  if (flag == FLG_DEBUG) {
    val = getDebugValue();
  } else {
    val = settings.value(appFlagName(flag), "Value not found in file qss-solver.ini").toString();
  }
  return val;
}

QString Utils::relativePath(AppDirs d) { return getDir(d); }

QString Utils::getDir(AppDirs dir)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  QString path = settings.value(appDirsName(dir), "Value not found in file qss-solver.ini").toString();
  return path;
}

void Utils::setCommand(AppCmds cmd, QString value)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  settings.setValue(appCommandsName(cmd), value);
}

QString Utils::appFlagName(AppFlags flag)
{
  QString flag_name("Flags/");
  switch (flag) {
  case FLG_FLAGS:
    flag_name.append("flags");
    break;
  case FLG_DBG_ALL:
    flag_name.append("dbgAll");
    break;
  case FLG_DBG_INIT_VALUES:
    flag_name.append("dbgInitValues");
    break;
  case FLG_DBG_EXTERNAL_EVENT:
    flag_name.append("dbgExternalEvent");
    break;
  case FLG_DBG_MEMORY:
    flag_name.append("dbgMemory");
    break;
  case FLG_DBG_SYNCHRONIZE:
    flag_name.append("dbgSynchronize");
    break;
  case FLG_DBG_STEP_INFO:
    flag_name.append("dbgStepInfo");
    break;
  case FLG_DBG_VAR_CHANGES:
    flag_name.append("dbgVarChanges");
    break;
  case FLG_DBG_WAIT_FOR:
    flag_name.append("dbgWaitFor");
    break;
  case FLG_DBG_WEIGHTS:
    flag_name.append("dbgWeights");
    break;
  case FLG_DBG_DT:
    flag_name.append("dbgDt");
    break;
  case FLG_PLOT:
    flag_name.append("plotflags");
    break;
  default:
    break;
  }
  return flag_name;
}

QString Utils::appCommandsName(AppCmds cmd)
{
  QString command_name("Commands/");
  switch (cmd) {
  case CMD_COMPILE:
    command_name.append("comp");
    break;
  case CMD_BUILD:
    command_name.append("build");
    break;
  case CMD_SIMULATE:
    command_name.append("simulate");
    break;
  case CMD_PLOT:
    command_name.append("plot");
    break;
  case CMD_SBML:
    command_name.append("sbml");
    break;
  case CMD_LOG:
    command_name.append("log");
    break;
  default:
    break;
  }
  return command_name;
}

QString Utils::appDirsName(AppDirs dir)
{
  QString dir_name;
  switch (dir) {
  case MMOC_SOURCE:
    dir_name = "src";
    break;
  case MMOC_MODELS:
    dir_name = "models";
    break;
  case MMOC_OUTPUT:
    dir_name = "output";
    break;
  case MMOC_BUILD:
    dir_name = "build";
    break;
  case MMOC_ENGINE:
    dir_name = "engine";
    break;
  case MMOC_LIBS:
    dir_name = "libs";
    break;
  case MMOC_BIN:
    dir_name = "bin";
    break;
  case MMOC_PACKAGES:
    dir_name = "packages";
    break;
  case MMOC_LIBRARIES:
    dir_name = "libraries";
    break;
  case MMOC_INCLUDE:
    dir_name = "include";
    break;
  default:
    break;
  }
  return dir_name;
}

void Utils::setDir(AppDirs dir, QString value)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  settings.setValue(appDirsName(dir), value);
}

void Utils::setFlag(AppFlags flag, QString value)
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  settings.setValue(appFlagName(flag), value);
}
