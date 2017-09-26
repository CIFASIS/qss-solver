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

#include "comboboxdelegate.h"
#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *
ComboBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
    {
  QComboBox *editor = new QComboBox(parent);
  editor->addItem("lines");
  editor->addItem("steps");
  editor->addItem("impulses");
  editor->addItem("None");
  editor->setCurrentIndex(0);
  return editor;
}

void
ComboBoxDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const
    {
  QString value = index.model()->data(index, Qt::EditRole).toString();

  QComboBox *cBox = static_cast<QComboBox*>(editor);
  cBox->setCurrentIndex(cBox->findText(value));
}

void
ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
    {
  QComboBox *cBox = static_cast<QComboBox*>(editor);
  QString value = cBox->currentText();

  model->setData(index, value, Qt::EditRole);
}

void
ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option,
    const QModelIndex &/* index */) const
    {
  editor->setGeometry(option.rect);
}
