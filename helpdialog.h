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
        eFIG1212 = 5,
        eFIG1214 = 6,
        eFIG1220 = 7,
        eFIG1221 = 8,

        eEX1205 = 0,
        eEX1206A = 1,
        eEX1206B = 2,
        eEX1206C = 3,
        eEX1206D = 4,
        eEX1206E = 5,
        eEX1206F = 6,
        eEX1206G = 7,
        eEX1206H = 8,
        eEX1206I = 9,
        eEX1206J = 10,
        eEX1206K = 11,
        eEX1206L = 12,
        eEX1206M = 13,
        eEX1206N = 14,
        eEX1206O = 15,
        eEX1206P = 16,
        eEX1206Q = 17,
        eEX1206R = 18,
        eEX1206S = 19,
        eEX1206T = 20,
        eEX1206U = 21,
        eEX1206V = 22,
        eEX1206W = 23,
        eEX1206X = 24,
        eEX1207 = 25,
    };

private slots:
    void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

signals:
    void copyToMicrocodeClicked();

};

#endif // HELPDIALOG_H
