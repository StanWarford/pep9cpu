// File: code.cpp
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

#include "code.h"
#include "cpugraphicsitems.h"
#include "pep.h"

MicroCode::MicroCode()
{
    // -1 is initialization value, - tested elsewhere
    // The union of one-byte data bus model and two-byte data bus model.
    // Elements not in the current model are simply not used.

    cLoadCk = -1;
    cC = -1;
    cB = -1;
    cA = -1;
    cMARMux = -1; // Two-byte model only
    cMARCk = -1;
    cMDRCk = -1; // One-byte model only
    cMDROCk = -1; // Two-byte model only
    cMDRECk = -1; // Two-byte model only
    cAMux = -1;
    cMDRMux = -1; // One-byte model only
    cMDROMux = -1; // Two-byte model only
    cMDREMux = -1; // Two-byte model only
    cEOMux = -1; // Two-byte model only
    cCMux = -1;
    cALU = -1;
    cCSMux = -1;
    cSCk = -1;
    cCCk = -1;
    cVCk = -1;
    cAndZ = -1;
    cZCk = -1;
    cNCk = -1;
    cMemWrite = -1;
    cMemRead = -1;
    cComment = "";
}

bool MicroCode::isMicrocode() { return true; }

#pragma message("In code.cpp, to do when CpuGraphicsItems is completed")
void MicroCode::setCpuLabels(CpuGraphicsItems *cpuPaneItems)
{
    cpuPaneItems->loadCk->setChecked(cLoadCk != -1);
    cpuPaneItems->cLineEdit->setText(cC == -1 ? "" : QString("%1").arg(cC));
    cpuPaneItems->bLineEdit->setText(cB == -1 ? "" : QString("%1").arg(cB));
    cpuPaneItems->aLineEdit->setText(cA == -1 ? "" : QString("%1").arg(cA));
    cpuPaneItems->MARCk->setChecked(cMARCk != -1);
    cpuPaneItems->MDRCk->setChecked(cMDRCk != -1);
    cpuPaneItems->MDRECk->setChecked(cMDRECk != -1);
    cpuPaneItems->MDROCk->setChecked(cMDROCk != -1);
    cpuPaneItems->aMuxTristateLabel->setState(cAMux);
    cpuPaneItems->MDRMuxTristateLabel->setState(cMDRMux);
    cpuPaneItems->MDREMuxTristateLabel->setState(cMDREMux);
    cpuPaneItems->MDROMuxTristateLabel->setState(cMDROMux);
    cpuPaneItems->EOMuxTristateLabel->setState(cEOMux);
    cpuPaneItems->MARMuxTristateLabel->setState(cMARMux);
    cpuPaneItems->cMuxTristateLabel->setState(cCMux);
    cpuPaneItems->ALULineEdit->setText(cALU == -1 ? "" : QString("%1").arg(cALU));
    cpuPaneItems->CSMuxTristateLabel->setState(cCSMux);
    cpuPaneItems->SCkCheckBox->setChecked(cSCk != -1);
    cpuPaneItems->CCkCheckBox->setChecked(cCCk != -1);
    cpuPaneItems->VCkCheckBox->setChecked(cVCk != -1);
    cpuPaneItems->AndZTristateLabel->setState(cAndZ);
    cpuPaneItems->ZCkCheckBox->setChecked(cZCk != -1);
    cpuPaneItems->NCkCheckBox->setChecked(cNCk != -1);
    cpuPaneItems->MemReadTristateLabel->setState(cMemRead);
    cpuPaneItems->MemWriteTristateLabel->setState(cMemWrite);
}

