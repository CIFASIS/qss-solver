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

#ifndef RUNFORM_H_
#define RUNFORM_H_

#include <QtGui>
#include "./ui/ui_run.h"
#include "utils.h"

/**
 *
 */
class RunDlg: public QDialog, public Ui::RunForm
{
  Q_OBJECT
    public:
    /**
     *
     * @param parent
     */
    RunDlg(QWidget *parent = NULL);
    /**
     *
     */
    ~RunDlg() {};
    /**
     *
     * @return
     */
    inline QString
    absTolerance() { return _absTolerance->text(); };
    /**
     *
     * @return
     */
    inline QString
    derDelta() { return _derDelta->text(); };
    /**
     *
     * @return
     */
    inline QString
    DT() { return _dt->text(); };
    /**
     *
     * @return
     */
    inline QString
    description() { return _description->toPlainText(); };
    /**
     *
     * @return
     */
    inline QString
    minStep() { return _minStep->text(); };
    /**
     *
     * @return
     */
    inline QString
    output() { return _output->text(); };
    /**
     *
     * @return
     */
    inline QString
    outputType() { return getOutputTypeString(_commInterval->currentIndex()); };
    /**
     *
     * @return
     */
    inline QString
    period() { return _period->text(); };
    /**
     *
     * @return
     */
    inline QString
    scheduler() { return getSchedulerString(_scheduler->currentIndex()); };
    /**
     *
     * @return
     */
    inline QString
    solver() { return getSolverString(_solver->currentIndex()); };
    /**
     *
     * @return
     */
    inline QString
    startTime() { return _startTime->text(); };
    /**
     *
     * @return
     */
    inline QString
    stopTime() { return _stopTime->text(); };
    /**
     *
     * @return
     */
    inline QString
    symDiff() { return getComboBoolString(_symDiff->currentIndex()); };
    /**
     *
     * @return
     */
    inline QString
    jacobian() { return getJacobianString(_jacobian->currentIndex()); };
    /**
     *
     * @return
     */
    inline QString
    tolerance() { return _tolerance->text(); };
    /**
     *
     * @return
     */
    inline QString
    zcHyst() { return _zcHyst->text(); };
    /**
     *
     * @return
     */
    inline QString
    LPS() { return _lps->text(); };
    /**
     *
     * @param str
     */
    inline void
    setAbsTolerance(QString str) { _absTolerance->setText(str.remove("{").remove("}")); };
    /**
     *
     * @param str
     */
    inline void
    setDerDelta(QString str) { _derDelta->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setDescription(QString str) { _description->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setMinStep(QString str) { _minStep->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setDT(QString str) { _dt->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setOutput(QString str) { _output->setText(str.remove("{").remove("}")); };
    /**
     *
     * @param str
     */
    inline void
    setOutputType(QString str) { _commInterval->setCurrentIndex(getOutputTypeIdx(str)); };
    /**
     *
     * @param str
     */
    inline void
    setPeriod(QString str) { _period->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setScheduler(QString str) { _scheduler->setCurrentIndex(getSchedulerIdx(str)); };
    /**
     *
     * @param str
     */
    inline void
    setSolver(QString str) { _solver->setCurrentIndex(getSolverIdx(str)); };
    /**
     *
     * @param str
     */
    inline void
    setStartTime(QString str) { _startTime->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setStopTime(QString str) { _stopTime->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setSymDiff(QString str) { _symDiff->setCurrentIndex(getComboBoolIdx(str)); };
    /**
     *
     * @param str
     */
    inline void
    setTolerance(QString str) { _tolerance->setText(str.remove("{").remove("}")); };
    /**
     *
     * @param str
     */
    inline void
    setZcHyst(QString str) { _zcHyst->setText(str); };
    /**
     *
     * @param str
     */
    inline void
    setLPS(QString str) { _lps->setText(str); };
    inline void
    setJacobian(QString str) { _jacobian->setCurrentIndex(str == "Sparse" ? 0 : 1); };
    /**
     *
     * @return
     */
    inline QString
    parallel() { return getComboBoolString(_parallel->currentIndex()); };
    /**
     *
     * @return
     */
    inline QString
    partitionMethod() { return getPartitionMethodString(_partitionMethod->currentIndex()); };
    /**
     *
     * @param str
     */
    inline void
    setParallel(QString str) { _parallel->setCurrentIndex(getComboBoolIdx(str)); };
    /**
     *
     * @param str
     */
    inline void
    setPartitionMethod(QString str) { _partitionMethod->setCurrentIndex(getPartitionMethodIdx(str)); };
    /**
     *
     * @return
     */
    inline QString
    dtSynch() { return getDtSynchString(_dtSynch->currentIndex()); };
    /**
     *
     * @param str
     */
    inline void
    setDtSynch(QString str) { _dtSynch->setCurrentIndex(getDtSynchIdx(str)); };
    /**
     *
     * @return
     */
    inline bool
    enableDebug() { return _debugChk->checkState() == Qt::Checked; };
    inline QString
    patohSettings() { return _patohSettings->text(); };
    inline void
    setPatohSettings(QString str) { _patohSettings->setText(str.remove("{").remove("}")); };
    inline QString
    scotchSettings() { return _scotchSettings->text(); };
    inline void
    setScotchSettings(QString str) { _scotchSettings->setText(str.remove("{").remove("}")); };
    inline QString
    metisSettings() { return _metisSettings->text(); };
    inline void
    setMetisSettings(QString str) { _metisSettings->setText(str.remove("{").remove("}")); };
    inline bool
    semiStaticPartitioning() { return _semiStaticChk->checkState() == Qt::Checked; };
    inline void
    setSemiStaticPartitioning(bool st) { _semiStaticChk->setChecked(st); };
    /**
     *
     * @return
     */
    inline QString
    generateArch() { return getComboBoolString(_generateArchCbx->currentIndex()); };
    /**
     *
     * @param str
     */
    inline void
    setGenerateArch(QString str) { _generateArchCbx->setCurrentIndex(getComboBoolIdx(str)); };
    /**
     *
     * @return
     */
    inline QString
    reorderPartition() { return getComboBoolString(_reorderPartitionCbx->currentIndex()); };
    /**
     *
     * @param str
     */
    inline void
    setReorderPartition(QString str) { _reorderPartitionCbx->setCurrentIndex(getComboBoolIdx(str)); };
    /**
     *
     * @return
     */
    inline QString
    debugGraph() { return getComboBoolString(_debugGraphCbx->currentIndex()); };
    /**
     *
     * @param str
     */
    inline void
    setDebugGraph(QString str) { _debugGraphCbx->setCurrentIndex(getComboBoolIdx(str)); };
    /**
     *
     * @return
     */
    inline QString
    imbalance() { return _imbalance->text(); };
     /**
     *
     * @param str
     */
    inline void
    setImbalance(QString str) { _imbalance->setText(str); };
    inline QString
    BDFPart() { return _BDFPart->text(); };
    inline void
    setBDFPart(QString str) { _BDFPart->setText(str.remove("{").remove("}")); };
    inline QString
    BDFPartitionDepth() { return QString::number(_BDFPartitionDepth->value()); };
    inline void
    setBDFPartitionDepth(QString d) { _BDFPartitionDepth->setValue(d.toInt()); };
  private slots:
    void
    on__commInterval_currentIndexChanged(int index);
    void
    on__parallel_currentIndexChanged(int index);
    void
    on__showAll_stateChanged(int state);
    void
    on__dtSynch_currentIndexChanged(int index);
  private:
    int
    getOutputTypeIdx(QString str);
    QString
    getOutputTypeString(int idx);
    int
    getSchedulerIdx(QString str);
    QString
    getSchedulerString(int idx);
    int
    getSolverIdx(QString str);
    QString
    getSolverString(int idx);
    QString
    getJacobianString(int idx);
    int
    getComboBoolIdx(QString str);
    QString
    getComboBoolString(int idx);
    int
    getPartitionMethodIdx(QString str);
    QString
    getPartitionMethodString(int idx);
    int
    getDtSynchIdx(QString str);
    int
    getJacobianIdx(QString str);
    QString
    getDtSynchString(int idx);
    Utils*            _utils;
    QDoubleValidator* _validate;
    QIntValidator*    _validateInt;
};

#endif  /* RUNFORM_H_ */
