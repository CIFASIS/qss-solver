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

#include <QFileDialog>
#include <QMessageBox>
#include <Qt>
#include <modeleditor.h>
#include <mmohighlight.h>
#include <modelinfo.h>
#include <codeeditor.h>

ModelEditor::ModelEditor(QWidget *parent, QString name) :
    QDialog(parent), _annotations(), _defaultValues(), _startTime(), _stopTime(), _tolerance(), _absTolerance(), _solver(), _output(), _outputType(), _period(), _lps(), _description(), _minstep(), _zchyst(), _derdelta(), _scheduler(), _symdiff(), _dtSynch(), _dtPeriod(), _dtStepLog(), _dt(), _hl(
    NULL)
{
  setupUi(this);
  _defaultValues["MMO_Description"] = "";
  _defaultValues["MMO_Solver"] = "QSS3";
  _defaultValues["StartTime"] = "0.0";
  _defaultValues["StopTime"] = "1.0";
  _defaultValues["Tolerance"] = "1e-3";
  _defaultValues["AbsTolerance"] = "1e-3";
  _defaultValues["MMO_SymDiff"] = "true";
  _defaultValues["MMO_Output"] = "";
  _defaultValues["MMO_OutputType"] = "CI_Step";
  _defaultValues["MMO_Period"] = "1e-2";
  _defaultValues["MMO_DT_Synch"] = "SD_DT_Asynchronous";
  _defaultValues["MMO_LPS"] = "1";
  _defaultValues["MMO_MinStep"] = "1e-14";
  _defaultValues["MMO_ZCHyst"] = "1e-12";
  _defaultValues["MMO_DerDelta"] = "1e-8";
  _defaultValues["MMO_Scheduler"] = "ST_Binary";
  _defaultValues["MMO_NodeSize"] = "10000";
  _defaultValues["MMO_StoreData"] = "SD_Memory";
  _defaultValues["MMO_Parallel"] = "false";
  _defaultValues["MMO_PartitionMethod"] = "MetisCut";
  _defaultValues["MMO_DT_Min"] = "0";
  _model_editor_tab = new QTabWidget(this);
  _model_editor_tab->setTabsClosable(true);
  _models = new QList<ModelInfo>();
  _utils = new Utils();
  connect(_model_editor_tab, SIGNAL(tabCloseRequested(int)), this,
      SLOT(on__model_editor_tab_tabCloseRequested(int)));
  connect(_model_editor_tab, SIGNAL(currentChanged(int)), this,
      SLOT(on__model_editor_tab_currentChanged(int)));
  _model_editor_hl->addWidget(_model_editor_tab);
  editModel(name);
}

ModelEditor::~ModelEditor()
{
  if(_hl != NULL)
  {
    delete _hl;
  }
  delete _model_editor_tab;
  delete _models;
  delete _utils;
}

void
ModelEditor::editModel(QString name)
{
  CodeEditor *_textEditor = new CodeEditor(this);
  QString _mname(tr("New Model"));
  QFile file(name);
  QSettings settings(QCoreApplication::applicationDirPath() + "/qss-solver.ini",
      QSettings::IniFormat);
  int _tab = settings.value("Editor/tab",
      "Value not found in file qss-solver.ini").toInt();
  _textEditor->setTabStopWidth(_tab);
  if(!file.fileName().isEmpty())
  {
    QFileInfo fi(name);
    _mname = fi.fileName();
    QIODevice::OpenModeFlag om = QIODevice::ReadWrite;
    if(file.open(om))
    {
      _textEditor->setPlainText(file.readAll());
    }
    else
    {
      QMessageBox::critical(this, QString(tr("Error")),
          QString(tr("Can't open file.")));
      return;
    }
  }
  QTextDocument *td = new QTextDocument(_textEditor->toPlainText(), this);
  _textEditor->setDocument(td);
  if(name.endsWith(QString(".log")))
  {
    _hl = new MmoHighlighter(_textEditor->document(), MmoHighlighter::MMO_LOG);
  }
  else
  {
    _hl = new MmoHighlighter(_textEditor->document(),
        MmoHighlighter::MMO_MODEL);
  }
  connect(_textEditor, SIGNAL(modificationChanged(bool)), this,
      SLOT(on__textEditor_textChanged(bool)));
  setWindowTitle(_mname);
  _models->append(ModelInfo(name));
  _model_editor_tab->setCurrentIndex(
      _model_editor_tab->addTab(_textEditor, _mname));
}

