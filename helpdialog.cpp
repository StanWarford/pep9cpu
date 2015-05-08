// File: helpdialog.cpp
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
    else if (!isHelpSubCat && row == ePEP9REFERENCE) { // Pep/9 Reference
        ui->helpSplitter->widget(1)->hide();
        ui->helpTopWebView->show();
        ui->helpTopWebView->load(QUrl("qrc:/help/pep9reference.html"));
    }
    else if ((!isHelpSubCat && row == eONEBYTEBUSEXAMPLES) || parentRow == eONEBYTEBUSEXAMPLES) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/onebytebusexamples.html"));
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
                ui->helpFigureLabel->setText("<b>Figure 12.10</b> The control signals to implement the store word instruction with direct addressing.");
            }
            else if (row == eFIG1211) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1211.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.11</b> The control signals to implement the add instruction with immediate addressing.");
            }
            else if (row == eFIG1212) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1212.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.12</b> The control signals to implement the load instruction with indirect addressing.");
            }
            else if (row == eFIG1214) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1214.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.14</b> The control signals to implement the unary ASRA instruction.");
            }
        }
    }
    else if ((!isHelpSubCat && row == eTWOBYTEBUSEXAMPLES) || parentRow == eTWOBYTEBUSEXAMPLES) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/twobytebusexamples.html"));
        }
        else {
            ui->helpSplitter->widget(0)->hide();
            ui->helpSplitter->widget(1)->show();
            if (row == eFIG1220) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1220.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.20</b> The control signals to fetch the instruction specifier and increment PC by 1.");
            }
            else if (row == eFIG1221) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/fig1221.pepcpu"));
                ui->helpFigureLabel->setText("<b>Figure 12.21</b> The control signals to retrieve the pre-fetched instruction specifier.");
            }
        }
    }
    else if ((!isHelpSubCat && row == eONEBYTEBUSPROBLEMS) || parentRow == eONEBYTEBUSPROBLEMS) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/onebytebusproblems.html"));
        }
        else {
            ui->helpSplitter->widget(0)->hide();
            ui->helpSplitter->widget(1)->show();
            if (row == ePR1250) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1250.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.50</b> Specification to fetch the operand specifier and increment PC.");
            }
            else if (row == ePR1251A) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251a.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (a)</b> Specification for <code>MOVSPA</code>.");
            }
            else if (row == ePR1251B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (b)</b> Specification for <code>MOVFLGA</code>.");
            }
            else if (row == ePR1251C) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251c.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (c)</b> Specification for <code>MOVAFLG</code>.");
            }
            else if (row == ePR1251D) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251d.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (d)</b> Specification for <code>NOTA</code>.");
            }
            else if (row == ePR1251E) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251e.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (e)</b> Specification for <code>NEGA</code>.");
            }
            else if (row == ePR1251F) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251f.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (f)</b> Specification for <code>ROLA</code>.");
            }
            else if (row == ePR1251G) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1251g.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.51 (g)</b> Specification for <code>RORA</code>.");
            }
            else if (row == ePR1252) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1252.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.52</b> Specification for <code>ASLA</code>.");
            }
            else if (row == ePR1253A) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253a.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (a)</b> Specification for <code>SUBA this,i</code>.");
            }
            else if (row == ePR1253B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (b)</b> Specification for <code>ANDA this,i</code>.");
            }
            else if (row == ePR1253C) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253c.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (c)</b> Specification for <code>ORA this,i</code>.");
            }
            else if (row == ePR1253D) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253d.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (d)</b> Specification for <code>CPWA this,i</code>.");
            }
            else if (row == ePR1253E) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253e.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (e)</b> Specification for <code>CPBA this,i</code>.");
            }
            else if (row == ePR1253F) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253f.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (f)</b> Specification for <code>LDWA this,i</code>.");
            }
            else if (row == ePR1253G) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1253g.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.53 (g)</b> Specification for <code>LDBA this,i</code>.");
            }
            else if (row == ePR1254A) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254a.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (a)</b> Specification for <code>LDWA here,d	</code>.");
            }
            else if (row == ePR1254B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (b)</b> Specification for <code>LDWA here,s</code>.");
            }
            else if (row == ePR1254C) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254c.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (c)</b> Specification for <code>LDWA here,sf</code>.");
            }
            else if (row == ePR1254D) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254d.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (d)</b> Specification for <code>LDWA here,x</code>.");
            }
            else if (row == ePR1254E) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254e.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (e)</b> Specification for <code>LDWA here,sx</code>.");
            }
            else if (row == ePR1254F) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254f.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (f)</b> Specification for <code>LDWA here,sfx</code>.");
            }
            else if (row == ePR1254G) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254g.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (g)</b> Specification for <code>STWA there,n</code>.");
            }
            else if (row == ePR1254H) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254h.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (h)</b> Specification for <code>STWA there,s</code>.");
            }
            else if (row == ePR1254I) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254i.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (i)</b> Specification for <code>STWA there,sf</code>.");
            }
            else if (row == ePR1254J) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254j.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (j)</b> Specification for <code>STWA there,x</code>.");
            }
            else if (row == ePR1254K) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254k.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (k)</b> Specification for <code>STWA there,sx</code>.");
            }
            else if (row == ePR1254L) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1254l.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.54 (l)</b> Specification for <code>STWA there,sfx</code>.");
            }
            else if (row == ePR1255A) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1255a.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.55 (a)</b> Specification for <code>BR main</code>.");
            }
            else if (row == ePR1255B) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1255b.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.55 (b)</b> Specification for <code>BR guessJT,x</code>.");
            }
            else if (row == ePR1255C) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1255c.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.55 (c)</b> Specification for <code>CALL alpha</code>.");
            }
            else if (row == ePR1255D) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1255d.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.55 (d)</b> Specification for <code>RET</code>.");
            }
            else if (row == ePR1255E) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1255e.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.55 (e)</b> Specification for <code>DECO num,i</code>.");
            }
            else if (row == ePR1255F) {
                microcodeEditor->setPlainText(Pep::resToString(":/help/figures/prob1255f.pepcpu"));
                ui->helpFigureLabel->setText("<b>Problem 12.55 (f)</b> Specification for <code>RETTR</code>.");
            }
        }
    }
    else if ((!isHelpSubCat && row == eTWOBYTEBUSPROBLEMS) || parentRow == eTWOBYTEBUSPROBLEMS) {
        if (!isHelpSubCat) {
            ui->helpSplitter->widget(1)->hide();
            ui->helpTopWebView->show();
            ui->helpTopWebView->load(QUrl("qrc:/help/twobytebusproblems.html"));
        }
        else {
            ui->helpSplitter->widget(0)->hide();
            ui->helpSplitter->widget(1)->show();
            if (row == eEX1206X) {
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

