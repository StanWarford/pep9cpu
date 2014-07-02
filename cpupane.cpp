// File: cpupane.cpp
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

#include "cpupane.h"
#include "ui_cpupane.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QGraphicsItem>
#include <QErrorMessage>
#include <QMessageBox>

#include <QDebug>

#include "cpupanegraphicsitems.h"
#include "tristatelabel.h"
#include "pep.h"
#include "code.h"
#include "sim.h"

using namespace Enu;

CpuPane::CpuPane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CpuPane)
{
    ui->setupUi(this);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(zoomFactorChanged(int)));

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setFont(QFont(Pep::cpuFont, Pep::cpuFontSize));

    cpuPaneItems = new CpuPaneGraphicsItems(ui->graphicsView, 0, scene);
    ui->graphicsView->scene()->addItem(cpuPaneItems);

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    connect(cpuPaneItems->loadCk, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->cLineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));
    connect(cpuPaneItems->bLineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));
    connect(cpuPaneItems->aLineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MARCk, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MDRCk, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->aMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->aMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->cMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->cMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->ALULineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));

    connect(cpuPaneItems->CCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->VCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->ANDZTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->ANDZTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->ZCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->NCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->MemReadTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->MemReadTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MemWriteTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->MemWriteTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->nBitLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->zBitLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->vBitLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->cBitLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));

    // Simulation control connections
    connect(ui->clockPushButton, SIGNAL(clicked()), this, SLOT(clockButtonPushed()));
    connect(ui->singleStepPushButton, SIGNAL(clicked()), this, SLOT(singleStepButtonPushed()));
    connect(ui->resumePushButton, SIGNAL(clicked()), this, SLOT(resumeButtonPushed()));

    // Register editing connnections
    connect(cpuPaneItems->aRegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->xRegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->spRegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->pcRegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->irRegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->t1RegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->t2RegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->t3RegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->t4RegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->t5RegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));
    connect(cpuPaneItems->t6RegLineEdit, SIGNAL(textEdited(QString)), this, SLOT(regTextEdited(QString)));

    connect(cpuPaneItems->aRegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->xRegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->spRegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->pcRegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->irRegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->t1RegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->t2RegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->t3RegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->t4RegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->t5RegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));
    connect(cpuPaneItems->t6RegLineEdit, SIGNAL(editingFinished()), this, SLOT(regTextFinishedEditing()));

    connect(cpuPaneItems->ALULineEdit, SIGNAL(textChanged(QString)), this, SLOT(ALUTextEdited(QString)));

    ui->spinBox->hide();
    ui->singleStepPushButton->setEnabled(false);
}

CpuPane::~CpuPane()
{
    delete ui;
}

