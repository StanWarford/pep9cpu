// File: pep.cpp
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

#include <QFile>
#include <QDebug>
#include <QString>
#include <QStringList>

#include "pep.h"

using namespace Enu;

// Fonts
const QString Pep::codeFont = getSystem() == "Windows" ? "Courier" : (getSystem() == "Mac" ? "Courier" : "Ubuntu Mono,Courier New");
const int Pep::codeFontSize = getSystem() == "Windows" ? 9 : (getSystem() == "Mac" ? 11 : 11);
const int Pep::codeFontSizeSmall = getSystem() == "Windows" ? 7 : (getSystem() == "Mac" ? 10 : 10);
const int Pep::codeFontSizeLarge = getSystem() == "Windows" ? 9 : (getSystem() == "Mac" ? 11 : 11);
const QString Pep::labelFont = getSystem() == "Windows" ? "Verdana" : (getSystem() == "Mac" ? "Lucida Grande" : "Ubuntu");
const int Pep::labelFontSize = getSystem() == "Windows" ? 8 : (getSystem() == "Mac" ? 11 : 9);
const int Pep::labelFontSizeSmall = getSystem() == "Windows" ? 7 : (getSystem() == "Mac" ? 10 : 8);
const QString Pep::cpuFont = getSystem() == "Windows" ? "Verdana" : (getSystem() == "Mac" ? "Lucida Grande" : "Ubuntu");
const int Pep::cpuFontSize = getSystem() == "Windows" ? 8 : (getSystem() == "Mac" ? 11 : 8);

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

// Machine model state:
Enu::CPUType Pep::cpuFeatures = OneByteDataBus;

