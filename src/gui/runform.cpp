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

RunDlg::RunDlg(QWidget *parent) : QDialog(parent)
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

  connect(_test_methods_cbx, &QCheckBox::stateChanged, this, &RunDlg::updateTestMethods);

  _test_methods_cbx->setVisible(false);
  _test_methods_lbl->setVisible(false);
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
  static const QStringList solvers = {"QSS",   "CQSS",    "LIQSS",     "QSS2",     "LIQSS2", "LIQSS_BDF", "QSS3",    "LIQSS3", "QSS4",
                                      "DASSL", "DOPRI",   "CVODE_BDF", "CVODE_AM", "IDA",    "mLIQSS",    "mLIQSS2", "CQSS1",  "CQSS2",
                                      "CQSS3", "CHEQSS1", "CHEQSS2",   "CHEQSS3",  "EQSS1",  "EQSS2",     "EQSS3"};
  return solvers.indexOf(str.trimmed());
}

QString RunDlg::getSolverString(int idx)
{
  static const QStringList solvers = {"QSS",   "CQSS",    "LIQSS",     "QSS2",     "LIQSS2", "LIQSS_BDF", "QSS3",    "LIQSS3", "QSS4",
                                      "DASSL", "DOPRI",   "CVODE_BDF", "CVODE_AM", "IDA",    "mLIQSS",    "mLIQSS2", "CQSS1",  "CQSS2",
                                      "CQSS3", "CHEQSS1", "CHEQSS2",   "CHEQSS3",  "EQSS1",  "EQSS2",     "EQSS3"};
  return (idx >= 0 && idx < solvers.size()) ? solvers[idx] : QString();
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

void RunDlg::updateTestMethods(int state)
{
  if (state == Qt::Unchecked) {
    setSolver("QSS");
  }
}

void RunDlg::filterComboBox(QComboBox* combo, const QString& filter)
{
  if (!_originalLists.contains(combo)) {
    return;
  }

  QStringList filteredList;
  const QStringList& original = _originalLists[combo];

  if (filter.isEmpty()) {
    filteredList = original;
  } else {
    for (const QString& item : original) {
      if (item.contains(filter, Qt::CaseInsensitive)) {
        filteredList.append(item);
      }
    }
  }

  QStringListModel* model = qobject_cast<QStringListModel*>(combo->model());
  if (model) {
    model->setStringList(filteredList);
  }
}
