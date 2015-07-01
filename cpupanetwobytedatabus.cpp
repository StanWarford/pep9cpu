// File: cpupanetwobytedatabus.cpp
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

#include "cpupanetwobytedatabus.h"
#include "ui_cpupane.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QGraphicsItem>
#include <QErrorMessage>
#include <QMessageBox>

#include "tristatelabel.h"
#include "pep.h"
#include "code.h"
#include "sim.h"

#include "cpupanebasegraphicsitems.h"

#include <QDebug>

CpuPaneTwoByteDataBus::CpuPaneTwoByteDataBus(QWidget *parent) :
    //CpuPane(parent),
    ui(new Ui::CpuPane)
{

}

void CpuPaneTwoByteDataBus::updateMainBusState()
{
    bool marChanged = false;
    if (cpuPaneItems->MARCk->isChecked()) {
        quint8 mara, marb;
        // temporary, any errors here will be caught in the MARCk section of step():
        QString errorString;
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
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead (1st)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (1st)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        //else: mainBusState = None, but it already is.
        break;
    case Enu::MemReadFirstWait:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead (2nd with unchanged MAR)
            Sim::mainBusState = Enu::MemReadSecondWait;
        }
        else if (marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead
            // do nothing, already MemReadfirstWait - need another MemRead
            //  because the MAR changed
            qDebug() << "MAR changed - don't read yet";
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (after a single MemRead)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemReadSecondWait:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead (3rd with unchanged MAR)
            Sim::mainBusState = Enu::MemReadReady;
        }
        else if (marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (after 2 MemReads)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemReadReady:
        if (!marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead again (more than 3 in a row)
            // do nothing, already MemReadReady
        }
        else if (marChanged && cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead
            Sim::mainBusState = Enu::MemReadFirstWait;
            // Go back to MemReadFirstWait because the MAR changed
        }
        else if (cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (after 3+ MemReads)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteFirstWait:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead (after a MemWrite)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (2nd in a row)
            Sim::mainBusState = Enu::MemWriteSecondWait;
        }
        else if (marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (with changed MAR)
            // Do nothing, MAR changed, still MemWriteSecondWait
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteSecondWait:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead (after 2 MemWrites)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (2nd in a row)
            Sim::mainBusState = Enu::MemWriteReady;
        }
        else if (marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (with changed MAR)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    case Enu::MemWriteReady:
        if (cpuPaneItems->MemReadTristateLabel->text() == "1") {
            // MemRead (after 3+ MemWrites)
            Sim::mainBusState = Enu::MemReadFirstWait;
        }
        else if (!marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (after 3+ in a row)
            // do nothing, already MemWriteReady
        }
        else if (marChanged && cpuPaneItems->MemWriteTristateLabel->text() == "1") {
            // MemWrite (with changed MAR)
            Sim::mainBusState = Enu::MemWriteFirstWait;
        }
        else {
            Sim::mainBusState = Enu::None;
        }
        break;
    default:
        Sim::mainBusState = Enu::None;
        // Just in case the Sim::mBS is malformed somehow
        break;
    }
}

bool CpuPaneTwoByteDataBus::step(QString &errorString)
{
    // Clear modified bytes for simulation view:
    Sim::modifiedBytes.clear();

    // Update Bus State
    // FSM that sets Sim::mainBusState to Enu::BusState - 5 possible states
    updateMainBusState();

    // Status bit calculations
    int aluFn = cpuPaneItems->ALULineEdit->text().toInt();
    int carry;
    int overflow;
    quint8 result, a, b;

    QString errtemp;
    getALUOut(result, a, b, carry, overflow, errtemp); // ignore boolean returned - error would have been handled earlier

    if (Sim::mainBusState == Enu::MemReadReady) {
        // we are performing a 2nd consecutive MemRead
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
        if (getMARMuxOut(a, b, errorString)) {
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

    // MDROCk
    if (cpuPaneItems->MDROCk->isChecked()) {
        quint8 out = 0;
        if (getMDROMuxOut(out, errorString)) {
            setRegister(Enu::MDRO, out);
        }
        else {
            return false;
        }
    }

    // MDRECk
    if (cpuPaneItems->MDRECk->isChecked()) {
        quint8 out = 0;
        if (getMDREMuxOut(out, errorString)) {
            setRegister(Enu::MDRE, out);
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
            if (cpuPaneItems->AndZTristateLabel->text() == ""){
                errorString.append("ZCk without AndZ.");
                return false;
            }
            if (cpuPaneItems->AndZTristateLabel->text() == "0") { // zOut from ALU goes straight through
                setStatusBit(Enu::Z, result == 0);
            }
            else if (cpuPaneItems->AndZTristateLabel->text() == "1") { // zOut && zCurr
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

        // SCk
        if (cpuPaneItems->SCkCheckBox->isChecked()) {
            setStatusBit(Enu::S, carry & 0x1);
        }
    }

    return true;
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

bool CpuPaneTwoByteDataBus::getMARMuxOut(quint8& mara, quint8 &marb,
                                         QString& errorString)
{
    if (cpuPaneItems->MARMuxTristateLabel->text() == "0") {
        if (getMDREMuxOut(mara, errorString)
                && getMDROMuxOut(marb, errorString)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else if (cpuPaneItems->MARMuxTristateLabel->text() == "1") {
        if (getABusOut(mara, errorString)
                && getBBusOut(marb, errorString)) {
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
    if (cpuPaneItems->EOMuxTristateLabel->text() == "1") {
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

