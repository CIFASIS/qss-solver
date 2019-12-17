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

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <Qt>

#include <mmomegui.h>
#include <editor.h>
#include <treemodel.h>
#include <utils.h>
#include <comboboxdelegate.h>
#include <runform.h>
#include <settings.h>

#ifdef Q_OS_LINUX
#include <sys/types.h>
#include <signal.h>
#endif

#define SLASH '/'

MmomeGui *mainWindow;

MmomeGui::MmomeGui() : QMainWindow(), _sbmlFile()
{
  setupUi(this);
  _iniFile = "/qss-solver.ini";
  _proc = NULL;
  _plot = NULL;
  _log = NULL;
  _sim_progress->setVisible(false);
  QStringList headers;
  headers << tr("File") << tr("Variable") << tr("Settings");
  _cboxd = new ComboBoxDelegate(_model_variables);
  _model = new TreeModel(headers, this);
  _utils = new Utils();
  _compiler_msg->setTextColor(Qt::black);
  _compiler_msg->setReadOnly(true);
  setWindowState(Qt::WindowMaximized);
  createActions();
  addToolBarItems();
  addMenuBarItems();
  enableActions(true);
  _settings_only = false;
}

MmomeGui::~MmomeGui() { Editor::drop(); }

void MmomeGui::enableActions(bool f)
{
  action_New->setEnabled(f);
  action_Load->setEnabled(f);
  actionImport->setEnabled(f);
  action_Save->setEnabled(f);
  action_Save_As->setEnabled(f);
  actionSa_ve_All->setEnabled(f);
  actionRun->setEnabled(f);
  actionRun_2->setEnabled(f);
  actionSettings->setEnabled(f);
  actionCompile->setEnabled(!f);
  actionDebug->setEnabled(f);
  actionLog->setEnabled(f);
  actionGraphics->setEnabled(f);
  actionClear_Log->setEnabled(f);
  actionMicroModelica_Language_Scpefication->setEnabled(f);
  return;
}
void MmomeGui::addToolBarItems()
{
  _model_toolbar->addAction(action_New);
  _model_toolbar->addAction(action_Load);
  _model_toolbar->addAction(actionImport);
  _model_toolbar->addSeparator();
  _model_toolbar->addAction(action_Save);
  _model_toolbar->addAction(action_Save_As);
  _model_toolbar->addAction(actionSa_ve_All);
  _model_toolbar->addAction(actionSettings);
  _model_toolbar->addSeparator();
  _model_toolbar->addAction(actionRun);
  _model_toolbar->addAction(actionRun_2);
  _model_toolbar->addAction(actionCompile);
  _model_toolbar->addAction(actionDebug);
  _model_toolbar->addSeparator();
  _model_toolbar->addAction(actionLog);
  _model_toolbar->addAction(actionGraphics);
  _model_toolbar->addAction(actionClear_Log);
}

void MmomeGui::addMenuBarItems()
{
  menu_View->addAction(_model_plot->toggleViewAction());
  menu_View->addAction(_model_messages->toggleViewAction());
}

void MmomeGui::done(QString name, QString ext)
{
  QMdiSubWindow *sw = mdiArea->currentSubWindow();
  if (sw) {
    if (!ext.endsWith(".log")) deleteVariables(name);
    if (Editor::instance()->count() == 0) {
      Editor::drop();
      sw->close();
      _model_variables->setModel(NULL);
      delete sw;
    }
  }
}

void MmomeGui::closeEvent(QCloseEvent *event)
{
  QMdiSubWindow *sw = mdiArea->currentSubWindow();
  if (sw) {
    Editor::instance()->closeFiles();
  }
  Editor::drop();
  event->accept();
}

void MmomeGui::on_action_Save_As_triggered()
{
  QMdiSubWindow *sw = mdiArea->currentSubWindow();
  if (sw) {
    Editor::instance()->saveAs(Editor::instance()->newFileName());
  }
}

void MmomeGui::on_actionSettings_triggered()
{
  _settingsDlg = new SettingsDlg(this);
  connect(_settingsDlg, SIGNAL(finished(int)), this, SLOT(settingsDlgClosed()));
  enableActions(false);
  _settingsDlg->show();
}