QString MicroCode::getObjectCode()
{
    // QString QString::arg(int a, int fieldWidth = 0, ...)
    // fieldWidth specifies the minimum amount of space that
    //  a is padded to and filled with the character fillChar.
    // A positive value produces right-aligned text; a negative
    //  value produces left-aligned text.

    QString str = "";
    if (Pep::cpuFeatures == Enu::OneByteDataBus) {
        str.append(cLoadCk == -1 ? "  " : QString("%1").arg(cLoadCk, -2));
        str.append(cC == -1 ? "   " : QString("%1").arg(cC, -3));
        str.append(cB == -1 ? "   " : QString("%1").arg(cB, -3));
        str.append(cA == -1 ? "   " : QString("%1").arg(cA, -3));
        str.append(cMARCk == -1 ? "  " : QString("%1").arg(cMARCk, -2));
        str.append(cMDRCk == -1 ? "  " : QString("%1").arg(cMDRCk, -2));
        str.append(cAMux == -1 ? "  " : QString("%1").arg(cAMux, -2));
        str.append(cMDRMux == -1 ? "  " : QString("%1").arg(cMDRMux, -2));
        str.append(cCMux == -1 ? "  " : QString("%1").arg(cCMux, -2));
        str.append(cALU == -1 ? "   " : QString("%1").arg(cALU, -3));
        str.append(cCSMux == -1 ? "  " : QString("%1").arg(cCSMux, -2));
        str.append(cSCk == -1 ? "  " : QString("%1").arg(cSCk, -2));
        str.append(cCCk == -1 ? "  " : QString("%1").arg(cCCk, -2));
        str.append(cVCk == -1 ? "  " : QString("%1").arg(cVCk, -2));
        str.append(cAndZ == -1 ? "  " : QString("%1").arg(cAndZ, -2));
        str.append(cZCk == -1 ? "  " : QString("%1").arg(cZCk, -2));
        str.append(cNCk == -1 ? "  " : QString("%1").arg(cNCk, -2));
        str.append(cMemWrite == -1 ? "  " : QString("%1").arg(cMemWrite, -2));
        str.append(cMemRead == -1 ? "  " : QString("%1").arg(cMemRead, -2));
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        str.append(cLoadCk == -1 ? "  " : QString("%1").arg(cLoadCk, -2));
        str.append(cC == -1 ? "   " : QString("%1").arg(cC, -3));
        str.append(cB == -1 ? "   " : QString("%1").arg(cB, -3));
        str.append(cA == -1 ? "   " : QString("%1").arg(cA, -3));
        str.append(cMARMux == -1 ? "  " : QString("%1").arg(cMARMux, -2));
        str.append(cMARCk == -1 ? "  " : QString("%1").arg(cMARCk, -2));
        str.append(cMDROCk == -1 ? "  " : QString("%1").arg(cMDROCk, -2));
        str.append(cMDROMux == -1 ? "  " : QString("%1").arg(cMDROMux, -2));
        str.append(cMDRECk == -1 ? "  " : QString("%1").arg(cMDRECk, -2));
        str.append(cMDREMux == -1 ? "  " : QString("%1").arg(cMDREMux, -2));
        str.append(cEOMux == -1 ? "  " : QString("%1").arg(cEOMux, -2));
        str.append(cAMux == -1 ? "  " : QString("%1").arg(cAMux, -2));
        str.append(cCMux == -1 ? "  " : QString("%1").arg(cCMux, -2));
        str.append(cALU == -1 ? "   " : QString("%1").arg(cALU, -3));
        str.append(cCSMux == -1 ? "  " : QString("%1").arg(cCSMux, -2));
        str.append(cSCk == -1 ? "  " : QString("%1").arg(cSCk, -2));
        str.append(cCCk == -1 ? "  " : QString("%1").arg(cCCk, -2));
        str.append(cVCk == -1 ? "  " : QString("%1").arg(cVCk, -2));
        str.append(cAndZ == -1 ? "  " : QString("%1").arg(cAndZ, -2));
        str.append(cZCk == -1 ? "  " : QString("%1").arg(cZCk, -2));
        str.append(cNCk == -1 ? "  " : QString("%1").arg(cNCk, -2));
        str.append(cMemWrite == -1 ? "  " : QString("%1").arg(cMemWrite, -2));
        str.append(cMemRead == -1 ? "  " : QString("%1").arg(cMemRead, -2));
    }
    str.append("\n");
    return str;
}

