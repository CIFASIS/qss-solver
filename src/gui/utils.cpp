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

/**
 * @brief Retrieves the application command associated with the specified command enum.
 * 
 * @param cmd The command enum for which to retrieve the associated command string.
 * @return QString The command string associated with the specified command enum.
 */
QString Utils::appCommand(AppCmds cmd)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
    return settings.value(appCommandsName(cmd), "Value not found in file qss-solver.ini").toString();
}

/**
 * @brief Retrieves the absolute path of the specified application directory.
 * 
 * @param d The directory enum for which to retrieve the absolute path.
 * @return QString The absolute path of the specified directory.
 */
QString Utils::appDir(AppDirs d)
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd(getDir(d));
    return dir.absolutePath();
}

/**
 * @brief Checks if the specified application flag is set in the settings.
 * 
 * @param flag The flag enum to check.
 * @return true If the flag is set to true.
 * @return false If the flag is not set or is set to false.
 */
bool Utils::isSet(AppFlags flag)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
    QString val = settings.value(appFlagName(flag), "Value not found in file qss-solver.ini").toString();
    return val == "true";
}

/**
 * @brief Constructs a string of debug flags based on the current settings.
 * 
 * @return QString A string containing the debug flags that are set.
 */
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

/**
 * @brief Retrieves the value of the specified application flag.
 * 
 * @param flag The flag enum for which to retrieve the value.
 * @return QString The value of the specified flag.
 */
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

/**
 * @brief Retrieves the relative path of the specified application directory.
 * 
 * @param d The directory enum for which to retrieve the relative path.
 * @return QString The relative path of the specified directory.
 */
QString Utils::relativePath(AppDirs d) { return getDir(d); }

/**
 * @brief Retrieves the absolute path of the specified application directory.
 * 
 * @param dir The directory enum for which to retrieve the absolute path.
 * @return QString The absolute path of the specified directory.
 */
QString Utils::getDir(AppDirs dir)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
    QString path = settings.value(appDirsName(dir), "Value not found in file qss-solver.ini").toString();
    return path;
}

/**
 * @brief Sets the application command associated with the specified command enum.
 * 
 * @param cmd The command enum for which to set the associated command string.
 * @param value The command string to set.
 */
void Utils::setCommand(AppCmds cmd, QString value)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
    settings.setValue(appCommandsName(cmd), value);
}

/**
 * @brief Retrieves the name of the application flag based on the specified flag enum.
 * 
 * @param flag The flag enum for which to retrieve the name.
 * @return QString The name of the specified flag.
 */
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

/**
 * @brief Retrieves the name of the application command based on the specified command enum.
 * 
 * @param cmd The command enum for which to retrieve the name.
 * @return QString The name of the specified command.
 */
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

/**
 * @brief Retrieves the name of the application directory based on the specified directory enum.
 * 
 * @param dir The directory enum for which to retrieve the name.
 * @return QString The name of the specified directory.
 */
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

/**
 * @brief Sets the path of the specified application directory in the settings.
 * 
 * @param dir The directory enum for which to set the path.
 * @param value The path to set for the specified directory.
 */
void Utils::setDir(AppDirs dir, QString value)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
    settings.setValue(appDirsName(dir), value);
}

/**
 * @brief Sets the value of the specified application flag in the settings.
 * 
 * @param flag The flag enum for which to set the value.
 * @param value The value to set for the specified flag.
 */
void Utils::setFlag(AppFlags flag, QString value)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
    settings.setValue(appFlagName(flag), value);
}
