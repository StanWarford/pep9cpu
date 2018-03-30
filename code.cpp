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
#include <QMetaEnum>
MicroCode::MicroCode():mnemonicMap()
{
    for(auto memLines : Pep::memControlToMnemonMap.keys())
    {
        mnemonicMap[memLines]=-1;
    }
    for(auto mainCtrlLines : Pep::decControlToMnemonMap.keys())
    {
        mnemonicMap[mainCtrlLines]=-1;
    }
    for(auto clockLines : Pep::clockControlToMnemonMap.keys())
    {
        mnemonicMap[clockLines]=-1;
    }
}

bool MicroCode::isMicrocode() { return true; }

void MicroCode::setCpuLabels(CpuGraphicsItems *cpuPaneItems)
{
    cpuPaneItems->loadCk->setChecked(mnemonicMap[Enu::LoadCk] == 1);
    cpuPaneItems->cLineEdit->setText(mnemonicMap[Enu::C] == -1 ? "" : QString("%1").arg(mnemonicMap[Enu::C]));
    cpuPaneItems->bLineEdit->setText(mnemonicMap[Enu::B] == -1 ? "" : QString("%1").arg(mnemonicMap[Enu::B]));
    cpuPaneItems->aLineEdit->setText(mnemonicMap[Enu::A] == -1 ? "" : QString("%1").arg(mnemonicMap[Enu::A]));
    cpuPaneItems->MARCk->setChecked(mnemonicMap[Enu::MARCk] == 1);
    cpuPaneItems->MDRCk->setChecked(mnemonicMap[Enu::MDRCk] == 1);
    cpuPaneItems->MDRECk->setChecked(mnemonicMap[Enu::MDRECk] == 1);
    cpuPaneItems->MDROCk->setChecked(mnemonicMap[Enu::MDROCk] == 1);
    cpuPaneItems->aMuxTristateLabel->setState(mnemonicMap[Enu::AMux]);
    cpuPaneItems->MDRMuxTristateLabel->setState(mnemonicMap[Enu::MDRMux]);
    cpuPaneItems->MDREMuxTristateLabel->setState(mnemonicMap[Enu::MDREMux]);
    cpuPaneItems->MDROMuxTristateLabel->setState(mnemonicMap[Enu::MDROMux]);
    cpuPaneItems->EOMuxTristateLabel->setState(mnemonicMap[Enu::EOMux]);
    cpuPaneItems->MARMuxTristateLabel->setState(mnemonicMap[Enu::MARMux]);
    cpuPaneItems->cMuxTristateLabel->setState(mnemonicMap[Enu::CMux]);
    cpuPaneItems->ALULineEdit->setText(mnemonicMap[Enu::ALU] == -1 ? "" : QString("%1").arg(mnemonicMap[Enu::ALU]));
    cpuPaneItems->CSMuxTristateLabel->setState(mnemonicMap[Enu::CSMux]);
    cpuPaneItems->SCkCheckBox->setChecked(mnemonicMap[Enu::SCk] == 1);
    cpuPaneItems->CCkCheckBox->setChecked(mnemonicMap[Enu::CCk] == 1);
    cpuPaneItems->VCkCheckBox->setChecked(mnemonicMap[Enu::VCk] == 1);
    cpuPaneItems->AndZTristateLabel->setState(mnemonicMap[Enu::AndZ]);
    cpuPaneItems->ZCkCheckBox->setChecked(mnemonicMap[Enu::ZCk] == 1);
    cpuPaneItems->NCkCheckBox->setChecked(mnemonicMap[Enu::NCk] == 1);
    cpuPaneItems->MemReadTristateLabel->setState(mnemonicMap[Enu::MemRead]);
    cpuPaneItems->MemWriteTristateLabel->setState(mnemonicMap[Enu::MemWrite]);
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
        str.append(mnemonicMap[Enu::LoadCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::LoadCk], -2));
        str.append(mnemonicMap[Enu::C] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::C], -3));
        str.append(mnemonicMap[Enu::B] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::B], -3));
        str.append(mnemonicMap[Enu::A] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::A], -3));
        str.append(mnemonicMap[Enu::MARCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MARCk], -2));
        str.append(mnemonicMap[Enu::MDRCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MDRCk], -2));
        str.append(mnemonicMap[Enu::AMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::AMux], -2));
        str.append(mnemonicMap[Enu::MDRMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MDRMux], -2));
        str.append(mnemonicMap[Enu::CMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::CMux], -2));
        str.append(mnemonicMap[Enu::ALU] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::ALU], -3));
        str.append(mnemonicMap[Enu::CSMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::CSMux], -2));
        str.append(mnemonicMap[Enu::SCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::SCk], -2));
        str.append(mnemonicMap[Enu::CCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::CCk], -2));
        str.append(mnemonicMap[Enu::VCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::VCk], -2));
        str.append(mnemonicMap[Enu::AndZ] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::AndZ], -2));
        str.append(mnemonicMap[Enu::ZCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::ZCk], -2));
        str.append(mnemonicMap[Enu::NCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::NCk], -2));
        str.append(mnemonicMap[Enu::MemWrite] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MemWrite], -2));
        str.append(mnemonicMap[Enu::MemRead] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MemRead], -2));
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        str.append(mnemonicMap[Enu::LoadCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::LoadCk], -2));
        str.append(mnemonicMap[Enu::C] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::C], -3));
        str.append(mnemonicMap[Enu::B] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::B], -3));
        str.append(mnemonicMap[Enu::A] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::A], -3));
        str.append(mnemonicMap[Enu::MARMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MARMux], -2));
        str.append(mnemonicMap[Enu::MARCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MARCk], -2));
        str.append(mnemonicMap[Enu::MDROCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MDROCk], -2));
        str.append(mnemonicMap[Enu::MDROMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MDROMux], -2));
        str.append(mnemonicMap[Enu::MDRECk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MDRECk], -2));
        str.append(mnemonicMap[Enu::MDREMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MDREMux], -2));
        str.append(mnemonicMap[Enu::EOMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::EOMux], -2));
        str.append(mnemonicMap[Enu::AMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::AMux], -2));
        str.append(mnemonicMap[Enu::CMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::CMux], -2));
        str.append(mnemonicMap[Enu::ALU] == -1 ? "   " : QString("%1").arg(mnemonicMap[Enu::ALU], -3));
        str.append(mnemonicMap[Enu::CSMux] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::CSMux], -2));
        str.append(mnemonicMap[Enu::SCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::SCk], -2));
        str.append(mnemonicMap[Enu::CCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::CCk], -2));
        str.append(mnemonicMap[Enu::VCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::VCk], -2));
        str.append(mnemonicMap[Enu::AndZ] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::AndZ], -2));
        str.append(mnemonicMap[Enu::ZCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::ZCk], -2));
        str.append(mnemonicMap[Enu::NCk] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::NCk], -2));
        str.append(mnemonicMap[Enu::MemWrite] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MemWrite], -2));
        str.append(mnemonicMap[Enu::MemRead] == -1 ? "  " : QString("%1").arg(mnemonicMap[Enu::MemRead], -2));
    }
    str.append("\n");
    return str;
}