QString MicroCode::getSourceCode()
{
    QString str = "";
    if (Pep::cpuFeatures == Enu::OneByteDataBus) {
        if (cMemRead != -1) { str.append("MemRead, "); }
        if (cMemWrite != -1) { str.append("MemWrite, "); }
        if (cA != -1) { str.append("A=" + QString("%1").arg(cA) + ", "); }
        if (cB != -1) { str.append("B=" + QString("%1").arg(cB) + ", "); }
        if (cAMux != -1) { str.append("AMux=" + QString("%1").arg(cAMux) + ", "); }
        if (cCSMux  != -1) { str.append("CSMux=" + QString("%1").arg(cCSMux) + ", "); }
        if (cALU != -1) { str.append("ALU=" + QString("%1").arg(cALU) + ", "); }
        if (cAndZ != -1) { str.append("AndZ=" + QString("%1").arg(cAndZ) + ", "); }
        if (cCMux != -1) { str.append("CMux=" + QString("%1").arg(cCMux) + ", "); }
        if (cMDRMux != -1) { str.append("MDRMux=" + QString("%1").arg(cMDRMux) + ", "); }
        if (cC != -1) { str.append("C=" + QString("%1").arg(cC) + ", "); }

        if (str != "") { str.chop(2); str.append("; "); }

        if (cNCk != -1) { str.append("NCk, "); }
        if (cZCk != -1) { str.append("ZCk, "); }
        if (cVCk != -1) { str.append("VCk, "); }
        if (cCCk != -1) { str.append("CCk, "); }
        if (cSCk != -1) { str.append("SCk, "); }
        if (cMARCk != -1) { str.append("MARCk, "); }
        if (cLoadCk != -1) { str.append("LoadCk, "); }
        if (cMDRCk != -1) { str.append("MDRCk, "); }

        if (str.endsWith(", ") || str.endsWith("; ")) { str.chop(2); }
        if (!cComment.isEmpty()) {
            str.append(" " + cComment);
        }
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        if (cMemRead != -1) { str.append("MemRead, "); }
        if (cMemWrite != -1) { str.append("MemWrite, "); }
        if (cA != -1) { str.append("A=" + QString("%1").arg(cA) + ", "); }
        if (cB != -1) { str.append("B=" + QString("%1").arg(cB) + ", "); }
        if (cMARMux != -1) { str.append("MARMux=" + QString("%1").arg(cMARMux) + ", "); }
        if (cEOMux != -1) { str.append("EOMux=" + QString("%1").arg(cEOMux) + ", "); }
        if (cAMux != -1) { str.append("AMux=" + QString("%1").arg(cAMux) + ", "); }
        if (cCSMux  != -1) { str.append("CSMux=" + QString("%1").arg(cCSMux) + ", "); }
        if (cALU != -1) { str.append("ALU=" + QString("%1").arg(cALU) + ", "); }
        if (cAndZ != -1) { str.append("AndZ=" + QString("%1").arg(cAndZ) + ", "); }
        if (cCMux != -1) { str.append("CMux=" + QString("%1").arg(cCMux) + ", "); }
        if (cMDREMux != -1) { str.append("MDREMux=" + QString("%1").arg(cMDREMux) + ", "); }
        if (cMDROMux != -1) { str.append("MDROMux=" + QString("%1").arg(cMDROMux) + ", "); }
        if (cC != -1) { str.append("C=" + QString("%1").arg(cC) + ", "); }

        if (str != "") { str.chop(2); str.append("; "); }

        if (cNCk != -1) { str.append("NCk, "); }
        if (cZCk != -1) { str.append("ZCk, "); }
        if (cVCk != -1) { str.append("VCk, "); }
        if (cCCk != -1) { str.append("CCk, "); }
        if (cSCk != -1) { str.append("SCk, "); }
        if (cMARCk != -1) { str.append("MARCk, "); }
        if (cLoadCk != -1) { str.append("LoadCk, "); }
        if (cMDRECk != -1) { str.append("MDRECk, "); }
        if (cMDROCk != -1) { str.append("MDROCk, "); }

        if (str.endsWith(", ") || str.endsWith("; ")) { str.chop(2); }
        if (!cComment.isEmpty()) {
            str.append(" " + cComment);
        }
    }
    return str;
}