void
ModelEditor::keyReleaseEvent(QKeyEvent * event)
{
  if(event->nativeScanCode() == 117 && (event->nativeModifiers() & 4))
  {
    _model_editor_tab->setCurrentIndex(
        (_model_editor_tab->currentIndex() + 1) % _model_editor_tab->count());
  }
  else if(event->nativeScanCode() == 112 && (event->nativeModifiers() & 4))
  {
    _model_editor_tab->setCurrentIndex(
        _model_editor_tab->currentIndex() > 0 ?
            _model_editor_tab->currentIndex() - 1 :
            _model_editor_tab->count() - 1);
  }
  else
  {
    QWidget::keyReleaseEvent(event);
  }
}

QString
ModelEditor::activeFileName()
{
  int idx = _model_editor_tab->currentIndex();
  return fileName(idx);
}

bool
ModelEditor::activeDirty()
{
  int idx = _model_editor_tab->currentIndex();
  if(idx < 0)
    return false;
  return _models->value(idx).dirty();
}

QString
ModelEditor::fileName(int idx)
{
  if(idx < 0)
    return QString();
  return _models->value(idx).name();
}

QString
ModelEditor::activeBaseFileName()
{
  int idx = _model_editor_tab->currentIndex();
  return baseFileName(idx);
}

QString
ModelEditor::baseFileName(int idx)
{
  if(idx < 0)
    return QString();
  return _models->value(idx).baseName();
}

QString
ModelEditor::activeFullFileName()
{
  int idx = _model_editor_tab->currentIndex();
  return fullFileName(idx);
}

int
ModelEditor::activeFileIndex()
{
  return _model_editor_tab->currentIndex();
}

QString
ModelEditor::fullFileName(int idx)
{
  if(idx < 0)
    return QString();
  return _models->value(idx).fullname();
}

void
ModelEditor::on__textEditor_textChanged(bool changed)
{
  if(changed == false)
  {
    return;
  }
  int idx = _model_editor_tab->currentIndex();
  if(!_models->value(idx).name().endsWith(".log"))
  {
    if(!_models->value(idx).dirty())
    {
      ModelInfo mi = _models->value(idx);
      mi.setDirty(true);
      _models->replace(idx, mi);
      _model_editor_tab->setTabText(idx,
          _model_editor_tab->tabText(idx) + QString("*"));
    }
  }
}

void
ModelEditor::on__model_editor_tab_currentChanged(int index)
{
  if(index < 0)
    return;
  setWindowTitle(_model_editor_tab->tabText(index));
}

void
ModelEditor::on__model_editor_tab_tabCloseRequested(int index)
{
  if(index < 0)
    return;
  ModelInfo mi = _models->value(index);
  if(!mi.init())
  {
    if(mi.dirty())
    {
      QMessageBox::StandardButton res = QMessageBox::question(this, "Save",
          QString("Save changes to file ") + mi.name() + QString("?"),
          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      if(res == QMessageBox::Save)
      {
        _save(index);
      }
      else if(res == QMessageBox::Cancel)
      {
        return;
      }
    }
  }
  else
  {
    mi.setInit(false);
    _models->replace(index, mi);
  }
  _delete(index);
}

void
ModelEditor::closeFiles()
{
  foreach(ModelInfo mi, *_models)
  {  on__model_editor_tab_tabCloseRequested(0);
}
}

void
ModelEditor::_delete(int tab)
{
  QString name = baseFileName(tab);
  QString ext = fullFileName(tab);
  _models->removeAt(tab);
  QTextEdit *_textEditor = qobject_cast<QTextEdit*>(
      _model_editor_tab->widget(tab));
  _model_editor_tab->removeTab(tab);
  delete _textEditor;
  emit done(name, ext);
}

void
ModelEditor::save()
{
  int idx = _model_editor_tab->currentIndex();
  if(idx < 0)
    return;
  if(checkPackage())
  {
    _deletePackageFiles(idx);
  }
  _save(idx);
  emit clean(idx);
}

void
ModelEditor::saveAs(QString name)
{
  int idx = _model_editor_tab->currentIndex();
  if(idx < 0)
    return;
  ModelInfo mi = _models->value(idx);
  if(mi.fullname().isEmpty())
  {
    _save(idx);
  }
  else
  {
    QFile file(mi.fullname());
    file.copy(name);
    file.close();
  }
  mi.setFullname(name);
  mi.setDirty(false);
  _models->replace(idx, mi);
  _model_editor_tab->setTabText(idx, mi.name());
}

void
ModelEditor::saveAll()
{
  int _tab = 0;
  foreach(ModelInfo mi, *_models)
  {  if(mi.dirty())
  {
    _save(_tab);
  }
  _tab++;
}
}

QString
ModelEditor::newFileName()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save"),
      _utils->appDir(MMOC_MODELS), "MicroModelica *.mo (*.mo)");
  if(!fileName.isEmpty() && !fileName.endsWith(".mo"))
  {
    fileName.append(".mo");
  }
  return fileName;
}