void MmomeGui::on_actionImport_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load SBML Model"), _utils->appDir(MMOC_MODELS), "XML *.xml (*.xml)");
  if (fileName.isNull()) {
    return;
  }
  QFileInfo sbmlfi(fileName);
  QDir modelsDir(_utils->appDir(MMOC_MODELS));
  modelsDir.mkdir(sbmlfi.baseName());
  modelsDir.cd(sbmlfi.baseName());
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nImport SBML model: ") + fileName);
  _proc = new QProcess(this);
  connect(_proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(importFinished(int, QProcess::ExitStatus)));
  _sbmlFile = modelsDir.absolutePath() + SLASH + sbmlfi.baseName() + ".mo";
  QStringList args;
  args << "-o " + _sbmlFile;
  args << fileName;
  QString sim = _utils->appCommand(CMD_SBML);
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("MMOC_LIBS", QDir(_utils->appDir(MMOC_LIBS)).absolutePath());  // Add an environment variable
  _proc->setProcessEnvironment(env);
  _proc->start(_utils->appDir(MMOC_BIN) + QString(SLASH) + sim, args);
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + "\n" + _utils->appDir(MMOC_BIN) + SLASH + sim + " " + _sbmlFile + " " +
                              fileName);
}

void MmomeGui::on_action_New_triggered() { editModel(QString()); }

void MmomeGui::editModel(QString name)
{
  QMdiSubWindow *sw = mdiArea->currentSubWindow();
  if (sw) {
    Editor::instance()->editModel(name);
  } else {
    QMdiSubWindow *sw = new QMdiSubWindow(this, Qt::CustomizeWindowHint);
    sw->setWidget(Editor::instance(this, name));
    mdiArea->addSubWindow(sw);
    connect(Editor::instance(), SIGNAL(done(QString, QString)), this, SLOT(done(QString, QString)));
    connect(Editor::instance(), SIGNAL(clean(int)), this, SLOT(cleanBuildDir(int)));
    connect(action_Save, SIGNAL(triggered(void)), Editor::instance(), SLOT(save(void)));
    connect(actionSa_ve_All, SIGNAL(triggered(void)), Editor::instance(), SLOT(saveAll(void)));
    Editor::instance()->setWindowState(Qt::WindowMaximized);
    Editor::instance()->show();
  }
}

void MmomeGui::on_action_Load_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load Model"), _utils->appDir(MMOC_MODELS), "MicroModelica *.mo (*.mo)");
  if (fileName.isNull()) {
    return;
  }
  loadFile(fileName);
}

void MmomeGui::on_actionDebug_triggered()
{
  if (Editor::instance()->activeBaseFileName().isEmpty()) return;
  _compiler_msg->clear();
  enableActions(false);
  compile(true);
}

void MmomeGui::on_actionMicroModelica_Language_Scpefication_triggered()
{
  QString link = "http://www.fceia.unr.edu.ar/control/modelica/micromodelicaspec.pdf";
  QDesktopServices::openUrl(QUrl(link));
}

void MmomeGui::on_actionQSS_Solver_Engine_Documentation_triggered()
{
  QString link = "http://www.cifasis-conicet.gov.ar/jfernandez/engine/";
  QDesktopServices::openUrl(QUrl(link));
}

void MmomeGui::on_actionMicroModelica_Compiler_Documentation_triggered()
{
  QString link = "http://www.cifasis-conicet.gov.ar/jfernandez/mmoc/";
  QDesktopServices::openUrl(QUrl(link));
}

void MmomeGui::on_actionSBML_Translator_Documentation_triggered()
{
  QString link = "http://www.cifasis-conicet.gov.ar/jfernandez/sbml/";
  QDesktopServices::openUrl(QUrl(link));
}

void MmomeGui::run_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit && exitCode == 0) {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + _proc->readAllStandardError());
    QByteArray logs = _proc->readAllStandardOutput();
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + logs);
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("Simulation ended."));
    QDir buildDir(_utils->appDir(MMOC_BUILD));
    buildDir.refresh();
    QString _name = Editor::instance()->activeBaseFileName();
    deleteVariables(_name);
    addVariables();
    selectVariables();
  } else {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + _proc->readAllStandardError());
    QByteArray logs = _proc->readAllStandardOutput();
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + logs);
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + "Simulation failed. Exit code: " + QString("%1").arg(exitCode));
  }
  delete _proc;
  _proc = NULL;
  _compiler_msg->moveCursor(QTextCursor::End);
  _compiler_msg->ensureCursorVisible();
  enableActions(true);
  _sim_progress->setVisible(false);
  _settings_only = false;
}

void MmomeGui::settingsDlgClosed() { enableActions(true); }

