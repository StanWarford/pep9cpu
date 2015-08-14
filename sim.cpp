// File: sim.cpp
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

#include "sim.h"
#include "enu.h"
#include "pep.h"

#include "cpugraphicsitems.h"

#include <QDebug>

using namespace Enu;


QVector<quint8> Sim::Mem(65536);
bool Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit, Sim::sBit;

QVector<quint8> Sim::regBank(32);

quint8 Sim::MARA    = 0;
quint8 Sim::MARB    = 0;
quint8 Sim::MDR     = 0;
quint8 Sim::MDROdd  = 0;
quint8 Sim::MDREven = 0;

Enu::MainBusState Sim::mainBusState;

QSet<int> Sim::modifiedBytes;

QList<Code *> Sim::codeList;
int Sim::cycleCount; // used to store the number of cycles in a simulation
int Sim::microProgramCounter;
int Sim::microCodeCurrentLine;

QStringList Sim::microcodeSourceList;
void Sim::setMicrocodeSourceList() {
    Sim::microcodeSourceList.clear();
    for (int i = 0; i < Sim::codeList.length(); i++) {
        Sim::microcodeSourceList.append(Sim::codeList.at(i)->getSourceCode());
    }
}

int Sim::readByte(int memAddr)
{
    return Mem[memAddr & 0xffff];
}

void Sim::writeByte(int memAddr, int value)
{
    qDebug() << "Wrote byte " << memAddr << " with value " << QString("0x%1").arg(value, 4, 16, QLatin1Char('0'));

    Mem[memAddr & 0xffff] = value;
    modifiedBytes.insert(memAddr & 0xffff);
}

bool Sim::aluFnIsUnary(int fn)
{
    switch (fn) {
    case 0:
        return true;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return false;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return true;
    default:
        break;
    }
    return false;
}

bool Sim::atEndOfSim()
{
    return microProgramCounter >= cycleCount;
    // we use this because of special cases with
    // some simulations being very short (2 lines in particular).
}

void Sim::initMRegs()
{
    regBank[0]  = 0x00;
    regBank[1]  = 0x00;
    regBank[2]  = 0x00;
    regBank[3]  = 0x00;
    regBank[4]  = 0x00;
    regBank[5]  = 0x00;
    regBank[6]  = 0x00;
    regBank[7]  = 0x00;
    regBank[8]  = 0x00;
    regBank[9]  = 0x00;
    regBank[11] = 0x00;
    regBank[12] = 0x00;
    regBank[13] = 0x00;
    regBank[14] = 0x00;
    regBank[15] = 0x00;
    regBank[16] = 0x00;
    regBank[17] = 0x00;
    regBank[18] = 0x00;
    regBank[19] = 0x00;
    regBank[20] = 0x00;
    regBank[21] = 0x00;
    regBank[22] = 0x00;
    regBank[23] = 0x01;
    regBank[24] = 0x02;
    regBank[25] = 0x03;
    regBank[26] = 0x04;
    regBank[27] = 0x08;
    regBank[28] = 0xF0;
    regBank[29] = 0xF6;
    regBank[30] = 0xFE;
    regBank[31] = 0xFF;
}

void Sim::clearMemory()
{
    for (int i = 0; i < 65536; i++) {
        Mem[i] = 0;
    }
}

void Sim::initNZVCS()
{
    nBit = false;
    zBit = false;
    vBit = false;
    cBit = false;
    sBit = false;
}

void Sim::initCPUState()
{
    mainBusState = Enu::None;
    modifiedBytes.clear();
    MARA    = 0;
    MARB    = 0;
    MDR     = 0;
    MDROdd  = 0;
    MDREven = 0;

}

// new stuff
bool Sim::testRegPostcondition(Enu::EMnemonic reg, int value) {
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

bool Sim::testStatusPostcondition(Enu::EMnemonic bit, bool value) {
    switch (bit) {
    case Enu::N:
        return Sim::nBit == value;
    case Enu::Z:
        return Sim::zBit == value;
    case Enu::V:
        return Sim::vBit == value;
    case Enu::C:
        return Sim::cBit == value;
    case Enu::S:
        return Sim::sBit == value;
    default:
        break;
    }
    return true;
}

bool Sim::getABusOut(quint8 &out, QString &errorString,
                     CpuGraphicsItems *cpuPaneItems)
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

bool Sim::getBBusOut(quint8 &out, QString &errorString,
                     CpuGraphicsItems *cpuPaneItems)
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

bool Sim::isCorrectALUInput(int ALUFn, CpuGraphicsItems *cpuPaneItems) {
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        return OneByteModel::isCorrectALUInput(ALUFn, cpuPaneItems);
        break;
    case Enu::TwoByteDataBus:
        return TwoByteModel::isCorrectALUInput(ALUFn, cpuPaneItems);
        break;
    default:
        break;
    }
    qDebug() << "CPU model not specified in isCorrectALUInput.";
    return false;
}

