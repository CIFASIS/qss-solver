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

/*
 *      treeitem.cpp
 *
 *      A container for items of data supplied by the simple tree model.
 **/

#include <QStringList>
#include <QtGui>
#include <QtGui>

#include "treeitem.h"

#define SLASH '/'

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
  parentItem = parent;
  itemData = data;
}

TreeItem::~TreeItem()
{
  qDeleteAll(childItems);
}

TreeItem *
TreeItem::child(int number)
{
  return childItems.value(number);
}

int
TreeItem::childCount() const
{
  return childItems.count();
}

int
TreeItem::childNumber() const
{
  if(parentItem)
    return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
  return 0;
}

int
TreeItem::columnCount() const
{
  return itemData.count();
}

QVariant
TreeItem::data(int column) const
    {
  return itemData.value(column);
}

bool
TreeItem::insertChildren(int position, int count, int columns)
{
  if(position < 0 || position > childItems.size())
    return false;
  for(int row = 0; row < count; ++row)
  {
    QVector<QVariant> data(columns);
    TreeItem *item = new TreeItem(data, this);
    childItems.insert(position, item);
  }
  return true;
}

bool
TreeItem::insertColumns(int position, int columns)
{
  if(position < 0 || position > itemData.size())
    return false;
  for(int column = 0; column < columns; ++column)
    itemData.insert(position, QVariant());
  foreach(TreeItem * child, childItems)
  {  child->insertColumns(position, columns);
}
  return true;
}

TreeItem *
TreeItem::parent()
{
  return parentItem;
}

bool
TreeItem::removeChildren(int position, int count)
{
  if(position < 0 || position + count > childItems.size())
    return false;
  for(int row = 0; row < count; ++row)
    delete childItems.takeAt(position);
  return true;
}

bool
TreeItem::removeColumns(int position, int columns)
{
  if(position < 0 || position + columns > itemData.size())
    return false;
  for(int column = 0; column < columns; ++column)
    itemData.remove(position);
  foreach(TreeItem * child, childItems)
  child->removeColumns(position, columns);
  return true;
}

bool
TreeItem::setData(int column, const QVariant &value)
{
  if(column < 0 || column >= itemData.size())
    return false;
  itemData[column] = value;
  return true;
}

FileItem::FileItem(QString v, bool l, bool s, bool p, bool i, QString output)
{
  lines = l;
  steps = s;
  plot = p;
  impulses = i;
  variable = v;
  out = output;
}

FileItem::FileItem()
{
  lines = false;
  steps = false;
  plot = false;
  impulses = false;
  variable = QString();
}

QString
FileItem::plotStr()
{
  QString ret;
  if(plot)
  {
    ret.append("\"").append(out).append(variable).append(".dat\"");
    if(lines)
      ret.append("with lines");
    if(steps)
      ret.append("with steps");
    ret.append(" title \"").append(variable).append("\"");
  }
  return ret;
}

FileItem::~FileItem()
{
}

MapFiles::MapFiles(QString output)
{
  _files = new QMap<QString, QList<FileItem> >();
  _output = output;
}

MapFiles::~MapFiles()
{
  delete _files;
}

QList<FileItem>
MapFiles::files(QString file)
{
  return _files->value(file);
}

void
MapFiles::clear()
{
  _files->clear();
}

void
MapFiles::addFiles(QString dir, bool init)
{
  QDir d(dir);
  QFileInfoList fil = d.entryInfoList(QStringList("*.dat"));
  if(fil.isEmpty())
    return;
  QList<FileItem> fitems;
  foreach(QFileInfo f, fil)
  {  FileItem fitem(f.baseName(),true,false,init,false,_output+QString(SLASH)+dir+QString(SLASH));
  fitems << fitem;
}
  _files->insert(d.dirName(), fitems);
}

void
MapFiles::removeFiles(QString dir)
{
  QDir d(dir);
  _files->remove(d.dirName());
}

void
MapFiles::clearFiles(QString dir)
{
  _changeFiles(dir, false);
}

void
MapFiles::setFiles(QString dir)
{
  _changeFiles(dir, true);
}

FileItem
MapFiles::fileItem(QString dir, QString name)
{
  /*	QList<FileItem> fitems = _files->value(dir);
   int idx = fitems.indexOf(name);
   if(idx > 0 && idx < fitems.size())
   return fitems.at(fitems.indexOf(name));
   else*/
  return FileItem();
}

void
MapFiles::setFileItem(FileItem it, QString dir, QString name)
{
  /*	QList<FileItem> fitems = _files->value(dir);
   int idx = fitems.indexOf(it);
   if(idx < 0 && idx >= fitems.size()) return;
   fitems.replace(idx,it);*/
}

void
MapFiles::_changeFiles(QString dir, bool st)
{
  QList<FileItem> fitems = _files->value(dir);
  for(int i = 0; i < fitems.count(); i++)
  {
    FileItem fitem = fitems.at(i);
    fitem.plot = st;
    fitems.replace(i, fitem);
  }
  _files->insert(dir, fitems);
}

void
MapFiles::clearAllFiles()
{
  QMap<QString, QList<FileItem> >::const_iterator it;
  for(it = _files->constBegin(); it != _files->constEnd(); it++)
  {
    clearFiles(it.key());
  }
}

void
MapFiles::setAllFiles()
{
  QMap<QString, QList<FileItem> >::const_iterator it;
  for(it = _files->constBegin(); it != _files->constEnd(); it++)
  {
    setFiles(it.key());
  }
}
