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

#ifndef MODELEDITOR_H_
#define MODELEDITOR_H_

#include <QWidget>
#include <QTextCursor>
#include <ui/ui_modeleditor.h>
#include <utils.h>

class ModelInfo;
class MmoHighlighter;

/**
 *
 */
class ModelEditor : public QDialog, public Ui::ModelEditorForm {
  Q_OBJECT
  public:
  /**
   *
   * @param parent
   * @param name
   */
  ModelEditor(QWidget *parent = NULL, QString name = QString());
  /**
   *
   */
  ~ModelEditor();
  /**
   *
   * @return
   */
  inline QString absTolerance() { return getAnnotations("AbsTolerance"); };
  /**
   *
   * @return
   */
  QString activeBaseFileName();
  /**
   *
   * @return
   */
  bool activeDirty();
  /**
   *
   * @return
   */
  QString activeFileName();
  /**
   *
   * @return
   */
  inline int activeFileIndex() { return _model_editor_tab->currentIndex(); };
  /**
   *
   * @return
   */
  QString activeFullFileName();
  /**
   *
   * @param idx
   * @return
   */
  QString baseFileName(int idx);
  /**
   *
   * @return
   */
  inline bool checkModel() { return checkToken("model "); };
  /**
   *
   * @return
   */
  inline bool checkPackage() { return checkToken("package "); };
  /**
   *
   */
  void closeFiles();
  /**
   *
   * @return
   */
  inline int count() { return _model_editor_tab->count(); };
  /**
   *
   * @return
   */
  inline QString derDelta() { return getAnnotations("MMO_DerDelta"); };
  /**
   *
   * @return
   */
  inline QString description() { return getAnnotations("MMO_Description").remove('"'); };
  /**
   *
   * @param name
   */
  void editModel(QString name = QString());
  /**
   *
   * @param idx
   * @return
   */
  QString fileName(int idx);
  /**
   *
   * @return
   */
  QStringList fileNames();
  /**
   *
   * @param idx
   * @return
   */
  QString fullFileName(int idx);
  /**
   *
   * @return
   */
  inline QString minStep() { return getAnnotations("MMO_MinStep"); };
  /**
   *
   * @return
   */
  QString modelName();
  /**
   *
   * @return
   */
  QString newFileName();
  /**
   *
   * @return
   */
  inline QString output() { return getAnnotations("MMO_Output"); };
  /**
   *
   * @return
   */
  inline QString outputType() { return getAnnotations("MMO_OutputType"); };
  /**
   *
   * @return
   */
  inline QString period() { return getAnnotations("MMO_Period"); };
  /**
   *
   * @return
   */
  inline QString scheduler() { return getAnnotations("MMO_Scheduler"); };
  /**
   *
   * @return
   */
  inline QString solver() { return getAnnotations("MMO_Solver"); };
  /**
   *
   * @return
   */
  inline QString startTime() { return getAnnotations("StartTime"); };
  /**
   *
   * @return
   */
  inline QString stopTime() { return getAnnotations("StopTime"); };
  /**
   *
   * @return
   */
  inline QString symDiff() { return getAnnotations("MMO_SymDiff"); };
  /**
   *
   * @return
   */
  inline QString parallel() { return getAnnotations("MMO_Parallel"); };
  /**
   *
   * @return
   */
  inline QString partitionMethod() { return getAnnotations("MMO_PartitionMethod"); };
  /**
   *
   * @return
   */
  inline QString DT() { return getAnnotations("MMO_DT_Min"); };
  /**
   *
   * @return
   */
  inline QString tolerance() { return getAnnotations("Tolerance"); };
  /**
   *
   * @param str
   */
  inline void setAbsTolerance(QString str) { _absTolerance = str; };
  /**
   *
   * @param str
   */
  inline void setDerDelta(QString str) { _derdelta = findValue("MMO_DerDelta", str); };
  /**
   *
   * @param str
   */
  inline void setDescription(QString str)
  {
    _description.clear();
    _description.append("\"").append(str).append("\"");
  };
  /**
   *
   * @param str
   */
  inline void setMinStep(QString str) { _minstep = findValue("MMO_MinStep", str); };
  /**
   *
   * @param str
   */
  inline void setOutput(QString str) { _output = str; };
  /**
   *
   * @param str
   */
  inline void setOutputType(QString str) { _outputType = findValue("MMO_OutputType", str); };
  /**
   *
   * @param str
   */
  inline void setPeriod(QString str) { _period = findValue("MMO_Period", str); };
  /**
   *
   * @param str
   */
  inline void setScheduler(QString str) { _scheduler = findValue("MMO_Scheduler", str); };
  /**
   *
   * @param str
   */
  inline void setSolver(QString str) { _solver = str; };
  /**
   *
   * @param str
   */
  inline void setStartTime(QString str) { _startTime = str; };
  /**
   *
   * @param str
   */
  inline void setStopTime(QString str) { _stopTime = str; };
  /**
   *
   * @param str
   */
  inline void setSymDiff(QString str) { _symdiff = findValue("MMO_SymDiff", str); };
  /**
   *
   * @param str
   */
  inline void setTolerance(QString str) { _tolerance = str; };
  /**
   *
   * @param str
   */
  inline void setZcHyst(QString str) { _zchyst = findValue("MMO_ZCHyst", str); };
  /**
   *
   * @param str
   */
  inline void setLPS(QString str) { _lps = findValue("MMO_LPS", str); };
  /**
   *
   */
  void writeAnnotations();
  /**
   *
   * @return
   */
  inline QString zcHyst() { return getAnnotations("MMO_ZCHyst"); };
  /**
   *
   * @return
   */
  inline QString LPS() { return getAnnotations("MMO_LPS"); };
  /**
   *
   * @param str
   */
  inline void setParallel(QString str) { _parallel = findValue("MMO_Parallel", str); };
  /**
   *
   * @return
   */
  inline bool isParallel() { return _parallel.compare("true") == 0; };
  /**
   *
   * @param str
   */
  inline void setPartitionMethod(QString str) { _partitionMethod = findValue("MMO_PartitionMethod", str); };
  /**
   *
   * @param str
   */
  inline void setJacobian(QString str) { _jacobian = findValue("Jacobian", str); };
  inline QString jacobian() { return _jacobian; };
  /**
   *
   * @param str
   */
  inline void setDT(QString str) { _dt = findValue("MMO_DT_Min", str); };
  /**
   *
   * @param str
   */
  inline void setDtSynch(QString str) { _dtSynch = findValue("MMO_DT_Synch", str); };
  /**
   *
   * @return
   */
  inline QString dtSynch() { return getAnnotations("MMO_DT_Synch"); };
  /**
   *
   * @param str
   */
  inline void setDtPeriod(QString str) { _dtPeriod = findValue("MMO_DT_SynchPeriod", str); };
  inline QString dtPeriod() { return getAnnotations("MMO_DT_SynchPeriod"); };
  /**
   *
   * @param str
   */
  inline void setDtStepLog(QString str) { _dtStepLog = findValue("MMO_DT_StepLog", str); };
  /**
   *
   * @return
   */
  inline QString dtStepLog() { return getAnnotations("MMO_DT_StepLog"); };
  inline void setPatohSettings(QString str) { _patohSettings = str; };
  inline void setScotchSettings(QString str) { _scotchSettings = str; };
  inline void setMetisSettings(QString str) { _metisSettings = str; };
  inline QString patohSettings() { return getAnnotations("MMO_PatohSettings"); };
  inline QString scotchSettings() { return getAnnotations("MMO_ScotchSettings"); };
  inline QString metisSettings() { return getAnnotations("MMO_MetisSettings"); };
  inline void setSemiStaticPartitioning(bool st) { _semiStaticPartitioning = st; };
  inline bool semiStaticPartitioning() { return _semiStaticPartitioning; };
  /**
   *
   * @param str
   */
  inline void setImbalance(QString str) { _imbalance = findValue("MMO_Imbalance", str); };
  /**
   *
   * @return
   */
  inline QString imbalance() { return getAnnotations("MMO_Imbalance"); };
  /**
   *
   * @param str
   */
  inline void setReorderPartition(QString str) { _reorderPartition = findValue("MMO_ReorderPartition", str); };
  /**
   *
   * @return
   */
  inline QString reorderPartition() { return getAnnotations("MMO_ReorderPartition"); };
  /**
   *
   * @param str
   */
  inline void setDebugGraph(QString str) { _debugGraph = findValue("MMO_DebugGraph", str); };
  /**
   *
   * @return
   */
  inline QString debugGraph() { return getAnnotations("MMO_DebugGraph"); };
  /**
   *
   * @param str
   */
  inline void setGenerateArch(QString str) { _generateArch = findValue("MMO_GenerateArch", str); };
  /**
   *
   * @return
   */
  inline QString generateArch() { return getAnnotations("MMO_GenerateArch"); };
  inline QString BDFPart() { return getAnnotations("MMO_BDF_Part"); };
  inline void setBDFPart(QString str) { _BDFPart = str; };
  inline QString BDFPartitionDepth() { return getAnnotations("MMO_BDF_PDepth"); };
  inline void setBDFPartitionDepth(QString str) { _BDFPartitionDepth = str; };

