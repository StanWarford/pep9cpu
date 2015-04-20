// File: helpdialog.h
/*
    Pep9CPU is a CPU simulator for executing microcode sequences to
    implement instructions in the instruction set of the Pep/8 computer.

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
        eEXAMPLES = 2,
        eEXCERCISES = 3,

        eCPU = 0,
        eMICROCODE = 1,
        eDEBUGGING = 2,

        eFIG1205 = 0,
        eFIG1207 = 1,
        eFIG1209 = 2,
        eFIG1210 = 3,
        eFIG1211 = 4,
        eFIG1213 = 5,
        eFIG1220 = 6,
        eFIG1221 = 7,

        eEX1205 = 0,
        eEX1206 = 1,
        eEX1207A = 2,
        eEX1207B = 3,
        eEX1207C = 4,
        eEX1207D = 5,
        eEX1207E = 6,
        eEX1207F = 7,
        eEX1207G = 8,
        eEX1207H = 9,
        eEX1207I = 10,
        eEX1207J = 11,
        eEX1207K = 12,
        eEX1207L = 13,
        eEX1207M = 14,
        eEX1207N = 15,
        eEX1207O = 16,
        eEX1207P = 17,
        eEX1207Q = 18,
        eEX1207R = 19,
        eEX1207S = 20,
        eEX1207T = 21,
        eEX1207U = 22,
        eEX1207V = 23,
        eEX1207W = 24,
        eEX1207X = 25,
        eEX1208 = 26,
    };

private slots:
    void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

signals:
    void copyToMicrocodeClicked();

};

#endif // HELPDIALOG_H