void MmomeGui::run(QString name)
{
  QDir outputDir(_utils->appDir(MMOC_OUTPUT));
  outputDir.mkdir(Editor::instance()->activeBaseFileName());
  outputDir.cd(Editor::instance()->activeBaseFileName());
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nCreating/cleaning directory: ") + outputDir.absolutePath());
  on_actionClear_Log_triggered();
  _proc = new QProcess(this);
  connect(_proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(run_finished(int, QProcess::ExitStatus)));
  QStringList args;
  args << name;
  if (Editor::instance()->isParallel()) {
    args << QString("true");
    QFileInfo _mo_file(Editor::instance()->activeFullFileName());
    args << _mo_file.absolutePath();
  } else {
    args << QString("false");
  }
  if (Editor::instance()->semiStaticPartitioning()) {
    args << QString("true");
  } else {
    args << QString("false");
  }
  connect(_proc, SIGNAL(readyReadStandardError()), this, SLOT(simulation_message()));
  _timeInterval = Editor::instance()->stopTime().toDouble() - Editor::instance()->startTime().toDouble();
  _sim_progress->setMaximum(100);
  _sim_progress->reset();
  _sim_progress->setVisible(true);
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nStarting simulation.\n"));
  _compiler_msg->moveCursor(QTextCursor::End);
  _compiler_msg->ensureCursorVisible();
  QString sim = _utils->appCommand(CMD_SIMULATE);
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("MMOC_BUILD", QDir(_utils->appDir(MMOC_BUILD)).absolutePath());
  env.insert("MMOC_BIN", QDir(_utils->appDir(MMOC_BIN)).absolutePath());
  env.insert("MMOC_ENGINE", QDir(_utils->appDir(MMOC_ENGINE)).absolutePath());
  env.insert("MMOC_LIBS", QDir(_utils->appDir(MMOC_LIBS)).absolutePath());
  env.insert("MMOC_MODELS", QDir(_utils->appDir(MMOC_MODELS)).absolutePath());
  env.insert("MMOC_SRC", QDir(_utils->appDir(MMOC_SOURCE)).absolutePath());
  env.insert("MMOC_PACKAGES", QDir(_utils->appDir(MMOC_PACKAGES)).absolutePath());
  env.insert("MMOC_LIBRARIES", QDir(_utils->appDir(MMOC_LIBRARIES)).absolutePath());
  env.insert("MMOC_INCLUDE", QDir(_utils->appDir(MMOC_INCLUDE)).absolutePath());
  env.insert("MMOC_OUTPUT", QDir(_utils->appDir(MMOC_OUTPUT)).absolutePath());
  env.insert("LD_LIBRARY_PATH", QDir(_utils->appDir(MMOC_LIBRARIES)).absolutePath() + "/../lib");
  _proc->setProcessEnvironment(env);
  _proc->start(_utils->appDir(MMOC_BIN) + SLASH + sim, args);
}

void MmomeGui::simulation_message()
{
  QString out(_proc->readAllStandardError().constData());
  int _init = out.lastIndexOf('*');
  if (_init >= 0) {
    int pc = (out.mid(_init + 1).toDouble() / _timeInterval) * 100.0;
    _sim_progress->setValue(pc);
  }
}

void MmomeGui::runDlgRejected() { _settings_only = false; }

void MmomeGui::runDlgClose()
{
  Editor::instance()->setStartTime(_runDlg->startTime());
  Editor::instance()->setStopTime(_runDlg->stopTime());
  Editor::instance()->setTolerance(_runDlg->tolerance());
  Editor::instance()->setAbsTolerance(_runDlg->absTolerance());
  Editor::instance()->setSolver(_runDlg->solver());
  Editor::instance()->setScheduler(_runDlg->scheduler());
  Editor::instance()->setOutput(_runDlg->output());
  Editor::instance()->setOutputType(_runDlg->outputType());
  Editor::instance()->setPeriod(_runDlg->period());
  Editor::instance()->setSymDiff(_runDlg->symDiff());
  Editor::instance()->setZcHyst(_runDlg->zcHyst());
  Editor::instance()->setDerDelta(_runDlg->derDelta());
  Editor::instance()->setMinStep(_runDlg->minStep());
  Editor::instance()->setParallel(_runDlg->parallel());
  Editor::instance()->setLPS(_runDlg->LPS());
  Editor::instance()->setDT(_runDlg->DT());
  Editor::instance()->setDtSynch(_runDlg->dtSynch());
  Editor::instance()->setJacobian(_runDlg->jacobian());
  Editor::instance()->setPartitionMethod(_runDlg->partitionMethod());
  Editor::instance()->setPatohSettings(_runDlg->patohSettings());
  Editor::instance()->setScotchSettings(_runDlg->scotchSettings());
  Editor::instance()->setMetisSettings(_runDlg->metisSettings());
  Editor::instance()->setGenerateArch(_runDlg->generateArch());
  Editor::instance()->setDebugGraph(_runDlg->debugGraph());
  Editor::instance()->setReorderPartition(_runDlg->reorderPartition());
  Editor::instance()->setImbalance(_runDlg->imbalance());
  Editor::instance()->setBDFPart(_runDlg->BDFPart());
  Editor::instance()->setBDFPartitionDepth(_runDlg->BDFPartitionDepth());
  Editor::instance()->setBDFMaxStep(_runDlg->BDFMaxStep());
  Editor::instance()->setSemiStaticPartitioning(_runDlg->semiStaticPartitioning());
  Editor::instance()->setDescription(_runDlg->description());
  Editor::instance()->writeAnnotations();
  bool debugFlag = _runDlg->enableDebug();
  enableActions(false);
  if (Editor::instance()->activeDirty()) {
    Editor::instance()->save();
    compile(debugFlag);
  } else {
    QDir buildDir(_utils->appDir(MMOC_BUILD));
    buildDir.cd(Editor::instance()->activeBaseFileName());
    QFileInfo exefi(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName());
    if (!exefi.exists()) {
      enableActions(false);
      _settings_only = false;
    }
    compile(debugFlag);
  }
  delete _runDlg;
}

