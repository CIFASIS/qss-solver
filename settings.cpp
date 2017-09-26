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

#include <settings.h>
#include <utils.h>

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent)
{
  setupUi(this);
  _utils = new Utils();
  _models->setText(_utils->relativePath(MMOC_MODELS));
  _packages->setText(_utils->relativePath(MMOC_PACKAGES));
  _include->setText(_utils->relativePath(MMOC_INCLUDE));
  _output->setText(_utils->relativePath(MMOC_OUTPUT));
  _build->setText(_utils->relativePath(MMOC_BUILD));
  _engine->setText(_utils->relativePath(MMOC_ENGINE));
  _source->setText(_utils->relativePath(MMOC_SOURCE));
  _libs->setText(_utils->relativePath(MMOC_LIBS));
  _bin->setText(_utils->relativePath(MMOC_BIN));
  _externalLibs->setText(_utils->relativePath(MMOC_LIBRARIES));
  _comp->setText(_utils->appCommand(CMD_COMPILE));
  _buildCmd->setText(_utils->appCommand(CMD_BUILD));
  _simulate->setText(_utils->appCommand(CMD_SIMULATE));
  _plot->setText(_utils->appCommand(CMD_PLOT));
  _sbml->setText(_utils->appCommand(CMD_SBML));
  _flags->setText(_utils->appFlag(FLG_FLAGS));
  _plotFlags->setText(_utils->appFlag(FLG_PLOT));
  _setCheckBoxValue(_allCbx, _utils->appFlag(FLG_DBG_ALL));
  _setCheckBoxValue(_initialValuesCbx, _utils->appFlag(FLG_DBG_INIT_VALUES));
  _setCheckBoxValue(_externalEventsCbx,
      _utils->appFlag(FLG_DBG_EXTERNAL_EVENT));
  _setCheckBoxValue(_memoryCbx, _utils->appFlag(FLG_DBG_MEMORY));
  _setCheckBoxValue(_synchronizeCbx, _utils->appFlag(FLG_DBG_SYNCHRONIZE));
  _setCheckBoxValue(_stepInfoCbx, _utils->appFlag(FLG_DBG_STEP_INFO));
  _setCheckBoxValue(_varChangesCbx, _utils->appFlag(FLG_DBG_VAR_CHANGES));
  _setCheckBoxValue(_waitForCbx, _utils->appFlag(FLG_DBG_WAIT_FOR));
  _setCheckBoxValue(_weightsCbx, _utils->appFlag(FLG_DBG_WEIGHTS));
  _setCheckBoxValue(_dtCbx, _utils->appFlag(FLG_DBG_DT));
  connect(this, SIGNAL(accepted()), this, SLOT(_save()));
}

SettingsDlg::~SettingsDlg()
{
  delete _utils;
}

void
SettingsDlg::_save()
{
  _utils->setDir(MMOC_MODELS, _models->text());
  _utils->setDir(MMOC_PACKAGES, _packages->text());
  _utils->setDir(MMOC_INCLUDE, _include->text());
  _utils->setDir(MMOC_OUTPUT, _output->text());
  _utils->setDir(MMOC_BUILD, _build->text());
  _utils->setDir(MMOC_ENGINE, _engine->text());
  _utils->setDir(MMOC_SOURCE, _source->text());
  _utils->setDir(MMOC_LIBS, _libs->text());
  _utils->setDir(MMOC_BIN, _bin->text());
  _utils->setDir(MMOC_LIBRARIES, _externalLibs->text());
  _utils->setCommand(CMD_COMPILE, _comp->text());
  _utils->setCommand(CMD_BUILD, _buildCmd->text());
  _utils->setCommand(CMD_SIMULATE, _simulate->text());
  _utils->setCommand(CMD_PLOT, _plot->text());
  _utils->setCommand(CMD_SBML, _sbml->text());
  _utils->setFlag(FLG_FLAGS, _flags->text());
  _utils->setFlag(FLG_DEBUG, _getDebugValue());
  _utils->setFlag(FLG_DBG_ALL, _getCheckBoxValue(_allCbx));
  _utils->setFlag(FLG_DBG_INIT_VALUES, _getCheckBoxValue(_initialValuesCbx));
  _utils->setFlag(FLG_DBG_EXTERNAL_EVENT,
      _getCheckBoxValue(_externalEventsCbx));
  _utils->setFlag(FLG_DBG_MEMORY, _getCheckBoxValue(_memoryCbx));
  _utils->setFlag(FLG_DBG_SYNCHRONIZE, _getCheckBoxValue(_synchronizeCbx));
  _utils->setFlag(FLG_DBG_STEP_INFO, _getCheckBoxValue(_stepInfoCbx));
  _utils->setFlag(FLG_DBG_VAR_CHANGES, _getCheckBoxValue(_varChangesCbx));
  _utils->setFlag(FLG_DBG_WAIT_FOR, _getCheckBoxValue(_waitForCbx));
  _utils->setFlag(FLG_DBG_WEIGHTS, _getCheckBoxValue(_weightsCbx));
  _utils->setFlag(FLG_DBG_DT, _getCheckBoxValue(_dtCbx));
  _utils->setFlag(FLG_PLOT, _plotFlags->text());
}

QString
SettingsDlg::_getCheckBoxValue(QCheckBox *chkBox)
{
  if(chkBox->checkState() == Qt::Checked)
  {
    return "true";
  }
  else
  {
    return "false";
  }
}

void
SettingsDlg::_setCheckBoxValue(QCheckBox *chkBox, QString value)
{
  if(value.compare("true") == 0)
  {
    chkBox->setCheckState(Qt::Checked);
  }
  else
  {
    chkBox->setCheckState(Qt::Unchecked);
  }
}

QString
SettingsDlg::_getDebugValue()
{
  QString dbg;
  if(_allCbx->isChecked())
  {
    dbg += "-d SD_DBG_All ";
  }
  if(_initialValuesCbx->isChecked())
  {
    dbg += "-d SD_DBG_InitValues ";
  }
  if(_externalEventsCbx->isChecked())
  {
    dbg += "-d SD_DBG_ExternalEvent ";
  }
  if(_memoryCbx->isChecked())
  {
    dbg += "-d SD_DBG_Memory ";
  }
  if(_synchronizeCbx->isChecked())
  {
    dbg += "-d SD_DBG_Synchronize ";
  }
  if(_stepInfoCbx->isChecked())
  {
    dbg += "-d SD_DBG_StepInfo ";
  }
  if(_varChangesCbx->isChecked())
  {
    dbg += "-d SD_DBG_VarChanges ";
  }
  if(_waitForCbx->isChecked())
  {
    dbg += "-d SD_DBG_WaitFor ";
  }
  if(_weightsCbx->isChecked())
  {
    dbg += "-d SD_DBG_Weights ";
  }
  if(_dtCbx->isChecked())
  {
    dbg += "-d SD_DBG_Dt ";
  }
  return dbg;
}