void
ModelEditor::_save(int tab)
{
  ModelInfo mi = _models->value(tab);
  if(mi.fullname().isEmpty())
  {
    mi.setFullname(newFileName());
  }
  if(mi.fullname().isEmpty())
  {
    return;
  }
  QFile file(mi.fullname());
  if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
  {
    QMessageBox::critical(this, QString(tr("Error")),
        QString(tr("Can't open file ")) + mi.name());
    file.close();
    return;
  }
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  if(!_textEditor)
  {
    QMessageBox::critical(this, QString(tr("Error")),
        QString(tr("Can't open text editor.")));
    file.close();
    return;
  }
  QString _data = _textEditor->toPlainText();
  if(file.write(_data.toStdString().c_str()) == -1)
  {
    QMessageBox::critical(this, QString(tr("Error")),
        QString(tr("Can't write data ")) + mi.name());
    file.close();
    return;
  }
  if(!file.flush())
  {
    QMessageBox::critical(this, QString(tr("Error")),
        QString(tr("Can't save file ")) + mi.name());
    file.close();
    return;
  }
  _model_editor_tab->setTabText(tab, mi.name());
  _textEditor->document()->setModified(false);
  mi.setDirty(false);
  _models->replace(tab, mi);
}

QString
ModelEditor::_getAnnotationValue(QString value, QString token)
{
  QStringList values = value.split("=");
  QString tmpValue;
  if(token == "MMO_Output")
  {
    int index = value.indexOf("=");
    for(int a = index + 1; a < value.size(); a++)
    {
      tmpValue += value[a];
    }
  }
  else
  {
    tmpValue = values[1];
  }
  if(tmpValue.size() == 1)
  {
    return QString();
  }
  values = tmpValue.split("{");
  if(values.size() > 1)
  {
    return values[1].split("}")[0];
  }
  return tmpValue.split(",")[0];
}

QString
ModelEditor::parallel()
{
  return _getAnnotations("MMO_Parallel");
}

QString
ModelEditor::partitionMethod()
{
  return _getAnnotations("MMO_PartitionMethod");
}

int
ModelEditor::_tokenPosition(QString token)
{
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  if(_textEditor->find(token))
  {
    return _textEditor->textCursor().position();
  }
  return 0;
}

QStringList
ModelEditor::fileNames()
{
  QStringList ret;
  for(int i = 0; i < _model_editor_tab->count(); i++)
  {
    ret << baseFileName(i);
  }
  return ret;
}

bool
ModelEditor::_lineEmpty()
{
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.select(QTextCursor::LineUnderCursor);
  QString line = tc.selection().toPlainText();
  return line.isEmpty();
}

void
ModelEditor::_addLine(QString str)
{
  _annotations << str;
}

