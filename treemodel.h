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

#ifndef TREEMODEL_H_
#define TREEMODEL_H_

#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;
class FileItem;

/**
 *
 */
class TreeModel: public QStandardItemModel
{
  Q_OBJECT
    public:
    /**+
     *
     * @param headers
     * @param parent
     */
    TreeModel(const QStringList &headers, QObject *parent = 0);
    /**
     *
     */
    ~TreeModel();
    /**
     *
     * @param index
     * @return
     */
    Qt::ItemFlags
    flags(const QModelIndex &index) const;
    /**
     *
     * @param dir
     */
    void
    addFiles(QString dir);
};

#endif  /* TREEMODEL_H_ */
