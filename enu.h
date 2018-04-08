// File: enu.h
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
#ifndef ENU_H
#define ENU_H
#include <QtCore>
#include <QException>
namespace Enu
{
    Q_NAMESPACE
    static const quint8 maxRegisterNumber = 0;
    static const quint8 signalDisabled= 255;
    class InvalidCPUMode : public QException
    {
        void raise() const { throw *this; }
        InvalidCPUMode *clone() const { return new InvalidCPUMode(*this); }
    };
    // Instruction mnemonics
    enum EMask // For ALU function 15
    {
        NMask = 0x08,
        ZMask = 0x04,
        VMask = 0x02,
        CMask = 0x01,
    };

    enum MainBusState {
        None,
        MemReadFirstWait,
        MemReadSecondWait,
        MemReadReady,
        MemWriteFirstWait,
        MemWriteSecondWait,
        MemWriteReady,
    };
    enum EBranchFunctions{
        Unconditional=0,Stop=15
    };
    enum EControlSignals
    {
        MemRead, MemWrite,
        A,B,EOMux, MARMux,MARA, MARB, AMux, ALU,CSMux, AndZ,
        CMux, C,
        MDRMux, MDROMux, MDREMux,MDR, MDRE, MDRO,
    };
    enum EClockSignals{
        NCk,ZCk,VCk,CCk,SCk,MARCk,LoadCk,MDRCk, MDROCk, MDRECk,
    };
    enum EMemoryRegisters
    {
        MEM_MARA,MEM_MARB,MEM_MDR,MEM_MDRO,MEM_MDRE
    };
    enum EALUFunc
    {

    };
    enum EKeywords {

        Pre=255-1, Post=255-2,
        Mem=255-3, Acc=255-24, X=255-4, SP=255-5, PC=255-6, IR=255-7,
        T1=255-8, T2=255-9, T3=255-10, T4=255-11, T5=255-12, T6=255-13,
        N=255-15, Z=255-16, V=255-17,Cbit=255-25, S=255-18,
        MARAREG=255-19,MARBREG=255-20,
        MDRREG=255-21,MDREREG=255-22,MDROREG=255-23
    };

//Q_ENUM_NS(EMnemonic); //This is a declaration, despite whatever QT Creator says.


    enum CPUType {
        OneByteDataBus,
        TwoByteDataBus
    };

    // For our drawing/shapes classes:
    enum Direction {
        Up,
        Down,
        Left,
        Right,
    };

}

#endif // ENU_H