void
ModelEditor::_setAnnotations(QString tag, QString value, bool separator)
{
  QString add = value;
  if(tag == "Tolerance" || tag == "AbsTolerance" || tag == "MMO_Output"
      || tag == "MMO_Period" || tag == "MMO_PatohSettings"
      || tag == "MMO_ScotchSettings"
      || tag == "MMO_MetisSettings")
  {
    add.prepend("{");
    add.append("}");
  }
  if(separator)
  {
    _addLine(QString("\t\t").append(tag).append("=").append(add).append(","));
  }
  else
  {
    _addLine(QString("\t\t").append(tag).append("=").append(add));
  }
}

int
ModelEditor::_beginModel()
{
  return _tokenPosition("model ");
}

int
ModelEditor::_endModel()
{
  return _tokenPosition("end " + modelName());
}

bool
ModelEditor::_checkToken(QString str)
{
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  return _textEditor->find(str);
}

bool
ModelEditor::_checkAnnotations()
{
  QString mName = modelName();
  int endModel = _endModel();
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  QString line;
  QString end = "end ";
  end.append(mName).append(";");
  if(_textEditor->find("model "))
  {
    tc = _textEditor->textCursor();
    int beginModel = tc.position();
    int pos = _checkFunctions(beginModel, endModel);
    tc.setPosition(pos);
    _textEditor->setTextCursor(tc);
    if(_textEditor->find(QString("experiment")))
    {
      QString annotEnd("));");
      tc = _textEditor->textCursor();
      if(tc.position() > endModel)
      {
        return true;
      }
      tc.select(QTextCursor::LineUnderCursor);
      line = tc.selection().toPlainText();
      if(!line.endsWith(annotEnd))
      {
        _textEditor->moveCursor(QTextCursor::Down);
        tc = _textEditor->textCursor();
        tc.select(QTextCursor::LineUnderCursor);
        line = tc.selection().toPlainText();
        int c = 0;
        while(!line.endsWith(annotEnd) && c < 100)
        {
          _textEditor->moveCursor(QTextCursor::Down);
          tc = _textEditor->textCursor();
          tc.select(QTextCursor::LineUnderCursor);
          line = tc.selection().toPlainText();
          c++;
        }
      }
      _textEditor->moveCursor(QTextCursor::Down);
      tc = _textEditor->textCursor();
      tc.select(QTextCursor::LineUnderCursor);
      line = tc.selection().toPlainText();
      int c = 0;
      while(line.isEmpty() && c < 100)
      {
        _textEditor->moveCursor(QTextCursor::Down);
        tc = _textEditor->textCursor();
        tc.select(QTextCursor::LineUnderCursor);
        line = tc.selection().toPlainText();
        c++;
      }
      return line.contains(end);
    }
  }
  return true;
}

bool
ModelEditor::checkModel()
{
  return _checkToken("model ");
}

bool
ModelEditor::checkPackage()
{
  return _checkToken("package ");
}

QString
ModelEditor::_getAnnotations(QString str)
{
  int endModel = _endModel();
  QString mName = modelName();
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  QString line;
  QString end = "end ";
  end.append(mName).append(";");
  QString annotations;
  if(_textEditor->find(end))
  {
    if(_textEditor->find("experiment", QTextDocument::FindBackward))
    {
      tc = _textEditor->textCursor();
      if(_textEditor->find("annotation", QTextDocument::FindBackward))
      {
        tc = _textEditor->textCursor();
        if(tc.position() > endModel)
        {
          return QString();
        }
        int np = tc.position();
        tc.select(QTextCursor::BlockUnderCursor);
        line = tc.selection().toPlainText();
        if(!line.contains("experiment"))
        {
          _textEditor->moveCursor(QTextCursor::Down);
          tc = _textEditor->textCursor();
          int ep = _controlEmptyLines(tc.position());
          if(ep > endModel)
          {
            return QString();
          }
          tc.setPosition(ep);
          tc.select(QTextCursor::BlockUnderCursor);
          line = tc.selection().toPlainText();
          if(!line.contains("experiment"))
          {
            return QString();
          }
        }
        tc.setPosition(np);
        tc.select(QTextCursor::BlockUnderCursor);
        line = tc.selection().toPlainText();
        if(line.contains(end))
          return QString();
        tc = _textEditor->textCursor();
        tc.select(QTextCursor::BlockUnderCursor);
        line = tc.selection().toPlainText();
        int c = 0;
        while(!line.contains(end) && c < 100)
        {
          tc.select(QTextCursor::BlockUnderCursor);
          _textEditor->moveCursor(QTextCursor::Down);
          tc = _textEditor->textCursor();
          tc.select(QTextCursor::BlockUnderCursor);
          line = tc.selection().toPlainText();
          annotations.append(line.trimmed());
          c++;
        }
      }
      else
      {
        _textEditor->moveCursor(QTextCursor::Down);
      }
    }
  }
  QStringList annotationValue = annotations.split(str);
  if(annotationValue.size() > 1)
  {
    return _getAnnotationValue(annotationValue[1], str);
  }
  else if(_defaultValues.contains(str))
  {
    return _defaultValues[str];
  }
  return QString();
}