bool MicroCode::has(Enu::EMnemonic field) {
    if (Pep::cpuFeatures == Enu::OneByteDataBus) {
        switch (field) {
        case Enu::LoadCk: return cLoadCk != -1;
        case Enu::C: return cC != -1;
        case Enu::B: return cB != -1;
        case Enu::A: return cA != -1;
        case Enu::MARCk: return cMARCk != -1;
        case Enu::MDRCk: return cMDRCk != -1;
        case Enu::AMux: return cAMux != -1;
        case Enu::MDRMux: return cMDRMux != -1;
        case Enu::CMux: return cCMux != -1;
        case Enu::ALU: return cALU != -1;
        case Enu::CSMux: return cCSMux != -1;
        case Enu::SCk: return cSCk != -1;
        case Enu::CCk: return cCCk != -1;
        case Enu::VCk: return cVCk != -1;
        case Enu::AndZ: return cAndZ != -1;
        case Enu::ZCk: return cZCk != -1;
        case Enu::NCk: return cNCk != -1;
        case Enu::MemWrite: return cMemWrite != -1;
        case Enu::MemRead: return cMemRead != -1;
        default: return true;
        }
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        switch (field) {
        case Enu::LoadCk: return cLoadCk != -1;
        case Enu::C: return cC != -1;
        case Enu::B: return cB != -1;
        case Enu::A: return cA != -1;
        case Enu::MARMux: return cMARMux != -1;
        case Enu::MARCk: return cMARCk != -1;
        case Enu::MDROCk: return cMDROCk != -1;
        case Enu::MDROMux: return cMDROMux != -1;
        case Enu::MDRECk: return cMDRECk != -1;
        case Enu::MDREMux: return cMDREMux != -1;
        case Enu::EOMux: return cEOMux != -1;
        case Enu::AMux: return cAMux != -1;
        case Enu::CMux: return cCMux != -1;
        case Enu::ALU: return cALU != -1;
        case Enu::CSMux: return cCSMux != -1;
        case Enu::SCk: return cSCk != -1;
        case Enu::CCk: return cCCk != -1;
        case Enu::VCk: return cVCk != -1;
        case Enu::AndZ: return cAndZ != -1;
        case Enu::ZCk: return cZCk != -1;
        case Enu::NCk: return cNCk != -1;
        case Enu::MemWrite: return cMemWrite != -1;
        case Enu::MemRead: return cMemRead != -1;
        default: return true;
        }
    }
    return true; // Suppress compiler warning
}

void MicroCode::set(Enu::EMnemonic field, int value) {
    if (Pep::cpuFeatures == Enu::OneByteDataBus) {
        switch (field) {
        case Enu::LoadCk: cLoadCk = value; break;
        case Enu::C: cC = value; break;
        case Enu::B: cB = value; break;
        case Enu::A: cA = value; break;
        case Enu::MARCk: cMARCk = value; break;
        case Enu::MDRCk: cMDRCk = value; break;
        case Enu::AMux: cAMux = value; break;
        case Enu::MDRMux: cMDRMux = value; break;
        case Enu::CMux: cCMux = value; break;
        case Enu::ALU: cALU = value; break;
        case Enu::CSMux: cCSMux = value; break;
        case Enu::SCk: cSCk = value; break;
        case Enu::CCk: cCCk = value; break;
        case Enu::VCk: cVCk = value; break;
        case Enu::AndZ: cAndZ = value; break;
        case Enu::ZCk: cZCk = value; break;
        case Enu::NCk: cNCk = value; break;
        case Enu::MemWrite: cMemWrite = value; break;
        case Enu::MemRead: cMemRead = value; break;
        default: break;
        }
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        switch (field) {
        case Enu::LoadCk: cLoadCk = value; break;
        case Enu::C: cC = value; break;
        case Enu::B: cB = value; break;
        case Enu::A: cA = value; break;
        case Enu::MARMux: cMARMux = value; break;
        case Enu::MARCk: cMARCk = value; break;
        case Enu::MDROCk: cMDROCk = value; break;
        case Enu::MDROMux: cMDROMux = value; break;
        case Enu::MDRECk: cMDRECk = value; break;
        case Enu::MDREMux: cMDREMux = value; break;
        case Enu::EOMux: cEOMux = value; break;
        case Enu::AMux: cAMux = value; break;
        case Enu::CMux: cCMux = value; break;
        case Enu::ALU: cALU = value; break;
        case Enu::CSMux: cCSMux = value; break;
        case Enu::SCk: cSCk = value; break;
        case Enu::CCk: cCCk = value; break;
        case Enu::VCk: cVCk = value; break;
        case Enu::AndZ: cAndZ = value; break;
        case Enu::ZCk: cZCk = value; break;
        case Enu::NCk: cNCk = value; break;
        case Enu::MemWrite: cMemWrite = value; break;
        case Enu::MemRead: cMemRead = value; break;
        default: break;
        }
    }
}

