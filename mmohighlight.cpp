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

#include "mmohighlight.h"

MmoHighlighter::MmoHighlighter(QTextDocument *parent, h_type file) :
    QSyntaxHighlighter(parent)
{
  MmoHighlightingRule rule;
  QStringList keywordPatterns;
  QStringList functionPatterns;
  QStringList annotationPatterns;
  QStringList qssTagsPatterns;
  switch(file)
  {
    case MMO_MODEL:
      keywordFormat.setForeground(Qt::darkBlue);
      keywordPatterns << "\\bin\\b" << "\\bif\\b" << "\\bmodel\\b"
          << "\\bfunction\\b" << "\\belse\\b" << "\\belsewhen\\b"
          << "\\belseif\\b" << "\\bfor\\b" << "\\bloop\\b" << "\\balgorithm\\b"
          << "\\btime\\b" << "\\bend\\b" << "\\boutput\\b" << "\\binput\\b"
          << "\\bparameter\\b" << "\\bconstant\\b" << "\\bBoolean\\b"
          << "\\bdiscrete\\b" << "\\bstart\\b" << "\\bReal\\b"
          << "\\bInteger\\b" << "\\bMMO_Description\\b" << "\\bStartTime\\b"
          << "\\bStopTime\\b" << "\\bTolerance\\b" << "\\bAbsTolerance\\b"
          << "\\beach\\b" << "\\bMMO_Solver\\b" << "\\band\\b" << "\\bor\\b"
          << "\\bnot\\b" << "\\bMMO_Output\\b" << "\\bpackage\\b"
          << "\\breturn\\b" << "\\bMMO_SymDiff\\b" << "\\MMO_OutputType\\b"
          << "\\bMMO_Scheduler\\b" << "\\bMMO_LPS\\b" << "\\bimport\\b"
          << "\\bexternal\\b" << "\\bprotected\\b" << "\\bMMO_Period\\b"
          << "\\binitial\\b" << "\\bwhen\\b" << "\\bthen\\b"
          << "\\bMMO_Weight\\b" << "\\bequation\\b" << "\\bMMO_MinStep\\b"
          << "\\bMMO_ZCHyst\\b" << "\\bMMO_DerDelta\\b" << "\\bInclude\\b"
          << "\\bIncludeDirectory\\b" << "\\bLibrary\\b" << "\\bJacobian\\b"
          << "\\bLibraryDirectory\\b" << "\\bMMO_NodeSize\\b"
          << "\\bMMO_Parallel\\b" << "\\bMMO_PartitionMethod\\b"
          << "\\bMMO_DT_Min\\b"
          << "\\bMMO_DT_Synch\\b" << "\\bMMO_PatohSettings\\b"
          << "\\bMMO_ScotchSettings\\b" << "\\bMMO_MetisSettings\\b"
          << "\\bMMO_StoreData\\b" << "\\bderivative\\b";

      foreach (const QString &pattern, keywordPatterns)
      {      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
    }
    functionFormat.setForeground(Qt::blue);
    functionFormat.setFontItalic(true);
    functionPatterns << "\\bpre\\b" << "\\bsum\\b" << "\\bterminate\\b"
    << "\\breinit\\b" << "\\bder\\b" << "\\babs\\b"
    << "\\bdiv\\b" << "\\bfloor\\b" << "\\bceil\\b"
    << "\\bmod\\b" << "\\bsin\\b" << "\\bcos\\b"
    << "\\btan\\b" << "\\basin\\b" << "\\bacos\\b"
    << "\\batan\\b" << "\\batan2\\b" << "\\bsinh\\b"
    << "\\bconh\\b" << "\\btanh\\b" << "\\bexp\\b"
    << "\\blog\\b" << "\\blog10\\b"
    << "\\bmax\\b" << "\\bmin\\b" << "\\bproduct\\b";

    foreach (const QString &pattern, functionPatterns)
    {
      rule.pattern = QRegExp(pattern);
      rule.format = functionFormat;
      highlightingRules.append(rule);
    }
    annotationFormat.setForeground(Qt::darkGreen);
    annotationFormat.setFontItalic(true);
    annotationPatterns << "\\bannotation\\b" << "\\bexperiment\\b";

    foreach (const QString &pattern, annotationPatterns)
    {
      rule.pattern = QRegExp(pattern);
      rule.format = annotationFormat;
      highlightingRules.append(rule);
    }
    qssTagsFormat.setForeground(Qt::darkRed);
    qssTagsPatterns << "\\bST_Binary\\b" << "\\bST_Linear\\b"
    << "\\bST_Random\\b"
    << "\\bQSS\\b" << "\\bCQSS\\b" << "\\bLIQSS\\b" << "\\bQSS2\\b"
    << "\\bLIQSS2\\b" << "\\bQSS3\\b" << "\\bLIQSS3\\b"
    << "\\bCVODE_AM\\b" << "\\bCVODE_BDF\\b" << "\\bIDA\\b"
    << "\\bCI_Sampled\\b" << "\\bCI_Step\\b" << "\\bCI_Dense\\b"
    << "\\bSD_DT_Fixed\\b" << "\\bSD_DT_Asynchronous\\b"
    << "\\bMetis\\b" << "\\bHMetis\\b" << "\\bDense\\b" << "\\bSparse\\b"
    << "\\bScotch\\b" << "\\bPatoh\\b" << "\\bManual\\b"
    << "\\bMTPL\\b" << "\\bMTPL_IT\\b"
    << "\\btrue\\b" << "\\bfalse\\b" << "\\bDOPRI\\b" << "\\bDASSL\\b";

    foreach (const QString &pattern, qssTagsPatterns)
    {
      rule.pattern = QRegExp(pattern);
      rule.format = qssTagsFormat;
      highlightingRules.append(rule);
    }
    singleLineCommentFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    multiLineCommentFormat.setForeground(Qt::darkGray);
    quotationFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    break;
    case MMO_LOG:
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    keywordPatterns << "\\bSimulation\\b" << "\\btime\\b" << "\\boutput\\b"
    << "\\bInit\\b" << "\\bSave\\b" << "\\bdata\\b" << "\\bTotal\\b"
    << "\\bDt\\b" << "\\bdt\\b" << "\\bsum\\b"
    << "\\bTime\\b" << "\\bState\\b" << "\\bVariable\\b"
    << "\\bdependencies\\b"
    << "\\bSeconds\\b" << "\\bms\\b" << "\\bchanges\\b"
    << "\\bInitialize\\b"
    << "\\bsimulation\\b" << "\\bInitial\\b" << "\\bvalue\\b"
    << "\\bdone\\b"
    << "\\bderivative\\b" << "\\bsolver\\b" << "\\bstate\\b"
    << "\\bderivatives\\b"
    << "\\binput\\b" << "\\bevents\\b" << "\\bvariables\\b" << "\\bBegin\\b"
    << "\\bAllocated\\b" << "\\bmemory\\b"
    << "\\bvalues\\b" << "\\bEvent\\b" << "\\bnext\\b"
    << "\\btransitions\\b" << "\\b-\\b"
    << "\\bFunction\\b" << "\\bevaluations\\b" << "\\bScalar\\b"
    << "\\bfunction\\b" << "\\breported\\b" << "\\bMiliseconds\\b"
    << "\\bVariables\\b" << "\\bHandler\\b" << "\\bexecution\\b"
    << "\\bAdditional\\b" << "\\ballocated\\b"
    << "\\bby\\b" << "\\bDASSL\\b" << "\\bJacobian\\b" << "\\bcrossing\\b"
    << "\\bIndividual\\b" << "\\bCrossings\\b"
    << "\\bResults\\b" << "\\bfor\\b" << "\\bPartitioning\\b" << "\\bLPS\\b"
    << "\\bexternal\\b" << "\\bpast\\b"
    << "\\bmin\\b" << "\\bmax\\b" << "\\bmessages\\b" << "\\bProcessed\\b"
    << "\\bsent\\b" << "\\bExternal\\b" << "\\bMessages\\b"
    << "\\bvariable\\b" << "\\breinits\\b" << "\\bCPU\\b" << "\\bper\\b"
    << "\\btransition\\b" << "\\bParallel\\b" << "\\bStatistics\\b"
    << "\\bInitialization\\b" << "\\bEstimated\\b" << "\\bsequential\\b"
    << "\\bspeedup\\b" << "\\befficiency\\b" << "\\bload\\b"
    << "\\bAverage\\b"
    << "\\bOutput\\b" << "\\bsteps\\b" << "\\bEvents\\b" << "\\bdetected\\b"
    << "\\bZero\\b" << "\\bLP\\b" << "\\bstep\\b" << "\\bcost\\b";
    foreach (const QString &pattern, keywordPatterns)
    {
      rule.pattern = QRegExp(pattern);
      rule.format = keywordFormat;
      highlightingRules.append(rule);
    }
    break;
  }
  commentStartExpression = QRegExp("/\\*");
  commentEndExpression = QRegExp("\\*/");
}

void
MmoHighlighter::highlightBlock(const QString &text)
{
  foreach(const MmoHighlightingRule &rule, highlightingRules)
  {  QRegExp expression(rule.pattern);
  int index = expression.indexIn(text);
  while (index >= 0)
  {
    int length = expression.matchedLength();
    setFormat(index, length, rule.format);
    index = expression.indexIn(text, index + length);
  }
}
setCurrentBlockState(0);
int startIndex = 0;
if(previousBlockState() != 1)
startIndex = commentStartExpression.indexIn(text);
while(startIndex >= 0)
{
  int endIndex = commentEndExpression.indexIn(text, startIndex);
  int commentLength;
  if(endIndex == -1)
  {
    setCurrentBlockState(1);
    commentLength = text.length() - startIndex;
  }
  else
  {
    commentLength = endIndex - startIndex
    + commentEndExpression.matchedLength();
  }
  setFormat(startIndex, commentLength, multiLineCommentFormat);
  startIndex = commentStartExpression.indexIn(text,
      startIndex + commentLength);
}
}
