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

#ifndef UTILS_H_
#define UTILS_H_

#include <QString>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>

/**
 *
 */
typedef enum
{
  MMOC_MODELS,   //!< MMOC_MODELS
  MMOC_OUTPUT,   //!< MMOC_OUTPUT
  MMOC_BUILD,    //!< MMOC_BUILD
  MMOC_ENGINE,   //!< MMOC_ENGINE
  MMOC_LIBS,     //!< MMOC_LIBS
  MMOC_BIN,      //!< MMOC_BIN
  MMOC_SOURCE,   //!< MMOC_SOURCE
  MMOC_PACKAGES, //!< MMOC_PACKAGES
  MMOC_LIBRARIES, //!< MMOC_LIBRARIES
  MMOC_INCLUDE   //!< MMOC_INCLUDE
} AppDirs;

/**
 *
 */
typedef enum
{
  FLG_DEBUG,     //!< FLG_DEBUG
  FLG_DBG_ALL,   //!< FLG_DEBUG_ALL
  FLG_DBG_INIT_VALUES,   //!< FLG_DBG_INIT_VALUES
  FLG_DBG_EXTERNAL_EVENT,   //!< FLG_DBG_EXTERNAL_EVENT
  FLG_DBG_MEMORY,   //!< FLG_DBG_MEMORY
  FLG_DBG_SYNCHRONIZE,   //!< FLG_DEBUG_SYNCHRONIZE
  FLG_DBG_STEP_INFO,   //!< FLG_DBG_STEP_INFO
  FLG_DBG_VAR_CHANGES,   //!< FLG_DEBUG_VAR_CHANGES
  FLG_DBG_WAIT_FOR,   //!< FLG_DBG_WAIT_FOR
  FLG_DBG_WEIGHTS,   //!< FLG_DBG_WEIGHTS
  FLG_DBG_DT,   //!< FLG_DBG_DT
  FLG_FLAGS,    //!< FLG_FLAGS
  FLG_PLOT,      //!< FLG_PLOT
  FLG_VERSION,      //!< FLG_VERSION
  FLG_REVISION      //!< FLG_REVISION
} AppFlags;

/**
 *
 */
typedef enum
{
  CMD_COMPILE, //!< CMD_COMPILE
  CMD_BUILD,   //!< CMD_BUILD
  CMD_SIMULATE,   //!< CMD_SIMULATE
  CMD_PLOT,    //!< CMD_PLOT
  CMD_SBML,     //!< CMD_SBML
  CMD_LOG       //!< CMD_LOG
} AppCmds;

/**
 *
 */
class Utils
{
  public:
    /**
     *
     */
    Utils()
    {
    }
    ;
    /**
     *
     */
    ~Utils()
    {
    }
    ;
    /**
     *
     * @param c
     * @return
     */
    QString
    appCommand(AppCmds c);
    /**
     *
     * @param d
     * @return
     */
    QString
    appDir(AppDirs d);
    /**
     *
     * @param f
     * @return
     */
    QString
    appFlag(AppFlags f);
    /**
     *
     * @param tok
     * @return
     */
    QString
    defaultValue(QString tok);
    /**
     *
     * @param f
     * @return
     */
    bool
    isSet(AppFlags f);
    /**
     *
     * @param d
     * @return
     */
    QString
    relativePath(AppDirs d);
    /**
     *
     * @param c
     * @param value
     */
    void
    setCommand(AppCmds c, QString value);
    /**
     *
     * @param d
     * @param value
     */
    void
    setDir(AppDirs d, QString value);
    /**
     *
     * @param f
     * @param value
     */
    void
    setFlag(AppFlags f, QString value);
    private:
    QString
    _getDir(AppDirs d);
};

#endif  /* UTILS_H_ */