void CpuPane::highlightOnFocus()
{
    if (ui->graphicsView->hasFocus() || ui->spinBox->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool CpuPane::hasFocus()
{
    return ui->graphicsView->hasFocus() || ui->spinBox->hasFocus();
}

void CpuPane::giveFocus()
{
    ui->graphicsView->setFocus();
}

void CpuPane::startDebugging()
{
    ui->resumePushButton->setEnabled(true);
    ui->singleStepPushButton->setEnabled(true);

    ui->clockPushButton->setEnabled(false);
    ui->copyToMicrocodePushButton->setEnabled(false);

    // reinitialize these to zero for this simulation
    Sim::microProgramCounter = 0;
    Sim::microCodeCurrentLine = 0;

    // Clear memread/write state from previous simulations
    Sim::mainBusState = Enu::None;
    Sim::memReadPrevStep = false;
    Sim::memWritePrevStep = false;

    Code *code = Sim::codeList.at(Sim::microCodeCurrentLine);
    while (!Sim::atEndOfSim() && !code->isMicrocode()) {
        Sim::microCodeCurrentLine++;
        code = Sim::codeList.at(Sim::microCodeCurrentLine);
    }
    code->setCpuLabels(cpuPaneItems);
    emit updateSimulation();
}

void CpuPane::stopDebugging()
{
    ui->resumePushButton->setEnabled(false);
    ui->singleStepPushButton->setEnabled(false);

    ui->clockPushButton->setEnabled(true);
    ui->copyToMicrocodePushButton->setEnabled(true);
}

void CpuPane::setRegister(Enu::EMnemonic reg, int value) {
    switch (reg) {
    case Enu::A:
        Sim::regBank[0] = (value & 65280) / 256;
        Sim::regBank[1] = (value & 255);
        cpuPaneItems->aRegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::X:
        Sim::regBank[2] = (value & 65280) / 256;
        Sim::regBank[3] = (value & 255);
        cpuPaneItems->xRegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::SP:
        Sim::regBank[4] = (value & 65280) / 256;
        Sim::regBank[5] = (value & 255);
        cpuPaneItems->spRegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::PC:
        Sim::regBank[6] = (value & 65280) / 256;
        Sim::regBank[7] = (value & 255);
        cpuPaneItems->pcRegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::IR:
        Sim::regBank[8] = (value & 0xFF0000) / 65536;
        Sim::regBank[9] = (value & 65280) / 256;
        Sim::regBank[10] = (value & 255);
        cpuPaneItems->irRegLineEdit->setText("0x" + QString("%1").arg(value, 6, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::T1:
        Sim::regBank[11] = (value & 255);
        cpuPaneItems->t1RegLineEdit->setText("0x" + QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::T2:
        Sim::regBank[12] = (value & 65280) / 256;
        Sim::regBank[13] = (value & 255);
        cpuPaneItems->t2RegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::T3:
        Sim::regBank[14] = (value & 65280) / 256;
        Sim::regBank[15] = (value & 255);
        cpuPaneItems->t3RegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::T4:
        Sim::regBank[16] = (value & 65280) / 256;
        Sim::regBank[17] = (value & 255);
        cpuPaneItems->t4RegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::T5:
        Sim::regBank[18] = (value & 65280) / 256;
        Sim::regBank[19] = (value & 255);
        cpuPaneItems->t5RegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::T6:
        Sim::regBank[20] = (value & 65280) / 256;
        Sim::regBank[21] = (value & 255);
        cpuPaneItems->t6RegLineEdit->setText("0x" + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::MARA:
        Sim::MARA = value;
        cpuPaneItems->MARALabel->setText("0x" + QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::MARB:
        Sim::MARB = value;
        cpuPaneItems->MARBLabel->setText("0x" + QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper());
        break;
    case Enu::MDR:
        Sim::MDR = value;
        cpuPaneItems->MDRLabel->setText("0x" + QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper());
        break;
    default:
        // the remainder of the array is 'read only' in our simulated CPU
        break;
    }
}

void CpuPane::setRegisterByte(int reg, quint8 value) {
    QLatin1Char ch = QLatin1Char('0');
    switch (reg) {
    case 0:
        Sim::regBank[0] = value;
        cpuPaneItems->aRegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[1], 4, 16, ch).toUpper());
        break;
    case 1:
        Sim::regBank[1] = value;
        cpuPaneItems->aRegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[0] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 2:
        Sim::regBank[2] = value;
        cpuPaneItems->xRegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[3], 4, 16, ch).toUpper());
        break;
    case 3:
        Sim::regBank[3] = value;
        cpuPaneItems->xRegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[2] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 4:
        Sim::regBank[4] = value;
        cpuPaneItems->spRegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[5], 4, 16, ch).toUpper());
        break;
    case 5:
        Sim::regBank[5] = value;
        cpuPaneItems->spRegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[4] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 6:
        Sim::regBank[6] = value;
        cpuPaneItems->pcRegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[7], 4, 16, ch).toUpper());
        break;
    case 7:
        Sim::regBank[7] = value;
        cpuPaneItems->pcRegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[6] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 8:
        Sim::regBank[8] = value;
        cpuPaneItems->irRegLineEdit->setText("0x" + QString("%1").arg(value * 65536 + Sim::regBank[9] * 256 + Sim::regBank[10], 6, 16, ch).toUpper());
        break;
    case 9:
        Sim::regBank[9] = value;
        cpuPaneItems->irRegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[8] * 65536 + value * 256 + Sim::regBank[10], 6, 16, ch).toUpper());
        break;
    case 10:
        Sim::regBank[10] = value;
        cpuPaneItems->irRegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[8] * 65536 + Sim::regBank[9] * 256 + value, 6, 16, ch).toUpper());
        break;
    case 11:
        Sim::regBank[11] = value;
        cpuPaneItems->t1RegLineEdit->setText("0x" + QString("%1").arg(value, 2, 16, ch).toUpper());
        break;
    case 12:
        Sim::regBank[12] = value;
        cpuPaneItems->t2RegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[13], 4, 16, ch).toUpper());
        break;
    case 13:
        Sim::regBank[13] = value;
        cpuPaneItems->t2RegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[12] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 14:
        Sim::regBank[14] = value;
        cpuPaneItems->t3RegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[15], 4, 16, ch).toUpper());
        break;
    case 15:
        Sim::regBank[15] = value;
        cpuPaneItems->t3RegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[14] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 16:
        Sim::regBank[16] = value;
        cpuPaneItems->t4RegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[17], 4, 16, ch).toUpper());
        break;
    case 17:
        Sim::regBank[17] = value;
        cpuPaneItems->t4RegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[16] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 18:
        Sim::regBank[18] = value;
        cpuPaneItems->t5RegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[19], 4, 16, ch).toUpper());
        break;
    case 19:
        Sim::regBank[19] = value;
        cpuPaneItems->t5RegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[18] * 256 + value, 4, 16, ch).toUpper());
        break;
    case 20:
        Sim::regBank[20] = value;
        cpuPaneItems->t6RegLineEdit->setText("0x" + QString("%1").arg(value * 256 + Sim::regBank[21], 4, 16, ch).toUpper());
        break;
    case 21:
        Sim::regBank[21] = value;
        cpuPaneItems->t6RegLineEdit->setText("0x" + QString("%1").arg(Sim::regBank[20] * 256 + value, 4, 16, ch).toUpper());
        break;
    default:
        // the remainder of the array is 'read only' in our simulated CPU, or outside the bounds
        break;
    }
}

