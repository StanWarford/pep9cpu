// File: helpdialog.cpp
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

#include "helpdialog.h"
#include "ui_helpdialog.h"

#include "pep.h"
#include <QClipboard>
#include <QDebug>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    connect(ui->helpTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this,
            SLOT(onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    // Forward the helpCopyToMicrocodeButton_clicked() signal from this to the main window
    connect(ui->helpCopyToMicrocodeButton, SIGNAL(clicked()), this, SIGNAL(copyToMicrocodeClicked()));

    ui->helpSplitter->widget(1)->hide();
    ui->helpTreeWidget->expandAll();
    ui->helpTreeWidget->itemAt(0,0)->setSelected(true);

    delete ui->helpTextEdit;

    microcodeEditor = new MicrocodeEditor(this, false, true);
    ui->verticalLayout->insertWidget(0, microcodeEditor);

    leftHighlighter = new PepHighlighter(microcodeEditor->document());

    ui->helpTreeWidget->setFont(QFont(Pep::labelFont, Pep::labelFontSize));

    microcodeEditor->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::selectItem(QString string) {
    QTreeWidgetItemIterator it(ui->helpTreeWidget);
    while (*it) {
        if ((*it)->text(0) == string) {
            (*it)->setSelected(true);
            ui->helpTreeWidget->setCurrentItem((*it));
        } else {
            (*it)->setSelected(false);
        }
        ++it;
    }
}

QString HelpDialog::getExampleText()
{
    return microcodeEditor->toPlainText();
}

void HelpDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void HelpDialog::onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*) {
    // Is this a subcategory?
    bool isHelpSubCat = ui->helpTreeWidget->currentIndex().parent().isValid();
    // Parent row (if it has a parent, -1 else)
    int parentRow = ui->helpTreeWidget->currentIndex().parent().row();
    // Row (if it has a parent, this is the child row)
    int row = ui->helpTreeWidget->currentIndex().row();

//    qDebug() << "Selected: " << ui->helpTreeWidget->currentIndex();

    if (!isHelpSubCat && row == eUSINGPEP8CPU) { // Using Pep/8 CPU
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/usingpep8cpu.html"));
    }
    else if (isHelpSubCat && parentRow == eUSINGPEP8CPU && row == eCPU) { // Interactive use
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/cpu.html"));
    }
    else if (isHelpSubCat && parentRow == eUSINGPEP8CPU && row == eMICROCODE) { // Microcode use
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/microcode.html"));
    }
    else if (isHelpSubCat && parentRow == eUSINGPEP8CPU && row == eDEBUGGING) { // Debugging use
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/debugging.html"));
    }
    else if (!isHelpSubCat && row == ePEP8REFERENCE) { // Pep/8 Reference
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/pep8reference.html"));
    }
    else if ((!isHelpSubCat && row == eEXAMPLES) || parentRow == eEXAMPLES) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/examples.html"));
        }
        else {
            ui->helpSplitter->widget(0)->hide();
            ui->helpSplitter->widget(1)->show();
            if (row == eFIG1205) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1205.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.5</b> The control signals to fetch the instruction specifier and increment PC by 1.");
            }
            else if (row == eFIG1205B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1205b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.5b</b> Combining cycles 1 and 3 of Figure 12.5.");
            }
            else if (row == eFIG1208) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1208.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.8</b> The control signals to implement the store byte instruction with direct addressing.");
            }
            else if (row == eFIG1209) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1209.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.9</b> The control signals to implement the add instruction with immediate addressing.");
            }
            else if (row == eFIG1210) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1210.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.10</b> The control signals to implement the load instruction with indirect addressing.");
            }
            else if (row == eFIG1212) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1212.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.12</b> The control signals to implement the unary ASRA instruction.");
            }
        }
    }
    else if ((!isHelpSubCat && row == eEXCERCISES) || parentRow == eEXCERCISES) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/exercises.html"));
        }
        else {
            ui->helpSplitter->widget(0)->hide();
            ui->helpSplitter->widget(1)->show();
            if (row == eEX1204) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1204.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.4</b> The control signals to fetch the instruction specifier and increment PC by 1.");
            }
            else if (row == eEX1205) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1205.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.5</b> Specification to fetch the operand specifier and increment PC.");
            }
            else if (row == eEX1206) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6</b> The control signals to implement the load instruction with indirect addressing.");
            }
            else if (row == eEX1207A) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207a.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (a)</b> Specification for <code>STBYTEA there,n</code>.");
            }
            else if (row == eEX1207B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (b)</b> Specification for <code>STBYTEA there,s</code>.");
            }
            else if (row == eEX1207C) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207c.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (c)</b> Specification for <code>STBYTEA there,sf</code>.");
            }
            else if (row == eEX1207D) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207d.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (d)</b> Specification for <code>STBYTEA there,x</code>.");
            }
            else if (row == eEX1207E) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207e.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (e)</b> Specification for <code>STBYTEA there,sx</code>.");
            }
            else if (row == eEX1207F) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207f.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (f)</b> Specification for <code>STBYTEA there,sxf</code>.");
            }
            else if (row == eEX1207G) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207g.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (g)</b> Specification for <code>BR there</code>.");
            }
            else if (row == eEX1207H) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207h.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (h)</b> Specification for <code>CALL there</code>.");
            }
            else if (row == eEX1207I) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207i.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (i)</b> Specification for <code>NOTA</code>.");
            }
            else if (row == eEX1207J) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207j.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (j)</b> Specification for <code>NEGA</code>.");
            }
            else if (row == eEX1207K) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207k.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (k)</b> Specification for <code>ROLA</code>.");
            }
            else if (row == eEX1207L) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207l.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (l)</b> Specification for <code>RORA</code>.");
            }
            else if (row == eEX1207M) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207m.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (m)</b> Specification for <code>RET4</code>.");
            }
            else if (row == eEX1207N) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207n.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (n)</b> Specification for <code>ADDSP this,i</code>.");
            }
            else if (row == eEX1207O) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207o.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (o)</b> Specification for <code>SUBSP this,i</code>.");
            }
            else if (row == eEX1207P) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207p.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (p)</b> Specification for <code>SUBA this,i</code>.");
            }
            else if (row == eEX1207Q) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207q.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (q)</b> Specification for <code>ANDA this,i</code>.");
            }
            else if (row == eEX1207R) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207r.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (r)</b> Specification for <code>ORA this,i</code>.");
            }
            else if (row == eEX1207S) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207s.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (s)</b> Specification for <code>CPA this,i</code>.");
            }
            else if (row == eEX1207T) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207t.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (t)</b> Specification for <code>LDBYTEA this,i</code>.");
            }
            else if (row == eEX1207U) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207u.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (u)</b> Specification for <code>LDBYTEA this,d</code>.");
            }
            else if (row == eEX1207V) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207v.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (v)</b> Specification for <code>MOVSPA</code>.");
            }
            else if (row == eEX1207W) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207w.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (w)</b> Specification for <code>MOVFLGA</code>.");
            }
            else if (row == eEX1207X) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207x.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7 (x)</b> Specification for <code>RETTR</code>.");
            }
            else if (row == eEX1208) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1208.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.8</b> Specification for <code>DECO num,i</code>, which is a trap instruction.");
            }
        }
    }
}

void HelpDialog::copy()
{
    if (microcodeEditor->hasFocus()) {
        microcodeEditor->copy();
    } else if (ui->helpTopWebView->hasFocus()) {
        QApplication::clipboard()->setText(ui->helpTopWebView->selectedText());
    }
}

bool HelpDialog::hasFocus()
{
    return microcodeEditor->hasFocus() || ui->helpTopWebView->hasFocus();
}