QString
ModelEditor::_findValue(QString token, QString str)
{
  if(_defaultValues.contains(token))
  {
    if(_defaultValues[token] == str)
    {
      return QString();
    }
  }
  return str;
}

int
ModelEditor::_checkFunctions(int modelInit, int modelEnd)
{
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.setPosition(modelInit);
  _textEditor->setTextCursor(tc);
  if(_textEditor->find("function "))
  {
    QString fname;
    tc = _textEditor->textCursor();
    if(tc.position() <= modelEnd)
    {
      tc.select(QTextCursor::LineUnderCursor);
      fname = tc.selection().toPlainText().remove("function ").trimmed();
      int lastFunction = tc.position();
      _textEditor->moveCursor(QTextCursor::Down);
      while(_textEditor->find("function "))
      {
        tc = _textEditor->textCursor();
        if(tc.position() > modelEnd)
        {
          break;
        }
        tc.select(QTextCursor::LineUnderCursor);
        fname = tc.selection().toPlainText().remove("function ").trimmed();
        lastFunction = tc.position();
        _textEditor->moveCursor(QTextCursor::Down);
      }
      tc.setPosition(lastFunction);
      _textEditor->setTextCursor(tc);
      if(_textEditor->find("end " + fname))
      {
        return _textEditor->textCursor().position();
      }
    }
  }
  return modelInit;
}

int
ModelEditor::_controlEmptyLines(int position)
{
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.setPosition(position);
  _textEditor->setTextCursor(tc);
  tc.select(QTextCursor::LineUnderCursor);
  QString line = tc.selection().toPlainText();
  int c = 0;
  while(line.isEmpty() && c < 100)
  {
    _textEditor->moveCursor(QTextCursor::Down);
    tc = _textEditor->textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    line = tc.selection().toPlainText();
    c++;
  }
  return tc.position();
}

void
ModelEditor::_deleteAnnotations()
{
  if(!_checkAnnotations())
  {
    QMessageBox::information(this, "Simulation Settings",
        "Simulation annotations must be located at the end of the file.");
    return;
  }
  int endModel = _endModel();
  QString mName = modelName();
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  QString line;
  QString end = "end ";
  end.append(mName).append(";");
  if(_textEditor->find(end))
  {
    if(_textEditor->find("experiment", QTextDocument::FindBackward))
    {
      tc = _textEditor->textCursor();
      if(_textEditor->find("annotation", QTextDocument::FindBackward))
      {
        tc = _textEditor->textCursor();
        if(tc.position() > endModel)
        {
          return;
        }
        int np = tc.position();
        tc.select(QTextCursor::BlockUnderCursor);
        line = tc.selection().toPlainText();
        if(!line.contains("experiment"))
        {
          _textEditor->moveCursor(QTextCursor::Down);
          tc = _textEditor->textCursor();
          int ep = _controlEmptyLines(tc.position());
          if(ep > endModel)
          {
            return;
          }
          tc.setPosition(ep);
          tc.select(QTextCursor::BlockUnderCursor);
          line = tc.selection().toPlainText();
          if(!line.contains("experiment"))
          {
            return;
          }
        }
        tc.setPosition(np);
        tc.select(QTextCursor::BlockUnderCursor);
        line = tc.selection().toPlainText();
        tc.removeSelectedText();
        if(line.contains(end))
          return;
        tc = _textEditor->textCursor();
        tc.select(QTextCursor::BlockUnderCursor);
        line = tc.selection().toPlainText();
        int c = 0;
        while(!line.contains(end) && c < 100)
        {
          tc.removeSelectedText();
          tc.select(QTextCursor::BlockUnderCursor);
          _textEditor->moveCursor(QTextCursor::Down);
          tc = _textEditor->textCursor();
          tc.select(QTextCursor::BlockUnderCursor);
          line = tc.selection().toPlainText();
          c++;
        }
      }
      else
      {
        _textEditor->moveCursor(QTextCursor::Down);
      }
      QString text = _textEditor->toPlainText();
      text = text.trimmed();
      _textEditor->setPlainText(text);
    }
  }
}

