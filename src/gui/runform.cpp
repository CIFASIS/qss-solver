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

#include <Qt>
#include <QtGui>
#include <QListView>

#include <runform.hpp>
#include <utils.hpp>

static const QStringList first_order_solvers = {"QSS1", "EQSS1", "CHEQSS1", "LIQSS1"};

static const QStringList second_order_solvers = {"QSS2", "EQSS2", "CHEQSS2", "LIQSS2", "LIQSS_BDF"};

static const QStringList third_order_solvers = {"QSS3", "EQSS3", "CHEQSS3", "LIQSS3"};

static const QStringList fourth_order_solvers = {"QSS4"};

static const QStringList classic_solvers = {"DASSL", "DOPRI", "CVODE_BDF", "CVODE_AM", "IDA"};

static const QStringList experimental_solvers = {"mLIQSS", "mLIQSS2", "CQSS1", "CQSS2", "CQSS3"};

static const QMap<RunDlg::SolverFilter, QStringList> solvers_map = {
    {RunDlg::SolverFilter::QSS1, first_order_solvers}, {RunDlg::SolverFilter::QSS2, second_order_solvers},
    {RunDlg::SolverFilter::QSS3, third_order_solvers}, {RunDlg::SolverFilter::QSS4, fourth_order_solvers},
    {RunDlg::SolverFilter::CLASSIC, classic_solvers},  {RunDlg::SolverFilter::EXPERIMENTAL, experimental_solvers}};

RunDlg::RunDlg(QWidget* parent) : QDialog(parent)
{
  setupUi(this);
  _utils = new Utils();
  _validate = new QDoubleValidator();

  // Set validators for input fields
  _start_time->setValidator(_validate);
  _stop_time->setValidator(_validate);
  _tolerance->setValidator(_validate);
  _dt->setValidator(_validate);
  _abs_tolerance->setValidator(_validate);
  _min_step->setValidator(_validate);
  _der_delta->setValidator(_validate);
  _zc_hyst->setValidator(_validate);

  _extended_frame->setVisible(false);
  _debug_chk->setCheckState(Qt::Unchecked);

  on__parallel_currentIndexChanged(_parallel->currentIndex());

  _qss_order_spb->setMinimum(1);
  _qss_order_spb->setMaximum(4);

  connect(_qss_chk, &QCheckBox::stateChanged, this, &RunDlg::updateQSSSolvers);
  connect(_qss_order_spb, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() { updateQSSSolvers(); });
  connect(_classic_chk, &QCheckBox::stateChanged, this,
          [this]() { updateSolversChk(RunDlg::SolverFilter::CLASSIC, _classic_chk->checkState(), _qss_chk, _exp_chk); });
  connect(_exp_chk, &QCheckBox::stateChanged, this,
          [this]() { updateSolversChk(RunDlg::SolverFilter::EXPERIMENTAL, _exp_chk->checkState(), _qss_chk, _classic_chk); });
}

void RunDlg::on__show_all_stateChanged(int state) { _extended_frame->setVisible(state == Qt::Checked); }

void RunDlg::on__comm_interval_currentIndexChanged(int index) { _period->setEnabled(index != 0); }

void RunDlg::on__dt_synch_currentIndexChanged(int index) { _dt_lbl->setText(index == 0 ? "Dt tolerance" : "Dt value"); }

void RunDlg::on__parallel_currentIndexChanged(int index)
{
  bool is_enabled = (index == 1);
  _lps->setEnabled(is_enabled);
  _partition_method->setEnabled(is_enabled);
  _dt->setEnabled(is_enabled);
  _dt_synch->setEnabled(is_enabled);
  _scotch_settings->setEnabled(is_enabled);
  _metis_settings->setEnabled(is_enabled);
  _patoh_settings->setEnabled(is_enabled);
  _semi_static_chk->setEnabled(is_enabled);
  _generate_arch_cbx->setEnabled(is_enabled);
  _debug_graph_cbx->setEnabled(is_enabled);
  _reorder_partition_cbx->setEnabled(is_enabled);
  _imbalance->setEnabled(is_enabled);
}

int RunDlg::getComboBoolIdx(QString str)
{
  static const QStringList types = {"false", "true"};
  return types.indexOf(str.trimmed());
}

QString RunDlg::getComboBoolString(int idx)
{
  static const QStringList types = {"false", "true"};
  return (idx >= 0 && idx < types.size()) ? types[idx] : QString();
}

int RunDlg::getPartitionMethodIdx(QString str)
{
  static const QStringList types = {"Metis", "HMetis", "Scotch", "Patoh", "Manual"};
  return types.indexOf(str.trimmed());
}

QString RunDlg::getPartitionMethodString(int idx)
{
  static const QStringList types = {"Metis", "HMetis", "Scotch", "Patoh", "Manual"};
  return (idx >= 0 && idx < types.size()) ? types[idx] : QString();
}