void MmomeGui::on_actionRun_2_triggered()
{
  _settings_only = true;
  on_actionRun_triggered();
}

void MmomeGui::on_actionRun_triggered()
{
  if (Editor::instance()->activeBaseFileName().isEmpty()) {
    return;
  }
  if (!Editor::instance()->checkModel()) {
    QMessageBox::information(this, "Simulation", "No model definition found.");
    return;
  }
  _compiler_msg->clear();
  _runDlg = new RunDlg(this);
  connect(_runDlg, SIGNAL(accepted()), this, SLOT(runDlgClose()));
  connect(_runDlg, SIGNAL(rejected()), this, SLOT(runDlgRejected()));
  _runDlg->setStartTime(Editor::instance()->startTime());
  _runDlg->setStopTime(Editor::instance()->stopTime());
  _runDlg->setTolerance(Editor::instance()->tolerance());
  _runDlg->setAbsTolerance(Editor::instance()->absTolerance());
  _runDlg->setSolver(Editor::instance()->solver());
  _runDlg->setScheduler(Editor::instance()->scheduler());
  _runDlg->setOutput(Editor::instance()->output());
  _runDlg->setOutputType(Editor::instance()->outputType());
  _runDlg->setPeriod(Editor::instance()->period());
  _runDlg->setSymDiff(Editor::instance()->symDiff());
  _runDlg->setZcHyst(Editor::instance()->zcHyst());
  _runDlg->setDerDelta(Editor::instance()->derDelta());
  _runDlg->setMinStep(Editor::instance()->minStep());
  _runDlg->setParallel(Editor::instance()->parallel());
  _runDlg->setLPS(Editor::instance()->LPS());
  _runDlg->setDT(Editor::instance()->DT());
  _runDlg->setPartitionMethod(Editor::instance()->partitionMethod());
  _runDlg->setPatohSettings(Editor::instance()->patohSettings());
  _runDlg->setScotchSettings(Editor::instance()->scotchSettings());
  _runDlg->setMetisSettings(Editor::instance()->metisSettings());
  _runDlg->setGenerateArch(Editor::instance()->generateArch());
  _runDlg->setReorderPartition(Editor::instance()->reorderPartition());
  _runDlg->setDebugGraph(Editor::instance()->debugGraph());
  _runDlg->setImbalance(Editor::instance()->imbalance());
  _runDlg->setBDFPart(Editor::instance()->BDFPart());
  _runDlg->setBDFPartitionDepth(Editor::instance()->BDFPartitionDepth());
  _runDlg->setBDFMaxStep(Editor::instance()->BDFMaxStep());
  _runDlg->setSemiStaticPartitioning(Editor::instance()->semiStaticPartitioning());
  _runDlg->setDtSynch(Editor::instance()->dtSynch());
  _runDlg->setDescription(Editor::instance()->description());
  _runDlg->setJacobian(Editor::instance()->jacobian());
  _runDlg->show();
}