void
ModelEditor::_deletePackageFiles(int idx)
{
  ModelInfo mi = _models->value(idx);
  QString bfn = mi.baseName();
  QString path = mi.path();
  QFile file(path + "/" + "pkg_" + bfn + ".moo");
  file.remove();
}

QString
ModelEditor::startTime()
{
  return _getAnnotations("StartTime");
}

QString
ModelEditor::stopTime()
{
  return _getAnnotations("StopTime");
}

QString
ModelEditor::tolerance()
{
  return _getAnnotations("Tolerance");
}

QString
ModelEditor::absTolerance()
{
  return _getAnnotations("AbsTolerance");
}

QString
ModelEditor::DT()
{
  return _getAnnotations("MMO_DT_Min");
}

QString
ModelEditor::solver()
{
  return _getAnnotations("MMO_Solver");
}

QString
ModelEditor::minStep()
{
  return _getAnnotations("MMO_MinStep");
}

QString
ModelEditor::modelName()
{
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  QString line;
  if(_textEditor->find(QString("model ")))
  {
    tc = _textEditor->textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    line = tc.selection().toPlainText();
    return line.remove("model ").trimmed();
  }
  return "";
}

QString
ModelEditor::zcHyst()
{
  return _getAnnotations("MMO_ZCHyst");
}

QString
ModelEditor::derDelta()
{
  return _getAnnotations("MMO_DerDelta");
}

QString
ModelEditor::symDiff()
{
  return _getAnnotations("MMO_SymDiff");
}

QString
ModelEditor::description()
{
  return _getAnnotations("MMO_Description").remove('"');
}

QString
ModelEditor::output()
{
  return _getAnnotations("MMO_Output");
}

QString
ModelEditor::outputType()
{
  return _getAnnotations("MMO_OutputType");
}

QString
ModelEditor::period()
{
  return _getAnnotations("MMO_Period");
}

QString
ModelEditor::scheduler()
{
  return _getAnnotations("MMO_Scheduler");
}

QString
ModelEditor::LPS()
{
  return _getAnnotations("MMO_LPS");
}

