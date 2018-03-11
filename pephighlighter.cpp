//File: pephighlighter.cpp
/*
    Pep9CPU is a CPU simulator for executing microcode sequences to
    implement instructions in the instruction set of the Pep/9 computer.

    Copyright (C) 2010  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pephighlighter.h"
#include "pep.h"

PepHighlighter::PepHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent),forcedFeatures(false)
{
    HighlightingRule rule;

    numFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("(0x)?[0-9a-fA-F]+(?=(,|;|(\\s)*$|\\]|(\\s)*//))");
    rule.format = numFormat;
    highlightingRulesOne.append(rule);
    highlightingRulesTwo.append(rule);
    oprndFormat.setForeground(Qt::darkBlue);
    oprndFormat.setFontWeight(QFont::Bold);
    QStringList oprndPatterns;
        oprndPatterns << "\\bLoadCk\\b" << "\\bC\\b" << "\\bB\\b"
                << "\\bA\\b" << "\\bMARCk\\b" << "\\bMDRCk\\b"
                << "\\bAMux\\b" << "\\bMDRMux\\b" << "\\bCMux\\b"
                << "\\bALU\\b" << "\\bCSMux\\b" << "\\bSCk\\b" << "\\bCCk\\b" << "\\bVCk\\b"
                << "\\bAndZ\\b" << "\\bZCk\\b" << "\\bNCk\\b"
                << "\\bMemRead\\b" << "\\bMemWrite\\b" << "^(\\s)*UnitPre(?=:)\\b" << "^(\\s)*UnitPost(?=:)\\b"
                   // pre/post symbols:
                << "\\bN\\b" << "\\bZ\\b" << "\\bV\\b" << "\\bS\\b"
                << "\\bX\\b" << "\\bSP\\b" << "\\bPC\\b" << "\\bIR\\b"
                << "\\bT1\\b" << "\\bT2\\b" << "\\bT3\\b" << "\\bT4\\b"
                << "\\bT5\\b" << "\\bT6\\b" << "\\bMem\\b";
        foreach (const QString &pattern, oprndPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = oprndFormat;
            highlightingRulesOne.append(rule);
        }
        oprndPatterns.clear();
        oprndPatterns << "\\bLoadCk\\b" << "\\bC\\b" << "\\bB\\b"
                << "\\bA\\b" << "\\bMARCk\\b" << "\\bMARMux\\b"
                << "\\bMDROCk\\b" << "\\bMDRECk\\b" << "\\bMDROMux\\b" << "\\bMDREMux\\b" << "\\bEOMux\\b" << "\\bCMux\\b"
                << "\\bAMux\\b"<< "\\bALU\\b" << "\\bCSMux\\b" << "\\bSCk\\b" << "\\bCCk\\b" << "\\bVCk\\b"
                << "\\bAndZ\\b" << "\\bZCk\\b" << "\\bNCk\\b"
                << "\\bMemRead\\b" << "\\bMemWrite\\b" << "^(\\s)*UnitPre(?=:)\\b" << "^(\\s)*UnitPost(?=:)\\b"
                   // pre/post symbols:
                << "\\bN\\b" << "\\bZ\\b" << "\\bV\\b" << "\\bS\\b"
                << "\\bX\\b" << "\\bSP\\b" << "\\bPC\\b" << "\\bIR\\b"
                << "\\bT1\\b" << "\\bT2\\b" << "\\bT3\\b" << "\\bT4\\b"
                << "\\bT5\\b" << "\\bT6\\b" << "\\bMem\\b";
        foreach (const QString &pattern, oprndPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = oprndFormat;
            highlightingRulesTwo.append(rule);
        }


    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//.*");
    rule.format = singleLineCommentFormat;
    highlightingRulesOne.append(rule);
    highlightingRulesTwo.append(rule);
    highlightingRulesAll.append(highlightingRulesOne);
    highlightingRulesAll.append(highlightingRulesTwo);
    multiLineCommentFormat.setForeground(Qt::white);
    multiLineCommentFormat.setBackground(Qt::red);

    commentStartExpression = QRegExp("//\\sERROR:[\\s]");
    commentEndExpression = QRegExp("$");
}

void PepHighlighter::forceAllFeatures(bool features)
{
    forcedFeatures=features;
}
void PepHighlighter::highlightBlock(const QString &text)
{

    QVector<HighlightingRule> highlightingRules;
    if(forcedFeatures){
        highlightingRules=highlightingRulesAll;
    }
    else{
        highlightingRules=Pep::cpuFeatures==Enu::CPUType::OneByteDataBus?highlightingRulesOne:highlightingRulesTwo;
    }

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        expression.setCaseSensitivity(Qt::CaseInsensitive);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
