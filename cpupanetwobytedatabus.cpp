// File: cpupanetwobytedatabus.cpp
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
#include "cpupanetwobytedatabus.h"
#include "ui_cpupane.h"

#include "pep.h"
#include "code.h"
#include "sim.h"

#include <QDebug>

CpuPaneTwoByteDataBus::CpuPaneTwoByteDataBus(QWidget *parent) :
    CpuPane(parent),
    ui(new Ui::CpuPane)
{
    ui->setupUi(this);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(zoomFactorChanged(int)));

    cpuPaneItems = NULL;
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setFont(QFont(Pep::cpuFont, Pep::cpuFontSize));

    initModel();

    ui->spinBox->hide();
    ui->singleStepPushButton->setEnabled(false);

}

void CpuPaneTwoByteDataBus::initModel()
{
    if (cpuPaneItems != NULL) {
        // disconnect all signals from this object before deleting it:
#warning todo: delete all signals from object before deletion
        delete cpuPaneItems;
    }

#warning todo: fix for the two byte model
    cpuPaneItems = new CpuPaneBaseGraphicsItems(ui->graphicsView, 0, scene);
    ui->graphicsView->scene()->addItem(cpuPaneItems);

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    connect(cpuPaneItems->loadCk, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->cLineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));
    connect(cpuPaneItems->bLineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));
    connect(cpuPaneItems->aLineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MARCk, SIGNAL(clicked()), scene, SLOT(invalidate()));
    //connect(cpuPaneItems->MDRCk, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MDROCk, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MDRECk, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->aMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->aMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    //connect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    //connect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MDROMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->MDROMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->MDREMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->MDREMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->EOMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->EOMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->MARMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->MARMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->cMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->cMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->ALULineEdit, SIGNAL(textChanged(QString)), scene, SLOT(invalidate()));

    connect(cpuPaneItems->CSMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->CSMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->SCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->CCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));
    connect(cpuPaneItems->VCkCheckBox, SIGNAL(clicked()), scene, SLOT(invalidate()));

    connect(cpuPaneItems->AndZTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    connect(cpuPaneItems->AndZTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

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
    connect(cpuPaneItems->sBitLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));

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


    // disconnect all cpuPaneItems that differ between models:
    disconnect(cpuPaneItems->MDRCk, SIGNAL(clicked()), scene, SLOT(invalidate()));

    disconnect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
    disconnect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));

    if (Pep::cpuFeatures == Enu::OneByteDataBus) {
        // connect items for this model
        connect(cpuPaneItems->MDRCk, SIGNAL(clicked()), scene, SLOT(invalidate()));

        connect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), this, SLOT(labelClicked()));
        connect(cpuPaneItems->MDRMuxTristateLabel, SIGNAL(clicked()), scene, SLOT(invalidate()));
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        // connect items for *this* model
#warning: todo, once the cpuPaneItems has been updated to include the new items.
    }

}

void CpuPaneTwoByteDataBus::clearCpu()
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
    setRegister(Enu::MDRE, 0);
    setRegister(Enu::MDRO, 0);

    setStatusBit(Enu::S, false);
    setStatusBit(Enu::C, false);
    setStatusBit(Enu::V, false);
    setStatusBit(Enu::Z, false);
    setStatusBit(Enu::N, false);

}

void CpuPaneTwoByteDataBus::clearCpuControlSignals()
{
    cpuPaneItems->loadCk->setChecked(false);
    cpuPaneItems->cLineEdit->setText("");
    cpuPaneItems->bLineEdit->setText("");
    cpuPaneItems->aLineEdit->setText("");
    cpuPaneItems->MARCk->setChecked(false);
    cpuPaneItems->MDRECk->setChecked(false);
    cpuPaneItems->MDROCk->setChecked(false);
    cpuPaneItems->aMuxTristateLabel->setText("");
    cpuPaneItems->MDROMuxTristateLabel->setText("");
    cpuPaneItems->MDREMuxTristateLabel->setText("");
    cpuPaneItems->EOMuxTristateLabel->setText("");
    cpuPaneItems->MARMuxTristateLabel->setText("");
    cpuPaneItems->cMuxTristateLabel->setText("");
    cpuPaneItems->ALULineEdit->setText("");
    cpuPaneItems->CSMuxTristateLabel->setText("");
    cpuPaneItems->SCkCheckBox->setChecked(false);
    cpuPaneItems->CCkCheckBox->setChecked(false);
    cpuPaneItems->VCkCheckBox->setChecked(false);
    cpuPaneItems->AndZTristateLabel->setText("");
    cpuPaneItems->ZCkCheckBox->setChecked(false);
    cpuPaneItems->NCkCheckBox->setChecked(false);
    cpuPaneItems->MemReadTristateLabel->setText("");
    cpuPaneItems->MemWriteTristateLabel->setText("");
}