void CpuPane::setStatusBit(Enu::EMnemonic bit, bool value)
{
    switch (bit) {
    case Enu::N:
        Sim::nBit = value;
        cpuPaneItems->nBitLabel->setText(QString("%1").arg(value ? 1 : 0));
        break;
    case Enu::Z:
        Sim::zBit = value;
        cpuPaneItems->zBitLabel->setText(QString("%1").arg(value ? 1 : 0));
        break;
    case Enu::V:
        Sim::vBit = value;
        cpuPaneItems->vBitLabel->setText(QString("%1").arg(value ? 1 : 0));
        break;
    case Enu::C:
        Sim::cBit = value;
        cpuPaneItems->cBitLabel->setText(QString("%1").arg(value ? 1 : 0));
        break;
    default:
        break;
    }
}

void CpuPane::setRegPrecondition(Enu::EMnemonic reg, int value)
{
    setRegister(reg, value);
}

void CpuPane::setStatusPrecondition(Enu::EMnemonic bit, bool value)
{
    setStatusBit(bit, value);
}

bool CpuPane::testRegPostcondition(Enu::EMnemonic reg, int value) {
    switch (reg) {
    case Enu::A:
        return Sim::regBank[0] * 256 + Sim::regBank[1] == value;
    case Enu::X:
        return Sim::regBank[2] * 256 + Sim::regBank[3] == value;
    case Enu::SP:
        return Sim::regBank[4] * 256 + Sim::regBank[5] == value;
    case Enu::PC:
        return Sim::regBank[6] * 256 + Sim::regBank[7] == value;
    case Enu::IR:
        return Sim::regBank[8] * 65536 + Sim::regBank[9] * 256 + Sim::regBank[10] == value;
    case Enu::T1:
        return Sim::regBank[11] == value;
    case Enu::T2:
        return Sim::regBank[12] * 256 + Sim::regBank[13] == value;
    case Enu::T3:
        return Sim::regBank[14] * 256 + Sim::regBank[15] == value;
    case Enu::T4:
        return Sim::regBank[16] * 256 + Sim::regBank[17] == value;
    case Enu::T5:
        return Sim::regBank[18] * 256 + Sim::regBank[19] == value;
    case Enu::T6:
        return Sim::regBank[20] * 256 + Sim::regBank[21] == value;
    case Enu::MARA:
        return Sim::MARA == value;
    case Enu::MARB:
        return Sim::MARB == value;
    case Enu::MDR:
        return Sim::MDR == value;
    default:
        break;
    }
    return true;
}

bool CpuPane::testStatusPostcondition(Enu::EMnemonic bit, bool value) {
    switch (bit) {
    case Enu::N:
        return Sim::nBit == value;
    case Enu::Z:
        return Sim::zBit == value;
    case Enu::V:
        return Sim::vBit == value;
    case Enu::C:
        return Sim::cBit == value;
    default:
        break;
    }
    return true;
}

void CpuPane::clearCpu()
{
    clearCpuControlSignals();

    setRegister(Enu::A, 0);
    setRegister(Enu::X, 0);
    setRegister(Enu::SP, 0);
    setRegister(Enu::PC, 0);
    setRegister(Enu::IR, 0);
    setRegister(Enu::T1, 0);
    setRegister(Enu::T2, 0);
    setRegister(Enu::T3, 0);
    setRegister(Enu::T4, 0);
    setRegister(Enu::T5, 0);
    setRegister(Enu::T6, 0);

    setRegister(Enu::MARA, 0);
    setRegister(Enu::MARB, 0);
    setRegister(Enu::MDR, 0);

    setStatusBit(Enu::C, false);
    setStatusBit(Enu::V, false);
    setStatusBit(Enu::Z, false);
    setStatusBit(Enu::N, false);
}

void CpuPane::clearCpuControlSignals()
{
    cpuPaneItems->loadCk->setChecked(false);
    cpuPaneItems->cLineEdit->setText("");
    cpuPaneItems->bLineEdit->setText("");
    cpuPaneItems->aLineEdit->setText("");
    cpuPaneItems->MARCk->setChecked(false);
    cpuPaneItems->MDRCk->setChecked(false);
    cpuPaneItems->aMuxTristateLabel->setText("");
    cpuPaneItems->MDRMuxTristateLabel->setText("");
    cpuPaneItems->cMuxTristateLabel->setText("");
    cpuPaneItems->ALULineEdit->setText("");
    cpuPaneItems->CCkCheckBox->setChecked(false);
    cpuPaneItems->VCkCheckBox->setChecked(false);
    cpuPaneItems->ANDZTristateLabel->setText("");
    cpuPaneItems->ZCkCheckBox->setChecked(false);
    cpuPaneItems->NCkCheckBox->setChecked(false);
    cpuPaneItems->MemReadTristateLabel->setText("");
    cpuPaneItems->MemWriteTristateLabel->setText("");
}

