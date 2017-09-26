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

#ifndef MMOHIGHLIGHTER_H_
#define MMOHIGHLIGHTER_H_

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

/**
 *
 */
class MmoHighlighter: public QSyntaxHighlighter
{
  Q_OBJECT
    public:
    /**
     *
     */
    typedef enum
    {
      MMO_MODEL, //!< MMO_MODEL
      MMO_LOG    //!< MMO_LOG
    } h_type;
    /**
     *
     * @param parent
     * @param file
     */
    MmoHighlighter(QTextDocument *parent = 0, h_type file = MMO_MODEL);
    protected:
    void
    highlightBlock(const QString &text);
    private:
    struct MmoHighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<MmoHighlightingRule> highlightingRules;
    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat keywordFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat annotationFormat;
    QTextCharFormat qssTagsFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
};

#endif  /* MMOHIGHLIGHTER_H_ */
