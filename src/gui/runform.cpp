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

#include <runform.h>
#include <utils.h>

RunDlg::RunDlg(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  _utils = new Utils();
  _validate = new QDoubleValidator();
  _startTime->setValidator(_validate);
  _stopTime->setValidator(_validate);
  _tolerance->setValidator(_validate);
  _dt->setValidator(_validate);
  _absTolerance->setValidator(_validate);
  _minStep->setValidator(_validate);
  _derDelta->setValidator(_validate);
  _zcHyst->setValidator(_validate);
  _extendedFrame->setVisible(false);
  _debugChk->setCheckState(Qt::Unchecked);
  on__parallel_currentIndexChanged(_parallel->currentIndex());
  connect(_test_methods_cbx, &QCheckBox::stateChanged, this, &RunDlg::updateTestMethods);
  updateTestMethods(_test_methods_cbx->checkState());
}

void RunDlg::on__showAll_stateChanged(int state)
{
  switch (state) {
  case Qt::Unchecked:
    _extendedFrame->setVisible(false);
    break;
  case Qt::Checked:
    _extendedFrame->setVisible(true);
    break;
  default:
    break;
  }
}

void RunDlg::on__commInterval_currentIndexChanged(int index)
{
  if (index == 0) {
    _period->setEnabled(false);
  } else {
    _period->setEnabled(true);
  }
}

void RunDlg::on__dtSynch_currentIndexChanged(int index)
{
  if (index == 0) {
    _dtLbl->setText("Dt tolerance");
  } else {
    _dtLbl->setText("Dt value");
  }
}

void RunDlg::on__parallel_currentIndexChanged(int index)
{
  if (index == 1) {
    _lps->setEnabled(true);
    _partitionMethod->setEnabled(true);
    _dt->setEnabled(true);
    _dtSynch->setEnabled(true);
    _scotchSettings->setEnabled(true);
    _metisSettings->setEnabled(true);
    _patohSettings->setEnabled(true);
    _semiStaticChk->setEnabled(true);
    _generateArchCbx->setEnabled(true);
    _debugGraphCbx->setEnabled(true);
    _reorderPartitionCbx->setEnabled(true);
    _imbalance->setEnabled(true);
  } else {
    _lps->setEnabled(false);
    _partitionMethod->setEnabled(false);
    _dt->setEnabled(false);
    _dtSynch->setEnabled(false);
    _scotchSettings->setEnabled(false);
    _metisSettings->setEnabled(false);
    _patohSettings->setEnabled(false);
    _semiStaticChk->setEnabled(false);
    _generateArchCbx->setEnabled(false);
    _debugGraphCbx->setEnabled(false);
    _reorderPartitionCbx->setEnabled(false);
    _imbalance->setEnabled(false);
  }
}

int RunDlg::getSolverIdx(QString str)
{
  if (str.trimmed() == "QSS") return 0;
  if (str.trimmed() == "CQSS") return 1;
  if (str.trimmed() == "LIQSS") return 2;
  if (str.trimmed() == "QSS2") return 3;
  if (str.trimmed() == "LIQSS2") return 4;
  if (str.trimmed() == "LIQSS_BDF") return 5;
  if (str.trimmed() == "QSS3") return 6;
  if (str.trimmed() == "LIQSS3") return 7;
  if (str.trimmed() == "QSS4") return 8;
  if (str.trimmed() == "DASSL") return 9;
  if (str.trimmed() == "DOPRI") return 10;
  if (str.trimmed() == "CVODE_BDF") return 11;
  if (str.trimmed() == "CVODE_AM") return 12;
  if (str.trimmed() == "IDA") return 13;
  if (str.trimmed() == "mLIQSS") return 14;
  if (str.trimmed() == "mLIQSS2") return 15;
  return -1;
}

