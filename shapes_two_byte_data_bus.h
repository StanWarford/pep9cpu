// File: shapes_two_byte_data_bus.h
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
#ifndef SHAPES_TWO_BYTE_DATA_BUS_H
#define SHAPES_TWO_BYTE_DATA_BUS_H

#include <QPair>
#include <QPoint>
#include <QLine>
#include <QVector>
#include <QPolygon>
#include <QImage>

#include <QVector>

#include "shapes_one_byte_data_bus.h"

namespace TwoByteShapes
{

// generic shapes:
enum Shapes {
    checkW    = 60,
    checkH    = 20,
    check2W   = 80,
    check2H   = checkH,
    labelW    = 42,
    labelH    = 20,
    dataLabelW = 69,
    dataLabelH = 19,
    labelTriW = 25,
    labelTriH = labelH,
    lineEditW = 25,
    lineEditH = 21,
    regLineEditW = 60,
    regLineEditH = 19,

    arrowHDepth = 20, // 20 with arrowHOffset, "really" 15
    arrowHOffset = 5,
    arrowLeftOff = 12,
    iRegXOffset = 50,
    MDREOOffset = 100,
    selectYOffset = 9,
    selectSlashOffset = 5,
    incrementerOffset = 10,

    aluSelOff = 57,
    selLineOff = 15,

};

enum CommonPositions {
    ctrlLabelX = 579 + 50,
    ctrlInputX = 550 + 50,
    interfaceRegsX = 175,
    combCircX = interfaceRegsX - iRegXOffset,
    statusBitsX = 526,//476,

};


// input/label/control section:
const QRect AddrBus = QRect(40, 151, 20, 600);
const QRect DataBus = QRect(AddrBus.x()+AddrBus.width(), AddrBus.top() + 100, 20, 500);

const QRect loadCkCheckbox          = QRect(ctrlInputX, 18, check2W,   check2H);
const QRect cLineEdit               = QRect(ctrlInputX, 39, lineEditW, lineEditH);
const QRect cLabel                  = QRect(ctrlLabelX, 41, labelW,    labelH);
const QRect bLineEdit               = QRect(ctrlInputX, 61, lineEditW, lineEditH);
const QRect bLabel                  = QRect(ctrlLabelX, 63, labelW,    labelH);
const QRect aLineEdit               = QRect(ctrlInputX, 83, lineEditW, lineEditH);
const QRect aLabel                  = QRect(ctrlLabelX, 85, labelW,    labelH);

const QRect MARCkCheckbox           = QRect(ctrlInputX, 169, check2W, check2H);
const QRect MARALabel               = QRect(combCircX, 202, dataLabelW, dataLabelH);
const QRect MARBLabel               = QRect(combCircX, 132, dataLabelW, dataLabelH);

const QRect MDROCkCheckbox          = QRect(ctrlInputX, 189, checkW+10, checkH);
const QRect MDRECkCheckbox          = QRect(ctrlInputX, 209, checkW+10, checkH);

const QRect MARMuxTristateLabel     = QRect(ctrlInputX, 149, labelTriW, labelTriH);
const QRect MDROMuxTristateLabel    = QRect(ctrlInputX, 245, labelTriW, labelTriH);
const QRect MDREMuxTristateLabel    = QRect(ctrlInputX, 265, labelTriW, labelTriH);
const QRect EOMuxTristateLabel      = QRect(ctrlInputX, 316, labelTriW, labelTriH);

const QRect MARMuxLabel             = QRect(ctrlLabelX, MARMuxTristateLabel.y(), labelW+20, labelH);
const QRect MDROMuxLabel            = QRect(ctrlLabelX, MDROMuxTristateLabel.y(), labelW+20, labelH);
const QRect MDREMuxLabel            = QRect(ctrlLabelX, MDREMuxTristateLabel.y(), labelW+20, labelH);
const QRect EOMuxLabel              = QRect(ctrlLabelX, EOMuxTristateLabel.y(), labelW, labelH);
const QRect EOMuxerDataLabel        = QRect(combCircX,350, dataLabelW, dataLabelH);

const QRect MDROMuxerDataLabel      = QRect(combCircX,293, dataLabelW, dataLabelH);
const QRect MDREMuxerDataLabel      = QRect(combCircX,393, dataLabelW, dataLabelH);

const QRect MDROLabel               = QRect(combCircX, 254, dataLabelW, dataLabelH);
const QRect MDRELabel               = QRect(combCircX, 354, dataLabelW, dataLabelH);

const QRect aMuxTristateLabel       = QRect(ctrlInputX, 295, labelTriW, 21);
const QRect aMuxLabel               = QRect(ctrlLabelX, aMuxTristateLabel.y(), labelW, labelH);
const QRect aMuxerDataLabel         = QRect(306, 293, dataLabelW, dataLabelH);
const QRect cMuxTristateLabel       = QRect(ctrlInputX, 348, labelTriW, labelTriH);
const QRect cMuxLabel               = QRect(ctrlLabelX, cMuxTristateLabel.y(), labelW, labelH);
const QRect cMuxerLabel             = QRect(250, 374, dataLabelW, dataLabelH);
const QRect ALULineEdit             = QRect(ctrlInputX,  368, 26,     lineEditH);
const QRect ALULabel                = QRect(ctrlLabelX,  370, 31,     labelH);
const QRect ALUFunctionLabel        = QRect(332, 355, 98, 20);

// status bits:
const QRect CSMuxLabel              = QRect(ctrlLabelX,  399, labelW, labelH);
const QRect CSMuxerDataLabel        = QRect(statusBitsX+19-69, 399, dataLabelW, dataLabelH);
const QRect CSMuxTristateLabel      = QRect(ctrlInputX,  399, 25,     21);
const QRect SCkCheckBox             = QRect(ctrlInputX,  437, checkW, checkH);
const QRect sBitLabel               = QRect(statusBitsX, 437, 19,     dataLabelH);
const QRect CCkCheckBox             = QRect(ctrlInputX,  464, checkW, checkH);
const QRect cBitLabel               = QRect(statusBitsX, 463, 19,     dataLabelH);
const QRect VCkCheckBox             = QRect(ctrlInputX,  491, checkW, checkH);
const QRect vBitLabel               = QRect(statusBitsX, 491, 19,     dataLabelH);
const QRect AndZLabel               = QRect(ctrlLabelX,  517, 45,     20);
const QRect AndZTristateLabel       = QRect(ctrlInputX, 517, labelTriW,labelTriH);
const QRect AndZMuxLabel            = QRect(416, 544, 41,21);
const QRect ZCkCheckBox             = QRect(ctrlInputX, 544, 60, 20);
const QRect zBitLabel               = QRect(statusBitsX, 544, 19, dataLabelH);
const QRect NCkCheckBox             = QRect(ctrlInputX, 582+4, checkW, checkH);
const QRect nBitLabel               = QRect(statusBitsX, 582+4, 19, dataLabelH);

const QRect MemWriteLabel           = QRect(ctrlLabelX, 711, check2W, check2H);
const QRect MemWriteTristateLabel   = QRect(ctrlInputX, 711, labelTriW, labelTriH);
const QRect MemReadLabel            = QRect(ctrlLabelX, 731, check2W, check2H);
const QRect MemReadTristateLabel    = QRect(ctrlInputX, 731, labelTriW, labelTriH);

const Arrow loadCkSelect = Arrow(QVector<QPoint>() << QPoint(499, 24),
                                 QVector<QLine>()  << QLine(ctrlInputX - 7,
                                                            loadCkCheckbox.y() + selectYOffset,
                                                            499,
                                                            loadCkCheckbox.y() + selectYOffset));
const Arrow CSelect = Arrow(QVector<QPoint>() << QPoint(499, 47),
                            QVector<QLine>()  <<  QLine(ctrlInputX - 7, cLabel.y() + selectYOffset,
                                                        499, cLabel.y() + selectYOffset)
                                              <<  QLine(523, cLabel.y() + selectYOffset - 5,
                                                        533, cLabel.y() + selectYOffset + 5));
const Arrow BSelect = Arrow(QVector<QPoint>() << QPoint(499, 69),
                            QVector<QLine>()  <<  QLine(ctrlInputX - 7, bLabel.y() + selectYOffset,
                                                        499, bLabel.y() + selectYOffset)
                                              <<  QLine(523, bLabel.y() + selectYOffset - selectSlashOffset,
                                                        533, bLabel.y() + selectYOffset + selectSlashOffset));
const Arrow ASelect = Arrow(QVector<QPoint>() << QPoint(499, 91),
                            QVector<QLine>()  <<  QLine(ctrlInputX - 7, aLabel.y() + selectYOffset,
                                                        499, aLabel.y() + selectYOffset)
                                              <<  QLine(523, aLabel.y() + selectYOffset - selectSlashOffset,
                                                        533, aLabel.y() + selectYOffset + selectSlashOffset));
const Arrow MARCk = Arrow(QVector<QPoint>() << QPoint(232-40,155)
                                        << QPoint(232-40,191),
                          QVector<QLine > () << QLine(428,177, 543,177)
                                        << QLine(367,177, 416,177)
                                        << QLine(291,177, 355,177)
                                        << QLine(235-40,177, 279,177)
                                        << QLine(235-40,163, 235-40,191));
//const Arrow MDRCk                   = OneByteShapes::MDRCk;
const Arrow AMuxSelect                = OneByteShapes::AMuxSelect;
const QPolygon AMuxBus                = OneByteShapes::AMuxBus;
const Arrow CMuxSelect                = OneByteShapes::CMuxSelect;
const QPolygon CMuxBus                = OneByteShapes::CMuxBus;
const QPolygon ALUPoly                = OneByteShapes::ALUPoly;
const QRect MDRBusOutRect             = OneByteShapes::MDRBusOutRect;
const QPolygon MDRBusOutArrow         = OneByteShapes::MDRBusOutArrow;
const QPolygon MARBus = QPolygon(QVector<QPoint>()
                                 << QPoint(combCircX + 30,151)
                                 << QPoint(combCircX + 30,167)
                                 // arrow:
                                 << QPoint(AddrBus.x()+AddrBus.width()+arrowHDepth,167)
                                 << QPoint(AddrBus.x()+AddrBus.width()+arrowHDepth,162)
                                 << QPoint(AddrBus.x()+AddrBus.width()+arrowHOffset,177)
                                 << QPoint(AddrBus.x()+AddrBus.width()+arrowHDepth,192)
                                 << QPoint(AddrBus.x()+AddrBus.width()+arrowHDepth,187)
                                 << QPoint(combCircX + 30,187)
                                 << QPoint(combCircX + 30,202)
                                 << QPoint(combCircX + 30 + 10,202)
                                 // black line in the middle:
                                 << QPoint(combCircX + 30 + 10,151)
                                 << QPoint(combCircX + 30 + 10,177)
                                 << QPoint(TwoByteShapes::AddrBus.right()+arrowHDepth,177)
                                 << QPoint(combCircX + 30 + 10,177)
                                 << QPoint(combCircX + 30 + 10,151));
const QPolygon NZVCDataPath = OneByteShapes::NZVCDataPath;

// registers
const QRect RegBank                         = OneByteShapes::RegBank;
const QRect aRegLineEdit                    = OneByteShapes::aRegLineEdit;
const QRect xRegLineEdit                    = OneByteShapes::xRegLineEdit;
const QRect spRegLineEdit                   = OneByteShapes::spRegLineEdit;
const QRect pcRegLineEdit                   = OneByteShapes::pcRegLineEdit;
const QRect irRegLineEdit                   = OneByteShapes::irRegLineEdit;
const QRect t1RegLineEdit                   = OneByteShapes::t1RegLineEdit;
const QRect t2RegLineEdit                   = OneByteShapes::t2RegLineEdit;
const QRect t3RegLineEdit                   = OneByteShapes::t3RegLineEdit;
const QRect t4RegLineEdit                   = OneByteShapes::t4RegLineEdit;
const QRect t5RegLineEdit                   = OneByteShapes::t5RegLineEdit;
const QRect t6RegLineEdit                   = OneByteShapes::t6RegLineEdit;
const QRect m1RegLabel                      = OneByteShapes::m1RegLabel;
const QRect m2RegLabel                      = OneByteShapes::m2RegLabel;
const QRect m3RegLabel                      = OneByteShapes::m3RegLabel;
const QRect m4RegLabel                      = OneByteShapes::m4RegLabel;
const QRect m5RegLabel                      = OneByteShapes::m5RegLabel;
const QPolygon BBus1;
const QRect BBusRect;
const QPolygon BBus2;
const QPolygon ABus1;
const QPolygon ABus2;
const QPolygon CBus;
const QPolygon AddrArrow                    = OneByteShapes::AddrArrow;
//const QPolygon DataToMDRMuxBus;
const QPolygon DataToMDROMuxBus = QPolygon(QVector<QPoint>()
                                           // foot:
                                           << QPoint(190 - iRegXOffset, 344)
                                           << QPoint(80,  344)
                                           << QPoint(80,  334)
                                           << QPoint(180 - iRegXOffset, 334)
                                           // arrowhead:
                                           << QPoint(180 - iRegXOffset, 326)
                                           << QPoint(175 - iRegXOffset, 326)
                                           << QPoint(185 - iRegXOffset, 316)
                                           << QPoint(195 - iRegXOffset, 326)
                                           << QPoint(190 - iRegXOffset, 326));

const QPolygon DataToMDREMuxBus = QPolygon(QVector<QPoint>()
                                           // foot:
                                           << QPoint(190 - iRegXOffset, 344+100)
                                           << QPoint(80,  344+100) << QPoint(80,  334+100)
                                           << QPoint(180 - iRegXOffset, 334+100)
                                           // arrowhead:
                                           << QPoint(180 - iRegXOffset, 326+100)
                                           << QPoint(175 - iRegXOffset, 326+100)
                                           << QPoint(185 - iRegXOffset, 316+100)
                                           << QPoint(195 - iRegXOffset, 326+100)
                                           << QPoint(190 - iRegXOffset, 326+100));
//const QPolygon MDRToDataBus;
const QPolygon MDROToDataBus = QPolygon(QVector<QPoint>()  << QPoint(MDROLabel.x(), 258)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 258)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 253)
                                        << QPoint(DataBus.x()+DataBus.width()+3, 263)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 273)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 268)
                                        << QPoint(MDROLabel.x(), 268));

