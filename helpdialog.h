// File: helpdialog.h
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
#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "pephighlighter.h"
#include "microcodeeditor.h"

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog {
    Q_OBJECT
public:
    HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

    QString getExampleText();

    void copy();
    // Post: the text edit that has focus has the copy() operation performed

    bool hasFocus();
    // Returns if the webview or the textedit has focus

    void selectItem(QString string);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::HelpDialog *ui;

    MicrocodeEditor *microcodeEditor;
    PepHighlighter *leftHighlighter;

    enum Row {
        eUSINGPEP9CPU = 0,
        ePEP9REFERENCE = 1,
        eONEBYTEBUSEXAMPLES = 2,
        eTWOBYTEBUSEXAMPLES = 3,
        eONEBYTEBUSPROBLEMS = 4,
        eTWOBYTEBUSPROBLEMS = 5,

        eCPU = 0,
        eMICROCODE = 1,
        eDEBUGGING = 2,

        eFIG1205 = 0,
        eFIG1207 = 1,
        eFIG1209 = 2,
        eFIG1210 = 3,
        eFIG1211 = 4,
        eFIG1212 = 5,
        eFIG1214 = 6,

        eFIG1220 = 0,
        eFIG1221 = 1,

        ePR1250 = 0,
        ePR1251A = 1,
        ePR1251B = 2,
        ePR1251C = 3,
        ePR1251D = 4,
        ePR1251E = 5,
        ePR1251F = 6,
        ePR1251G = 7,
        ePR1252 = 8,
        ePR1253A = 9,
        ePR1253B = 10,
        ePR1253C = 11,
        ePR1253D = 12,
        ePR1253E = 13,
        ePR1253F = 14,
        ePR1253G = 15,
        ePR1254A = 16,
        ePR1254B = 17,
        ePR1254C = 18,
        ePR1254D = 19,
        ePR1254E = 20,
        ePR1254F = 21,
        ePR1254G = 22,
        ePR1254H = 23,
        ePR1254I = 24,
        ePR1254J = 25,
        ePR1254K = 26,
        ePR1254L = 27,
        ePR1255A = 28,
        ePR1255B = 29,
        ePR1255C = 30,
        ePR1255D = 31,
        ePR1255E = 32,
        ePR1255F = 33,

        eEX1206X = 0,
        eEX1207 = 1,
    };

private slots:
    void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

signals:
    void copyToMicrocodeClicked();

};

#endif // HELPDIALOG_H