QString RunDlg::getSolverString(int idx)
{
  switch (idx) {
  case 0:
    return "QSS";
  case 1:
    return "CQSS";
  case 2:
    return "LIQSS";
  case 3:
    return "QSS2";
  case 4:
    return "LIQSS2";
  case 5:
    return "LIQSS_BDF";
  case 6:
    return "QSS3";
  case 7:
    return "LIQSS3";
  case 8:
    return "QSS4";
  case 9:
    return "DASSL";
  case 10:
    return "DOPRI";
  case 11:
    return "CVODE_BDF";
  case 12:
    return "CVODE_AM";
  case 13:
    return "IDA";
  case 14:
    _test_methods_cbx->setCheckState(Qt::Checked);
    return "mLIQSS";
  case 15:
    _test_methods_cbx->setCheckState(Qt::Checked);
    return "mLIQSS2";
  }
  return QString();
}

QString RunDlg::getJacobianString(int idx)
{
  switch (idx) {
  case 0:
    return "Sparse";
  case 1:
    return "Dense";
  }
  return "Sparse";
}

int RunDlg::getOutputTypeIdx(QString str)
{
  if (str.trimmed() == "CI_Step") return 0;
  if (str.trimmed() == "CI_Sampled") return 1;
  if (str.trimmed() == "CI_Dense") return 2;
  return 0;
}

QString RunDlg::getOutputTypeString(int idx)
{
  switch (idx) {
  case 0:
    return "CI_Step";
  case 1:
    return "CI_Sampled";
  case 2:
    return "CI_Dense";
  }
  return QString();
}

int RunDlg::getSchedulerIdx(QString str)
{
  if (str.trimmed() == "ST_Binary") return 0;
  if (str.trimmed() == "ST_Random") return 1;
  if (str.trimmed() == "ST_Linear") return 2;
  return 0;
}

QString RunDlg::getSchedulerString(int idx)
{
  switch (idx) {
  case 0:
    return "ST_Binary";
  case 1:
    return "ST_Random";
  case 2:
    return "ST_Linear";
  }
  return QString();
}

int RunDlg::getComboBoolIdx(QString str)
{
  if (str.trimmed() == "false") return 0;
  if (str.trimmed() == "true") return 1;
  return 0;
}

QString RunDlg::getComboBoolString(int idx)
{
  switch (idx) {
  case 0:
    return "false";
  case 1:
    return "true";
  }
  return "false";
}

int RunDlg::getPartitionMethodIdx(QString str)
{
  if (str.trimmed() == "Metis") return 0;
  if (str.trimmed() == "HMetis") return 1;
  if (str.trimmed() == "Scotch") return 2;
  if (str.trimmed() == "Patoh") return 3;
  if (str.trimmed() == "Manual") return 4;
  return 0;
}

QString RunDlg::getPartitionMethodString(int idx)
{
  switch (idx) {
  case 0:
    return "Metis";
  case 1:
    return "HMetis";
  case 2:
    return "Scotch";
  case 3:
    return "Patoh";
  case 4:
    return "Manual";
  }
  return "Metis";
}

int RunDlg::getJacobianIdx(QString str)
{
  if (str.trimmed() == "Dense") return 1;
  if (str.trimmed() == "Sparse") return 0;
  return 0;
}

int RunDlg::getDtSynchIdx(QString str)
{
  if (str.trimmed() == "SD_DT_Fixed") return 1;
  if (str.trimmed() == "SD_DT_Asynchronous") return 0;
  return 0;
}

QString RunDlg::getDtSynchString(int idx)
{
  switch (idx) {
  case 1:
    return "SD_DT_Fixed";
  case 0:
    return "SD_DT_Asynchronous";
  }
  return "SD_DT_Asynchronous";
}

void RunDlg::updateTestMethods(int state)
{
  bool hide = state == Qt::Unchecked;
  QListView *solver_list = qobject_cast<QListView *>(_solver->view());
  solver_list->setRowHidden(getSolverIdx("mLIQSS"), hide);
  solver_list->setRowHidden(getSolverIdx("mLIQSS2"), hide);
  if (hide) {
    setSolver("QSS");
  }
}