//QMap<Enu::EMnemonic, QString> Pep::decControlToMnemonMap; // unused as of this writing
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
    mnemonToDecControlMap.clear();
    mnemonToDecControlMap.insert("C", C);
    mnemonToDecControlMap.insert("B", B);
    mnemonToDecControlMap.insert("A", A);
    mnemonToDecControlMap.insert("ANDZ", AndZ);
    mnemonToDecControlMap.insert("AMUX", AMux);
    mnemonToDecControlMap.insert("CMUX", CMux);
    mnemonToDecControlMap.insert("ALU", ALU);
    mnemonToDecControlMap.insert("CSMUX", CSMux);
    if (Pep::cpuFeatures == OneByteDataBus) {
        mnemonToDecControlMap.insert("MDRMUX", MDRMux);
    }
    else if (Pep::cpuFeatures == TwoByteDataBus){
        mnemonToDecControlMap.insert("MARMUX", MARMux);
        mnemonToDecControlMap.insert("MDROMUX", MDROMux);
        mnemonToDecControlMap.insert("MDREMUX", MDREMux);
        mnemonToDecControlMap.insert("EOMUX", EOMux);
    }

    memControlToMnemonMap.clear();                      mnemonToMemControlMap.clear();
    memControlToMnemonMap.insert(MemWrite, "MemWrite"); mnemonToMemControlMap.insert("MEMWRITE", MemWrite);
    memControlToMnemonMap.insert(MemRead, "MemRead");   mnemonToMemControlMap.insert("MEMREAD", MemRead);

    clockControlToMnemonMap.clear();                    mnemonToClockControlMap.clear();
    clockControlToMnemonMap.insert(LoadCk, "LoadCk");   mnemonToClockControlMap.insert("LOADCK", LoadCk);
    clockControlToMnemonMap.insert(MARCk, "MARCk");     mnemonToClockControlMap.insert("MARCK", MARCk);
    clockControlToMnemonMap.insert(SCk, "SCk");         mnemonToClockControlMap.insert("SCK", SCk);
    clockControlToMnemonMap.insert(CCk, "CCk");         mnemonToClockControlMap.insert("CCK", CCk);
    clockControlToMnemonMap.insert(VCk, "VCk");         mnemonToClockControlMap.insert("VCK", VCk);
    clockControlToMnemonMap.insert(ZCk, "ZCk");         mnemonToClockControlMap.insert("ZCK", ZCk);
    clockControlToMnemonMap.insert(NCk, "NCk");         mnemonToClockControlMap.insert("NCK", NCk);
    if (Pep::cpuFeatures == OneByteDataBus) {
        clockControlToMnemonMap.insert(MDRCk, "MDRCk");     mnemonToClockControlMap.insert("MDRCK", MDRCk);
    }
    else if (Pep::cpuFeatures == TwoByteDataBus){
        clockControlToMnemonMap.insert(MDROCk, "MDROCk");     mnemonToClockControlMap.insert("MDROCK", MDROCk);
        clockControlToMnemonMap.insert(MDRECk, "MDRECk");     mnemonToClockControlMap.insert("MDRECK", MDRECk);
    }

    specificationToMnemonMap.clear();                   mnemonToSpecificationMap.clear();
    specificationToMnemonMap.insert(Pre, "UnitPre:");   mnemonToSpecificationMap.insert("UNITPRE:", Pre);
    specificationToMnemonMap.insert(Post, "UnitPost:"); mnemonToSpecificationMap.insert("UNITPOST:", Post);

    memSpecToMnemonMap.clear();                         mnemonToMemSpecMap.clear();
    memSpecToMnemonMap.insert(Mem, "Mem");              mnemonToMemSpecMap.insert("MEM", Mem);

    regSpecToMnemonMap.clear();                         mnemonToRegSpecMap.clear();
    regSpecToMnemonMap.insert(A, "A");                  mnemonToRegSpecMap.insert("A", A);
    regSpecToMnemonMap.insert(X, "X");                  mnemonToRegSpecMap.insert("X", X);
    regSpecToMnemonMap.insert(SP, "SP");                mnemonToRegSpecMap.insert("SP", SP);
    regSpecToMnemonMap.insert(PC, "PC");                mnemonToRegSpecMap.insert("PC", PC);
    regSpecToMnemonMap.insert(IR, "IR");                mnemonToRegSpecMap.insert("IR", IR);
    regSpecToMnemonMap.insert(T1, "T1");                mnemonToRegSpecMap.insert("T1", T1);
    regSpecToMnemonMap.insert(T2, "T2");                mnemonToRegSpecMap.insert("T2", T2);
    regSpecToMnemonMap.insert(T3, "T3");                mnemonToRegSpecMap.insert("T3", T3);
    regSpecToMnemonMap.insert(T4, "T4");                mnemonToRegSpecMap.insert("T4", T4);
    regSpecToMnemonMap.insert(T5, "T5");                mnemonToRegSpecMap.insert("T5", T5);
    regSpecToMnemonMap.insert(T6, "T6");                mnemonToRegSpecMap.insert("T6", T6);
    regSpecToMnemonMap.insert(MARA, "MARA");            mnemonToRegSpecMap.insert("MARA", MARA);
    regSpecToMnemonMap.insert(MARB, "MARB");            mnemonToRegSpecMap.insert("MARB", MARB);
    regSpecToMnemonMap.insert(MDR, "MDR");              mnemonToRegSpecMap.insert("MDR", MDR);

    statusSpecToMnemonMap.clear();                      mnemonToStatusSpecMap.clear();
    statusSpecToMnemonMap.insert(N, "N");               mnemonToStatusSpecMap.insert("N", N);
    statusSpecToMnemonMap.insert(Z, "Z");               mnemonToStatusSpecMap.insert("Z", Z);
    statusSpecToMnemonMap.insert(V, "V");               mnemonToStatusSpecMap.insert("V", V);
    statusSpecToMnemonMap.insert(C, "C");               mnemonToStatusSpecMap.insert("C", C);
    statusSpecToMnemonMap.insert(S, "S");               mnemonToStatusSpecMap.insert("S", S);
}