const QPolygon MDREToDataBus = QPolygon(QVector<QPoint>()  << QPoint(MDROLabel.x(), 358)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 358)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 353)
                                        << QPoint(DataBus.x()+DataBus.width()+3, 363)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 373)
                                        << QPoint(DataBus.x()+DataBus.width()+13, 368)
                                        << QPoint(MDROLabel.x(), 368));

//const QPolygon MDRMuxOutBus;
const QPolygon MDROMuxOutBus = QPolygon(QVector<QPoint>()
                                        << QPoint(combCircX + 25 + 05, MDROMuxerDataLabel.y()) // 293
                                        << QPoint(combCircX + 25 + 05, MDROMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 00, MDROMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 10, MDROMuxerDataLabel.y() - 17)
                                        << QPoint(combCircX + 25 + 20, MDROMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 15, MDROMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 15, MDROMuxerDataLabel.y()));

const QPolygon MDREMuxOutBus = QPolygon(QVector<QPoint>()
                                        << QPoint(combCircX + 25 + 05, MDREMuxerDataLabel.y()) // 293
                                        << QPoint(combCircX + 25 + 05, MDREMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 00, MDREMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 10, MDREMuxerDataLabel.y() - 17)
                                        << QPoint(combCircX + 25 + 20, MDREMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 15, MDREMuxerDataLabel.y() - 7)
                                        << QPoint(combCircX + 25 + 15, MDREMuxerDataLabel.y()));