bool Sim::getALUOut(quint8 &result, quint8& a, quint8& b, int& carry,
                    int& overflow, QString &errorString,
                    CpuGraphicsItems *cpuPaneItems)
{
    a = 0;
    b = 0;
    bool cin = false;
    int output = 0;
    carry = 0;
    overflow = 0;

    if (cpuPaneItems->ALULineEdit->text() == "") {
        errorString.append("ALU function not specified.\n");
        return false;
    }

    int ALUFn;
    ALUFn = cpuPaneItems->ALULineEdit->text().toInt();

    if (!Sim::isCorrectALUInput(ALUFn, cpuPaneItems)) {
//        qDebug() << "Incorrect or no ALU input";
        errorString.append("Incorrect or no ALU input.\n");
        return false;
    }

    switch(ALUFn) {
    case 0: // A
        if (getAMuxOut(a, errorString, cpuPaneItems)) {
            output = a;
            b = 0;
            result = output;
        }
        break;
    case 1: // A plus B
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = a + b;
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + (b & 0x7f)) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 2: // A plus B plus Cin
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems) && getCSMuxOut(cin, errorString, cpuPaneItems)) {
            output = a + b + (cin ? 1 : 0);
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + (b & 0x7f) + (cin ? 1 : 0)) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 3: // A plus ~B plus 1
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = a + ((~b) & 0xff) + 1;
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + ((~b) & 0x7f) + 1) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 4: // A plus ~B plus Cin
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems) && getCSMuxOut(cin, errorString, cpuPaneItems)) {
            output = a + ((~b) & 0xff) + (cin ? 1 : 0);
            carry = ((output & 0x1ff) >> 8) & 0x1;
            overflow = ((((a & 0x7f) + ((~b) & 0x7f) + (cin ? 1 : 0)) >> 7) & 0x1) ^ carry;
            result = output;
        }
        break;
    case 5: // A and B
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = a & b;
            result = output;
        }
        break;
    case 6: // ~(A and B)
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = ~(a & b) & 0xff;
            result = output;
        }
        break;
    case 7: // A + B
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = a | b;
            result = output;
        }
        break;
    case 8: // ~(A + B)
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = ~(a | b);
            result = output;
        }
        break;
    case 9: // A xor B
        if (getAMuxOut(a, errorString, cpuPaneItems) && getBBusOut(b, errorString, cpuPaneItems)) {
            output = (a ^ b) & 0xff;
            result = output;
        }
        break;
    case 10: // ~A
        if (getAMuxOut(a, errorString, cpuPaneItems)) {
            output = ~a;
            result = output;
        }
        break;
    case 11: // ASL A
        if (getAMuxOut(a, errorString, cpuPaneItems)) {
            output = (a << 1) & 0xfe; // 0xfe because 0 gets shifted in
            carry = (a & 0x80) >> 7;
            overflow = ((a & 0x40) >> 6) ^ carry;
            result = output;
        }
        break;
    case 12: // ROL A
        if (getAMuxOut(a, errorString, cpuPaneItems) && getCSMuxOut(cin, errorString, cpuPaneItems)) {
            output = ((a << 1) & 0xfe) | (cin ? 1 : 0);
            carry = (a & 0x80) >> 7;
            overflow = ((a & 0x40) >> 6) ^ carry;
            result = output;
        }
        break;
    case 13: // ASR A
        if (getAMuxOut(a, errorString, cpuPaneItems)) {
            output = ((a >> 1) & 0x7f) | (a & 0x80);
            carry = a & 1;
            result = output;
        }
        break;
    case 14: // ROR A
        if (getAMuxOut(a, errorString, cpuPaneItems) && getCSMuxOut(cin, errorString, cpuPaneItems)) {
            output = ((a >> 1) & 0x7f) | ((cin ? 1 : 0) << 7);
            carry = a & 1;
            result = output;
        }
        break;
    case 15: // NZVC A
        if (getAMuxOut(a, errorString, cpuPaneItems)) {
            result = 0;
        }
        break;
    default:
        return false;
    }

    return true;
}

bool Sim::getCSMuxOut(bool &out, QString &errorString,
                      CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->CSMuxTristateLabel->text() == "0") {
        out = Sim::cBit;
        return true;
    }
    else if (cpuPaneItems->CSMuxTristateLabel->text() == "1") {
        out = Sim::sBit;
        return true;
    }
    else {
        errorString.append("CSMux control signal not specified.\n");
    }
    return false;
}