QString MicroCode::getSourceCode()
{
    QString str = "";
    if (Pep::cpuFeatures == Enu::OneByteDataBus) {
        if (mnemonicMap[Enu::MemRead] != -1) { str.append("MemRead, "); }
        if (mnemonicMap[Enu::MemWrite] != -1) { str.append("MemWrite, "); }
        if (mnemonicMap[Enu::A] != -1) { str.append("A=" + QString("%1").arg(mnemonicMap[Enu::A]) + ", "); }
        if (mnemonicMap[Enu::B] != -1) { str.append("B=" + QString("%1").arg(mnemonicMap[Enu::B]) + ", "); }
        if (mnemonicMap[Enu::AMux] != -1) { str.append("AMux=" + QString("%1").arg(mnemonicMap[Enu::AMux]) + ", "); }
        if (mnemonicMap[Enu::CSMux]  != -1) { str.append("CSMux=" + QString("%1").arg(mnemonicMap[Enu::CSMux]) + ", "); }
        if (mnemonicMap[Enu::ALU] != -1) { str.append("ALU=" + QString("%1").arg(mnemonicMap[Enu::ALU]) + ", "); }
        if (mnemonicMap[Enu::AndZ] != -1) { str.append("AndZ=" + QString("%1").arg(mnemonicMap[Enu::AndZ]) + ", "); }
        if (mnemonicMap[Enu::CMux] != -1) { str.append("CMux=" + QString("%1").arg(mnemonicMap[Enu::CMux]) + ", "); }
        if (mnemonicMap[Enu::MDRMux] != -1) { str.append("MDRMux=" + QString("%1").arg(mnemonicMap[Enu::MDRMux]) + ", "); }
        if (mnemonicMap[Enu::C] != -1) { str.append("C=" + QString("%1").arg(mnemonicMap[Enu::C]) + ", "); }

        if (str != "") { str.chop(2); str.append("; "); }

        if (mnemonicMap[Enu::NCk] != -1) { str.append("NCk, "); }
        if (mnemonicMap[Enu::ZCk] != -1) { str.append("ZCk, "); }
        if (mnemonicMap[Enu::VCk] != -1) { str.append("VCk, "); }
        if (mnemonicMap[Enu::CCk] != -1) { str.append("CCk, "); }
        if (mnemonicMap[Enu::SCk] != -1) { str.append("SCk, "); }
        if (mnemonicMap[Enu::MARCk] != -1) { str.append("MARCk, "); }
        if (mnemonicMap[Enu::LoadCk] != -1) { str.append("LoadCk, "); }
        if (mnemonicMap[Enu::MDRCk] != -1) { str.append("MDRCk, "); }

        if (str.endsWith(", ") || str.endsWith("; ")) { str.chop(2); }
        if (!cComment.isEmpty()) {
            str.append(" " + cComment);
        }
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        if (mnemonicMap[Enu::MemRead] != -1) { str.append("MemRead, "); }
        if (mnemonicMap[Enu::MemWrite] != -1) { str.append("MemWrite, "); }
        if (mnemonicMap[Enu::A] != -1) { str.append("A=" + QString("%1").arg(mnemonicMap[Enu::A]) + ", "); }
        if (mnemonicMap[Enu::B] != -1) { str.append("B=" + QString("%1").arg(mnemonicMap[Enu::B]) + ", "); }
        if (mnemonicMap[Enu::MARMux] != -1) { str.append("MARMux=" + QString("%1").arg(mnemonicMap[Enu::MARMux]) + ", "); }
        if (mnemonicMap[Enu::EOMux] != -1) { str.append("EOMux=" + QString("%1").arg(mnemonicMap[Enu::EOMux]) + ", "); }
        if (mnemonicMap[Enu::AMux] != -1) { str.append("AMux=" + QString("%1").arg(mnemonicMap[Enu::AMux]) + ", "); }
        if (mnemonicMap[Enu::CSMux]  != -1) { str.append("CSMux=" + QString("%1").arg(mnemonicMap[Enu::CSMux]) + ", "); }
        if (mnemonicMap[Enu::ALU] != -1) { str.append("ALU=" + QString("%1").arg(mnemonicMap[Enu::ALU]) + ", "); }
        if (mnemonicMap[Enu::AndZ] != -1) { str.append("AndZ=" + QString("%1").arg(mnemonicMap[Enu::AndZ]) + ", "); }
        if (mnemonicMap[Enu::CMux] != -1) { str.append("CMux=" + QString("%1").arg(mnemonicMap[Enu::CMux]) + ", "); }
        if (mnemonicMap[Enu::MDREMux] != -1) { str.append("MDREMux=" + QString("%1").arg(mnemonicMap[Enu::MDREMux]) + ", "); }
        if (mnemonicMap[Enu::MDROMux] != -1) { str.append("MDROMux=" + QString("%1").arg(mnemonicMap[Enu::MDROMux]) + ", "); }
        if (mnemonicMap[Enu::C] != -1) { str.append("C=" + QString("%1").arg(mnemonicMap[Enu::C]) + ", "); }

        if (str != "") { str.chop(2); str.append("; "); }

        if (mnemonicMap[Enu::NCk] != -1) { str.append("NCk, "); }
        if (mnemonicMap[Enu::VCk] != -1) { str.append("ZCk, "); }
        if (mnemonicMap[Enu::VCk] != -1) { str.append("VCk, "); }
        if (mnemonicMap[Enu::CCk] != -1) { str.append("CCk, "); }
        if (mnemonicMap[Enu::SCk] != -1) { str.append("SCk, "); }
        if (mnemonicMap[Enu::MARCk] != -1) { str.append("MARCk, "); }
        if (mnemonicMap[Enu::LoadCk] != -1) { str.append("LoadCk, "); }
        if (mnemonicMap[Enu::MDRECk] != -1) { str.append("MDRECk, "); }
        if (mnemonicMap[Enu::MDROCk] != -1) { str.append("MDROCk, "); }

        if (str.endsWith(", ") || str.endsWith("; ")) { str.chop(2); }
        if (!cComment.isEmpty()) {
            str.append(" " + cComment);
        }
    }
    return str;
}

bool MicroCode::has(Enu::EMnemonic field) {
    return mnemonicMap[field]!=-1;
}

void MicroCode::set(Enu::EMnemonic field, int value) {
    mnemonicMap[field]=value;
}

int MicroCode::get(Enu::EMnemonic field) const
{
    return mnemonicMap[field];
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
