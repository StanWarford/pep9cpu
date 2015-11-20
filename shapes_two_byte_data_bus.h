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

    arrowHDepth = 20, // really 15, but 20 with arrowHOffset
    arrowHOffset = 5,
    iRegXOffset = 40,
    MDREOOffset = 100
};

enum CommonPositions {
    ctrlLabelX = 579,
    ctrlInputX = 550,
    interfaceRegsX = 175,
    combCircX = interfaceRegsX - iRegXOffset,
    statusBitsX = 476,

};


// input/label/control section:
const QRect AddrBus = OneByteShapes::AddrBus;
const QRect DataBus = QRect(AddrBus.x()+AddrBus.width(), AddrBus.top() + 100, 20, 399);

const QRect loadCkCheckbox              = OneByteShapes::loadCkCheckbox;
const QRect cLineEdit                   = OneByteShapes::cLineEdit;
const QRect cLabel                      = OneByteShapes::cLabel;
const QRect bLineEdit                   = OneByteShapes::bLineEdit;
const QRect bLabel                      = OneByteShapes::bLabel;
const QRect aLineEdit                   = OneByteShapes::aLineEdit;
const QRect aLabel                      = OneByteShapes::aLabel;
const QRect MARCkCheckbox               = OneByteShapes::MARCkCheckbox; // I think this stays the same
const QRect MARALabel                   = QRect(combCircX, 202, dataLabelW, dataLabelH);
const QRect MARBLabel                   = QRect(combCircX, 132, dataLabelW, dataLabelH);

const QRect MDROCkCheckbox       = QRect(ctrlInputX, 189, checkW+10, checkH);
const QRect MDRECkCheckbox       = QRect(ctrlInputX, 209, checkW+10, checkH);

const QRect MARMuxTristateLabel  = QRect(ctrlInputX, 149, labelTriW, labelTriH);
const QRect MDROMuxTristateLabel = QRect(ctrlInputX, 245, labelTriW, labelTriH);
const QRect MDREMuxTristateLabel = QRect(ctrlInputX, 265, labelTriW, labelTriH);
const QRect EOMuxTristateLabel   = QRect(ctrlInputX, 316, labelTriW, labelTriH);

const QRect MARMuxLabel          = QRect(ctrlLabelX, MARMuxTristateLabel.y(), labelW+20, labelH);
const QRect MDROMuxLabel         = QRect(ctrlLabelX, MDROMuxTristateLabel.y(), labelW+20, labelH);
const QRect MDREMuxLabel         = QRect(ctrlLabelX, MDREMuxTristateLabel.y(), labelW+20, labelH);
const QRect EOMuxLabel           = QRect(ctrlLabelX, EOMuxTristateLabel.y(), labelW, labelH);

const QRect MDROMuxerDataLabel   = QRect(combCircX,293, dataLabelW, dataLabelH);
const QRect MDREMuxerDataLabel   = QRect(combCircX,393, dataLabelW, dataLabelH);
const QRect EOMuxerDataLabel;

const QRect MDROLabel = QRect(combCircX, 254, dataLabelW, dataLabelH);
const QRect MDRELabel = QRect(combCircX, 354, dataLabelW, dataLabelH);


const QRect aMuxLabel                 = OneByteShapes::aMuxLabel;
const QRect aMuxerDataLabel           = OneByteShapes::aMuxerDataLabel;
const QRect aMuxTristateLabel         = OneByteShapes::aMuxTristateLabel;
const QRect cMuxLabel                 = OneByteShapes::cMuxLabel;
const QRect cMuxerLabel               = OneByteShapes::cMuxerLabel;
const QRect cMuxTristateLabel         = OneByteShapes::cMuxTristateLabel;
const QRect ALULineEdit               = OneByteShapes::ALULineEdit;
const QRect ALULabel                  = OneByteShapes::ALULabel;
const QRect ALUFunctionLabel          = OneByteShapes::ALUFunctionLabel;
const QRect CSMuxLabel                = OneByteShapes::CSMuxLabel;
const QRect CSMuxerDataLabel          = OneByteShapes::CSMuxerDataLabel;
const QRect CSMuxTristateLabel        = OneByteShapes::CSMuxTristateLabel;
const QRect SCkCheckBox               = OneByteShapes::SCkCheckBox;
const QRect sBitLabel                 = OneByteShapes::sBitLabel;
const QRect CCkCheckBox               = OneByteShapes::CCkCheckBox;
const QRect cBitLabel                 = OneByteShapes::cBitLabel;
const QRect VCkCheckBox               = OneByteShapes::VCkCheckBox;
const QRect vBitLabel                 = OneByteShapes::vBitLabel;
const QRect AndZLabel                 = OneByteShapes::AndZLabel;
const QRect AndZTristateLabel         = OneByteShapes::AndZTristateLabel;
const QRect AndZMuxLabel              = OneByteShapes::AndZMuxLabel;
const QRect ZCkCheckBox               = OneByteShapes::ZCkCheckBox;
const QRect zBitLabel                 = OneByteShapes::zBitLabel;
const QRect NCkCheckBox               = OneByteShapes::NCkCheckBox;
const QRect nBitLabel                 = OneByteShapes::nBitLabel;
const QRect MemWriteLabel             = OneByteShapes::MemWriteLabel;
const QRect MemWriteTristateLabel     = OneByteShapes::MemWriteTristateLabel;
const QRect MemReadLabel              = OneByteShapes::MemReadLabel;
const QRect MemReadTristateLabel      = OneByteShapes::MemReadTristateLabel;
const Arrow loadCkSelect              = OneByteShapes::loadCkSelect;
const Arrow CSelect                   = OneByteShapes::CSelect;
const Arrow BSelect                   = OneByteShapes::BSelect;
const Arrow ASelect                   = OneByteShapes::ASelect;
const Arrow MARCk                     = OneByteShapes::MARCk;
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


}



#endif // SHAPES_TWO_BYTE_DATA_BUS_H

