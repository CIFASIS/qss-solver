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

#include <QWidget>
#include <QProcess>
#include "./ui/ui_mmomegui.h"

class ComboBoxDelegate;
class RunDlg;
class SettingsDlg;
class TreeModel;
class Utils;

class MmomeGui : public QMainWindow, public Ui::MmomeGuiForm {
  Q_OBJECT
  public:
  MmomeGui();
  ~MmomeGui();
  private slots:
  void cleanBuildDir(int);
  void done(QString name, QString ext);
  void on_actionClear_Log_triggered();
  void on_actionClear_Messages_triggered();
  void on_actionCompile_triggered();
  void on_actionDebug_triggered();
  void on_actionExit_triggered();
  void on_actionGraphics_triggered();
  void on_actionImport_triggered();
  void on_action_Load_triggered();
  void on_actionLog_triggered();
  void on_action_New_triggered();
  void on_actionRun_triggered();
  void on_actionRun_2_triggered();
  void on_action_Save_As_triggered();
  void on_actionSettings_triggered();
  void on_actionMicroModelica_Language_Scpefication_triggered();
  void on_actionQSS_Solver_Engine_Documentation_triggered();
  void on_actionMicroModelica_Compiler_Documentation_triggered();
  void on_actionSBML_Translator_Documentation_triggered();
  void on_actionAbout_triggered();
  void runDlgClose();
  void runDlgRejected();
  void comp_finished(int exitCode, QProcess::ExitStatus exitStatus);
  void importFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void make_finished(int exitCode, QProcess::ExitStatus exitStatus);
  void plot_finished(int exitCode, QProcess::ExitStatus exitStatus);
  void run_finished(int exitCode, QProcess::ExitStatus exitStatus);
  void settingsDlgClosed();
  void simulation_message();
  void openRecentFiles();
  void log_finished(int exitCode, QProcess::ExitStatus exitStatus);

  protected:
  void closeEvent(QCloseEvent *event);

  private:
  QString strippedName(const QString &fullFileName);
  void setCurrentFile(const QString fileName);
  void loadFile(QString fileName);
  void updateRecentFileActions();
  void createActions();
  void addMenuBarItems();
  void addToolBarItems();
  void addVariables();
  bool compile(bool dbg);
  void deleteVariables(QString name);
  void editModel(QString name);
  void enableActions(bool f);
  void loadGraphics();
  bool plotScript();
  void run(QString name);
  void selectVariables();
  ComboBoxDelegate *_cboxd;
  QString _iniFile;
  TreeModel *_model;
  QProcess *_proc;
  QProcess *_plot;
  QProcess *_log;
  RunDlg *_runDlg;
  QString _sbmlFile;
  SettingsDlg *_settingsDlg;
  bool _settings_only;
  double _timeInterval;
  Utils *_utils;
  enum { MaxRecentFiles = 5 };
  QAction *recentFileActs[MaxRecentFiles];
  QString _curFile;
  QAction *separatorAct;
  QAction *exitAct;
};