void CpuPane::singleStep()
{
    singleStepButtonPushed();
}

void CpuPane::clock()
{
    clockButtonPushed();
}

void CpuPane::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CpuPane::updateMainBusState()
{
    bool marUnchanged = true;
    if (cpuPaneItems->MARCk->isChecked()) {
        quint8 a, b;
        QString errorString; // temporary, any errors here will be caught in the MARCk section of step()
        if (getABusOut(a, errorString) && getBBusOut(b, errorString)) {
            marUnchanged = (a == Sim::MARA) && (b == Sim::MARB);
        }
        else {
            // error: MARCk is checked but we have incorrect input.
            // This will be caught in step() under the MARCk section
        }
    }

    switch (Sim::mainBusState) {
    case Enu::None:
        if (marUnchanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (1st)
            Sim::mainBusState = Enu::MemReadWait;
        }
        else if (marUnchanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (1st)
            Sim::mainBusState = Enu::MemWriteWait;
        }
        //else: mainBusState = None, but it already is.
        break;
    case Enu::MemReadWait:
        if (marUnchanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (2nd with unchanged MAR)
            Sim::mainBusState = Enu::MemReadReady;
        }
        else if (!marUnchanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead
            // do nothing, already MemReadWait - need another MemRead because the MAR changed
            qDebug() << "MAR changed - don't read yet";
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after a sinle MemRead)
            Sim::mainBusState = Enu::MemWriteWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemReadReady:
        if (marUnchanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead again (more than 2 in a row)
            // do nothing, already MemReadReady
        }
        else if (!marUnchanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead
            Sim::mainBusState = Enu::MemReadWait; // go back to MemReadWait because the MAR changed
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after 2+ MemReads)
            Sim::mainBusState = Enu::MemWriteWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteWait:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (after a MemWrite)
            Sim::mainBusState = Enu::MemReadWait;
        }
        else if (marUnchanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (2nd in a row)
            Sim::mainBusState = Enu::MemWriteReady;
        }
        else if (!marUnchanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (with changed MAR)
            // do nothing, MAR changed, still MemReadWait
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteReady:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (after 2+ MemWrites)
            Sim::mainBusState = Enu::MemReadWait;
        }
        else if (marUnchanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after 2+ in a row)
            // do nothing, already MemWriteReady
        }
        else if (!marUnchanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (with changed MAR)
            Sim::mainBusState = Enu::MemWriteWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    default:
        Sim::mainBusState = Enu::None; // Just in case the Sim::mBS is malformed somehow
        break;
    }

    Sim::memReadPrevStep = cpuPaneItems->MemReadTristateLabel->text() == "1";
    Sim::memWritePrevStep = cpuPaneItems->MemWriteTristateLabel->text() == "1";
}

bool CpuPane::step(QString &errorString)
{
    // Clear modified bytes for simulation view:
    Sim::modifiedBytes.clear();

    // Update Bus State
    updateMainBusState(); // FSM that sets Sim::mainBusState to Enu::BusState - 5 possible states

    // Status bit calculations
    int aluFn = cpuPaneItems->ALULineEdit->text().toInt();
    int carry;
    int overflow;
    quint8 result, a, b;

    QString errtemp;
    getALUOut(result, a, b, carry, overflow, errtemp); // ignore boolean returned - error would have been handled earlier

    if (Sim::mainBusState == Enu::MemReadReady) { // we are performing a 2nd consecutive MemRead
        // do nothing - the memread is performed in the getMDRMuxOut fn
    }
    else if (Sim::mainBusState == Enu::MemWriteReady) {
        // we are performing a 2nd consecutive MemWrite
        int address = Sim::MARA * 256 + Sim::MARB;
        Sim::writeByte(address, Sim::MDR);
        emit writeByte(address);
    }

    // MARCk
    if (cpuPaneItems->MARCk->isChecked()) {
        quint8 a, b;
        if (getABusOut(a, errorString) && getBBusOut(b, errorString)) {
            setRegister(Enu::MARA, a);
            setRegister(Enu::MARB, b);
        }
        else {
            // error: MARCk is checked but we have incorrect input
            return false;
        }
    }

    // LoadCk
    if (cpuPaneItems->loadCk->isChecked()) {
        int cDest = cpuPaneItems->cLineEdit->text().toInt();
        quint8 out;
        if (cpuPaneItems->cLineEdit->text() == "") {
            errorString.append("No destination register specified for LoadCk.");
            return false;
        }
        if (getCMuxOut(out, errorString)) {
            setRegisterByte(cDest, out);
        }
        else {
            return false;
        }
    }

    // MDRCk
    if (cpuPaneItems->MDRCk->isChecked()) {
        quint8 out = 0;
        if (getMDRMuxOut(out, errorString)) {
            setRegister(Enu::MDR, out);
        }
        else {
            return false;
        }
    }

    if (aluFn == 15) {
        if (cpuPaneItems->NCkCheckBox->isChecked()) { // NCk
            setStatusBit(Enu::N, Enu::NMask & a);
        }
        if (cpuPaneItems->ZCkCheckBox->isChecked()) { // ZCk
            setStatusBit(Enu::Z, Enu::ZMask & a);
        }
        if (cpuPaneItems->VCkCheckBox->isChecked()) { // VCk
            setStatusBit(Enu::V, Enu::VMask & a);
        }
        if (cpuPaneItems->CCkCheckBox->isChecked()) { // CCk
            setStatusBit(Enu::C, Enu::CMask & a);
        }
    }
    else {
        // NCk
        if (cpuPaneItems->NCkCheckBox->isChecked()) {
            setStatusBit(Enu::N, result > 127);
        }

        // ZCk
        if (cpuPaneItems->ZCkCheckBox->isChecked()) {
            if (cpuPaneItems->ANDZTristateLabel->text() == ""){
                errorString.append("ZCk without ANDZ.");
                return false;
            }
            if (cpuPaneItems->ANDZTristateLabel->text() == "0") { // zOut from ALU goes straight through
                setStatusBit(Enu::Z, result == 0);
            }
            else if (cpuPaneItems->ANDZTristateLabel->text() == "1") { // zOut && zCurr
                setStatusBit(Enu::Z, result == 0 && Sim::zBit);
            }
        }

        // VCk
        if (cpuPaneItems->VCkCheckBox->isChecked()) {
            setStatusBit(Enu::V, overflow & 0x1);
        }

        // CCk
        if (cpuPaneItems->CCkCheckBox->isChecked()) {
            setStatusBit(Enu::C, carry & 0x1);
        }
    }

    return true;
}