void
ModelEditor::writeAnnotations()
{
  QString mName = modelName();
  _deleteAnnotations();
  int tab = _model_editor_tab->currentIndex();
  CodeEditor *_textEditor = qobject_cast<CodeEditor*>(
      _model_editor_tab->widget(tab));
  QString end = "end ";
  end.append(mName).append(";");
  _addLine(QString("\tannotation("));
  _addLine(QString("\texperiment("));
  _setAnnotations("MMO_Description", _description, true);
  _setAnnotations("MMO_Solver", _solver, true);
  if(!_minstep.isEmpty())
    _setAnnotations("MMO_MinStep", _minstep, true);
  if(!_zchyst.isEmpty())
    _setAnnotations("MMO_ZCHyst", _zchyst, true);
  if(!_derdelta.isEmpty())
    _setAnnotations("MMO_DerDelta", _derdelta, true);
  if(!_symdiff.isEmpty())
    _setAnnotations("MMO_SymDiff", _symdiff, true);
  if(!_period.isEmpty())
    _setAnnotations("MMO_Period", _period, true);
  if(!_parallel.isEmpty())
    _setAnnotations("MMO_Parallel", _parallel, true);
  if(!_partitionMethod.isEmpty())
    _setAnnotations("MMO_PartitionMethod", _partitionMethod, true);
  if(!_lps.isEmpty())
    _setAnnotations("MMO_LPS", _lps, true);
  if(!_dtSynch.isEmpty())
    _setAnnotations("MMO_DT_Synch", _dtSynch, true);
  if(!_dtPeriod.isEmpty())
    _setAnnotations("MMO_DT_SynchPeriod", _dtPeriod, true);
  if(!_dtStepLog.isEmpty())
    _setAnnotations("MMO_DT_StepLog", _dtStepLog, true);
  if(!_dt.isEmpty())
    _setAnnotations("MMO_DT_Min", _dt, true);
  if(!_output.isEmpty())
    _setAnnotations("MMO_Output", _output, true);
  if(!_outputType.isEmpty())
    _setAnnotations("MMO_OutputType", _outputType, true);
  if(!_scheduler.isEmpty())
    _setAnnotations("MMO_Scheduler", _scheduler, true);
  if(!_patohSettings.isEmpty())
    _setAnnotations("MMO_PatohSettings", _patohSettings, true);
  if(!_scotchSettings.isEmpty())
    _setAnnotations("MMO_ScotchSettings", _scotchSettings, true);
  if(!_metisSettings.isEmpty())
    _setAnnotations("MMO_MetisSettings", _metisSettings, true);
  if(!_jacobian.isEmpty())
    _setAnnotations("Jacobian", _jacobian, true);
  _setAnnotations("StartTime", _startTime, true);
  _setAnnotations("StopTime", _stopTime, true);
  _setAnnotations("Tolerance", _tolerance, true);
  _setAnnotations("AbsTolerance", _absTolerance, false);
  _addLine(QString("\t));"));
  QTextCursor tc = _textEditor->textCursor();
  tc.movePosition(QTextCursor::Start);
  _textEditor->setTextCursor(tc);
  if(_textEditor->find(end))
  {
    tc = _textEditor->textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    tc.removeSelectedText();
    _textEditor->moveCursor(QTextCursor::Up);
    tc = _textEditor->textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    QString line = tc.selection().toPlainText();
    int c = 0;
    while(line.isEmpty() && c < 100)
    {
      _textEditor->moveCursor(QTextCursor::Up);
      tc = _textEditor->textCursor();
      tc.select(QTextCursor::LineUnderCursor);
      line = tc.selection().toPlainText();
      c++;
    }
    _textEditor->moveCursor(QTextCursor::Down);
    foreach(QString an, _annotations)
    {    tc = _textEditor->textCursor();
    if(tc.atEnd())
    {
      _textEditor->insertPlainText("\n");
    }
    else
    {
      tc.select(QTextCursor::LineUnderCursor);
      line = tc.selection().toPlainText();
      if(!line.isEmpty())
      {
        _textEditor->moveCursor(QTextCursor::Up);
        _textEditor->insertPlainText("\n");
      }
    }
    _textEditor->insertPlainText(an);
    _textEditor->moveCursor(QTextCursor::Down);
  }
    tc = _textEditor->textCursor();
    if(tc.atEnd())
    {
      _textEditor->insertPlainText("\n");
    }
    else
    {
      tc.select(QTextCursor::LineUnderCursor);
      line = tc.selection().toPlainText();
      if(!line.isEmpty())
      {
        _textEditor->moveCursor(QTextCursor::Up);
        _textEditor->insertPlainText("\n");
      }
    }
    _textEditor->insertPlainText(end);
    tc = _textEditor->textCursor();
    if(tc.atEnd())
    {
      _textEditor->insertPlainText("\n");
    }
  }
  _annotations.clear();
}

QString
ModelEditor::patohSettings()
{
  return _getAnnotations("MMO_PatohSettings");
}

QString
ModelEditor::scotchSettings()
{
  return _getAnnotations("MMO_ScotchSettings");
}

QString
ModelEditor::metisSettings()
{
  return _getAnnotations("MMO_MetisSettings");
}

void
ModelEditor::setSemiStaticPartitioning(bool st)
{
  _semiStaticPartitioning = st;
}

bool
ModelEditor::semiStaticPartitioning()
{
  return _semiStaticPartitioning;
}
