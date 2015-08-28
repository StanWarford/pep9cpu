// File: pep.h
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
#ifndef PEP_H
#define PEP_H

#include <QString>
#include <QMap>

#include "enu.h"

class Pep
{
public:
    // Fonts:
    static const QString codeFont;
    static const int codeFontSize;
    static const int codeFontSizeSmall;
    static const int codeFontSizeLarge;
    static const QString labelFont;
    static const int labelFontSize;
    static const int labelFontSizeSmall;
    static const QString cpuFont;
    static const int cpuFontSize;

    static QString getSystem();

    // Function to read text from a resource file
    static QString resToString(QString fileName);
    static QString addCycleNumbers(QString codeString);

    // Machine model state:
    static Enu::CPUType cpuFeatures;

    // Maps between mnemonic enums and strings
    static QMap<Enu::EMnemonic, QString> decControlToMnemonMap;  // unused as of this writing
    static QMap<Enu::EMnemonic, QString> memControlToMnemonMap;
    static QMap<Enu::EMnemonic, QString> clockControlToMnemonMap;
    static QMap<Enu::EMnemonic, QString> specificationToMnemonMap;
    static QMap<Enu::EMnemonic, QString> memSpecToMnemonMap;
    static QMap<Enu::EMnemonic, QString> regSpecToMnemonMap;
    static QMap<Enu::EMnemonic, QString> statusSpecToMnemonMap;
    static QMap<QString, Enu::EMnemonic> mnemonToDecControlMap;
    static QMap<QString, Enu::EMnemonic> mnemonToMemControlMap;
    static QMap<QString, Enu::EMnemonic> mnemonToClockControlMap;
    static QMap<QString, Enu::EMnemonic> mnemonToSpecificationMap;
    static QMap<QString, Enu::EMnemonic> mnemonToMemSpecMap;
    static QMap<QString, Enu::EMnemonic> mnemonToRegSpecMap;
    static QMap<QString, Enu::EMnemonic> mnemonToStatusSpecMap;
    static void initEnumMnemonMaps();

};

#endif // PEP_H