// inRange tests the union of the elements in Pep::mnemonToDecControlMap
bool MicroCode::inRange(Enu::EMnemonic field, int value) {
    switch (field) {
    case Enu::C: return 0 <= value && value <= 31;
    case Enu::B: return 0 <= value && value <= 31;
    case Enu::A: return 0 <= value && value <= 31;
    case Enu::AMux: return 0 <= value && value <= 1;
    case Enu::MDRMux: return 0 <= value && value <= 1;
    case Enu::CMux: return 0 <= value && value <= 1;
    case Enu::ALU: return 0 <= value && value <= 15;
    case Enu::CSMux: return 0 <= value && value <= 1;
    case Enu::AndZ: return 0 <= value && value <= 1;
    case Enu::MARMux: return 0 <= value && value <= 1;
    case Enu::MDROMux: return 0 <= value && value <= 1;
    case Enu::MDREMux: return 0 <= value && value <= 1;
    case Enu::EOMux: return 0 <= value && value <= 1;
    default: return true;
    }
}

CommentOnlyCode::CommentOnlyCode(QString comment)
{
    cComment = comment;
}

QString CommentOnlyCode::getSourceCode() {
    return cComment;
}

UnitPreCode::~UnitPreCode() {
    while (!unitPreList.isEmpty()) {
        delete unitPreList.takeFirst();
    }
}

QString UnitPreCode::getSourceCode() {
    QString str = "UnitPre: ";
    for (int i = 0; i < unitPreList.size(); i++) {
        str.append(unitPreList.at(i)->getSourceCode() + ", ");
    }
    if (str.endsWith(", ")) {
        str.chop(2);
    }
    if (!cComment.isEmpty()) {
        str.append(" " + cComment);
    }
    return str;
}

bool UnitPreCode::hasUnitPre() {
    return !unitPreList.isEmpty();
}

void UnitPreCode::setUnitPre(MainMemory *mainMemory, CpuPane *cpuPane) {
    for (int i = 0; i < unitPreList.size(); i++) {
        unitPreList.at(i)->setUnitPre(mainMemory, cpuPane);
    }
}

void UnitPreCode::appendSpecification(Specification *specification) {
    unitPreList.append(specification);
}

void UnitPreCode::setComment(QString comment) {
    cComment = comment;
}

UnitPostCode::~UnitPostCode() {
    while (!unitPostList.isEmpty()) {
        delete unitPostList.takeFirst();
    }
}

QString UnitPostCode::getSourceCode() {
    QString str = "UnitPost: ";
    for (int i = 0; i < unitPostList.size(); i++) {
        str.append(unitPostList.at(i)->getSourceCode() + ", ");
    }
    if (str.endsWith(", ")) {
        str.chop(2);
    }
    if (!cComment.isEmpty()) {
        str.append(" " + cComment);
    }
    return str;
}

bool UnitPostCode::testPostcondition(MainMemory *mainMemory, CpuPane *cpuPane, QString &errorString) {
    for (int i = 0; i < unitPostList.size(); i++) {
        if (!unitPostList.at(i)->testUnitPost(mainMemory, cpuPane, errorString)) {
            return false;
        }
    }
    return true;
}

void UnitPostCode::appendSpecification(Specification *specification) {
    unitPostList.append(specification);
}

void UnitPostCode::setComment(QString comment) {
    cComment = comment;
}
