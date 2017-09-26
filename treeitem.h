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

#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QList>
#include <QVariant>
#include <QVector>

/**
 *
 */
class TreeItem
{
  public:
    /**
     *
     * @param data
     * @param parent
     */
    TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
    /**
     *
     */
    ~TreeItem();
    /**
     *
     * @param number
     * @return
     */
    TreeItem *
    child(int number);
    /**
     *
     * @return
     */
    int
    childCount() const;
    /**
     *
     * @return
     */
    int
    columnCount() const;
    /**
     *
     * @param column
     * @return
     */
    QVariant
    data(int column) const;
    /**
     *
     * @param position
     * @param count
     * @param columns
     * @return
     */
    bool
    insertChildren(int position, int count, int columns);
    /**
     *
     * @param position
     * @param columns
     * @return
     */
    bool
    insertColumns(int position, int columns);
    /**
     *
     * @return
     */
    TreeItem *
    parent();
    /**
     *
     * @param position
     * @param count
     * @return
     */
    bool
    removeChildren(int position, int count);
    /**
     *
     * @param position
     * @param columns
     * @return
     */
    bool
    removeColumns(int position, int columns);
    /**
     *
     * @return
     */
    int
    childNumber() const;
    /**
     *
     * @param column
     * @param value
     * @return
     */
    bool
    setData(int column, const QVariant &value);
    private:
    QList<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;
};

/**
 *
 */
class FileItem
{
  public:
    /**
     *
     */
    FileItem();
    /**
     *
     * @param v
     * @param l
     * @param s
     * @param p
     * @param i
     * @param output
     */
    FileItem(QString v, bool l, bool s, bool p, bool i, QString output);
    /**
     *
     */
    ~FileItem();
    /**
     *
     * @return
     */
    QString
    plotStr();
    /**
     *
     */
    bool lines;
    /**
     *
     */
    bool steps;
    /**
     *
     */
    bool plot;
    /**
     *
     */
    bool impulses;
    /**
     *
     */
    QString variable;
    /**
     *
     */
    QString out;
};

/**
 *
 */
class MapFiles
{
  public:
    /**
     *
     * @param output
     */
    MapFiles(QString output = QString());
    /**
     *
     */
    ~MapFiles();
    /**
     *
     * @param file
     * @return
     */
    QList<FileItem>
    files(QString file);
    /**
     *
     * @param dir
     * @param name
     * @return
     */
    FileItem
    fileItem(QString dir, QString name);
    /**
     *
     * @param it
     * @param dir
     * @param name
     */
    void
    setFileItem(FileItem it, QString dir, QString name);
    /**
     *
     * @param dir
     * @param init
     */
    void
    addFiles(QString dir, bool init = true);
    /**
     *
     * @param dir
     */
    void
    clearFiles(QString dir);
    /**
     *
     * @param dir
     */
    void
    setFiles(QString dir);
    /**
     *
     */
    void
    clearAllFiles();
    /**
     *
     */
    void
    setAllFiles();
    /**
     *
     */
    void
    clear();
    /**
     *
     * @param dir
     */
    void
    removeFiles(QString dir);
    /**
     *
     * @return
     */
    QMap<QString, QList<FileItem> >
    map()
    {
      return *_files;
    }
    ;
    private:
    void
    _changeFiles(QString file, bool st);
    QMap<QString, QList<FileItem> > *_files;
    QString _output;
};

#endif  /* TREEITEM_H_ */
