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

#include <QDir>
#include <QFileInfo>

#include "treemodel.h"
#include "comboboxdelegate.h"

TreeModel::TreeModel(const QStringList &headers, QObject *parent) :
    QStandardItemModel(parent)
{
  setHorizontalHeaderLabels(headers);
}

TreeModel::~TreeModel()
{
}

Qt::ItemFlags
TreeModel::flags(const QModelIndex &index) const
    {
  if(!index.isValid())
    return 0;
  if(index.column() == 0)
  {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }
  else
  {
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable
        | Qt::ItemIsSelectable;
  }
}

void
TreeModel::addFiles(QString dir)
{
  QDir d(dir);
  QFileInfoList fil = d.entryInfoList(QStringList("*.dat"));
  if(fil.isEmpty())
    return;
  QStringList lines;
  foreach(QFileInfo f, fil)
  { lines << f.baseName();
}
// Add file header.
  QStandardItem *headData = new QStandardItem();
  headData->setText(d.dirName());
  int number = 0;
  while(number < lines.count())
  {
    QString lineData = lines[number];
    if(!lineData.isEmpty())
    {
      QStandardItem *var = new QStandardItem();
      var->setText(lines[number]);
      var->setCheckable(true);
      var->setCheckState(Qt::Unchecked);
      QStandardItem *settings = new QStandardItem("lines");
      QList<QStandardItem *> childs;
      childs << new QStandardItem() << var << settings;
      headData->appendRow(childs);
    }
    number++;
  }
  appendRow(headData);
}
