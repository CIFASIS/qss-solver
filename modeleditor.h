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
class ModelEditor: public QDialog, public Ui::ModelEditorForm
{
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
    QString
    absTolerance();
    /**
     *
     * @return
     */
    QString
    activeBaseFileName();
    /**
     *
     * @return
     */
    bool
    activeDirty();
    /**
     *
     * @return
     */
    QString
    activeFileName();
    /**
     *
     * @return
     */
    int
    activeFileIndex();
    /**
     *
     * @return
     */
    QString
    activeFullFileName();
    /**
     *
     * @param idx
     * @return
     */
    QString
    baseFileName(int idx);
    /**
     *
     * @return
     */
    bool
    checkModel();
    /**
     *
     * @return
     */
    bool
    checkPackage();
    /**
     *
     */
    void
    closeFiles();
    /**
     *
     * @return
     */
    int
    count()
    {
      return _model_editor_tab->count();
    }
    ;
    /**
     *
     * @return
     */
    QString
    derDelta();
    /**
     *
     * @return
     */
    QString
    description();
    /**
     *
     * @param name
     */
    void
    editModel(QString name = QString());
    /**
     *
     * @param idx
     * @return
     */
    QString
    fileName(int idx);
    /**
     *
     * @return
     */
    QStringList
    fileNames();
    /**
     *
     * @param idx
     * @return
     */
    QString
    fullFileName(int idx);
    /**
     *
     * @return
     */
    QString
    minStep();
    /**
     *
     * @return
     */
    QString
    modelName();
    /**
     *
     * @return
     */
    QString
    newFileName();
    /**
     *
     * @return
     */
    QString
    output();
    /**
     *
     * @return
     */
    QString
    outputType();
    /**
     *
     * @return
     */
    QString
    period();
    /**
     *
     * @return
     */
    QString
    scheduler();
    /**
     *
     * @return
     */
    QString
    solver();
    /**
     *
     * @return
     */
    QString
    startTime();
    /**
     *
     * @return
     */
    QString
    stopTime();
    /**
     *
     * @return
     */
    QString
    symDiff();
    /**
     *
     * @return
     */
    QString
    parallel();
    /**
     *
     * @return
     */
    QString
    partitionMethod();
    /**
     *
     * @return
     */
    QString
    DT();
    /**
     *
     * @return
     */
    QString
    tolerance();
    /**
     *
     * @param str
     */
    void
    setAbsTolerance(QString str)
    {
      _absTolerance = str;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDerDelta(QString str)
    {
      _derdelta = _findValue("MMO_DerDelta", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDescription(QString str)
    {
      _description.clear();
      _description.append("\"").append(str).append("\"");
    }
    ;
    /**
     *
     * @param str
     */
    void
    setMinStep(QString str)
    {
      _minstep = _findValue("MMO_MinStep", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setOutput(QString str)
    {
      _output = str;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setOutputType(QString str)
    {
      _outputType = _findValue("MMO_OutputType", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setPeriod(QString str)
    {
      _period = _findValue("MMO_Period", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setScheduler(QString str)
    {
      _scheduler = _findValue("MMO_Scheduler", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setSolver(QString str)
    {
      _solver = str;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setStartTime(QString str)
    {
      _startTime = str;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setStopTime(QString str)
    {
      _stopTime = str;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setSymDiff(QString str)
    {
      _symdiff = _findValue("MMO_SymDiff", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setTolerance(QString str)
    {
      _tolerance = str;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setZcHyst(QString str)
    {
      _zchyst = _findValue("MMO_ZCHyst", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setLPS(QString str)
    {
      _lps = _findValue("MMO_LPS", str);
    }
    ;
    /**
     *
     */
    void
    writeAnnotations();
    /**
     *
     * @return
     */
    QString
    zcHyst();
    /**
     *
     * @return
     */
    QString
    LPS();
    /**
     *
     * @param str
     */
    void
    setParallel(QString str)
    {
      _parallel = _findValue("MMO_Parallel", str);
    }
    ;
    /**
     *
     * @return
     */
    bool
    isParallel()
    {
      return _parallel.compare("true") == 0;
    }
    ;
    /**
     *
     * @param str
     */
    void
    setPartitionMethod(QString str)
    {
      _partitionMethod = _findValue("MMO_PartitionMethod", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setJacobian(QString str)
    {
      _jacobian = _findValue("Jacobian", str);
    }
    ;
    QString
    jacobian()
    {
      return _jacobian;
    }
    ;

    /**
     *
     * @param str
     */
    void
    setDT(QString str)
    {
      _dt = _findValue("MMO_DT_Min", str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDtSynch(QString str)
    {
      _dtSynch = _findValue("MMO_DT_Synch", str);
    }
    ;
    /**
     *
     * @return
     */
    QString
    dtSynch()
    {
      return _getAnnotations("MMO_DT_Synch");
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDtPeriod(QString str)
    {
      _dtPeriod = _findValue("MMO_DT_SynchPeriod", str);
    }
    ;
    QString
    dtPeriod()
    {
      return _getAnnotations("MMO_DT_SynchPeriod");
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDtStepLog(QString str)
    {
      _dtStepLog = _findValue("MMO_DT_StepLog", str);
    }
    ;
    /**
     *
     * @return
     */
    QString
    dtStepLog()
    {
      return _getAnnotations("MMO_DT_StepLog");
    }
    ;
    void
    setPatohSettings(QString str)
    {
      _patohSettings = str;
    }
    ;
    void
    setScotchSettings(QString str)
    {
      _scotchSettings = str;
    }
    ;
    void
    setMetisSettings(QString str)
    {
      _metisSettings = str;
    }
    ;
    QString
    patohSettings();
    QString
    scotchSettings();
    QString
    metisSettings();
    void
    setSemiStaticPartitioning(bool st);
    bool
    semiStaticPartitioning();
    protected:
    void
    reject()
    {
    }
    ;
    public slots:
    void
    keyReleaseEvent(QKeyEvent *event);
    void
    save(void);
    void
    saveAll(void);
    void
    saveAs(QString name = QString());
  private slots:
    void
    on__model_editor_tab_currentChanged(int index);
    void
    on__model_editor_tab_tabCloseRequested(int index);
    void
    on__textEditor_textChanged(bool changed);signals:
    /**
     *
     */
    void
    clean(int);
    /**
     *
     * @param name
     * @param ext
     */
    void
    done(QString name, QString ext);
    private:
    void
    _addLine(QString str);
    int
    _beginModel();
    int
    _endModel();
    int
    _controlEmptyLines(int position);
    bool
    _checkToken(QString str);
    bool
    _checkAnnotations();
    int
    _checkFunctions(int modelInit, int modelEnd);
    void
    _delete(int tab);
    void
    _deleteAnnotations();
    void
    _deletePackageFiles(int idx);
    QString
    _findValue(QString token, QString str);
    QString
    _getAnnotations(QString str);
    QString
    _getAnnotationValue(QString value, QString token);
    bool
    _lineEmpty();
    void
    _setAnnotations(QString str, QString value, bool separator);
    void
    _save(int tab);
    int
    _tokenPosition(QString token);
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
    bool _semiStaticPartitioning;
    QTabWidget *_model_editor_tab;
    QList<ModelInfo> *_models;
    Utils *_utils;
    MmoHighlighter *_hl;
};

#endif  /* MODELEDITOR_H_ */
