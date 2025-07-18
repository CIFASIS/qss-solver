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

#include <utils.hpp>

static const QMap<AppFlags, QString> debug_flags = {
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

static const QMap<AppFlags, QString> flag_map = {
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

static const QMap<AppCmds, QString> command_map = {
    {CMD_COMPILE, "comp"},
    {CMD_BUILD, "build"},
    {CMD_SIMULATE, "simulate"},
    {CMD_PLOT, "plot"},
    {CMD_SBML, "sbml"},
    {CMD_LOG, "log"}
};

static const QMap<AppDirs, QString> dir_map = {
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


// Set defaults for each section.

static const QMap<AppFlags, QString> flag_default_map = {
    {FLG_FLAGS, "false"},
    {FLG_DBG_ALL, "false"},
    {FLG_DBG_INIT_VALUES, "false"},
    {FLG_DBG_EXTERNAL_EVENT, "false"},
    {FLG_DBG_MEMORY, "false"},
    {FLG_DBG_SYNCHRONIZE, "false"},
    {FLG_DBG_STEP_INFO, "false"},
    {FLG_DBG_VAR_CHANGES, "false"},
    {FLG_DBG_WAIT_FOR, "false"},
    {FLG_DBG_WEIGHTS, "false"},
    {FLG_DBG_DT, "false"},
    {FLG_PLOT, ""}
};

static const QMap<AppCmds, QString> command_default_map = {
    {CMD_COMPILE, "mmoc.sh"},
    {CMD_BUILD, "build.sh"},
    {CMD_SIMULATE, "simulate.sh"},
    {CMD_PLOT, "gnuplot"},
    {CMD_SBML, "sbml.sh"},
    {CMD_LOG, "createLog.sh"}
};

static const QMap<AppDirs, QString> dir_default_map = {
    {MMOC_SOURCE, "../src"},
    {MMOC_MODELS, "../models"},
    {MMOC_OUTPUT, "../output"},
    {MMOC_BUILD, "../build"},
    {MMOC_ENGINE, "../src/engine"},
    {MMOC_LIBS, "../lib"},
    {MMOC_BIN, "../bin"},
    {MMOC_PACKAGES, "../packages"},
    {MMOC_LIBRARIES, "../lib"},
    {MMOC_INCLUDE, "../src/usr/include"}
};