bool MmomeGui::plotScript()
{
  QString name = Editor::instance()->activeBaseFileName();
  QFile file(_utils->appDir(MMOC_OUTPUT) + SLASH + name + SLASH + name + QString(".plt"));
  bool ret = false;
  if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
    QMessageBox::critical(this, QString(tr("Error")), QString(tr("Can't open file ")) + name + QString(".plt"));
    file.close();
    return false;
  }
  QString _data;
  _data.append("set title \"").append(name).append("\"\n");
  _data.append("set ylabel \"State Variables\"\n");
  _data.append("set xlabel \"Time\"\n");
  QString buff;
  buff.append("set xrange [0:").append(Editor::instance()->stopTime()).append("]\n");
  _data.append(buff);
  _data.append("set grid\n");
  _data.append("plot ");
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem *it = _model->item(k);
    for (int g = 0; g < it->rowCount(); g++) {
      QStandardItem *c = it->child(g, 1);
      QString name = c->text();
      if (c->checkState() == Qt::Checked) {
        _data.append("\"")
            .append(_utils->appDir(MMOC_OUTPUT))
            .append(SLASH)
            .append(_model->item(k)->text())
            .append(SLASH)
            .append(c->text())
            .append(".dat\"");
        c = it->child(g, 2);
        if (c->text() != "None") {
          _data.append(" with ").append(c->text());
        }
        _data.append(" title \"").append(name).append("\"");
        _data.append(", ");
        ret = true;
      }
    }
  }
  int l = _data.length();
  _data = _data.remove(l - 2, 1);
  _data.append("\n");
  if (file.write(_data.toStdString().c_str()) == -1) {
    QMessageBox::critical(this, QString(tr("Error")), QString(tr("Can't write data ")) + name + QString(".plt"));
    file.close();
    return false;
  }
  if (!file.flush()) {
    QMessageBox::critical(this, QString(tr("Error")), QString(tr("Can't save file ")) + name + QString(".plt"));
    file.close();
    return false;
  }
  return ret;
}

void MmomeGui::cleanBuildDir(int idx)
{
  if (_settings_only) return;
  QDir buildDir(_utils->appDir(MMOC_BUILD));
  buildDir.mkdir(Editor::instance()->baseFileName(idx));
  buildDir.cd(Editor::instance()->baseFileName(idx));
  QFileInfo makefi(buildDir.absolutePath() + SLASH + Editor::instance()->baseFileName(idx) + QString(".makefile"));
  QFileInfo cfi(buildDir.absolutePath() + SLASH + Editor::instance()->baseFileName(idx) + QString(".c"));
  QFileInfo exefi(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName());
  if (makefi.exists()) {
    buildDir.remove(makefi.absoluteFilePath());
  }
  if (cfi.exists()) {
    buildDir.remove(cfi.absoluteFilePath());
  }
  if (exefi.exists()) {
    buildDir.remove(exefi.absoluteFilePath());
  }
  setCurrentFile(Editor::instance()->activeFullFileName());
}

bool MmomeGui::compile(bool dbg)
{
  _compiler_msg->clear();
  QFileInfo modelFile(Editor::instance()->activeFullFileName());
  QDir buildDir(_utils->appDir(MMOC_BUILD));
  buildDir.mkdir(Editor::instance()->activeBaseFileName());
  buildDir.cd(Editor::instance()->activeBaseFileName());
  QString fileName(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".part"));
  if (QFile::exists(fileName)) {
    QFile::remove(fileName);
  }
  QFile::copy(modelFile.absoluteDir().absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".part"), fileName);
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + buildDir.absolutePath());
  QFileInfo makefi(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".makefile"));
  QFileInfo cfi(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".c"));
  if (!_settings_only) {
    cleanBuildDir(Editor::instance()->activeFileIndex());
  }
  QString flags = _utils->appFlag(FLG_FLAGS);
  _proc = new QProcess(this);
  connect(_proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(comp_finished(int, QProcess::ExitStatus)));
  QStringList args;
  QString set;
  if (_settings_only) {
    set += "-s ";
  }
  if (!flags.isEmpty()) {
    if (Editor::instance()->isParallel() && !flags.contains("-p")) {
      set += "-p ";
    }
    set += flags + " ";
  } else if (Editor::instance()->isParallel()) {
    set += "-p ";
  }
  if (dbg) {
    flags = _utils->appFlag(FLG_DEBUG);
    if (flags.isEmpty()) {
      QMessageBox::warning(this, "Debug Flags", "Debug mode enabled but no debug flags found. They can be set from \"File->Settings\"");
    }
    flags.append(" ");
    set += flags;
  }
  set += "-o " + buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName();
  args << set;
  args << Editor::instance()->activeFullFileName();
  QString comp = _utils->appCommand(CMD_COMPILE);
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nCompiling model: "));
  _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + _utils->appDir(MMOC_BIN) + SLASH + comp + " " + set + " " +
                              Editor::instance()->activeFullFileName());
  _compiler_msg->moveCursor(QTextCursor::End);
  _compiler_msg->ensureCursorVisible();
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("MMOC_BUILD", QDir(_utils->appDir(MMOC_BUILD)).absolutePath());
  env.insert("MMOC_BIN", QDir(_utils->appDir(MMOC_BIN)).absolutePath());
  env.insert("MMOC_ENGINE", QDir(_utils->appDir(MMOC_ENGINE)).absolutePath());
  env.insert("MMOC_LIBS", QDir(_utils->appDir(MMOC_LIBS)).absolutePath());
  env.insert("MMOC_MODELS", QDir(_utils->appDir(MMOC_MODELS)).absolutePath());
  env.insert("MMOC_SRC", QDir(_utils->appDir(MMOC_SOURCE)).absolutePath());
  env.insert("MMOC_PACKAGES", QDir(_utils->appDir(MMOC_PACKAGES)).absolutePath());
  env.insert("MMOC_LIBRARIES", QDir(_utils->appDir(MMOC_LIBRARIES)).absolutePath());
  env.insert("MMOC_INCLUDE", QDir(_utils->appDir(MMOC_INCLUDE)).absolutePath());
  env.insert("MMOC_OUTPUT", QDir(_utils->appDir(MMOC_OUTPUT)).absolutePath());
  _proc->setProcessEnvironment(env);
  _proc->start(_utils->appDir(MMOC_BIN) + SLASH + comp, args);
  return true;
}

