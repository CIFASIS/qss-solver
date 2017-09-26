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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtGui>
#include "./ui/ui_settings.h"
#include "utils.h"

/**
 *
 */
class SettingsDlg: public QDialog, public Ui::Settings
{
  Q_OBJECT
    public:
    /**
     *
     * @param parent
     */
    SettingsDlg(QWidget *parent = NULL);
    /**
     *
     */
    ~SettingsDlg();
    private slots:
    void
    _save();
    QString
    _getCheckBoxValue(QCheckBox *chkBox);
    void
    _setCheckBoxValue(QCheckBox *chkBox, QString value);
    QString
    _getDebugValue();
    private:
    Utils *_utils;
};

#endif  /* SETTINGS_H_ */
