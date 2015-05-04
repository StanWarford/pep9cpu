// File: sim.h
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
#ifndef SIM_H
#define SIM_H

#include <QVector>
#include <QSet>

#include "code.h"
#include "enu.h"

class Sim
{
public:

    static QVector<quint8> Mem;
    static bool nBit, zBit, vBit, cBit, sBit;

    static QVector<quint8> regBank;

    static quint8 MARA;
    static quint8 MARB;
    static quint8 MDR;
    static quint8 MDROdd;
    static quint8 MDREven;

    static Enu::MainBusState mainBusState;

    // Used for keeping track of which bytes have been modified since last memory update:
    static QSet<int> modifiedBytes;

    static QList<Code *> codeList;
    static int cycleCount; // used for storing the number of cycles in a simulation
    static int microProgramCounter;
    static int microCodeCurrentLine;

    static QStringList microcodeSourceList;
    static void setMicrocodeSourceList();

    static int readByte(int memAddr);
    static void writeByte(int memAddr, int value);
    // Pre: 0 <= value < 256
    // Post: Value is stored in Mem[memAddr]

    static bool aluFnIsUnary(int fn);

    static bool atEndOfSim();

    static void initMRegs();

    static void clearMemory();

    static void initNZVCS();

    static void initCPUState();

};

#endif // SIM_H