const QPolygon ALUOutBus;

const Arrow ALUSelect      = Arrow(QVector<QPoint>() <<
                                   QPoint(ALUPoly.boundingRect().right() - 13,
                                          ALUPoly.boundingRect().bottom() - 21),
                                   QVector<QLine>() << QLine(439,376,
                                                             ctrlInputX - 7,
                                                             ALULineEdit.y() + selectYOffset - 1)
                                   << QLine(ctrlInputX - 17,
                                            ALULineEdit.y() + 13,
                                            ctrlInputX - 27,
                                            ALULineEdit.y() + 3)); // diagonal line

const Arrow ALUSelectOut = Arrow(QVector<QPoint>() <<
                                 QPoint(nBitLabel.left() - arrowLeftOff,
                                        nBitLabel.y() + arrowHOffset + 1) << // N
                                 QPoint(AndZMuxLabel.left() - arrowLeftOff,
                                        zBitLabel.y() + arrowHOffset + 1) << // Z
                                 QPoint(vBitLabel.left() - arrowLeftOff,
                                        vBitLabel.y() + arrowHOffset + 1) << // V
                                 QPoint(cBitLabel.left() - arrowLeftOff,
                                        cBitLabel.y() + arrowHOffset + 1) << // C
                                 QPoint(sBitLabel.left() - arrowLeftOff,
                                        sBitLabel.y() + arrowHOffset + 1),   // S
                                 QVector<QLine>() <<
                                 // N
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff,
                                       ALUPoly.boundingRect().bottom(),
                                       ALUPoly.boundingRect().left() + aluSelOff,
                                       nBitLabel.y() + selectYOffset) << //586+8
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff,
                                       nBitLabel.y() + selectYOffset,
                                       nBitLabel.left() - arrowLeftOff,
                                       nBitLabel.y() + selectYOffset) << //586+8
                                 // Z
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff,
                                       ALUPoly.boundingRect().bottom(),
                                       ALUPoly.boundingRect().left() + aluSelOff + selLineOff,
                                       zBitLabel.y() + selectYOffset) <<
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff,
                                       zBitLabel.y() + selectYOffset,
                                       AndZMuxLabel.left() - arrowLeftOff,
                                       zBitLabel.y() + selectYOffset) <<

                                 // V
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 2,
                                       ALUPoly.boundingRect().bottom(),
                                       ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 2,
                                       vBitLabel.y() + selectYOffset) <<
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 2,
                                       vBitLabel.y() + selectYOffset,
                                       vBitLabel.left() - arrowLeftOff,
                                       vBitLabel.y() + selectYOffset) <<

                                 // C
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 3,
                                       ALUPoly.boundingRect().bottom(),
                                       ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 3,
                                       cBitLabel.y() + selectYOffset) <<
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 3,
                                       cBitLabel.y() + selectYOffset,
                                       cBitLabel.left() - arrowLeftOff,
                                       cBitLabel.y() + selectYOffset) <<
                                 // S
                                 QLine(ALUPoly.boundingRect().left() + aluSelOff + selLineOff * 3,
                                       sBitLabel.y() + selectYOffset,
                                       sBitLabel.left() - arrowLeftOff,
                                       sBitLabel.y() + selectYOffset)
                                 );


