// File: helpdialog.cpp
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

    if (!isHelpSubCat && row == eUSINGPEP9CPU) { // Using Pep/9 CPU
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/usingpep9cpu.html"));
    }
    else if (isHelpSubCat && parentRow == eUSINGPEP9CPU && row == eCPU) { // Interactive use
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/cpu.html"));
    }
    else if (isHelpSubCat && parentRow == eUSINGPEP9CPU && row == eMICROCODE) { // Microcode use
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/microcode.html"));
    }
    else if (isHelpSubCat && parentRow == eUSINGPEP9CPU && row == eDEBUGGING) { // Debugging use
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/debugging.html"));
    }
    else if (!isHelpSubCat && row == ePEP9REFERENCE) { // Pep/8 Reference
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/pep9reference.html"));
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
            else if (row == eFIG1207) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1207.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.7</b> Combining cycles of Figure 12.5.");
            }
            else if (row == eFIG1209) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1209.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.9</b> The control signals to implement the store byte instruction with direct addressing.");
            }
            else if (row == eFIG1210) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1210.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.10</b> The control signals to implement the add instruction with immediate addressing.");
            }
            else if (row == eFIG1211) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1211.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.11</b> The control signals to implement the load instruction with indirect addressing.");
            }
            else if (row == eFIG1213) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1213.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.13</b> The control signals to implement the unary ASRA instruction.");
            }
            else if (row == eFIG1220) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1220.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.20</b> The control signals to fetch the instruction specifier and increment PC by 1.");
            }
            else if (row == eFIG1221) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1221.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.21</b> The control signals to retrieve the pre-fetched instruction specifier.");
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
            if (row == eEX1205) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1205.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.5</b> Specification to fetch the operand specifier and increment PC.");
            }
            else if (row == eEX1206A) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206a.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (a)</b> Specification for <code>STBA there,n</code>.");
            }
            else if (row == eEX1206B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (b)</b> Specification for <code>STBA there,s</code>.");
            }
            else if (row == eEX1206C) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206c.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (c)</b> Specification for <code>STBA there,sf</code>.");
            }
            else if (row == eEX1206D) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206d.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (d)</b> Specification for <code>STBA there,x</code>.");
            }
            else if (row == eEX1206E) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206e.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (e)</b> Specification for <code>STBA there,sx</code>.");
            }
            else if (row == eEX1206F) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206f.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (f)</b> Specification for <code>STBA there,sxf</code>.");
            }
            else if (row == eEX1206G) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206g.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (g)</b> Specification for <code>BR there</code>.");
            }
            else if (row == eEX1206H) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206h.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (h)</b> Specification for <code>CALL there</code>.");
            }
            else if (row == eEX1206I) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206i.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (i)</b> Specification for <code>NOTA</code>.");
            }
            else if (row == eEX1206J) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206j.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (j)</b> Specification for <code>NEGA</code>.");
            }
            else if (row == eEX1206K) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206k.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (k)</b> Specification for <code>ROLA</code>.");
            }
            else if (row == eEX1206L) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206l.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (l)</b> Specification for <code>RORA</code>.");
            }
            else if (row == eEX1206M) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206m.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (m)</b> Specification for <code>RET4</code>.");
            }
            else if (row == eEX1206N) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206n.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (n)</b> Specification for <code>ADDSP this,i</code>.");
            }
            else if (row == eEX1206O) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206o.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (o)</b> Specification for <code>SUBSP this,i</code>.");
            }
            else if (row == eEX1206P) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206p.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (p)</b> Specification for <code>SUBA this,i</code>.");
            }
            else if (row == eEX1206Q) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206q.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (q)</b> Specification for <code>ANDA this,i</code>.");
            }
            else if (row == eEX1206R) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206r.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (r)</b> Specification for <code>ORA this,i</code>.");
            }
            else if (row == eEX1206S) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206s.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (s)</b> Specification for <code>CPA this,i</code>.");
            }
            else if (row == eEX1206T) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206t.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (t)</b> Specification for <code>LDBA this,i</code>.");
            }
            else if (row == eEX1206U) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206u.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (u)</b> Specification for <code>LDBA this,d</code>.");
            }
            else if (row == eEX1206V) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206v.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (v)</b> Specification for <code>MOVSPA</code>.");
            }
            else if (row == eEX1206W) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206w.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (w)</b> Specification for <code>MOVFLGA</code>.");
            }
            else if (row == eEX1206X) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1206x.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.6 (x)</b> Specification for <code>RETTR</code>.");
            }
            else if (row == eEX1207) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/exer1207.pepcpu"));
                ui->helpFigureLabel->setText("<b>Exercise 12.7</b> Specification for <code>DECO num,i</code>, which is a trap instruction.");
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

