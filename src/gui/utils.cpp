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

#include <settings-values.hpp>
#include "utils.hpp"

Utils::Utils() { initialize(); }

void Utils::initialize() { migrateSettings(); }

void Utils::migrateSettings()
{
  auto migrate = _settings.value("Misc/migrate_settings", true).toBool();
  if (migrate) {
    QSettings init_settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);

    for (auto i = dir_map.cbegin(), end = dir_map.cend(); i != end; ++i) {
      QString val = init_settings.value(appDirsName(i.key()), "").toString();
      if (!val.isEmpty()) {
        setDir(i.key(), val);
      }
    }

    for (auto i = command_map.cbegin(), end = command_map.cend(); i != end; ++i) {
      QString val = init_settings.value(appCommandsName(i.key()), "").toString();
      if (!val.isEmpty()) {
        setCommand(i.key(), val);
      }
    }

    for (auto i = flag_map.cbegin(), end = flag_map.cend(); i != end; ++i) {
      if (i.key() != FLG_DEBUG) {
        QString val = init_settings.value(appFlagName(i.key()), "").toString();
        if (!val.isEmpty()) {
          setFlag(i.key(), val);
        }
      }
    }

    QStringList files = init_settings.value("Editor/recentFileList").toStringList();
    _settings.setValue("Editor/recentFileList", files);

    _settings.setValue("Misc/migrate_settings", false);
  }
}

QString Utils::appCommand(AppCmds cmd)
{
  QString default_value = command_default_map.value(cmd, "");
  return _settings.value(appCommandsName(cmd), default_value).toString();
}

QString Utils::appDir(AppDirs d)
{
  QDir dir(QCoreApplication::applicationDirPath());
  dir.cd(getDir(d));
  return dir.absolutePath();
}

bool Utils::isSet(AppFlags flag)
{
  QString default_value = flag_default_map.value(flag, "");
  QString val = _settings.value(appFlagName(flag), default_value).toString();
  return val == "true";
}

QString Utils::getDebugValue()
{
  QString dbg;
  for (const auto& flag : debug_flags.keys()) {
    if (isSet(flag)) {
      dbg += "-d " + debug_flags.value(flag) + " ";
    }
  }
  return dbg;
}

QString Utils::appFlag(AppFlags flag)
{
  QString val;
  if (flag == FLG_DEBUG) {
    val = getDebugValue();
  } else if (flag == FLG_VERSION) {
    return QCoreApplication::applicationVersion();
  } else {
    QString default_value = flag_default_map.value(flag, "");
    val = _settings.value(appFlagName(flag), default_value).toString();
  }
  return val;
}

QString Utils::relativePath(AppDirs d) { return getDir(d); }

QString Utils::getDir(AppDirs dir)
{
  QString default_value = dir_default_map.value(dir, "");
  return _settings.value(appDirsName(dir), default_value).toString();
}

void Utils::setCommand(AppCmds cmd, QString value) { _settings.setValue(appCommandsName(cmd), value); }

QString Utils::appFlagName(AppFlags flag) { return "Flags/" + flag_map.value(flag, ""); }

QString Utils::appCommandsName(AppCmds cmd) { return "Commands/" + command_map.value(cmd, ""); }

QString Utils::appDirsName(AppDirs dir) { return dir_map.value(dir, ""); }

void Utils::setDir(AppDirs dir, QString value) { _settings.setValue(appDirsName(dir), value); }

void Utils::setFlag(AppFlags flag, QString value) { _settings.setValue(appFlagName(flag), value); }
