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
#include <QMap>
#include <QStringListModel>
#include "./ui/ui_run.h"

#include "utils.hpp"

class RunDlg : public QDialog, public Ui::RunForm {
  Q_OBJECT
  public:
  enum class SolverFilter {
    QSS1,
    QSS2,
    QSS3,
    QSS4,
    CLASSIC,
    EXPERIMENTAL
  };

  RunDlg(QWidget* parent = nullptr);
  ~RunDlg() = default;
  inline QString absTolerance() { return _abs_tolerance->text(); };
  inline QString derDelta() { return _der_delta->text(); };
  inline QString DT() { return _dt->text(); };
  inline QString description() { return _description->toPlainText(); };
  inline QString minStep() { return _min_step->text(); };
  inline QString output() { return _output->text(); };
  inline QString outputType() { return getOutputTypeString(_comm_interval->currentIndex()); };
  inline QString period() { return _period->text(); };
  inline QString scheduler() { return getSchedulerString(_scheduler->currentIndex()); };
  inline QString solver() { return getSolverString(_solver->currentIndex()); };
  inline QString startTime() { return _start_time->text(); };
  inline QString stopTime() { return _stop_time->text(); };
  inline QString symDiff() { return getComboBoolString(_sym_diff->currentIndex()); };
  inline QString jacobian() { return getJacobianString(_jacobian->currentIndex()); };
  inline QString tolerance() { return _tolerance->text(); };
  inline QString zcHyst() { return _zc_hyst->text(); };
  inline void setAbsTolerance(QString str) { _abs_tolerance->setText(str.remove("{").remove("}")); };
  inline void setDerDelta(QString str) { _der_delta->setText(str); };
  inline void setDescription(QString str) { _description->setText(str); };
  inline void setMinStep(QString str) { _min_step->setText(str); };
  inline void setDT(QString str) { _dt->setText(str); };
  inline void setOutput(QString str) { _output->setText(str.remove("{").remove("}")); };
  inline void setOutputType(QString str) { _comm_interval->setCurrentIndex(getOutputTypeIdx(str)); };
  inline void setPeriod(QString str) { _period->setText(str); };
  inline void setScheduler(QString str) { _scheduler->setCurrentIndex(getSchedulerIdx(str)); };
  void setSolver(QString str);
  inline void setStartTime(QString str) { _start_time->setText(str); };
  inline void setStopTime(QString str) { _stop_time->setText(str); };
  inline void setSymDiff(QString str) { _sym_diff->setCurrentIndex(getComboBoolIdx(str)); };
  inline void setTolerance(QString str) { _tolerance->setText(str.remove("{").remove("}")); };
  inline void setZcHyst(QString str) { _zc_hyst->setText(str); };
  inline QString LPS() { return QString::number(_lps->value()); };
  inline void setLPS(QString lps) { _lps->setValue(lps.toInt()); };
  inline void setJacobian(QString str) { _jacobian->setCurrentIndex(str == "Sparse" ? 0 : 1); };
  inline QString parallel() { return getComboBoolString(_parallel->currentIndex()); };
  inline QString partitionMethod() { return getPartitionMethodString(_partition_method->currentIndex()); };
  inline void setParallel(QString str) { _parallel->setCurrentIndex(getComboBoolIdx(str)); };
  inline void setPartitionMethod(QString str) { _partition_method->setCurrentIndex(getPartitionMethodIdx(str)); };
  inline QString dtSynch() { return getDtSynchString(_dt_synch->currentIndex()); };
  inline void setDtSynch(QString str) { _dt_synch->setCurrentIndex(getDtSynchIdx(str)); };
  inline bool enableDebug() { return _debug_chk->checkState() == Qt::Checked; };
  inline QString patohSettings() { return _patoh_settings->text(); };
  inline void setPatohSettings(QString str) { _patoh_settings->setText(str.remove("{").remove("}")); };
  inline QString scotchSettings() { return _scotch_settings->text(); };
  inline void setScotchSettings(QString str) { _scotch_settings->setText(str.remove("{").remove("}")); };
  inline QString metisSettings() { return _metis_settings->text(); };
  inline void setMetisSettings(QString str) { _metis_settings->setText(str.remove("{").remove("}")); };
  inline bool semiStaticPartitioning() { return _semi_static_chk->checkState() == Qt::Checked; };
  inline void setSemiStaticPartitioning(bool st) { _semi_static_chk->setChecked(st); };
  inline QString generateArch() { return getComboBoolString(_generate_arch_cbx->currentIndex()); };
  inline void setGenerateArch(QString str) { _generate_arch_cbx->setCurrentIndex(getComboBoolIdx(str)); };
  inline QString reorderPartition() { return getComboBoolString(_reorder_partition_cbx->currentIndex()); };
  inline void setReorderPartition(QString str) { _reorder_partition_cbx->setCurrentIndex(getComboBoolIdx(str)); };
  inline QString debugGraph() { return getComboBoolString(_debug_graph_cbx->currentIndex()); };
  inline void setDebugGraph(QString str) { _debug_graph_cbx->setCurrentIndex(getComboBoolIdx(str)); };
  inline QString imbalance() { return _imbalance->text(); };
  inline void setImbalance(QString str) { _imbalance->setText(str); };
  inline QString BDFPart() { return _BDF_part->text(); };
  inline void setBDFPart(QString str) { _BDF_part->setText(str.remove("{").remove("}")); };
  inline QString BDFPartitionDepth() { return QString::number(_BDF_partition_depth->value()); };
  inline void setBDFPartitionDepth(QString d) { _BDF_partition_depth->setValue(d.toInt()); };
  inline QString BDFMaxStep() { return _BDF_max_step->text(); };
  inline void setBDFMaxStep(QString d) { _BDF_max_step->setText(d); };

  private slots:
  void on__comm_interval_currentIndexChanged(int index);
  void on__parallel_currentIndexChanged(int index);
  void on__show_all_stateChanged(int state);
  void on__dt_synch_currentIndexChanged(int index);

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
  void updateQSSSolvers();
  void updateSolvers(RunDlg::SolverFilter active_solvers);
  void updateSolversChk(RunDlg::SolverFilter enabled_solvers, Qt::CheckState state, QCheckBox* filter_cmb_1, QCheckBox* filter_cmb_2);

  Utils* _utils;
  QDoubleValidator* _validate;
  RunDlg::SolverFilter _current_solver_map_index{RunDlg::SolverFilter::QSS2};
};