void CpuPane::regTextEdited(QString str)
{
    qDebug() << "str: " << str;
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

    // Make sure the string isn't mangled
    if (str == "0") {
        str = "0x";
        lineEdit->setText(str);
    }
    else if (!str.startsWith("0x")) {
        str = str.toUpper();
        str.prepend("0x");
        lineEdit->setText(str);
    }
    else {
        str.remove(0, 2);
        str = str.toUpper();
        str.prepend("0x");
        int pos = lineEdit->cursorPosition();
        lineEdit->setText(str);
        lineEdit->setCursorPosition(pos);
    }

    // Get the hex value of the string
    int regValue = 0;
    str.remove(0, 2);
    if (str.length() > 0) {
        bool ok;
        regValue = str.toInt(&ok, 16);
    }
    else {
        // Exactly "0x" remains, so do nothing
        return;
    }

    if (lineEdit == cpuPaneItems->aRegLineEdit) {
        Sim::regBank[0] = (regValue & 0xFF00) / 256;
        Sim::regBank[1] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->xRegLineEdit) {
        Sim::regBank[2] = (regValue & 0xFF00) / 256;
        Sim::regBank[3] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->spRegLineEdit) {
        Sim::regBank[4] = (regValue & 0xFF00) / 256;
        Sim::regBank[5] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->pcRegLineEdit) {
        Sim::regBank[6] = (regValue & 0xFF00) / 256;
        Sim::regBank[7] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->irRegLineEdit) {
        Sim::regBank[8] = (regValue & 0xFF0000) / 65536;
        Sim::regBank[9] = (regValue & 0xFF00) / 256;
        Sim::regBank[10] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->t1RegLineEdit) {
        Sim::regBank[11] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->t2RegLineEdit) {
        Sim::regBank[12] = (regValue & 0xFF00) / 256;
        Sim::regBank[13] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->t3RegLineEdit) {
        Sim::regBank[14] = (regValue & 0xFF00) / 256;
        Sim::regBank[15] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->t4RegLineEdit) {
        Sim::regBank[16] = (regValue & 0xFF00) / 256;
        Sim::regBank[17] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->t5RegLineEdit) {
        Sim::regBank[18] = (regValue & 0xFF00) / 256;
        Sim::regBank[19] = regValue & 0xFF;
    }
    else if (lineEdit == cpuPaneItems->t6RegLineEdit) {
        Sim::regBank[20] = (regValue & 0xFF00) / 256;
        Sim::regBank[21] = regValue & 0xFF;
    }
}

void CpuPane::regTextFinishedEditing()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

    QString str = lineEdit->text();
    qDebug() << "str: " << str;

    // Get the hex value of the string
    int regValue = 0;
    bool ok;
    regValue = str.toInt(&ok, 16);

    qDebug() << "reg val: " << regValue;

    if (lineEdit == cpuPaneItems->irRegLineEdit) {
        lineEdit->setText(QString("0x%1").arg(regValue, 6, 16, QLatin1Char('0')).toUpper());
    }
    else if (lineEdit == cpuPaneItems->t1RegLineEdit) {
        lineEdit->setText(QString("0x") + QString("%1").arg(regValue, 2, 16, QLatin1Char('0')).toUpper());
    }
    else {
        lineEdit->setText(QString("0x") + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper());
    }

}

