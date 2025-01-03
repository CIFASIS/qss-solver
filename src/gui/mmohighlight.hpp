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

#pragma once

#include <QHash>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class MmoHighlighter : public QSyntaxHighlighter {
  Q_OBJECT
  public:
  using h_type = enum { MMO_MODEL, MMO_LOG };
  MmoHighlighter(QTextDocument *parent = nullptr, h_type file = MMO_MODEL);

  protected:
  void initialize(h_type file);
  void addHighlightingRule(const QString &pattern, const QTextCharFormat &format, bool bold = true);
  void setupModelHighlighting();
  void setupLogHighlighting();
  void highlightBlock(const QString &text) override;
  void highlightSyntax(const QString &text);
  void highlightComments(const QString &text);

  private:
  struct MmoHighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
  };

  // Keywords
  static const QStringList _model_keywords;
  static const QStringList _model_functions;
  static const QStringList _model_annotations;
  static const QStringList _model_qss_tags;
  static const QStringList _log_keywords;

  // Member variables
  QVector<MmoHighlightingRule> _highlighting_rules;
  QRegularExpression _comment_start_expression;
  QRegularExpression _comment_end_expression;
  QTextCharFormat _single_line_comment_format;
};
