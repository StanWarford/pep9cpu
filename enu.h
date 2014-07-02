// File: enu.h
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
#ifndef ENU_H
#define ENU_H

namespace Enu
{
    // Instruction mnemonics
    enum EMask
    {
        NMask = 0x08,
        ZMask = 0x04,
        VMask = 0x02,
        CMask = 0x01,
    };

    enum MainBusState {
        None,
        MemReadWait,
        MemReadReady,
        MemWriteWait,
        MemWriteReady,
    };

    enum EMnemonic {
        LoadCk, C, B, A, MARCk, MDRCk, AMux, MDRMux, CMux,
        ALU, CCk, VCk, ANDZ, ZCk, NCk, MemWrite, MemRead, Pre, Post,
        Mem, X, SP, PC, IR, T1, T2, T3, T4, T5, T6, N, Z, V,
        MARA, MARB, MDR,
    };

}

#endif // ENU_H
