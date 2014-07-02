// File: sim.cpp
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

#include "sim.h"
#include "enu.h"

#include <QDebug>

using namespace Enu;

QVector<quint8> Sim::Mem(65536);
bool Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit;

QVector<quint8> Sim::regBank(32);

quint8 Sim::MARA = 0;
quint8 Sim::MARB = 0;
quint8 Sim::MDR = 0;

Enu::MainBusState Sim::mainBusState;

QSet<int> Sim::modifiedBytes;

bool Sim::memReadPrevStep = false;
bool Sim::memWritePrevStep = false;

QList<Code *> Sim::codeList;
int Sim::cycleCount; // used to storing the number of cycles in a simulation
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
    qDebug() << "Wrote byte " << memAddr << " with value " << value;

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

//    return microCodeCurrentLine + 1 >= codeList.size() - 1; // old and tired
}

void Sim::initMRegs()
{
    regBank[22] = 0x00;
    regBank[23] = 0x01;
    regBank[24] = 0x02;
    regBank[25] = 0x03;
    regBank[26] = 0x04;
    regBank[27] = 0x08;
    regBank[28] = 0xFA;
    regBank[29] = 0xFC;
    regBank[30] = 0xFE;
    regBank[31] = 0xFF;
}