const QLine CSMuxSelect    = QLine(CSMuxLabel.right() + arrowHOffset - 120,
                                   CSMuxLabel.y() + selectYOffset +1,
                                   ctrlInputX - 7,
                                   CSMuxLabel.y() + selectYOffset + 1);
const QLine SBitSelect     = QLine(sBitLabel.right() + arrowHOffset,
                                   sBitLabel.y() + selectYOffset,
                                   ctrlInputX - 7,
                                   sBitLabel.y() + selectYOffset);
const QLine CBitSelect     = QLine(cBitLabel.right() + arrowHOffset,
                                   cBitLabel.y() + selectYOffset,
                                   ctrlInputX - 7,
                                   cBitLabel.y() + selectYOffset);
const QLine VBitSelect     = QLine(vBitLabel.right() + arrowHOffset,
                                   vBitLabel.y() + selectYOffset,
                                   ctrlInputX - 7,
                                   vBitLabel.y() + selectYOffset);
const QLine ZBitSelect     = QLine(zBitLabel.right() + arrowHOffset,
                                   zBitLabel.y() + selectYOffset,
                                   ctrlInputX - 7,
                                   zBitLabel.y() + selectYOffset);
const QLine NBitSelect     = QLine(nBitLabel.right() + arrowHOffset,
                                   nBitLabel.y() + selectYOffset,
                                   ctrlInputX - 7,
                                   nBitLabel.y() + selectYOffset);

const QLine MemReadSelect  = QLine(DataBus.right()   + arrowHOffset,
                                  MemReadLabel.y() + selectYOffset,
                                  ctrlInputX - 7,
                                  MemReadLabel.y() + selectYOffset);
const QLine MemWriteSelect = QLine(DataBus.right()   + arrowHOffset,
                                  MemWriteLabel.y() + selectYOffset,
                                  ctrlInputX - 7,
                                  MemWriteLabel.y() + selectYOffset);

}



#endif // SHAPES_TWO_BYTE_DATA_BUS_H

