// File: pep.cpp
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

#include "pep.h"
#include <QFile>
#include <QDebug>
#include <QString>
#include <QStringList>

using namespace Enu;

// Fonts
const QString Pep::codeFont = getSystem() == "Windows" ? "Courier" : (getSystem() == "Mac" ? "Courier" : "Courier 10 Pitch");
const int Pep::codeFontSize = getSystem() == "Mac" ? 11 : 10;
const QString Pep::labelFont = getSystem() == "Mac" ? "Lucida Grande" : "Verdana";
const int Pep::labelFontSize = getSystem() == "Mac" ? 11 : 8;
const QString Pep::cpuFont = getSystem() == "Mac" ? "Lucida Grande" : "Verdana";
const int Pep::cpuFontSize = getSystem() == "Mac" ? 11 : 8;

QString Pep::getSystem() {
    #ifdef Q_WS_X11
    return QString("Linux");
    #endif

    #ifdef Q_OS_OSX
    return QString("Mac");
    #endif

    #ifdef Q_WS_QWS
    return QString("Embedded Linux");
    #endif

    #ifdef Q_OS_WIN32
    return QString("Windows");
    #endif

    #ifdef Q_WS_WIN
    return QString("Windows");
    #endif

    return QString("No system");
}

QString Pep::resToString(QString fileName) {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString inString = "";
    while (!in.atEnd()) {
        QString line = in.readLine();
        inString.append(line + "\n");
    }
    QStringList microcodeList;
    microcodeList = inString.split('\n');
    for (int i = 0; i < microcodeList.size(); i++) {
        microcodeList[i].remove(QRegExp("^[0-9]+\\.?\\s*"));
    }
    inString = microcodeList.join("\n");
    return inString;
}

QString Pep::addCycleNumbers(QString codeString) {
    int lineNumber = 1;
    QStringList microcodeList = codeString.split("\n");
    for (int i = 0; i < microcodeList.size(); i++) {
        if (QRegExp("^//|^\\s*$|^unitpre|^unitpost", Qt::CaseInsensitive).indexIn(microcodeList.at(i)) != 0) {
            microcodeList[i].prepend(QString("%1. ").arg(lineNumber++));
        }
    }
    return microcodeList.join("\n");
}

QMap<Enu::EMnemonic, QString> Pep::decControlToMnemonMap;
QMap<Enu::EMnemonic, QString> Pep::memControlToMnemonMap;
QMap<Enu::EMnemonic, QString> Pep::clockControlToMnemonMap;
QMap<Enu::EMnemonic, QString> Pep::specificationToMnemonMap;
QMap<Enu::EMnemonic, QString> Pep::memSpecToMnemonMap;
QMap<Enu::EMnemonic, QString> Pep::regSpecToMnemonMap;
QMap<Enu::EMnemonic, QString> Pep::statusSpecToMnemonMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToDecControlMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToMemControlMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToClockControlMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToSpecificationMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToMemSpecMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToRegSpecMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToStatusSpecMap;