void CpuPane::zoomFactorChanged(int factor)
{
    QMatrix matrix;
    matrix.scale(factor * .01, factor * .01);
    ui->graphicsView->setMatrix(matrix);
    ui->graphicsView->update();
}

void CpuPane::labelClicked()
{
    TristateLabel *label = qobject_cast<TristateLabel *>(sender());
    label->toggle();

    Sim::nBit = cpuPaneItems->nBitLabel->text().toInt() == 0 ? false : true;
    Sim::zBit = cpuPaneItems->zBitLabel->text().toInt() == 0 ? false : true;
    Sim::vBit = cpuPaneItems->vBitLabel->text().toInt() == 0 ? false : true;
    Sim::cBit = cpuPaneItems->cBitLabel->text().toInt() == 0 ? false : true;
}

void CpuPane::clockButtonPushed()
{
    QString errorString;
    if (!step(errorString)) {
        // simulation had issues.
        QMessageBox::warning(0, "Pep/8", errorString);
        emit stopSimulation();
    }
    scene->invalidate();
    clearCpuControlSignals();
}

void CpuPane::singleStepButtonPushed()
{
    QString errorString = "";

    if (!step(errorString)) {
        // simulation had issues.
        QMessageBox::warning(0, "Pep/8", errorString);
        emit stopSimulation();

    }

    Sim::microProgramCounter++;

    if (Sim::atEndOfSim()) { // this should be detected on the previous step, but let's be defensive:
        emit simulationFinished();
        clearCpuControlSignals();
    }
    else {
        Sim::microCodeCurrentLine++;
        Code *code = Sim::codeList.at(Sim::microCodeCurrentLine);
        while (!code->isMicrocode() && !Sim::atEndOfSim()) {
            // iterate through the code list until we're at the end of the sim,
            // or until we're at another line of microcode
            Sim::microCodeCurrentLine++;
            code = Sim::codeList.at(Sim::microCodeCurrentLine);
        }
        if (!code->isMicrocode()) {
            // this will trigger if we're at the end of the simulation and have nothing more to execute
            emit simulationFinished();
            clearCpuControlSignals();
            scene->invalidate();
            return;
        }
        code->setCpuLabels(cpuPaneItems);
        emit updateSimulation();
    }

    scene->invalidate();

}

void CpuPane::resumeButtonPushed()
{

    QString errorString;
    bool finished = false;

    while (!finished) { // we set the flag to false when we're done with simulation, or have errors
        if (!step(errorString)) {
            // simulation had issues.
            QMessageBox::warning(0, "Pep/8", errorString);
            finished = true;
            emit stopSimulation();
//            emit simulationFinished();
            clearCpuControlSignals();
            return; // we'll just return here instead of letting it fail and go to the bottom
        }

        Sim::microProgramCounter++;

        if (Sim::atEndOfSim()) {
            finished = true; // this will fail the loop next time and go to the bottom
        }
        else {
            Sim::microCodeCurrentLine++;
            Code *code = Sim::codeList.at(Sim::microCodeCurrentLine);
            while (!code->isMicrocode() && !Sim::atEndOfSim()) {
                // iterate through the code list until we're at the end of the sim,
                // or we're at another line of microcode
                Sim::microCodeCurrentLine++;
                code = Sim::codeList.at(Sim::microCodeCurrentLine);
            }
            if (!code->isMicrocode()) {
                // this will trigger if we're at the end of the simulation and have nothing more to execute
                finished = true;
            }
            else {
                code->setCpuLabels(cpuPaneItems);
                emit updateSimulation();
            }
        }

        scene->invalidate();
    }

    emit simulationFinished();
    clearCpuControlSignals();

}