void MmomeGui::importFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit) {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + _proc->readAllStandardError());
    editModel(_sbmlFile);
    addVariables();
  } else {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nCan not import SBML file.") + QString(exitCode));
  }
  _sbmlFile.clear();
  delete _proc;
  _proc = NULL;
}

void MmomeGui::make_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit) {
    QString execName = _utils->appDir(MMOC_BUILD) + SLASH + Editor::instance()->activeBaseFileName() + QString(SLASH) +
                       Editor::instance()->activeBaseFileName();
#ifdef _WIN32
    execName += ".exe";
#endif
    QFileInfo exefi(execName);
    if (!exefi.exists()) {
      _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + _proc->readAllStandardError());
      _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nCan't generate binary file.") + QString(exitCode));
      enableActions(true);
      _settings_only = false;
      return;
    } else {
      _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nBinary file generated, ready to start simulation."));
    }
  } else {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nError generating binary file.).") + QString(exitCode));
    enableActions(true);
    _settings_only = false;
    return;
  }
  delete _proc;
  _proc = NULL;
  _compiler_msg->moveCursor(QTextCursor::End);
  _compiler_msg->ensureCursorVisible();
  run(Editor::instance()->activeBaseFileName());
}

void MmomeGui::comp_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit) {
    QDir buildDir(_utils->appDir(MMOC_BUILD));
    buildDir.mkdir(Editor::instance()->activeBaseFileName());
    buildDir.cd(Editor::instance()->activeBaseFileName());
    QFileInfo cfi(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".c"));
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + _proc->readAllStandardError());
    QByteArray logs = _proc->readAllStandardOutput();
    if (!logs.isEmpty()) _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + logs);
    if (!cfi.exists()) {
      _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nCan't generate C file."));
      enableActions(true);
      _settings_only = false;
      return;
    }
    QFileInfo modelfi = Editor::instance()->activeFullFileName();
    QDir modeldir = modelfi.absoluteDir();
    QFileInfoList filst = modeldir.entryInfoList(QStringList() << "*.c"
                                                               << "*.h");
    foreach (QFileInfo qfi, filst) {
      QFile file(qfi.filePath());
      file.copy(buildDir.absolutePath() + SLASH + qfi.fileName());
      file.close();
    }
    modeldir = QCoreApplication::applicationDirPath();
    filst = modeldir.entryInfoList(QStringList() << "*.im");
    foreach (QFileInfo qfi, filst) {
      QFile file(qfi.filePath());
      file.copy(buildDir.absolutePath() + SLASH + qfi.fileName());
      file.remove();
    }
    QFileInfo makefi(buildDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".makefile"));
    if (!makefi.exists()) {
      _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nCan't generate makefile."));
      enableActions(true);
      _settings_only = false;
      return;
    }
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nRunning makefile."));
    QStringList args;
    args.clear();
    args << makefi.baseName();
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n") + QString("make -f ") + buildDir.absolutePath() + SLASH +
                                makefi.fileName());
    QString build = _utils->appCommand(CMD_BUILD);
    delete _proc;
    _proc = new QProcess(this);
    connect(_proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(make_finished(int, QProcess::ExitStatus)));
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("MMOC_BUILD", QDir(_utils->appDir(MMOC_BUILD)).absolutePath());
    env.insert("MMOC_BIN", QDir(_utils->appDir(MMOC_BIN)).absolutePath());
    _proc->setProcessEnvironment(env);
    _proc->start(_utils->appDir(MMOC_BIN) + SLASH + build, args);
  } else {
    delete _proc;
    _proc = NULL;
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\nError during compilation.") + QString(exitCode));
    enableActions(true);
    _settings_only = false;
  }
  _compiler_msg->moveCursor(QTextCursor::End);
  _compiler_msg->ensureCursorVisible();
}

