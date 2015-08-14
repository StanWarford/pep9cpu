// File: code.h
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
#ifndef CODE_H
#define CODE_H

#include <QString>
#include <QTextEdit>

#include "enu.h"
#include "specification.h"
#include "mainmemory.h"
#include "cpupane.h"

// Abstract code class
class Code
{
public:
    virtual ~Code() { }
    virtual bool isMicrocode() { return false; }
    virtual void setCpuLabels(CpuGraphicsItems *) { }
    virtual QString getObjectCode() { return ""; }
    virtual QString getSourceCode() { return ""; }
    virtual bool hasUnitPre() { return false; }
    virtual void setUnitPre(MainMemory *, CpuPane *) { }
    virtual bool testPostcondition(MainMemory *, CpuPane *, QString &) { return true; }
};

// Concrete code classes
// Code is the union of the elements of the one-byte bus model and two-byte bus model
class MicroCode: public Code
{
    friend class Asm;
public:
    MicroCode();
    bool isMicrocode();
    void setCpuLabels(CpuGraphicsItems *cpuPaneItems);
    QString getObjectCode();
    QString getSourceCode();
    bool has(Enu::EMnemonic field);
    void set(Enu::EMnemonic field, int value);
    bool inRange(Enu::EMnemonic field, int value);
private:
    int cLoadCk;
    int cC;
    int cB;
    int cA;
    int cMARMux; // Two-byte model only
    int cMARCk;
    int cMDRCk; // One-byte model only
    int cMDROCk; // Two-byte model only
    int cMDRECk; // Two-byte model only
    int cAMux;
    int cMDRMux; // One-byte model only
    int cMDROMux; // Two-byte model only
    int cMDREMux; // Two-byte model only
    int cEOMux; // Two-byte model only
    int cCMux;
    int cALU;
    int cCSMux;
    int cSCk;
    int cCCk;
    int cVCk;
    int cAndZ;
    int cZCk;
    int cNCk;
    int cMemWrite;
    int cMemRead;
    QString cComment;
};

class CommentOnlyCode: public Code
{
public:
    CommentOnlyCode(QString comment);
    QString getSourceCode();
private:
    QString cComment;
};

class UnitPreCode: public Code
{
public:
    ~UnitPreCode();
    QString getSourceCode();
    bool hasUnitPre();
    void setUnitPre(MainMemory *mainMemory, CpuPane *cpuPane);
    void appendSpecification(Specification *specification);
    void setComment(QString comment);
private:
    QList<Specification *> unitPreList;
    QString cComment;
};

class UnitPostCode: public Code
{
public:
    ~UnitPostCode();
    QString getSourceCode();
    bool testPostcondition(MainMemory *mainMemory, CpuPane *cpuPane, QString &errorString);
    void appendSpecification(Specification *specification);
    void setComment(QString comment);
private:
    QList<Specification *> unitPostList;
    QString cComment;
};

class BlankLineCode: public Code
{
};

#endif // CODE_H
