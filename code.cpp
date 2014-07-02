// File: code.cpp
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

#include "code.h"
#include "cpupanegraphicsitems.h"

MicroCode::MicroCode()
{
    cLoadCk = -1;
    cC = -1;
    cB = -1;
    cA = -1;
    cMARCk = -1;
    cMDRCk = -1;
    cAMux = -1;
    cMDRMux = -1;
    cCMux = -1;
    cALU = -1;
    cCCk = -1;
    cVCk = -1;
    cANDZ = -1;
    cZCk = -1;
    cNCk = -1;
    cMemWrite = -1;
    cMemRead = -1;
    cComment = "";
}

bool MicroCode::isMicrocode() { return true; }

void MicroCode::setCpuLabels(CpuPaneGraphicsItems *cpuPaneItems)
{
    cpuPaneItems->loadCk->setChecked(cLoadCk != -1);
    cpuPaneItems->cLineEdit->setText(cC == -1 ? "" : QString("%1").arg(cC));
    cpuPaneItems->bLineEdit->setText(cB == -1 ? "" : QString("%1").arg(cB));
    cpuPaneItems->aLineEdit->setText(cA == -1 ? "" : QString("%1").arg(cA));
    cpuPaneItems->MARCk->setChecked(cMARCk != -1);
    cpuPaneItems->MDRCk->setChecked(cMDRCk != -1);
    cpuPaneItems->aMuxTristateLabel->setState(cAMux);
    cpuPaneItems->MDRMuxTristateLabel->setState(cMDRMux);
    cpuPaneItems->cMuxTristateLabel->setState(cCMux);
    cpuPaneItems->ALULineEdit->setText(cALU == -1 ? "" : QString("%1").arg(cALU));
    cpuPaneItems->CCkCheckBox->setChecked(cCCk != -1);
    cpuPaneItems->VCkCheckBox->setChecked(cVCk != -1);
    cpuPaneItems->ANDZTristateLabel->setState(cANDZ);
    cpuPaneItems->ZCkCheckBox->setChecked(cZCk != -1);
    cpuPaneItems->NCkCheckBox->setChecked(cNCk != -1);
    cpuPaneItems->MemReadTristateLabel->setState(cMemRead);
    cpuPaneItems->MemWriteTristateLabel->setState(cMemWrite);
}

QString MicroCode::getObjectCode()
{
    QString str = "";
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
    str.append(cCCk == -1 ? "  " : QString("%1").arg(cCCk, -2));
    str.append(cVCk == -1 ? "  " : QString("%1").arg(cVCk, -2));
    str.append(cANDZ == -1 ? "  " : QString("%1").arg(cANDZ, -2));
    str.append(cZCk == -1 ? "  " : QString("%1").arg(cZCk, -2));
    str.append(cNCk == -1 ? "  " : QString("%1").arg(cNCk, -2));
    str.append(cMemWrite == -1 ? "  " : QString("%1").arg(cMemWrite, -2));
    str.append(cMemRead == -1 ? "  " : QString("%1").arg(cMemRead, -2));
    str.append("\n");
    return str;
}

QString MicroCode::getSourceCode()
{
    QString str = "";
    if (cMemRead != -1) { str.append("MemRead, "); }
    if (cMemWrite != -1) { str.append("MemWrite, "); }
    if (cA != -1) { str.append("A=" + QString("%1").arg(cA) + ", "); }
    if (cB != -1) { str.append("B=" + QString("%1").arg(cB) + ", "); }
    if (cAMux != -1) { str.append("AMux=" + QString("%1").arg(cAMux) + ", "); }
    if (cALU != -1) { str.append("ALU=" + QString("%1").arg(cALU) + ", "); }
    if (cANDZ != -1) { str.append("ANDZ=" + QString("%1").arg(cANDZ) + ", "); }
    if (cCMux != -1) { str.append("CMux=" + QString("%1").arg(cCMux) + ", "); }
    if (cMDRMux != -1) { str.append("MDRMux=" + QString("%1").arg(cMDRMux) + ", "); }
    if (cC != -1) { str.append("C=" + QString("%1").arg(cC) + ", "); }

    if (str != "") { str.chop(2); str.append("; "); }

    if (cNCk != -1) { str.append("NCk, "); }
    if (cZCk != -1) { str.append("ZCk, "); }
    if (cVCk != -1) { str.append("VCk, "); }
    if (cCCk != -1) { str.append("CCk, "); }
    if (cMARCk != -1) { str.append("MARCk, "); }
    if (cLoadCk != -1) { str.append("LoadCk, "); }
    if (cMDRCk != -1) { str.append("MDRCk, "); }

    if (str.endsWith(", ") || str.endsWith("; ")) { str.chop(2); }
    if (!cComment.isEmpty()) {
        str.append(" " + cComment);
    }
    return str;
}

bool MicroCode::has(Enu::EMnemonic field) {
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
    case Enu::CCk: return cCCk != -1;
    case Enu::VCk: return cVCk != -1;
    case Enu::ANDZ: return cANDZ != -1;
    case Enu::ZCk: return cZCk != -1;
    case Enu::NCk: return cNCk != -1;
    case Enu::MemWrite: return cMemWrite != -1;
    case Enu::MemRead: return cMemRead != -1;
    default: return true;
    }
}

void MicroCode::set(Enu::EMnemonic field, int value) {
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
    case Enu::CCk: cCCk = value; break;
    case Enu::VCk: cVCk = value; break;
    case Enu::ANDZ: cANDZ = value; break;
    case Enu::ZCk: cZCk = value; break;
    case Enu::NCk: cNCk = value; break;
    case Enu::MemWrite: cMemWrite = value; break;
    case Enu::MemRead: cMemRead = value; break;
    default: break;
    }
}

bool MicroCode::inRange(Enu::EMnemonic field, int value) {
    switch (field) {
    case Enu::C: return 0 <= value && value <= 31;
    case Enu::B: return 0 <= value && value <= 31;
    case Enu::A: return 0 <= value && value <= 31;
    case Enu::AMux: return 0 <= value && value <= 1;
    case Enu::MDRMux: return 0 <= value && value <= 1;
    case Enu::CMux: return 0 <= value && value <= 1;
    case Enu::ALU: return 0 <= value && value <= 15;
    case Enu::ANDZ: return 0 <= value && value <= 1;
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