void MmomeGui::on_actionCompile_triggered()
{
  if (Editor::instance()->activeBaseFileName().isEmpty()) {
    return;
  }

#ifdef Q_OS_LINUX
  Q_PID pid = _proc->pid();
  QProcess killer;
  QStringList params;
  params << "--ppid";
  params << QString::number(pid);
  params << "-o";
  params << "pid";
  params << "--noheaders";
  killer.start("/bin/ps", params, QIODevice::ReadOnly);
  if (killer.waitForStarted(-1)) {
    if (killer.waitForFinished(-1)) {
      QByteArray temp = killer.readAllStandardOutput();
      QString str = QString::fromLocal8Bit(temp);
      QStringList list = str.split("\n");

      for (int i = 0; i < list.size(); i++) {
        if (!list.at(i).isEmpty()) ::kill(list.at(i).toInt(), SIGKILL);
      }
    }
  }
#endif
  _proc->kill();
  enableActions(true);
}

void MmomeGui::on_actionLog_triggered()
{
  QString name = Editor::instance()->activeBaseFileName();
  if (name.isEmpty()) {
    return;
  }
  QString logCmd = _utils->appCommand(CMD_LOG);
  _log = new QProcess(this);
  connect(_log, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(log_finished(int, QProcess::ExitStatus)));
  QStringList args;
  args << name;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("MMOC_OUTPUT", QDir(_utils->appDir(MMOC_OUTPUT)).absolutePath());
  env.insert("MMOC_BIN", QDir(_utils->appDir(MMOC_BIN)).absolutePath());
  _log->setProcessEnvironment(env);
  _log->start(_utils->appDir(MMOC_BIN) + SLASH + logCmd, args);
}

void MmomeGui::on_actionClear_Log_triggered()
{
  QDir outputDir(_utils->appDir(MMOC_OUTPUT));
  outputDir.mkdir(Editor::instance()->activeBaseFileName());
  outputDir.cd(Editor::instance()->activeBaseFileName());
  QStringList dirs = outputDir.entryList(QStringList() << "*.log"
                                                       << "*.dat"
                                                       << "*.plt");
  foreach (QString f, dirs) {
    outputDir.remove(f);
  }
}

void MmomeGui::on_actionClear_Messages_triggered() { _compiler_msg->clear(); }

void MmomeGui::deleteVariables(QString name)
{
  int idx = -1;
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem *it = _model->item(k);
    if (name == it->text()) {
      idx = k;
      break;
    }
  }
  if (idx < 0) return;
  _model->removeRows(idx, 1);
  _model_variables->setModel(_model);
  _model_variables->setItemDelegateForColumn(2, _cboxd);
  for (int column = 0; column < _model->columnCount(); ++column) {
    _model_variables->resizeColumnToContents(column);
  }
}

void MmomeGui::createActions()
{
  for (int i = 0; i < MaxRecentFiles; ++i) {
    recentFileActs[i] = new QAction(this);
    recentFileActs[i]->setVisible(false);
    connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFiles()));
  }
  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
  separatorAct = menu_File->addSeparator();
  for (int i = 0; i < MaxRecentFiles; ++i) menu_File->addAction(recentFileActs[i]);
  separatorAct->setVisible(false);
  menu_File->addSeparator();
  menu_File->addAction(exitAct);
  updateRecentFileActions();
}

void MmomeGui::openRecentFiles()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    loadFile(action->data().toString());
  }
}

void MmomeGui::updateRecentFileActions()
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);

  QStringList files = settings.value("Editor/recentFileList").toStringList();

  int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

  for (int i = 0; i < numRecentFiles; ++i) {
    QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
    recentFileActs[i]->setText(text);
    recentFileActs[i]->setData(files[i]);
    recentFileActs[i]->setVisible(true);
  }
  for (int j = numRecentFiles; j < MaxRecentFiles; ++j) recentFileActs[j]->setVisible(false);

  separatorAct->setVisible(numRecentFiles > 0);
}

void MmomeGui::loadFile(QString fileName)
{
  editModel(fileName);
  addVariables();
  setCurrentFile(fileName);
}

void MmomeGui::setCurrentFile(const QString fileName)
{
  _curFile = fileName;
  setWindowFilePath(_curFile);
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini", QSettings::IniFormat);
  QStringList files = settings.value("Editor/recentFileList").toStringList();
  files.removeAll(fileName);
  files.prepend(fileName);
  while (files.size() > MaxRecentFiles) files.removeLast();

  settings.setValue("Editor/recentFileList", files);
  updateRecentFileActions();
}

