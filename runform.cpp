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

#include <QtGui>
#include <Qt>

#include <runform.h>
#include <utils.h>

RunDlg::RunDlg (QWidget *parent) :
    QDialog (parent)
{
  setupUi (this);
  _utils = new Utils ();
  _validate = new QDoubleValidator ();
  _validateInt = new QIntValidator ();
  _startTime->setValidator (_validate);
  _stopTime->setValidator (_validate);
  _tolerance->setValidator (_validate);
  _dt->setValidator (_validate);
  _absTolerance->setValidator (_validate);
  _minStep->setValidator (_validate);
  _derDelta->setValidator (_validate);
  _zcHyst->setValidator (_validate);
  _lps->setValidator (_validateInt);
  _extendedFrame->setVisible (false);
  _debugChk->setCheckState (Qt::Unchecked);
#if defined (_WIN32) || defined (__MACH__)
  _parallel->setEnabled(false);
  _lps->setEnabled(false);
  _partitionMethod->setEnabled(false);
  _dt->setEnabled(false);
  _dtSynch->setEnabled (false);
#endif
}

RunDlg::~RunDlg ()
{
}

void
RunDlg::on__showAll_stateChanged (int state)
{
  switch (state)
    {
    case Qt::Unchecked:
      _extendedFrame->setVisible (false);
      break;
    case Qt::Checked:
      _extendedFrame->setVisible (true);
      break;
    default:
      break;
    }
}

void
RunDlg::on__commInterval_currentIndexChanged (int index)
{
  if (index == 0)
    {
      _period->setEnabled (false);
    }
  else
    {
      _period->setEnabled (true);
    }
}

void
RunDlg::on__dtSynch_currentIndexChanged (int index)
{
  if (index == 0)
    {
      _dtLbl->setText ("Dt tolerance");
    }
  else
    {
      _dtLbl->setText ("Dt value");
    }
}


void
RunDlg::on__parallel_currentIndexChanged (int index)
{
  if (index == 1)
    {
      _lps->setEnabled (true);
      _partitionMethod->setEnabled (true);
      _dt->setEnabled (true);
      _dtSynch->setEnabled (true);
    }
  else
    {
      _lps->setEnabled (false);
      _partitionMethod->setEnabled (false);
      _dt->setEnabled (false);
      _dtSynch->setEnabled (false);
    }
}

int
RunDlg::_getSolverIdx (QString str)
{
  if (str.trimmed () == "QSS")
    return (0);
  if (str.trimmed () == "CQSS")
    return (1);
  if (str.trimmed () == "LIQSS")
    return (2);
  if (str.trimmed () == "QSS2")
    return (3);
  if (str.trimmed () == "LIQSS2")
    return (4);
  if (str.trimmed () == "QSS3")
    return (5);
  if (str.trimmed () == "LIQSS3")
    return (6);
  if (str.trimmed () == "QSS4")
    return (7);
  if (str.trimmed () == "DASSL")
    return (8);
  if (str.trimmed () == "DOPRI")
    return (9);
  return (-1);
}

QString
RunDlg::_getSolverString (int idx)
{
  switch (idx)
    {
    case 0:
      return ("QSS");
    case 1:
      return ("CQSS");
    case 2:
      return ("LIQSS");
    case 3:
      return ("QSS2");
    case 4:
      return ("LIQSS2");
    case 5:
      return ("QSS3");
    case 6:
      return ("LIQSS3");
    case 7:
      return ("QSS4");
    case 8:
      return ("DASSL");
    case 9:
      return ("DOPRI");
    }
  return (QString ());
}

int
RunDlg::_getSymDiffIdx (QString str)
{
  if (str.trimmed () == "true")
    return (0);
  if (str.trimmed () == "false")
    return (1);
  return (0);
}

QString
RunDlg::_getSymDiffString (int idx)
{
  switch (idx)
    {
    case 0:
      return ("true");
    case 1:
      return ("false");
    }
  return ("true");
}

int
RunDlg::_getOutputTypeIdx (QString str)
{
  if (str.trimmed () == "CI_Step")
    return (0);
  if (str.trimmed () == "CI_Sampled")
    return (1);
  if (str.trimmed () == "CI_Dense")
    return (2);
  return (0);
}

QString
RunDlg::_getOutputTypeString (int idx)
{
  switch (idx)
    {
    case 0:
      return ("CI_Step");
    case 1:
      return ("CI_Sampled");
    case 2:
      return ("CI_Dense");
    }
  return (QString ());
}

int
RunDlg::_getSchedulerIdx (QString str)
{
  if (str.trimmed () == "ST_Binary")
    return (0);
  if (str.trimmed () == "ST_Random")
    return (1);
  if (str.trimmed () == "ST_Linear")
    return (2);
  return (0);
}

QString
RunDlg::_getSchedulerString (int idx)
{
  switch (idx)
    {
    case 0:
      return ("ST_Binary");
    case 1:
      return ("ST_Random");
    case 2:
      return ("ST_Linear");
    }
  return (QString ());
}

int
RunDlg::_getParallelIdx (QString str)
{
  if (str.trimmed () == "false")
    return (0);
  if (str.trimmed () == "true")
    return (1);
  return (0);
}

QString
RunDlg::_getParallelString (int idx)
{
  switch (idx)
    {
    case 0:
      return ("false");
    case 1:
      return ("true");
    }
  return ("false");
}

int
RunDlg::_getPartitionMethodIdx (QString str)
{
  if (str.trimmed () == "Metis")
    return (0);
  if (str.trimmed () == "HMetis")
    return (1);
  if (str.trimmed () == "Scotch")
    return (2);
  if (str.trimmed () == "Patoh")
    return (3);
  if (str.trimmed () == "Manual")
    return (4);
  return (0);
}

bool
RunDlg::enableDebug ()
{
  return (_debugChk->checkState () == Qt::Checked);
}

bool
RunDlg::semiStaticPartitioning ()
{
  return (_semiStaticChk->checkState () == Qt::Checked);
}

void
RunDlg::setSemiStaticPartitioning (bool st)
{
    _semiStaticChk->setChecked (st);
}

QString
RunDlg::_getPartitionMethodString (int idx)
{
  switch (idx)
    {
    case 0:
      return ("Metis");
    case 1:
      return ("HMetis");
    case 2:
      return ("Scotch");
    case 3:
      return ("Patoh");
    case 4:
      return ("Manual");
    }
  return ("Metis");
}

int
RunDlg::_getDtSynchIdx (QString str)
{
  if (str.trimmed () == "SD_DT_Fixed")
    return (1);
  if (str.trimmed () == "SD_DT_Asynchronous")
    return (0);
  return (0);
}

QString
RunDlg::_getDtSynchString (int idx)
{
  switch (idx)
    {
    case 1:
      return ("SD_DT_Fixed");
    case 0:
      return ("SD_DT_Asynchronous");
    }
  return ("SD_DT_Asynchronous");
}
