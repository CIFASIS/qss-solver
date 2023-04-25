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

#include <QtGui>
#include "./ui/ui_run.h"
#include "utils.h"

class RunDlg : public QDialog, public Ui::RunForm {
  Q_OBJECT
  public:
  RunDlg(QWidget* parent = nullptr);
  ~RunDlg() = default;
  inline QString absTolerance() { return _absTolerance->text(); };
  inline QString derDelta() { return _derDelta->text(); };
  inline QString DT() { return _dt->text(); };
  inline QString description() { return _description->toPlainText(); };
  inline QString minStep() { return _minStep->text(); };
  inline QString output() { return _output->text(); };
  inline QString outputType() { return getOutputTypeString(_commInterval->currentIndex()); };
  inline QString period() { return _period->text(); };
  inline QString scheduler() { return getSchedulerString(_scheduler->currentIndex()); };
  inline QString solver() { return getSolverString(_solver->currentIndex()); };
  inline QString startTime() { return _startTime->text(); };
  inline QString stopTime() { return _stopTime->text(); };
  inline QString symDiff() { return getComboBoolString(_symDiff->currentIndex()); };
  inline QString jacobian() { return getJacobianString(_jacobian->currentIndex()); };
  inline QString tolerance() { return _tolerance->text(); };
  inline QString zcHyst() { return _zcHyst->text(); };
  inline void setAbsTolerance(QString str) { _absTolerance->setText(str.remove("{").remove("}")); };
  inline void setDerDelta(QString str) { _derDelta->setText(str); };
  inline void setDescription(QString str) { _description->setText(str); };
  inline void setMinStep(QString str) { _minStep->setText(str); };
  inline void setDT(QString str) { _dt->setText(str); };
  inline void setOutput(QString str) { _output->setText(str.remove("{").remove("}")); };
  inline void setOutputType(QString str) { _commInterval->setCurrentIndex(getOutputTypeIdx(str)); };
  inline void setPeriod(QString str) { _period->setText(str); };
  inline void setScheduler(QString str) { _scheduler->setCurrentIndex(getSchedulerIdx(str)); };
  inline void setSolver(QString str) { _solver->setCurrentIndex(getSolverIdx(str)); };
  inline void setStartTime(QString str) { _startTime->setText(str); };
  inline void setStopTime(QString str) { _stopTime->setText(str); };
  inline void setSymDiff(QString str) { _symDiff->setCurrentIndex(getComboBoolIdx(str)); };
  inline void setTolerance(QString str) { _tolerance->setText(str.remove("{").remove("}")); };
  inline void setZcHyst(QString str) { _zcHyst->setText(str); };
  inline QString LPS() { return QString::number(_lps->value()); };
  inline void setLPS(QString lps) { _lps->setValue(lps.toInt()); };
  inline void setJacobian(QString str) { _jacobian->setCurrentIndex(str == "Sparse" ? 0 : 1); };
  inline QString parallel() { return getComboBoolString(_parallel->currentIndex()); };
  inline QString partitionMethod() { return getPartitionMethodString(_partitionMethod->currentIndex()); };
  inline void setParallel(QString str) { _parallel->setCurrentIndex(getComboBoolIdx(str)); };
  inline void setPartitionMethod(QString str) { _partitionMethod->setCurrentIndex(getPartitionMethodIdx(str)); };
  inline QString dtSynch() { return getDtSynchString(_dtSynch->currentIndex()); };
  inline void setDtSynch(QString str) { _dtSynch->setCurrentIndex(getDtSynchIdx(str)); };
  inline bool enableDebug() { return _debugChk->checkState() == Qt::Checked; };
  inline QString patohSettings() { return _patohSettings->text(); };
  inline void setPatohSettings(QString str) { _patohSettings->setText(str.remove("{").remove("}")); };
  inline QString scotchSettings() { return _scotchSettings->text(); };
  inline void setScotchSettings(QString str) { _scotchSettings->setText(str.remove("{").remove("}")); };
  inline QString metisSettings() { return _metisSettings->text(); };
  inline void setMetisSettings(QString str) { _metisSettings->setText(str.remove("{").remove("}")); };
  inline bool semiStaticPartitioning() { return _semiStaticChk->checkState() == Qt::Checked; };
  inline void setSemiStaticPartitioning(bool st) { _semiStaticChk->setChecked(st); };
  inline QString generateArch() { return getComboBoolString(_generateArchCbx->currentIndex()); };
  inline void setGenerateArch(QString str) { _generateArchCbx->setCurrentIndex(getComboBoolIdx(str)); };
  inline QString reorderPartition() { return getComboBoolString(_reorderPartitionCbx->currentIndex()); };
  inline void setReorderPartition(QString str) { _reorderPartitionCbx->setCurrentIndex(getComboBoolIdx(str)); };
  inline QString debugGraph() { return getComboBoolString(_debugGraphCbx->currentIndex()); };
  inline void setDebugGraph(QString str) { _debugGraphCbx->setCurrentIndex(getComboBoolIdx(str)); };
  inline QString imbalance() { return _imbalance->text(); };
  inline void setImbalance(QString str) { _imbalance->setText(str); };
  inline QString BDFPart() { return _BDFPart->text(); };
  inline void setBDFPart(QString str) { _BDFPart->setText(str.remove("{").remove("}")); };
  inline QString BDFPartitionDepth() { return QString::number(_BDFPartitionDepth->value()); };
  inline void setBDFPartitionDepth(QString d) { _BDFPartitionDepth->setValue(d.toInt()); };
  inline QString BDFMaxStep() { return _BDFMaxStep->text(); };
  inline void setBDFMaxStep(QString d) { _BDFMaxStep->setText(d); };

  private slots:
  void on__commInterval_currentIndexChanged(int index);
  void on__parallel_currentIndexChanged(int index);
  void on__showAll_stateChanged(int state);
  void on__dtSynch_currentIndexChanged(int index);
  void updateTestMethods(int state);

  private:
  int getOutputTypeIdx(QString str);
  QString getOutputTypeString(int idx);
  int getSchedulerIdx(QString str);
  QString getSchedulerString(int idx);
  int getSolverIdx(QString str);
  QString getSolverString(int idx);
  QString getJacobianString(int idx);
  int getComboBoolIdx(QString str);
  QString getComboBoolString(int idx);
  int getPartitionMethodIdx(QString str);
  QString getPartitionMethodString(int idx);
  int getDtSynchIdx(QString str);
  int getJacobianIdx(QString str);
  QString getDtSynchString(int idx);
  Utils* _utils;
  QDoubleValidator* _validate;
};
