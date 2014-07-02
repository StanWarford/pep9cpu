// File: helpdialog.h
/*
    Pep8CPU is a CPU simulator for executing microcode sequences to
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
        eUSINGPEP8CPU = 0,
        ePEP8REFERENCE = 1,
        eEXAMPLES = 2,
        eEXCERCISES = 3,

        eCPU = 0,
        eMICROCODE = 1,
        eDEBUGGING = 2,

        eFIG1205 = 0,
        eFIG1205B = 1,
        eFIG1208 = 2,
        eFIG1209 = 3,
        eFIG1210  = 4,
        eFIG1212  = 5,

        eEX1204 = 0,
        eEX1205 = 1,
        eEX1206 = 2,
        eEX1207A = 3,
        eEX1207B = 4,
        eEX1207C = 5,
        eEX1207D = 6,
        eEX1207E = 7,
        eEX1207F = 8,
        eEX1207G = 9,
        eEX1207H = 10,
        eEX1207I = 11,
        eEX1207J = 12,
        eEX1207K = 13,
        eEX1207L = 14,
        eEX1207M = 15,
        eEX1207N = 16,
        eEX1207O = 17,
        eEX1207P = 18,
        eEX1207Q = 19,
        eEX1207R = 20,
        eEX1207S = 21,
        eEX1207T = 22,
        eEX1207U = 23,
        eEX1207V = 24,
        eEX1207W = 25,
        eEX1207X = 26,
        eEX1208 = 27,
    };

private slots:
    void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

signals:
    void copyToMicrocodeClicked();

};

#endif // HELPDIALOG_H