void CpuPaneTwoByteDataBus::updateMainBusState()
{
    bool marChanged = false;
    if (cpuPaneItems->MARCk->isChecked()) {
        quint8 mara, marb;
        QString errorString; // temporary, any errors here will be caught in the MARCk section of step()
        if (getMARMuxOut(mara, marb, errorString)) {
            marChanged = (mara != Sim::MARA) || (marb != Sim::MARB);
        }
        else {
            // error: MARCk is checked but we have incorrect input.
            // This will be caught in step() under the MARCk section
            // For this function, we're happy to leave marChanged as false.
        }
    }

    switch (Sim::mainBusState) {
    case Enu::None:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (1st)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (1st)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        //else: mainBusState = None, but it already is.
        break;
    case Enu::MemReadFirstWait:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (2nd with unchanged MAR)
            Sim::mainBusState = Enu::MemReadSecondWait;
        }
        else if (marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead
            // do nothing, already MemReadfirstWait - need another MemRead because the MAR changed
            qDebug() << "MAR changed - don't read yet";
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after a single MemRead)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemReadSecondWait:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (3rd with unchanged MAR)
            Sim::mainBusState = Enu::MemReadReady;
        }
        else if (marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after 2 MemReads)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemReadReady:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead again (more than 3 in a row)
            // do nothing, already MemReadReady
        }
        else if (marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead
            Sim::mainBusState = Enu::MemReadFirstWait; // Go back to MemReadFirstWait because the MAR changed
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after 3+ MemReads)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteFirstWait:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (after a MemWrite)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (2nd in a row)
            Sim::mainBusState = Enu::MemWriteSecondWait;
        }
        else if (marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (with changed MAR)
            // Do nothing, MAR changed, still MemWriteSecondWait
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteSecondWait:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (after 2 MemWrites)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (2nd in a row)
            Sim::mainBusState = Enu::MemWriteReady;
        }
        else if (marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (with changed MAR)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteReady:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") { // MemRead (after 3+ MemWrites)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (after 3+ in a row)
            // do nothing, already MemWriteReady
        }
        else if (marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") { // MemWrite (with changed MAR)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    default:
        Sim::mainBusState = Enu::None; // Just in case the Sim::mBS is malformed somehow
        break;
    }
}

#warning todo: edit for two byte bus version
bool CpuPaneTwoByteDataBus::step(QString &errorString)
{
    return true;
}

