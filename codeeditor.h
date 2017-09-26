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

/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef CODEEDITOR_H_
#define CODEEDITOR_H_

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

/**
 *
 */
class LineNumberArea;

/**
 *
 */
class CodeEditor: public QPlainTextEdit
{
  Q_OBJECT

  public:
    /**
     *
     * @param parent
     */
    CodeEditor(QWidget *parent = 0);
    /**
     *
     * @param event
     */
    void
    lineNumberAreaPaintEvent(QPaintEvent *event);
    /**
     *
     * @return
     */
    int
    lineNumberAreaWidth();
    protected:
    /**
     *
     * @param event
     */
    void
    resizeEvent(QResizeEvent *event);

  private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void
    highlightCurrentLine();
    void
    updateLineNumberArea(const QRect &, int);
  private:
    QWidget *lineNumberArea;
};

/**
 *
 */
class LineNumberArea: public QWidget
{
  public:
    /**
     *
     * @param editor
     */
    LineNumberArea(CodeEditor *editor) :
        QWidget(editor)
    {
      codeEditor = editor;
    }
    /**
     *
     * @return
     */
    QSize
    sizeHint() const
    {
      return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }
  protected:
    void
    paintEvent(QPaintEvent *event)
    {
      codeEditor->lineNumberAreaPaintEvent(event);
    }
  private:
    CodeEditor *codeEditor;
};

#endif  /* CODEEDITOR_H_ */
