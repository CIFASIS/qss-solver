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

#include "utils.hpp"

QString Utils::appCommand(AppCmds cmd)
{
    return _settings.value(appCommandsName(cmd), "Value not found in file qss-solver.ini").toString();
}

QString Utils::appDir(AppDirs d)
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd(getDir(d));
    return dir.absolutePath();
}

bool Utils::isSet(AppFlags flag)
{
    QString val = _settings.value(appFlagName(flag), "Value not found in file qss-solver.ini").toString();
    return val == "true";
}

QString Utils::getDebugValue()
{
    static const QMap<AppFlags, QString> debugFlags = {
        {FLG_DBG_ALL, "SD_DBG_All"},
        {FLG_DBG_INIT_VALUES, "SD_DBG_InitValues"},
        {FLG_DBG_EXTERNAL_EVENT, "SD_DBG_ExternalEvent"},
        {FLG_DBG_MEMORY, "SD_DBG_Memory"},
        {FLG_DBG_SYNCHRONIZE, "SD_DBG_Synchronize"},
        {FLG_DBG_STEP_INFO, "SD_DBG_StepInfo"},
        {FLG_DBG_VAR_CHANGES, "SD_DBG_VarChanges"},
        {FLG_DBG_WAIT_FOR, "SD_DBG_WaitFor"},
        {FLG_DBG_WEIGHTS, "SD_DBG_Weights"},
        {FLG_DBG_DT, "SD_DBG_Dt"}
    };

    QString dbg;
    for (const auto& flag : debugFlags.keys()) {
        if (isSet(flag)) {
            dbg += "-d " + debugFlags.value(flag) + " ";
        }
    }
    return dbg;
}

QString Utils::appFlag(AppFlags flag)
{
    QString val;
    if (flag == FLG_DEBUG) {
        val = getDebugValue();
    } else {
        val = _settings.value(appFlagName(flag), "Value not found in file qss-solver.ini").toString();
    }
    return val;
}

QString Utils::relativePath(AppDirs d) { return getDir(d); }

QString Utils::getDir(AppDirs dir)
{
    return _settings.value(appDirsName(dir), "Value not found in file qss-solver.ini").toString();
}

void Utils::setCommand(AppCmds cmd, QString value)
{
    _settings.setValue(appCommandsName(cmd), value);
}

QString Utils::appFlagName(AppFlags flag)
{
    static const QMap<AppFlags, QString> flagMap = {
        {FLG_FLAGS, "flags"},
        {FLG_DBG_ALL, "dbgAll"},
        {FLG_DBG_INIT_VALUES, "dbgInitValues"},
        {FLG_DBG_EXTERNAL_EVENT, "dbgExternalEvent"},
        {FLG_DBG_MEMORY, "dbgMemory"},
        {FLG_DBG_SYNCHRONIZE, "dbgSynchronize"},
        {FLG_DBG_STEP_INFO, "dbgStepInfo"},
        {FLG_DBG_VAR_CHANGES, "dbgVarChanges"},
        {FLG_DBG_WAIT_FOR, "dbgWaitFor"},
        {FLG_DBG_WEIGHTS, "dbgWeights"},
        {FLG_DBG_DT, "dbgDt"},
        {FLG_PLOT, "plotflags"}
    };
    return "Flags/" + flagMap.value(flag, "");
}

QString Utils::appCommandsName(AppCmds cmd)
{
    static const QMap<AppCmds, QString> commandMap = {
        {CMD_COMPILE, "comp"},
        {CMD_BUILD, "build"},
        {CMD_SIMULATE, "simulate"},
        {CMD_PLOT, "plot"},
        {CMD_SBML, "sbml"},
        {CMD_LOG, "log"}
    };
    return "Commands/" + commandMap.value(cmd, "");
}

QString Utils::appDirsName(AppDirs dir)
{
    static const QMap<AppDirs, QString> dirMap = {
        {MMOC_SOURCE, "src"},
        {MMOC_MODELS, "models"},
        {MMOC_OUTPUT, "output"},
        {MMOC_BUILD, "build"},
        {MMOC_ENGINE, "engine"},
        {MMOC_LIBS, "libs"},
        {MMOC_BIN, "bin"},
        {MMOC_PACKAGES, "packages"},
        {MMOC_LIBRARIES, "libraries"},
        {MMOC_INCLUDE, "include"}
    };
    return dirMap.value(dir, "");
}

void Utils::setDir(AppDirs dir, QString value)
{
    _settings.setValue(appDirsName(dir), value);
}

void Utils::setFlag(AppFlags flag, QString value)
{
    _settings.setValue(appFlagName(flag), value);
}