void Pep::initEnumMnemonMaps()
{
    decControlToMnemonMap.insert(C, "C"); mnemonToDecControlMap.insert("C", C);
    decControlToMnemonMap.insert(B, "B"); mnemonToDecControlMap.insert("B", B);
    decControlToMnemonMap.insert(A, "A"); mnemonToDecControlMap.insert("A", A);
    decControlToMnemonMap.insert(ANDZ, "ANDZ"); mnemonToDecControlMap.insert("ANDZ", ANDZ);
    decControlToMnemonMap.insert(MDRMux, "MDRMux"); mnemonToDecControlMap.insert("MDRMUX", MDRMux);
    decControlToMnemonMap.insert(AMux, "AMux"); mnemonToDecControlMap.insert("AMUX", AMux);
    decControlToMnemonMap.insert(CMux, "CMux"); mnemonToDecControlMap.insert("CMUX", CMux);
    decControlToMnemonMap.insert(ALU, "ALU"); mnemonToDecControlMap.insert("ALU", ALU);
    decControlToMnemonMap.insert(ANDZ, "ANDZ"); mnemonToDecControlMap.insert("ANDZ", ANDZ);
    memControlToMnemonMap.insert(MemWrite, "MemWrite"); mnemonToMemControlMap.insert("MEMWRITE", MemWrite);
    memControlToMnemonMap.insert(MemRead, "MemRead"); mnemonToMemControlMap.insert("MEMREAD", MemRead);
    clockControlToMnemonMap.insert(LoadCk, "LoadCk"); mnemonToClockControlMap.insert("LOADCK", LoadCk);
    clockControlToMnemonMap.insert(MARCk, "MARCk"); mnemonToClockControlMap.insert("MARCK", MARCk);
    clockControlToMnemonMap.insert(MDRCk, "MDRCk"); mnemonToClockControlMap.insert("MDRCK", MDRCk);
    clockControlToMnemonMap.insert(CCk, "CCk"); mnemonToClockControlMap.insert("CCK", CCk);
    clockControlToMnemonMap.insert(VCk, "VCk"); mnemonToClockControlMap.insert("VCK", VCk);
    clockControlToMnemonMap.insert(ZCk, "ZCk"); mnemonToClockControlMap.insert("ZCK", ZCk);
    clockControlToMnemonMap.insert(NCk, "NCk"); mnemonToClockControlMap.insert("NCK", NCk);
    specificationToMnemonMap.insert(Pre, "UnitPre:"); mnemonToSpecificationMap.insert("UNITPRE:", Pre);
    specificationToMnemonMap.insert(Post, "UnitPost:"); mnemonToSpecificationMap.insert("UNITPOST:", Post);
    memSpecToMnemonMap.insert(Mem, "Mem"); mnemonToMemSpecMap.insert("MEM", Mem);
    regSpecToMnemonMap.insert(A, "A"); mnemonToRegSpecMap.insert("A", A);
    regSpecToMnemonMap.insert(X, "X"); mnemonToRegSpecMap.insert("X", X);
    regSpecToMnemonMap.insert(SP, "SP"); mnemonToRegSpecMap.insert("SP", SP);
    regSpecToMnemonMap.insert(PC, "PC"); mnemonToRegSpecMap.insert("PC", PC);
    regSpecToMnemonMap.insert(IR, "IR"); mnemonToRegSpecMap.insert("IR", IR);
    regSpecToMnemonMap.insert(T1, "T1"); mnemonToRegSpecMap.insert("T1", T1);
    regSpecToMnemonMap.insert(T2, "T2"); mnemonToRegSpecMap.insert("T2", T2);
    regSpecToMnemonMap.insert(T3, "T3"); mnemonToRegSpecMap.insert("T3", T3);
    regSpecToMnemonMap.insert(T4, "T4"); mnemonToRegSpecMap.insert("T4", T4);
    regSpecToMnemonMap.insert(T5, "T5"); mnemonToRegSpecMap.insert("T5", T5);
    regSpecToMnemonMap.insert(T6, "T6"); mnemonToRegSpecMap.insert("T6", T6);
    regSpecToMnemonMap.insert(MARA, "MARA"); mnemonToRegSpecMap.insert("MARA", MARA);
    regSpecToMnemonMap.insert(MARB, "MARB"); mnemonToRegSpecMap.insert("MARB", MARB);
    regSpecToMnemonMap.insert(MDR, "MDR"); mnemonToRegSpecMap.insert("MDR", MDR);
    statusSpecToMnemonMap.insert(N, "N"); mnemonToStatusSpecMap.insert("N", N);
    statusSpecToMnemonMap.insert(Z, "Z"); mnemonToStatusSpecMap.insert("Z", Z);
    statusSpecToMnemonMap.insert(V, "V"); mnemonToStatusSpecMap.insert("V", V);
    statusSpecToMnemonMap.insert(C, "C"); mnemonToStatusSpecMap.insert("C", C);
}