void MmomeGui::selectVariables()
{
  QString name = Editor::instance()->activeBaseFileName();
  bool found = false;
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem *it = _model->item(k);
    Qt::CheckState cs = Qt::Unchecked;
    if (name == it->text()) {
      cs = Qt::Checked;
      QModelIndex idx = _model->index(k, 0, QModelIndex());
      _model_variables->setExpanded(idx, true);
      found = true;
    } else {
      QModelIndex idx = _model->index(k, 0, QModelIndex());
      _model_variables->setExpanded(idx, false);
    }
    for (int g = 0; g < it->rowCount(); g++) {
      QStandardItem *c = it->child(g, 1);
      QString name = c->text();
      c->setCheckState(cs);
    }
  }
  if (!found) {
    addVariables();
    int k = _model->rowCount() - 1;
    if (k < 0) return;
    QStandardItem *it = _model->item(k);
    Qt::CheckState cs = Qt::Checked;
    QModelIndex idx = _model->index(k, 0, QModelIndex());
    _model_variables->setExpanded(idx, true);
    for (int g = 0; g < it->rowCount(); g++) {
      QStandardItem *c = it->child(g, 1);
      QString name = c->text();
      c->setCheckState(cs);
    }
  }
}

void MmomeGui::addVariables()
{
  QString name = Editor::instance()->activeBaseFileName();
  for (int k = 0; k < _model->rowCount(); k++) {
    QStandardItem *it = _model->item(k);
    if (name == it->text()) {
      return;
    }
  }
  if (!name.isEmpty()) {
    QString fdir;
    fdir.append(_utils->appDir(MMOC_OUTPUT)).append(SLASH).append(name);
    _model->addFiles(fdir);
    _model_variables->setModel(_model);
    _model_variables->setModel(_model);
    _model_variables->setItemDelegateForColumn(2, _cboxd);
    for (int column = 0; column < _model->columnCount(); ++column) {
      _model_variables->resizeColumnToContents(column);
    }
  }
}

void MmomeGui::on_actionGraphics_triggered()
{
  QString name = Editor::instance()->activeBaseFileName();
  if (name.isEmpty()) return;
  if (!plotScript()) {
    return;
  }
  _compiler_msg->clear();
  QString plotCmd = _utils->appCommand(CMD_PLOT);
  QString plotOptions = _utils->appFlag(FLG_PLOT);
  QString gpxdir = _utils->appDir(MMOC_OUTPUT) + SLASH + name;
  QFileInfo of(gpxdir + SLASH + name + QString(".plt"));
  if (!of.exists()) {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + QString("\n Can't find plot script: ") + name + QString(".plt"));
    return;
  }
  _plot = new QProcess(this);
  connect(_plot, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(plot_finished(int, QProcess::ExitStatus)));
  QStringList args;
  if (plotOptions.isEmpty()) {
    plotOptions = "-persist";
  }
  args << plotOptions;
  args << of.absoluteFilePath();
  _plot->start(plotCmd, args);
}

void MmomeGui::plot_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  QString msgs = _plot->readAllStandardError();
  if (exitStatus == QProcess::NormalExit && !msgs.isEmpty()) {
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + "\nPlot script error: " + QString(exitCode) + "\n" + msgs);
  }
  delete _plot;
  _plot = NULL;
}

void MmomeGui::on_actionExit_triggered() { close(); }

void MmomeGui::on_actionAbout_triggered()
{
  QString sh;
  sh.append("Modeling and simulation environment for continuous and hybrid systems.\n");
  sh.append("Developed by:\n");
  sh.append("Joaquin Fernandez (fernandez@cifasis-conicet.gov.ar)\n");
  sh.append("Federico Bergero (bergero@cifasis-conicet.gov.ar)\n");
  sh.append("\n");
  sh.append("Directed by:\n");
  sh.append("Ernesto Kofman (kofman@cifasis-conicet.gov.ar)\n");
  QString title("QSS Solver Version ");
  title.append(_utils->appFlag(FLG_VERSION));
  title.append(" Revision ");
  title.append(_utils->appFlag(FLG_REVISION));
  QMessageBox::about(this, title, sh);
}

QString MmomeGui::strippedName(const QString &fullFileName) { return QFileInfo(fullFileName).fileName(); }

void MmomeGui::log_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  QDir outputDir(_utils->appDir(MMOC_OUTPUT));
  outputDir.cd(Editor::instance()->activeBaseFileName());
  QFileInfo logfi(outputDir.absolutePath() + SLASH + Editor::instance()->activeBaseFileName() + QString(".log"));
  if (logfi.size()) {
    editModel(logfi.absoluteFilePath());
  } else if (exitStatus == QProcess::NormalExit) {
    QString msgs = _log->readAllStandardError();
    _compiler_msg->setPlainText(_compiler_msg->toPlainText() + "\nLog file error: " + QString(exitCode) + "\n" + msgs);
  }
  delete _log;
  _log = NULL;
}
