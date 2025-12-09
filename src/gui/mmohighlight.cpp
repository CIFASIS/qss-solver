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

#include "mmohighlight.hpp"

// Initialize static const QStringLists
const QStringList MmoHighlighter::_model_keywords = {"AbsTolerance",
                                                     "algorithm",
                                                     "and",
                                                     "bin",
                                                     "Boolean",
                                                     "constant",
                                                     "derivative",
                                                     "discrete",
                                                     "each",
                                                     "else",
                                                     "elseif",
                                                     "elsewhen",
                                                     "end",
                                                     "equation",
                                                     "external",
                                                     "for",
                                                     "function",
                                                     "if",
                                                     "import",
                                                     "Include",
                                                     "IncludeDirectory",
                                                     "initial",
                                                     "input",
                                                     "Integer",
                                                     "Jacobian",
                                                     "Library",
                                                     "LibraryDirectory",
                                                     "loop",
                                                     "MMO_BDF_Max_Step",
                                                     "MMO_BDF_Part",
                                                     "MMO_BDF_PDepth",
                                                     "MMO_CVODEMaxOrder",
                                                     "MMO_DebugGraph",
                                                     "MMO_DerDelta",
                                                     "MMO_Description",
                                                     "MMO_DH",
                                                     "MMO_DT_Min",
                                                     "MMO_DT_Synch",
                                                     "MMO_Event_Id",
                                                     "MMO_GenerateArch",
                                                     "MMO_HD",
                                                     "MMO_HH",
                                                     "MMO_HZ",
                                                     "MMO_Imbalance",
                                                     "MMO_LHS_DSC",
                                                     "MMO_LHS_ST",
                                                     "MMO_LPS",
                                                     "MMO_MetisSettings",
                                                     "MMO_MinStep",
                                                     "MMO_NodeSize",
                                                     "MMO_Output",
                                                     "MMO_OutputType",
                                                     "MMO_Parallel",
                                                     "MMO_PartitionMethod",
                                                     "MMO_PatohSettings",
                                                     "MMO_Period",
                                                     "MMO_RandomSeed",
                                                     "MMO_ReorderPartition",
                                                     "MMO_RHS_ST",
                                                     "MMO_Scheduler",
                                                     "MMO_ScotchSettings",
                                                     "MMO_SD",
                                                     "MMO_SH",
                                                     "MMO_Solver",
                                                     "MMO_StoreData",
                                                     "MMO_SymDiff",
                                                     "MMO_SZ",
                                                     "MMO_Weight",
                                                     "MMO_XOutput",
                                                     "MMO_ZCHyst",
                                                     "model",
                                                     "not",
                                                     "or",
                                                     "output",
                                                     "package",
                                                     "parameter",
                                                     "protected",
                                                     "Real",
                                                     "return",
                                                     "start",
                                                     "StartTime",
                                                     "StopTime",
                                                     "then",
                                                     "time",
                                                     "Tolerance",
                                                     "when"};

const QStringList MmoHighlighter::_model_functions = {"abs",     "acos",   "asin",  "atan", "atan2", "ceil", "conh", "cos",      "der",
                                                      "div",     "exp",    "floor", "log",  "log10", "max",  "min",  "mod",      "pre",
                                                      "product", "reinit", "sin",   "sinh", "sum",   "tan",  "tanh", "terminate"};

const QStringList MmoHighlighter::_model_annotations = {"annotation", "experiment"};

const QStringList MmoHighlighter::_model_qss_tags = {"CI_Dense",    "CI_Sampled", "CI_Step",
                                                     "CHEQSS1",     "CHEQSS2",    "CHEQSS3",
                                                     "EQSS1",       "EQSS2",      "EQSS3",
                                                     "CQSS1",      "CQSS2",
                                                     "CQSS3",       "CVODE_AM",   "CVODE_BDF",
                                                     "DASSL",       "Dense",      "DOPRI",
                                                     "false",       "HMetis",     "IDA",
                                                     "KaHIP",       "LIQSS",      "LIQSS2",
                                                     "LIQSS3",      "LIQSS_BDF",  "Manual",
                                                     "Metis",       "mLIQSS",     "mLIQSS2",
                                                     "MTPL",        "MTPL_IT",    "Patoh",
                                                     "QSS1",        "QSS2",       "QSS3",
                                                     "QSS4",        "Scotch",     "SD_DT_Asynchronous",
                                                     "SD_DT_Fixed", "Sparse",     "ST_Binary",
                                                     "ST_Linear",   "ST_Random",  "true"};

const QStringList MmoHighlighter::_log_keywords = {"-",          "Additional",  "Allocated",      "allocated",   "Average",
                                                   "Begin",      "by",          "changes",        "cost",        "CPU",
                                                   "crossing",   "Crossings",   "DASSL",          "data",        "dependencies",
                                                   "derivative", "derivatives", "detected",       "done",        "Dt",
                                                   "dt",         "efficiency",  "Estimated",      "evaluations", "Event",
                                                   "events",     "Events",      "execution",      "external",    "External",
                                                   "for",        "Function",    "function",       "Handler",     "Individual",
                                                   "Init",       "Initial",     "Initialization", "Initialize",  "input",
                                                   "iterations", "Jacobian",    "load",           "LP",          "LPS",
                                                   "max",        "memory",      "messages",       "Messages",    "Miliseconds",
                                                   "min",        "ms",          "Newton",         "next",        "output",
                                                   "Output",     "Parallel",    "Partitioning",   "past",        "per",
                                                   "performed",  "Processed",   "reinits",        "reported",    "Results",
                                                   "Save",       "Scalar",      "Seconds",        "sent",        "sequential",
                                                   "Simulation", "simulation",  "solver",         "speedup",     "State",
                                                   "state",      "Statistics",  "step",           "steps",       "sum",
                                                   "time",       "Time",        "Total",          "transition",  "transitions",
                                                   "value",      "values",      "Variable",       "variable",    "variables",
                                                   "Variables",  "Zero"};