bool Sim::getCMuxOut(quint8 &out, QString &errorString,
                     CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->cMuxTristateLabel->text() == "0") {
        out = (Sim::nBit ? 8 : 0) + (Sim::zBit ? 4 : 0) + (Sim::vBit ? 2 : 0) + (Sim::cBit ? 1 : 0);
        // qDebug() << QString("0x%1").arg(out, 4, 16, QLatin1Char('0'));
        return true;
    }
    else if (cpuPaneItems->cMuxTristateLabel->text() == "1") {
        quint8 a, b;
        int carry, overflow;
        return getALUOut(out, a, b, carry, overflow, errorString, cpuPaneItems);
    }
    else {
        errorString.append("CMux control signal not specified.\n");
    }
    return false;
}

bool Sim::getAMuxOut(quint8 &out, QString &errorString,
                     CpuGraphicsItems *cpuPaneItems)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        return OneByteModel::getAMuxOut(out, errorString, cpuPaneItems);
        break;
    case Enu::TwoByteDataBus:
        return TwoByteModel::getAMuxOut(out, errorString, cpuPaneItems);
        break;
    default:
        break;
    }
    errorString.append("CPU model not specified.");
    return false;
}

bool Sim::getMDRMuxOut(quint8& out, QString& errorString,
                       CpuGraphicsItems *cpuPaneItems)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        return OneByteModel::getMDRMuxOut(out, errorString, cpuPaneItems);
        break;
    case Enu::TwoByteDataBus:
        errorString.append("CPU model does not have an MDR.");
        return false;
        break;
    default:
        break;
    }
    errorString.append("CPU model not specified.");
    return false;
}

bool Sim::getMARMuxOut(quint8& mara, quint8& marb,
                       QString& errorString,
                       CpuGraphicsItems *cpuPaneItems)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        errorString.append("CPU model does not have an MARMux.");
        return false;
        break;
    case Enu::TwoByteDataBus:
        return TwoByteModel::getMARMuxOut(mara, marb, errorString, cpuPaneItems);
        break;
    default:
        break;
    }
    errorString.append("CPU model not specified.");
    return false;
}

bool Sim::getMDROMuxOut(quint8& out, QString& errorString,
                        CpuGraphicsItems *cpuPaneItems)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        errorString.append("CPU model does not have an MDROMux.");
        return false;
        break;
    case Enu::TwoByteDataBus:
        return TwoByteModel::getMDROMuxOut(out, errorString, cpuPaneItems);
        break;
    default:
        break;
    }
    errorString.append("CPU model not specified.");
    return false;
}

bool Sim::getMDREMuxOut(quint8& out, QString& errorString,
                        CpuGraphicsItems *cpuPaneItems)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        errorString.append("CPU model does not have an MDREMux.");
        return false;
        break;
    case Enu::TwoByteDataBus:
        return TwoByteModel::getMDREMuxOut(out, errorString, cpuPaneItems);
        break;
    default:
        break;
    }
    errorString.append("CPU model not specified.");
    return false;
}

bool Sim::getEOMuxOut(quint8& out, QString& errorString,
                      CpuGraphicsItems *cpuPaneItems)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        errorString.append("CPU model does not have an EOMux.");
        return false;
        break;
    case Enu::TwoByteDataBus:
        return TwoByteModel::getEOMuxOut(out, errorString, cpuPaneItems);
        break;
    default:
        break;
    }
    errorString.append("CPU model not specified.");
    return false;
}


// ***************************************************************************
// One byte model-specific functionality
// ***************************************************************************
bool OneByteModel::isCorrectALUInput(int ALUFn, CpuGraphicsItems *cpuPaneItems)
{
    bool abus = false;
    bool bbus = false;
    bool cin = false;

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

    if (cpuPaneItems->CSMuxTristateLabel->text() != "") {
        cin = true;
    }

    // test A and B bus input:
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

    // test CIN:
    switch(ALUFn) {
    case 2:
    case 4:
    case 12:
    case 14:
        if (!cin) {
            return false;
        }
        break;
    default:
        break;
    }

    return true;
}