void CpuPane::on_copyToMicrocodePushButton_clicked()
{
    MicroCode code;
    if (cpuPaneItems->loadCk->isChecked()) {
        code.set(Enu::LoadCk, 1);
    }
    if (cpuPaneItems->cLineEdit->text() != "") {
        code.set(Enu::C, cpuPaneItems->cLineEdit->text().toInt());
    }
    if (cpuPaneItems->bLineEdit->text() != "") {
        code.set(Enu::B, cpuPaneItems->bLineEdit->text().toInt());
    }
    if (cpuPaneItems->aLineEdit->text() != "") {
        code.set(Enu::A, cpuPaneItems->aLineEdit->text().toInt());
    }
    if (cpuPaneItems->MARCk->isChecked()) {
        code.set(Enu::MARCk, 1);
    }
    if (cpuPaneItems->MDRCk->isChecked()) {
        code.set(Enu::MDRCk, 1);
    }
    if (cpuPaneItems->aMuxTristateLabel->text() != "") {
        code.set(Enu::AMux, cpuPaneItems->aMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->MDRMuxTristateLabel->text() != "") {
        code.set(Enu::MDRMux, cpuPaneItems->MDRMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->cMuxTristateLabel->text() != "") {
        code.set(Enu::CMux, cpuPaneItems->cMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->ALULineEdit->text() != "") {
        code.set(Enu::ALU, cpuPaneItems->ALULineEdit->text().toInt());
    }
    if (cpuPaneItems->CCkCheckBox->isChecked()) {
        code.set(Enu::CCk, 1);
    }
    if (cpuPaneItems->VCkCheckBox->isChecked()) {
        code.set(Enu::VCk, 1);
    }
    if (cpuPaneItems->ANDZTristateLabel->text() != "") {
        code.set(Enu::ANDZ, cpuPaneItems->ANDZTristateLabel->text().toInt());
    }
    if (cpuPaneItems->ZCkCheckBox->isChecked()) {
        code.set(Enu::ZCk, 1);
    }
    if (cpuPaneItems->NCkCheckBox->isChecked()) {
        code.set(Enu::NCk, 1);
    }
    if (cpuPaneItems->MemReadTristateLabel->text() != "") {
        code.set(Enu::MemRead, cpuPaneItems->MemReadTristateLabel->text().toInt());
    }
    if (cpuPaneItems->MemWriteTristateLabel->text() != "") {
        code.set(Enu::MemWrite, cpuPaneItems->MemWriteTristateLabel->text().toInt());
    }
    emit appendMicrocodeLine(code.getSourceCode());
}

void CpuPane::ALUTextEdited(QString str)
{
    if (str == "") {
        cpuPaneItems->ALUFunctionLabel->setText("");
    }
    else {
        int num = str.toInt();
        switch (num) {
        case 0:
            cpuPaneItems->ALUFunctionLabel->setText("A");
            break;
        case 1:
            cpuPaneItems->ALUFunctionLabel->setText("A plus B");
            break;
        case 2:
            cpuPaneItems->ALUFunctionLabel->setText("A plus B plus Cin");
            break;
        case 3:
            cpuPaneItems->ALUFunctionLabel->setText("A plus \xAC B plus 1");
            break;
        case 4:
            cpuPaneItems->ALUFunctionLabel->setText("A plus \xAC B plus Cin");
            break;
        case 5:
            cpuPaneItems->ALUFunctionLabel->setText("A \xb7 B");
            break;
        case 6:
            cpuPaneItems->ALUFunctionLabel->setText("\xAC (A \xb7 B)");
            break;
        case 7:
            cpuPaneItems->ALUFunctionLabel->setText("A + B");
            break;
        case 8:
            cpuPaneItems->ALUFunctionLabel->setText("\xAC (A + B)");
            break;
        case 9:
            cpuPaneItems->ALUFunctionLabel->setText("A XOR B");
            break;
        case 10:
            cpuPaneItems->ALUFunctionLabel->setText("\xAC A");
            break;
        case 11:
            cpuPaneItems->ALUFunctionLabel->setText("ASL A");
            break;
        case 12:
            cpuPaneItems->ALUFunctionLabel->setText("ROL A");
            break;
        case 13:
            cpuPaneItems->ALUFunctionLabel->setText("ASR A");
            break;
        case 14:
            cpuPaneItems->ALUFunctionLabel->setText("ROR A");
            break;
        case 15:
            cpuPaneItems->ALUFunctionLabel->setText("NZVC A");
            break;
        default:
            break;
        }
    }
}

bool CpuPane::getALUOut(quint8 &result, quint8& a, quint8& b, int& carry, int& overflow, QString &errorString)
{
    a = 0;
    b = 0;
    int output = 0;
    carry = 0;
    overflow = 0;

    if (cpuPaneItems->ALULineEdit->text() == "") {
        errorString.append("ALU function not specified.\n");
        return false;
    }

    int ALUFn;
    ALUFn = cpuPaneItems->ALULineEdit->text().toInt();

    if (!isCorrectALUInput(ALUFn)) {
//        qDebug() << "Incorrect or no ALU input";
        errorString.append("Incorrect or no ALU input.\n");
        return false;
    }

    switch(ALUFn) {
    case 0: // A
        if (getAMuxOut(a, errorString)) {
            output = a;
            b = 0;
            result = output;
        }
        break;
    case 1: // A plus B
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = a + b;
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + (b & 0x7f)) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 2: // A plus B plus Cin
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = a + b + !!Sim::cBit;
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + (b & 0x7f) + !!Sim::cBit) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 3: // A plus ~B plus 1
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = a + ((~b) & 0xff) + 1;
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + ((~b) & 0x7f) + 1) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 4: // A plus ~B plus Cin
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = a + ((~b) & 0xff) + !!Sim::cBit;
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + ((~b) & 0x7f) + !!Sim::cBit) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 5: // A and B
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = a & b;
            result = output;
        }
        break;
    case 6: // ~(A and B)
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = ~(a & b) & 0xff;
            result = output;
        }
        break;
    case 7: // A + B
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = a | b;
            result = output;
        }
        break;
    case 8: // ~(A + B)
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = ~(a | b);
            result = output;
        }
        break;
    case 9: // A xor B
        if (getAMuxOut(a, errorString) && getBBusOut(b, errorString)) {
            output = (a ^ b) & 0xff;
            result = output;
        }
        break;
    case 10: // ~A
        if (getAMuxOut(a, errorString)) {
            output = ~a;
            result = output;
        }
        break;
    case 11: // ASL A
        if (getAMuxOut(a, errorString)) {
            output = (a << 1) & 0xfe; // 0xfe because 0 gets shifted in
            carry = (a & 0x80) >> 7;
            overflow = ((a & 0x40) >> 6) ^ carry;
            result = output;
        }
        break;
    case 12: // ROL A
        if (getAMuxOut(a, errorString)) {
            output = ((a << 1) & 0xfe) + !!Sim::cBit;
            carry = (a & 0x80) >> 7;
            result = output;
        }
        break;
    case 13: // ASR A
        if (getAMuxOut(a, errorString)) {
            output = ((a >> 1) & 0x7f) | (a & 0x80);
            carry = a & 1;
            result = output;
        }
        break;
    case 14: // ROR A
        if (getAMuxOut(a, errorString)) {
            output = ((a >> 1) & 0x7f) | (!!Sim::cBit << 7);
            carry = a & 1;
            result = output;
        }
        break;
    case 15: // NZVC A
        if (getAMuxOut(a, errorString)) {
            result = 0;
        }
        break;
    default:
        return false;
    }

    return true;
}