MmoHighlighter::MmoHighlighter(QTextDocument *parent, h_type file_type) : QSyntaxHighlighter(parent) { initialize(file_type); }

void MmoHighlighter::initialize(h_type file)
{
  switch (file) {
  case MMO_MODEL:
    setupModelHighlighting();
    break;
  case MMO_LOG:
    setupLogHighlighting();
    break;
  }
  _comment_start_expression = QRegularExpression("/\\*");
  _comment_end_expression = QRegularExpression("\\*/");
}

void MmoHighlighter::addHighlightingRule(const QString &pattern, const QTextCharFormat &format, bool bold)
{
  MmoHighlightingRule rule;
  QString add_pattern = bold ? "\\b" + pattern + "\\b" : pattern;
  rule.pattern = QRegularExpression(add_pattern);
  rule.format = format;
  _highlighting_rules.append(rule);
}

void MmoHighlighter::setupModelHighlighting()
{
  QTextCharFormat keyword_format;
  keyword_format.setForeground(Qt::darkBlue);

  for (const auto &pattern : _model_keywords) {
    addHighlightingRule(pattern, keyword_format);
  }

  QTextCharFormat function_format;
  function_format.setForeground(Qt::blue);
  function_format.setFontItalic(true);

  for (const auto &_pattern : _model_functions) {
    addHighlightingRule(_pattern, function_format);
  }

  QTextCharFormat annotation_format;
  annotation_format.setForeground(Qt::darkGreen);
  annotation_format.setFontItalic(true);

  for (const auto &_pattern : _model_annotations) {
    addHighlightingRule(_pattern, annotation_format);
  }

  QTextCharFormat qss_tags_format;
  qss_tags_format.setForeground(Qt::darkRed);

  for (const auto &_pattern : _model_qss_tags) {
    addHighlightingRule(_pattern, qss_tags_format);
  }

  _single_line_comment_format.setForeground(Qt::darkGray);
  const bool DONT_USE_BOLD = false;
  addHighlightingRule("//.*", _single_line_comment_format, DONT_USE_BOLD);

  QTextCharFormat quotation_format;
  quotation_format.setForeground(Qt::darkMagenta);
  addHighlightingRule("\".*\"", quotation_format);
}

void MmoHighlighter::setupLogHighlighting()
{
  QTextCharFormat keyword_format;
  keyword_format.setForeground(Qt::darkBlue);
  keyword_format.setFontWeight(QFont::Bold);

  for (const auto &_pattern : _log_keywords) {
    addHighlightingRule(_pattern, keyword_format);
  }
}

void MmoHighlighter::highlightBlock(const QString &text)
{
  highlightSyntax(text);
  highlightComments(text);
}

void MmoHighlighter::highlightSyntax(const QString &text)
{
  for (const MmoHighlightingRule &rule : _highlighting_rules) {
    QRegularExpression expression(rule.pattern);
    QRegularExpressionMatchIterator match_iterator = expression.globalMatch(text);

    while (match_iterator.hasNext()) {
      QRegularExpressionMatch match = match_iterator.next();
      int start_index = match.capturedStart();
      int length = match.capturedLength();
      setFormat(start_index, length, rule.format);
    }
  }
}

void MmoHighlighter::highlightComments(const QString &text)
{
  const int IN_COMMENT_BLOCK = 1;
  setCurrentBlockState(0);
  QTextCharFormat multi_line_comment_format;
  multi_line_comment_format.setForeground(Qt::darkGray);

  // Determine the starting index for comment highlighting
  int start_index = (previousBlockState() != IN_COMMENT_BLOCK) ? _comment_start_expression.match(text).capturedStart() : 0;

  while (start_index >= 0) {
    // Find the end index of the comment
    QRegularExpressionMatch end_match = _comment_end_expression.match(text, start_index);
    int length_of_comment;

    if (!end_match.hasMatch()) {
      // No end index found, means the comment continues to the end of the text
      setCurrentBlockState(IN_COMMENT_BLOCK);
      length_of_comment = text.length() - start_index;
    } else {
      // Calculate the length of the comment
      length_of_comment = end_match.capturedStart() - start_index + end_match.capturedLength();
    }

    // Apply formatting for the identified comment
    setFormat(start_index, length_of_comment, multi_line_comment_format);

    // Find the next comment start index
    QRegularExpressionMatch startMatch = _comment_start_expression.match(text, start_index + length_of_comment);
    start_index = startMatch.hasMatch() ? startMatch.capturedStart() : -1;  // Set start_index to -1 if no more comments are found
  }
}
