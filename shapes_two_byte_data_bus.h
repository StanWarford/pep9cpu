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


// input/label/control section:
const QRect loadCkCheckbox              = OneByteShapes::loadCkCheckbox;
const QRect cLineEdit                   = OneByteShapes::cLineEdit;
const QRect cLabel                      = OneByteShapes::cLabel;
const QRect bLineEdit                   = OneByteShapes::bLineEdit;
const QRect bLabel                      = OneByteShapes::bLabel;
const QRect aLineEdit                   = OneByteShapes::aLineEdit;
const QRect aLabel                      = OneByteShapes::aLabel;
const QRect MARCkCheckbox               = OneByteShapes::MARCkCheckbox; // I think this stays the same
const QRect MARALabel;
const QRect MARBLabel;
//const QRect MDRCkCheckbox;
const QRect MDROCkCheckbox;
const QRect MDRECkCheckbox;
//const QRect MDRMuxLabel;
const QRect MDROMuxLabel;
const QRect MDREMuxLabel;
//const QRect MDRMuxerDataLabel;
const QRect MDROMuxerDataLabel;
const QRect MDREMuxerDataLabel;
//const QRect MDRMuxTristateLabel;
const QRect MDROMuxTristateLabel;
const QRect MDREMuxTristateLabel;
//const QRect MDRLabel;
const QRect MDROLabel;
const QRect MDRELabel;

const QRect aMuxLabel;
const QRect aMuxerDataLabel;
const QRect aMuxTristateLabel;
const QRect cMuxLabel;
const QRect cMuxerLabel;
const QRect cMuxTristateLabel;
const QRect ALULineEdit;
const QRect ALULabel;
const QRect ALUFunctionLabel;
const QRect CSMuxLabel;
const QRect CSMuxerDataLabel;
const QRect CSMuxTristateLabel;
const QRect SCkCheckBox;
const QRect sBitLabel;
const QRect CCkCheckBox;
const QRect cBitLabel;
const QRect VCkCheckBox;
const QRect vBitLabel;
const QRect AndZLabel;
const QRect AndZTristateLabel;
const QRect AndZMuxLabel;
const QRect ZCkCheckBox;
const QRect zBitLabel;
const QRect NCkCheckBox;
const QRect nBitLabel;
const QRect MemWriteLabel;
const QRect MemWriteTristateLabel;
const QRect MemReadLabel;
const QRect MemReadTristateLabel;
const Arrow loadCkSelect;
const Arrow CSelect;
const Arrow BSelect;
const Arrow ASelect;
const Arrow MARCk;
const Arrow MDRCk;
const Arrow AMuxSelect;
const QPolygon AMuxBus;
const Arrow CMuxSelect;
const QPolygon CMuxBus;
const QPolygon ALUPoly;
const QRect MDRBusOutRect;
const QPolygon MDRBusOutArrow;
const QPolygon MARBus;
const QPolygon NZVCDataPath;
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
const QRect AddrBus                         = OneByteShapes::AddrBus;
const QRect DataBus                         = OneByteShapes::DataBus;
const QPolygon AddrArrow                    = OneByteShapes::AddrArrow;
//const QPolygon DataToMDRMuxBus;
//const QPolygon MDRToDataBus;
const QPolygon MDROToDataBus = QPolygon(QVector<QPoint>()  << QPoint(105, 258)
                                        << QPoint(83, 258) << QPoint(83, 253)
                                        << QPoint(73, 263) << QPoint(83, 273)
                                        << QPoint(83, 268) << QPoint(105, 268));

const QPolygon MDREToDataBus = QPolygon(QVector<QPoint>()  << QPoint(105, 298)
                                        << QPoint(83, 298) << QPoint(83, 293)
                                        << QPoint(73, 303) << QPoint(83, 313)
                                        << QPoint(83, 308) << QPoint(105, 308));
//const QPolygon MDRMuxOutBus;
const QPolygon ALUOutBus;





#endif // SHAPES_TWO_BYTE_DATA_BUS_H

