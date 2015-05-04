// File: specification.cpp
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

#include "specification.h"
#include <QDebug>

Specification::Specification()
{
}

MemSpecification::MemSpecification(int memoryAddress, int memoryValue, int numberBytes) {
    memAddress = memoryAddress;
    memValue = memoryValue;
    numBytes = numberBytes;
}

void MemSpecification::setUnitPre(MainMemory *mainMemory, CpuPane *) {
    if (numBytes == 1) {
        mainMemory->setMemPrecondition(memAddress, memValue);
    }
    else { // numBytes == 2
        mainMemory->setMemPrecondition(memAddress, memValue / 256);
        mainMemory->setMemPrecondition(memAddress + 1, memValue % 256);
    }
}

bool MemSpecification::testUnitPost(MainMemory *mainMemory, CpuPane *, QString &errorString) {
    if (numBytes == 1) {
        if (mainMemory->testMemPostcondition(memAddress, memValue)) {
            return true;
        }
        errorString = "// ERROR: Unit test failed for byte Mem[0x" +
                      QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper() + "].";
        return false;
    }
    else { // numBytes == 2
        if ((mainMemory->testMemPostcondition(memAddress, memValue / 256)) && (mainMemory->testMemPostcondition(memAddress + 1, memValue % 256))) {
            return true;
        }
        errorString = "// ERROR: Unit test failed for word Mem[0x" +
                      QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper() + "].";
        return false;
    }
}

QString MemSpecification::getSourceCode() {
    return "Mem[0x"
            + QString("%1").arg(memAddress, 4, 16, QLatin1Char('0')).toUpper()
            + "]=0x"
            + (numBytes == 1 ?
               QString("%1").arg(memValue, 2, 16, QLatin1Char('0')).toUpper() :
               QString("%1").arg(memValue, 4, 16, QLatin1Char('0')).toUpper()) ;
}

RegSpecification::RegSpecification(Enu::EMnemonic registerAddress, int registerValue) {
    regAddress = registerAddress;
    regValue = registerValue;
}

void RegSpecification::setUnitPre(MainMemory *, CpuPane *cpuPane) {
    cpuPane->setRegPrecondition(regAddress, regValue);
}

bool RegSpecification::testUnitPost(MainMemory *, CpuPane *cpuPane, QString &errorString) {
    if (cpuPane->testRegPostcondition(regAddress, regValue)) {
        return true;
    }
    switch (regAddress) {
    case Enu::A: errorString = "// ERROR: Unit test failed for register A."; return false;
    case Enu::X: errorString = "// ERROR: Unit test failed for register X."; return false;
    case Enu::SP: errorString = "// ERROR: Unit test failed for register SP."; return false;
    case Enu::PC: errorString = "// ERROR: Unit test failed for register PC."; return false;
    case Enu::IR: errorString = "// ERROR: Unit test failed for register IR."; return false;
    case Enu::T1: errorString = "// ERROR: Unit test failed for register T1."; return false;
    case Enu::T2: errorString = "// ERROR: Unit test failed for register T2."; return false;
    case Enu::T3: errorString = "// ERROR: Unit test failed for register T3."; return false;
    case Enu::T4: errorString = "// ERROR: Unit test failed for register T4."; return false;
    case Enu::T5: errorString = "// ERROR: Unit test failed for register T5."; return false;
    case Enu::T6: errorString = "// ERROR: Unit test failed for register T6."; return false;
    case Enu::MARA: errorString = "// ERROR: Unit test failed for MARA."; return false;
    case Enu::MARB: errorString = "// ERROR: Unit test failed for MARB."; return false;
    case Enu::MDR: errorString = "// ERROR: Unit test failed for MDR."; return false;
    default: return false;
    }
}

QString RegSpecification::getSourceCode() {
    switch (regAddress) {
    case Enu::A: return "A=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::X: return "X=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::SP: return "SP=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::PC: return "PC=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::IR: return "IR=0x" + QString("%1").arg(regValue, 6, 16, QLatin1Char('0')).toUpper();
    case Enu::T1: return "T1=0x" + QString("%1").arg(regValue, 2, 16, QLatin1Char('0')).toUpper();
    case Enu::T2: return "T2=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::T3: return "T3=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::T4: return "T4=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::T5: return "T5=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::T6: return "T6=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::MARA: return "MARA=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::MARB: return "MARB=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    case Enu::MDR: return "MDR=0x" + QString("%1").arg(regValue, 4, 16, QLatin1Char('0')).toUpper();
    default: return "";
    }
}

StatusBitSpecification::StatusBitSpecification(Enu::EMnemonic statusBitAddress, bool statusBitValue) {
    nzvcsAddress = statusBitAddress;
    nzvcsValue = statusBitValue;
}

void StatusBitSpecification::setUnitPre(MainMemory *, CpuPane *cpuPane) {
    cpuPane->setStatusPrecondition(nzvcsAddress, nzvcsValue);
}

bool StatusBitSpecification::testUnitPost(MainMemory *, CpuPane *cpuPane, QString &errorString) {
    if (cpuPane->testStatusPostcondition(nzvcsAddress, nzvcsValue)) {
        return true;
    }
    switch (nzvcsAddress) {
    case Enu::N: errorString = "// ERROR: Unit test failed for status bit N."; return false;
    case Enu::Z: errorString = "// ERROR: Unit test failed for status bit Z."; return false;
    case Enu::V: errorString = "// ERROR: Unit test failed for status bit V."; return false;
    case Enu::C: errorString = "// ERROR: Unit test failed for status bit C."; return false;
    case Enu::S: errorString = "// ERROR: Unit test failed for status bit S."; return false;
    default: return false;
    }
}

QString StatusBitSpecification::getSourceCode() {
    switch (nzvcsAddress) {
    case Enu::N: return "N=" + QString("%1").arg(nzvcsValue);
    case Enu::Z: return "Z=" + QString("%1").arg(nzvcsValue);
    case Enu::V: return "V=" + QString("%1").arg(nzvcsValue);
    case Enu::C: return "C=" + QString("%1").arg(nzvcsValue);
    case Enu::S: return "S=" + QString("%1").arg(nzvcsValue);
    default: return "";
    }
}