bool OneByteModel::getAMuxOut(quint8 &out, QString &errorString,
                              CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->aMuxTristateLabel->text() == "0") {
        out = Sim::MDR;
        return true;
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "1") {
        if (Sim::getABusOut(out, errorString, cpuPaneItems)) {
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

bool OneByteModel::getMDRMuxOut(quint8 &out, QString &errorString,
                           CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->MDRMuxTristateLabel->text() == "0") {
        if (Sim::mainBusState == Enu::MemReadReady) {
            // perform a memRead
            int address = Sim::MARA * 256 + Sim::MARB;
            out = Sim::readByte(address);
            return true;
        }
        else {
            errorString.append("Not ready for memread.\n");
        }
    }
    else if (cpuPaneItems->MDRMuxTristateLabel->text() == "1") {
        if (Sim::getCMuxOut(out, errorString, cpuPaneItems)) {
            return true;
        }
    }
    else {
        errorString.append("MDRCk is checked, but MDRMux isn't set.\n");
    }
    return false;
}

// ***************************************************************************
// Two byte model-specific functionality:
// ***************************************************************************
bool TwoByteModel::isCorrectALUInput(int ALUFn, CpuGraphicsItems *cpuPaneItems)
{
    bool abus = false;
    bool bbus = false;
    bool cin = false;

    if (cpuPaneItems->aMuxTristateLabel->text() == "") {
        abus = false;
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "0") {
        if (cpuPaneItems->EOMuxTristateLabel->text() == "") {
            abus = false;
        }
        else {
            abus = true;
        }
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

    if (cpuPaneItems->CSMuxTristateLabel->text() != "") {
        cin = true;
    }

    // test A and B bus input:
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

    // test CIN:
    switch(ALUFn) {
    case 2:
    case 4:
    case 12:
    case 14:
        if (!cin) {
            return false;
        }
        break;
    default:
        break;
    }

    return true;
}

bool TwoByteModel::getAMuxOut(quint8& out, QString& errorString,
                              CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->aMuxTristateLabel->text() == "0") {
        if (getEOMuxOut(out, errorString, cpuPaneItems)) {
            return true;
        }
        else {
            // Error string will [already] be populated with the correct error
        }
    }
    else if (cpuPaneItems->aMuxTristateLabel->text() == "1") {
        if (Sim::getABusOut(out, errorString, cpuPaneItems)) {
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

bool TwoByteModel::getMARMuxOut(quint8& mara, quint8 &marb,
                                QString& errorString,
                                CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->MARMuxTristateLabel->text() == "0") {
        mara = Sim::MDREven;
        marb = Sim::MDROdd;
        return true;
    }
    else if (cpuPaneItems->MARMuxTristateLabel->text() == "1") {
        if (Sim::getABusOut(mara, errorString, cpuPaneItems)
                && Sim::getBBusOut(marb, errorString, cpuPaneItems)) {
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

bool TwoByteModel::getMDROMuxOut(quint8& out, QString& errorString,
                                 CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->MDROMuxTristateLabel->text() == "0") {
        if (Sim::mainBusState == Enu::MemReadReady) {
            // perform a memRead
            // align the address to even number, as per pg. 614 fig 12.14
            int address = (Sim::MARA * 256 + Sim::MARB) & 0xFFFE;
            // address is odd (MDRO):
            address++;
            out = Sim::readByte(address);
            qDebug() << "MDRO just read " << QString("0x%1").arg(out, 4, 16, QLatin1Char('0'))
                     << " at address " << QString("0x%1").arg(address, 4, 16, QLatin1Char('0'));
            return true;
        }
        else {
            errorString.append("Not ready for memread.\n");
        }
    }
    else if (cpuPaneItems->MDROMuxTristateLabel->text() == "1") {
        if (Sim::getCMuxOut(out, errorString, cpuPaneItems)) {
            return true;
        }
    }
    else {
        errorString.append("MDROCk is checked, but MDROMux isn't set.\n");
    }
    return false;
}

bool TwoByteModel::getMDREMuxOut(quint8& out, QString& errorString,
                                 CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->MDREMuxTristateLabel->text() == "0") {
        if (Sim::mainBusState == Enu::MemReadReady) {
            // perform a memRead
            // align the address to even number, as per pg. 614 fig 12.14
            int address = (Sim::MARA * 256 + Sim::MARB) & 0xFFFE;
            out = Sim::readByte(address);
            qDebug() << "MDRE just read " << QString("0x%1").arg(out, 4, 16, QLatin1Char('0'))
                     << " at address " << QString("0x%1").arg(address, 4, 16, QLatin1Char('0'));
            return true;
        }
        else {
            errorString.append("Not ready for memread.\n");
        }
    }
    else if (cpuPaneItems->MDREMuxTristateLabel->text() == "1") {
        if (Sim::getCMuxOut(out, errorString, cpuPaneItems)) {
            return true;
        }
    }
    else {
        errorString.append("MDRECk is checked, but MDREMux isn't set.\n");
    }
    return false;
}

bool TwoByteModel::getEOMuxOut(quint8& out, QString& errorString,
                               CpuGraphicsItems *cpuPaneItems)
{
    if (cpuPaneItems->EOMuxTristateLabel->text() == "0") {
        out = Sim::MDREven;
        return true;
    }
    if (cpuPaneItems->EOMuxTristateLabel->text() == "1") {
        out = Sim::MDROdd;
        return true;
    }
    else {
        errorString.append("EOMux control signal not specified.\n");
    }
    return false;
}