bool CpuPane::isCorrectALUInput(int ALUFn) {
    bool abus = false;
    bool bbus = false;

    if (cpuPaneItems->aMuxTristateLabel->text() == "") {
        abus = false;
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "0") {
        abus = true;
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "1") {
        if (cpuPaneItems->aLineEdit->text() == "") {
            abus = false;
        }
        else {
            abus = true;
        }
    }

    if (cpuPaneItems->bLineEdit->text() == "") {
        bbus = false;
    }
    else {
        bbus = true;
    }

    switch(ALUFn) {
    case 0:
        if (!abus) {
            return false;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (!abus || !bbus) {
            return false;
        }
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        if (!abus) {
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

bool CpuPane::getCMuxOut(quint8 &out, QString &errorString)
{
    if (cpuPaneItems->cMuxTristateLabel->text() == "0") {
        out = (Sim::nBit ? 8 : 0) + (Sim::zBit ? 4 : 0) + (Sim::vBit ? 2 : 0) + (!!Sim::cBit);
        // qDebug() << QString("0x%1").arg(out, 4, 16, QLatin1Char('0'));
        return true;
    }
    else if (cpuPaneItems->cMuxTristateLabel->text() == "1") {
        quint8 a, b;
        int carry, overflow;
        return getALUOut(out, a, b, carry, overflow, errorString);
    }
    else {
        errorString.append("CMux control signal not specified.\n");
    }
    return false;
}

bool CpuPane::getAMuxOut(quint8 &out, QString &errorString)
{
    if (cpuPaneItems->aMuxTristateLabel->text() == "0") {
        out = Sim::MDR;
        return true;
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "1") {
        if (getABusOut(out, errorString)) {
            return true;
        }
        else {
            // Error string will be populated with the correct error
        }
    }
    else {
        errorString.append("Nothing on A bus.\n");
    }
    return false;
}

bool CpuPane::getMDRMuxOut(quint8 &out, QString &errorString)
{
    if (cpuPaneItems->MDRMuxTristateLabel->text() == "0") {
        if (Sim::mainBusState == Enu::MemReadReady) {
            // perform a memRead
            int address = Sim::MARA * 256 + Sim::MARB;
            out = Sim::readByte(address);
            emit readByte(address);
            return true;
        }
        else {
            errorString.append("Not ready for memread.\n");
        }
    }
    else if (cpuPaneItems->MDRMuxTristateLabel->text() == "1") {
        if (getCMuxOut(out, errorString)) {
            return true;
        }
    }
    else {
        errorString.append("MDRCk is checked, but MDRMux isn't set.\n");
    }
    return false;
}

bool CpuPane::getABusOut(quint8 &out, QString &errorString)
{
    if (cpuPaneItems->aLineEdit->text() != "") {
        out = Sim::regBank[cpuPaneItems->aLineEdit->text().toInt()];
        return true;
    }
    else {
        errorString.append("Nothing on A bus.\n");
    }
    return false;
}

bool CpuPane::getBBusOut(quint8 &out, QString &errorString)
{
    if (cpuPaneItems->bLineEdit->text() != "") {
        out = Sim::regBank[cpuPaneItems->bLineEdit->text().toInt()];
        return true;
    }
    else {
        errorString.append("Nothing on B bus.\n");
    }
    return false;
}

void CpuPane::run()
{
    // Run; these are really equivalent:
    resumeButtonPushed();
}