  protected:
  void reject(){};
  public slots:
  void keyReleaseEvent(QKeyEvent *event);
  void save(void);
  void saveAll(void);
  void saveAs(QString name = QString());
  private slots:
  void on__model_editor_tab_currentChanged(int index);
  void on__model_editor_tab_tabCloseRequested(int index);
  void on__textEditor_textChanged(bool changed);
  signals:
  /**
   *
   */
  void clean(int);
  /**
   *
   * @param name
   * @param ext
   */
  void done(QString name, QString ext);

  private:
  inline void addLine(QString str) { _annotations << str; };
  inline int beginModel() { return tokenPosition("model "); };
  inline int endModel() { return tokenPosition("end " + modelName()); };
  int controlEmptyLines(int position);
  bool checkToken(QString str);
  bool checkAnnotations();
  int checkFunctions(int modelInit, int modelEnd);
  void remove(int tab);
  void deleteAnnotations();
  void deletePackageFiles(int idx);
  QString findValue(QString token, QString str);
  QString getAnnotations(QString str);
  QString getAnnotationValue(QString value, QString token);
  bool lineEmpty();
  void setAnnotations(QString str, QString value, bool separator);
  void save(int tab);
  int tokenPosition(QString token);
  QStringList _annotations;
  QMap<QString, QString> _defaultValues;
  QString _startTime;
  QString _stopTime;
  QString _tolerance;
  QString _absTolerance;
  QString _solver;
  QString _output;
  QString _outputType;
  QString _period;
  QString _lps;
  QString _description;
  QString _minstep;
  QString _zchyst;
  QString _derdelta;
  QString _scheduler;
  QString _symdiff;
  QString _dtSynch;
  QString _dtPeriod;
  QString _dtStepLog;
  QString _dt;
  QString _parallel;
  QString _partitionMethod;
  QString _jacobian;
  QString _patohSettings;
  QString _scotchSettings;
  QString _metisSettings;
  QString _generateArch;
  QString _debugGraph;
  QString _reorderPartition;
  QString _imbalance;
  QString _BDFPart;
  QString _BDFPartitionDepth;
  bool _semiStaticPartitioning;
  QTabWidget *_model_editor_tab;
  QList<ModelInfo> *_models;
  Utils *_utils;
  MmoHighlighter *_hl;
};

#endif /* MODELEDITOR_H_ */