int RunDlg::getJacobianIdx(QString str)
{
  static const QStringList types = {"Sparse", "Dense"};
  return types.indexOf(str.trimmed());
}

QString RunDlg::getJacobianString(int idx)
{
  static const QStringList types = {"Sparse", "Dense"};
  return (idx >= 0 && idx < types.size()) ? types[idx] : QString();
}

int RunDlg::getDtSynchIdx(QString str)
{
  static const QStringList types = {"SD_DT_Fixed", "SD_DT_Asynchronous"};
  return types.indexOf(str.trimmed());
}

QString RunDlg::getDtSynchString(int idx)
{
  static const QStringList types = {"SD_DT_Fixed", "SD_DT_Asynchronous"};
  return (idx >= 0 && idx < types.size()) ? types[idx] : QString();
}

int RunDlg::getSchedulerIdx(QString str)
{
  static const QStringList types = {"ST_Binary", "ST_Random", "ST_Linear"};
  return types.indexOf(str.trimmed());
}

QString RunDlg::getSchedulerString(int idx)
{
  static const QStringList types = {"ST_Binary", "ST_Random", "ST_Linear"};
  return (idx >= 0 && idx < types.size()) ? types[idx] : QString();
}

int RunDlg::getSolverIdx(QString str)
{
  for (auto m = solvers_map.keyValueBegin(); m != solvers_map.keyValueEnd(); ++m) {
    if (m->second.contains(str.trimmed())) {
      _current_solver_map_index = m->first;
      return m->second.indexOf(str.trimmed());
    }
  }
  return -1;
}

void RunDlg::setSolver(QString str)
{
  int index = getSolverIdx(str);
  switch (_current_solver_map_index) {
  case RunDlg::SolverFilter::CLASSIC:
    _classic_chk->setCheckState(Qt::Checked);
    break;
  case RunDlg::SolverFilter::EXPERIMENTAL:
    _exp_chk->setCheckState(Qt::Checked);
    break;
  case RunDlg::SolverFilter::QSS1:
    _qss_order_spb->setValue(1);
    _qss_chk->setCheckState(Qt::Checked);
    break;
  case RunDlg::SolverFilter::QSS2:
    _qss_order_spb->setValue(2);
    _qss_chk->setCheckState(Qt::Checked);
    break;
  case RunDlg::SolverFilter::QSS3:
    _qss_order_spb->setValue(3);
    _qss_chk->setCheckState(Qt::Checked);
    break;
  case RunDlg::SolverFilter::QSS4:
    _qss_order_spb->setValue(4);
    _qss_chk->setCheckState(Qt::Checked);
  }
  _solver->setCurrentIndex(index);
};

QString RunDlg::getSolverString(int idx)
{
  return (idx >= 0 && idx < solvers_map[_current_solver_map_index].size()) ? solvers_map[_current_solver_map_index].at(idx) : QString();
}

int RunDlg::getOutputTypeIdx(QString str)
{
  static const QStringList output_types = {"CI_Step", "CI_Sampled", "CI_Dense"};
  return output_types.indexOf(str.trimmed());
}

QString RunDlg::getOutputTypeString(int idx)
{
  static const QStringList output_types = {"CI_Step", "CI_Sampled", "CI_Dense"};
  return (idx >= 0 && idx < output_types.size()) ? output_types[idx] : QString();
}

void RunDlg::updateSolversChk(RunDlg::SolverFilter enabled_solvers, Qt::CheckState state, QCheckBox* filter_cmb_1, QCheckBox* filter_cmb_2)
{
  if (state == Qt::Checked) {
    updateSolvers(enabled_solvers);
    filter_cmb_1->setCheckState(Qt::Unchecked);
    filter_cmb_2->setCheckState(Qt::Unchecked);
  } else if (_qss_chk->checkState() == Qt::Unchecked && _classic_chk->checkState() == Qt::Unchecked &&
             _exp_chk->checkState() == Qt::Unchecked) {
    _qss_order_spb->setValue(2);
    _qss_chk->setCheckState(Qt::Checked);
  }
}

void RunDlg::updateQSSSolvers()
{
  Qt::CheckState state = _qss_chk->checkState();
  switch (_qss_order_spb->value()) {
  case 1:
    updateSolversChk(RunDlg::SolverFilter::QSS1, state, _classic_chk, _exp_chk);
    break;
  case 2:
    updateSolversChk(RunDlg::SolverFilter::QSS2, state, _classic_chk, _exp_chk);
    break;
  case 3:
    updateSolversChk(RunDlg::SolverFilter::QSS3, state, _classic_chk, _exp_chk);
    break;
  case 4:
    updateSolversChk(RunDlg::SolverFilter::QSS4, state, _classic_chk, _exp_chk);
    break;
  default:
    break;
  }
}

void RunDlg::updateSolvers(RunDlg::SolverFilter active_solvers)
{
  _current_solver_map_index = active_solvers;
  _solver->clear();
  _solver->addItems(solvers_map[active_solvers]);
}