void CpuPaneTwoByteDataBus::on_copyToMicrocodePushButton_clicked()
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
    if (cpuPaneItems->MDRECk->isChecked()) {
        code.set(Enu::MDRCk, 1);
    }
    if (cpuPaneItems->MDROCk->isChecked()) {
        code.set(Enu::MDRCk, 1);
    }
    if (cpuPaneItems->aMuxTristateLabel->text() != "") {
        code.set(Enu::AMux, cpuPaneItems->aMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->MDROMuxTristateLabel->text() != "") {
        code.set(Enu::MDRMux, cpuPaneItems->MDROMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->MDREMuxTristateLabel->text() != "") {
        code.set(Enu::MDRMux, cpuPaneItems->MDREMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->EOMuxTristateLabel->text() != "") {
        code.set(Enu::MDRMux, cpuPaneItems->EOMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->MARMuxTristateLabel->text() != "") {
        code.set(Enu::MDRMux, cpuPaneItems->MARMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->cMuxTristateLabel->text() != "") {
        code.set(Enu::CMux, cpuPaneItems->cMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->ALULineEdit->text() != "") {
        code.set(Enu::ALU, cpuPaneItems->ALULineEdit->text().toInt());
    }
    if (cpuPaneItems->CSMuxTristateLabel->text() != "") {
        code.set(Enu::CMux, cpuPaneItems->CSMuxTristateLabel->text().toInt());
    }
    if (cpuPaneItems->SCkCheckBox->isChecked()) {
        code.set(Enu::SCk, 1);
    }
    if (cpuPaneItems->CCkCheckBox->isChecked()) {
        code.set(Enu::CCk, 1);
    }
    if (cpuPaneItems->VCkCheckBox->isChecked()) {
        code.set(Enu::VCk, 1);
    }
    if (cpuPaneItems->AndZTristateLabel->text() != "") {
        code.set(Enu::AndZ, cpuPaneItems->AndZTristateLabel->text().toInt());
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

bool CpuPaneTwoByteDataBus::getAMuxOut(quint8& out, QString& errorString)
{
    if (cpuPaneItems->aMuxTristateLabel->text() == "0") {
        //out = Sim::MDR;
        if (getEOMuxOut(out, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "1") {
        if (getABusOut(out, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else {
        errorString.append("Nothing on A bus.\n");
    }
    return false;
}

#warning todo: do.
bool CpuPaneTwoByteDataBus::getMARMuxOut(quint8& mara, quint8 &marb, QString& errorString)
{
    if (cpuPaneItems->MARMuxTristateLabel->text() == "0") {
        if (getMDREMuxOut(mara, errorString) && getMDROMuxOut(marb, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else if (cpuPaneItems->MARMuxTristateLabel->text() == "1") {
        if (getABusOut(mara, errorString) && getBBusOut(marb, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else {
        errorString.append("MARMux control signal not specified.\n");
    }
    return false;
}

bool CpuPaneTwoByteDataBus::getMDROMuxOut(quint8& out, QString& errorString)
{
    if (cpuPaneItems->MDROMuxTristateLabel->text() == "0") {
        if (Sim::mainBusState == Enu::MemReadReady) {
            // perform a memRead
            // align the address to even number, as per pg. 614 fig 12.14
            int address = (Sim::MARA * 256 + Sim::MARB) & 0xFFFE;
            // address is odd (MDRO):
            address++;
            out = Sim::readByte(address);
            emit readByte(address);
            return true;
        }
        else {
            errorString.append("Not ready for memread.\n");
        }
    }
    else if (cpuPaneItems->MDROMuxTristateLabel->text() == "1") {
        if (getCMuxOut(out, errorString)) {
            return true;
        }
    }
    else {
        errorString.append("MDROCk is checked, but MDROMux isn't set.\n");
    }
    return false;
}

bool CpuPaneTwoByteDataBus::getMDREMuxOut(quint8& out, QString& errorString)
{
    if (cpuPaneItems->MDREMuxTristateLabel->text() == "0") {
        if (Sim::mainBusState == Enu::MemReadReady) {
            // perform a memRead
            // align the address to even number, as per pg. 614 fig 12.14
            int address = (Sim::MARA * 256 + Sim::MARB) & 0xFFFE;
            out = Sim::readByte(address);
            emit readByte(address);
            return true;
        }
        else {
            errorString.append("Not ready for memread.\n");
        }
    }
    else if (cpuPaneItems->MDREMuxTristateLabel->text() == "1") {
        if (getCMuxOut(out, errorString)) {
            return true;
        }
    }
    else {
        errorString.append("MDRECk is checked, but MDREMux isn't set.\n");
    }
    return false;
}

bool CpuPaneTwoByteDataBus::getEOMuxOut(quint8& out, QString& errorString)
{
    if (cpuPaneItems->EOMuxTristateLabel->text() == "0") {
        if (getMDREMuxOut(out, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "1") {
        if (getMDROMuxOut(out, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else {
        errorString.append("EOMux control signal not specified.\n");
    }
    return false;
}

