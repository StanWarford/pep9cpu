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
    aluOffsetY = 160,
    selLineOff = 15,

};

enum CommonPositions {
    controlOffsetX = 50,
    ctrlLabelX = 579 + controlOffsetX,
    ctrlInputX = 550 + controlOffsetX,
    interfaceRegsX = 175,               // x-center of MARB, MARA, ...
    combCircX = interfaceRegsX - iRegXOffset-20, //Combinational circuits need to be moved further left to fit.
    combCircY = 142, //Memory Combinational circuits start at this height. Originally 132
    statusBitsX = 526,//476,
    BottomOfAlu=OneByteShapes::ALUBottomBound+aluOffsetY, //Y coordinate of the bottom of the ALU

};

//Enumeration that controls the distance between certain items in the diagram. Hopefully this makes spacing easier to adjust.
enum CommonOffsets{
    AMuxYOffsetFromALUPoly=40,  //The number of pixels between AMux and the ALU Polygon
    MARMUXOffestFromMARA=25,    //Number of pixels between MARMux and (MARA, MARB) horizontally.
    MARAOffsetFromMARB=70,      //Number of pixels vertically between MARA and MARB
    MDREOffsetFromCombY=120,    //Number of pixels vertically between MDRO register and the combCircY origin.
    MDRORegOffsetFromMDREMux=42,//Number of pixels vertically between MDROMux and MDREMux
    MDRRegOffsetFromMDRMux=20,  //Number of pixels between the bottom of MDRO,MDRE registers and the top of MDROMux, MDREMux
    SCKYOffsetFromALU=43,       //Number of pixel between bottom of ALU and top of SCk controls
    CCkYOffsetFromALU=70,       //Bottom of ALU and top of CCk
    VCkYOffsetFromALU=97,       //Bottom of ALU and top of VCk
    ANDZYOffsetFromALU=123,     //Bottom of ALU and top of ANDZ
    ZCkYOffsetFromALU=150,      //Bottom of ALU and top of ZCk
    NCkYOffsetFromALU=192,      //Bottom of ALU and top of NCk
    MemReadYOffsetFromALU=237,  //Bottom of ALU to the MemReadLine
    MemWriteYOffsetFromALU=217, //Bottom of ALU to the MemWriteLine
    ALULabelYOffsetFromALU=-25, //Bottom of ALU to top of the ALULineEdit
    EOMuxOffsetFromMDREMux=10,  //Bottom of MDREMux to top of EOMux

  };

// input/label/control section:
const QRect AddrBus = QRect(40, 151, 20, 600);
const QRect DataBus = QRect(AddrBus.x()+AddrBus.width(), AddrBus.top() + 100, 20, 500);

// LoadCk and its control
const QRect loadCkCheckbox          = QRect(ctrlInputX, 18, check2W,   check2H);
const Arrow loadCkSelect            = Arrow(QVector<QPoint>() << QPoint(499, 24),
                                            QVector<QLine>()
                                            << QLine(ctrlInputX - 7, loadCkCheckbox.y() + selectYOffset,
                                                      499, loadCkCheckbox.y() + selectYOffset));

// C and its control
const QRect cLineEdit               = QRect(ctrlInputX, 39, lineEditW, lineEditH);
const QRect cLabel                  = QRect(ctrlLabelX, 41, labelW,    labelH);
const Arrow CSelect                 = Arrow(QVector<QPoint>() << QPoint(499, 47),
                                            QVector<QLine>()
                                            << QLine(ctrlInputX - 7, cLabel.y() + selectYOffset,
                                                     499, cLabel.y() + selectYOffset)
                                            << QLine(523, cLabel.y() + selectYOffset - 5,
                                                     533, cLabel.y() + selectYOffset + 5));

// B and its control
const QRect bLineEdit               = QRect(ctrlInputX, 61, lineEditW, lineEditH);
const QRect bLabel                  = QRect(ctrlLabelX, 63, labelW,    labelH);
const Arrow BSelect                 = Arrow(QVector<QPoint>() << QPoint(499, 69),
                                            QVector<QLine>()
                                            << QLine(ctrlInputX - 7, bLabel.y() + selectYOffset,
                                                     499, bLabel.y() + selectYOffset)
                                            << QLine(523, bLabel.y() + selectYOffset - selectSlashOffset,
                                                     533, bLabel.y() + selectYOffset + selectSlashOffset));

// A and its control
const QRect aLineEdit               = QRect(ctrlInputX, 83, lineEditW, lineEditH);
const QRect aLabel                  = QRect(ctrlLabelX, 85, labelW,    labelH);
const Arrow ASelect                 = Arrow(QVector<QPoint>() << QPoint(499, 91),
                                            QVector<QLine>()
                                            << QLine(ctrlInputX - 7, aLabel.y() + selectYOffset,
                                                     499, aLabel.y() + selectYOffset)
                                            << QLine(523, aLabel.y() + selectYOffset - selectSlashOffset,
                                                     533, aLabel.y() + selectYOffset + selectSlashOffset));

// MARMux and its control
const QRect MARMuxerDataLabel       = QRect((combCircX+dataLabelW)+MARMUXOffestFromMARA, combCircY, dataLabelH+MARAOffsetFromMARB, dataLabelH+MARAOffsetFromMARB); // 89 x 89 square from bottom of MARA to top of MARB
const QRect MARMuxTristateLabel     = QRect(ctrlInputX, MARMuxerDataLabel.y()-28, labelTriW, labelTriH);
const QRect MARMuxLabel             = QRect(ctrlLabelX, MARMuxTristateLabel.y(), labelW+20, labelH);
const Arrow MARMuxSelect            = Arrow(QVector<QPoint>() << QPoint(MARMuxerDataLabel.x()+MARMuxerDataLabel.width()/2, MARMuxerDataLabel.y()-12),
                                            QVector<QLine>()
                                            << QLine(MARMuxerDataLabel.x()+MARMuxerDataLabel.width()/2+3, MARMuxerDataLabel.y()-4,
                                                     MARMuxerDataLabel.x()+MARMuxerDataLabel.width()/2+3, MARMuxerDataLabel.y()-19)
                                            << QLine(MARMuxerDataLabel.x()+MARMuxerDataLabel.width()/2+3, MARMuxTristateLabel.y()+9,
                                                     326, MARMuxTristateLabel.y()+9)
                                            << QLine(350, MARMuxTristateLabel.y()+9,
                                                     ctrlInputX - 7, MARMuxTristateLabel.y()+9));

// MARCk and its control
const QRect MARCkCheckbox           = QRect(ctrlInputX, 169, check2W, check2H);
const QRect MARALabel               = QRect(combCircX, combCircY+MARAOffsetFromMARB, dataLabelW, dataLabelH); // MARA register.
const QRect MARBLabel               = QRect(combCircX, combCircY, dataLabelW, dataLabelH); // MARB register
const Arrow MARCk                   = Arrow(QVector<QPoint>()
                                            //The Arrows intersecting MAR,MARB should be roughly 5/7 of the way down the circuits.
                                            << QPoint(combCircX+5*dataLabelW/7+7,combCircY+dataLabelH+3)
                                            << QPoint(combCircX+5*dataLabelW/7+7,combCircY+MARAOffsetFromMARB-11),
                                            QVector<QLine> ()
                                            << QLine(428,177, 543,177)
                                            << QLine(367,177, 416,177)
                                            << QLine(291,177, 355,177)
                                            << QLine(235-50,177, 279,177)
                                            //The vertical lines intersecting MAR,MARB should be roughly 5/7 of the way down the circuits.
                                            << QLine(combCircX+5*dataLabelW/7+10,combCircY+dataLabelH+3,
                                                     combCircX+5*dataLabelW/7+10,combCircY+MARAOffsetFromMARB-3));  //Vertical line at left end of MARck
// MARMux output busses
const QPolygon MARMuxToMARABus = QPolygon(QVector<QPoint>()
                                        << QPoint(MARMuxerDataLabel.x(), MARALabel.y()+MARALabel.height()/2-5)               //Top Right Corner
                                        << QPoint(MARMuxerDataLabel.x(), MARALabel.y()+MARALabel.height()/2+5)               //Bottom Right Corner
                                        << QPoint(MARALabel.right()+arrowHDepth-5, MARALabel.y()+MARALabel.height()/2+5)     //Arrow Bottom Left Inner edge
                                        << QPoint(MARALabel.right()+arrowHDepth-5, MARALabel.y()+MARALabel.height()/2+10)    //Arrow Bottom Left Outer Edge
                                        << QPoint(MARALabel.right()+arrowHOffset, MARALabel.y()+MARALabel.height()/2)        //Arrow Middle Point
                                        << QPoint(MARALabel.right()+arrowHDepth-5, MARALabel.y()+MARALabel.height()/2-10)    //Arrow Top Left Outer Edge
                                        << QPoint(MARALabel.right()+arrowHDepth-5, MARALabel.y()+MARALabel.height()/2-5));   //Arrow Top Left Inner Edge

const QPolygon MARMuxToMARBBus = QPolygon(QVector<QPoint>()
                                        << QPoint(MARMuxerDataLabel.x(), MARBLabel.y()+MARALabel.height()/2-5)               //Top Right Corner
                                        << QPoint(MARMuxerDataLabel.x(), MARBLabel.y()+MARALabel.height()/2+5)               //Bottom Right Corner
                                        << QPoint(MARBLabel.right()+arrowHDepth-5, MARBLabel.y()+MARALabel.height()/2+5)     //Arrow Bottom Left Inner edge
                                        << QPoint(MARBLabel.right()+arrowHDepth-5, MARBLabel.y()+MARALabel.height()/2+10)    //Arrow Bottom Left Outer Edge
                                        << QPoint(MARBLabel.right()+arrowHOffset, MARBLabel.y()+MARALabel.height()/2)        //Arrow Middle Point
                                        << QPoint(MARBLabel.right()+arrowHDepth-5, MARBLabel.y()+MARALabel.height()/2-10)    //Arrow Top Left Outer Edge
                                        << QPoint(MARBLabel.right()+arrowHDepth-5, MARBLabel.y()+MARALabel.height()/2-5));   //Arrow Top Left Inner Edge

// MDROdd, MDROCk and its control
const QRect MDROLabel               = QRect(combCircX, combCircY+MDREOffsetFromCombY, dataLabelW, dataLabelH);
const QRect MDROCkCheckbox          = QRect(ctrlInputX, MDROLabel.y()-20, checkW+10, checkH);
const Arrow MDROck              = Arrow(QVector<QPoint>() << QPoint(MDROLabel.x()+MDROLabel.width()/2, MDROLabel.y()-12),
                                            QVector<QLine>()
                                            << QLine(MDROLabel.x()+MDROLabel.width()/2+3, MDROLabel.y()-4,
                                                     MDROLabel.x()+MDROLabel.width()/2+3, MDROLabel.y()-19)
                                            << QLine(MDROLabel.x()+MDROLabel.width()/2+3, MDROCkCheckbox.y()+9,
                                                     326, MDROCkCheckbox.y()+9)
                                            << QLine(350, MDROCkCheckbox.y()+9,
                                                     ctrlInputX - 7, MDROCkCheckbox.y()+9));

// MDROMux and its control
const QRect MDROMuxerDataLabel      = QRect(combCircX, MDROLabel.bottom()+MDRRegOffsetFromMDRMux, dataLabelW, dataLabelH);
const QRect MDROMuxTristateLabel    = QRect(ctrlInputX, MDROMuxerDataLabel.y()-20, labelTriW, labelTriH);
const QRect MDROMuxLabel            = QRect(ctrlLabelX, MDROMuxTristateLabel.y(), labelW+20, labelH);
const Arrow MDROMuxSelect           = Arrow(QVector<QPoint>()
                                            <<QPoint(MDROMuxerDataLabel.right(),MDROMuxTristateLabel.y()+MDROMuxTristateLabel.height()/2-3),
                                            QVector<QLine>()
                                            <<QLine(MDROMuxTristateLabel.x(),MDROMuxTristateLabel.y()+MDROMuxTristateLabel.height()/2,
                                                    MDROMuxerDataLabel.right()+5,MDROMuxTristateLabel.y()+MDROMuxTristateLabel.height()/2));

// MDREven, MDRECk and its control
const QRect MDRELabel               = QRect(combCircX, MDROMuxerDataLabel.bottom()+MDRORegOffsetFromMDREMux, dataLabelW, dataLabelH);
const QRect MDRECkCheckbox          = QRect(ctrlInputX, MDRELabel.y()-20, checkW+10, checkH);

// MDREMux and its control
const QRect MDREMuxerDataLabel      = QRect(combCircX,MDRELabel.bottom()+MDRRegOffsetFromMDRMux, dataLabelW, dataLabelH);
const QRect MDREMuxTristateLabel    = QRect(ctrlInputX, MDREMuxerDataLabel.y()-20, labelTriW, labelTriH);
const QRect MDREMuxLabel            = QRect(ctrlLabelX, MDREMuxTristateLabel.y(), labelW+20, labelH);
const Arrow MDREMuxSelect           = Arrow(QVector<QPoint>()
                                            << QPoint(MDREMuxerDataLabel.right(),MDREMuxTristateLabel.y()+MDREMuxTristateLabel.height()/2-3),
                                            QVector<QLine>()
                                            <<QLine(MDREMuxTristateLabel.x(),MDREMuxTristateLabel.y()+MDREMuxTristateLabel.height()/2,
                                                    MDREMuxerDataLabel.right()+5,MDREMuxTristateLabel.y()+MDREMuxTristateLabel.height()/2));
// EOMux and its control
const QRect EOMuxerDataLabel        = QRect(MARMuxerDataLabel.x()+MARMuxerDataLabel.width()/2-dataLabelW/2, //Center EOMux horizontally on MARMux
                                            MDREMuxerDataLabel.y()+EOMuxOffsetFromMDREMux, dataLabelW, dataLabelH);
const QRect EOMuxTristateLabel      = QRect(ctrlInputX, EOMuxerDataLabel.y(), labelTriW, labelTriH);
const QRect EOMuxLabel              = QRect(ctrlLabelX, EOMuxTristateLabel.y(), labelW, labelH);
const Arrow EOMuxSelect             = Arrow(QVector<QPoint>() << QPoint(EOMuxerDataLabel.right()+4,
                                                                        EOMuxTristateLabel.y()+6),
                                            QVector<QLine>()
                                            << QLine(EOMuxerDataLabel.right()+5, EOMuxTristateLabel.y()+9,
                                                     326, EOMuxTristateLabel.y()+9)
                                            << QLine(350, EOMuxTristateLabel.y()+9,
                                                     ctrlInputX - 7, EOMuxTristateLabel.y()+9));


// ALU and its control
const QRect ALULineEdit             = QRect(ctrlInputX, BottomOfAlu+ALULabelYOffsetFromALU, 26,     lineEditH);
const QRect ALULabel                = QRect(ctrlLabelX,  BottomOfAlu+ALULabelYOffsetFromALU, 31,     labelH);
const QRect ALUFunctionLabel        = OneByteShapes::ALUFunctionLabel.translated(controlOffsetX,
                                                                                 aluOffsetY);
// CMux and its control
const QRect cMuxerLabel             = OneByteShapes::cMuxerLabel.translated(controlOffsetX, aluOffsetY);
const QRect cMuxTristateLabel       = QRect(ctrlInputX, ALULineEdit.y()-labelTriH, labelTriW, labelTriH);
const QRect cMuxLabel               = QRect(ctrlLabelX, cMuxTristateLabel.y(), labelW, labelH);
const Arrow CMuxSelect              = OneByteShapes::CMuxSelect.translated(controlOffsetX, aluOffsetY);

// CSMux and its control
const QRect CSMuxLabel              = QRect(ctrlLabelX,  BottomOfAlu+5, labelW, labelH);
const QRect CSMuxerDataLabel        = QRect(statusBitsX+19-69, BottomOfAlu+5, dataLabelW, dataLabelH);
const QRect CSMuxTristateLabel      = QRect(ctrlInputX,  BottomOfAlu+5, 25,     21);

// Status bit S, SCk and its control
const QRect SCkCheckBox             = QRect(ctrlInputX,  BottomOfAlu+SCKYOffsetFromALU, checkW, checkH);
const QRect sBitLabel               = QRect(statusBitsX, BottomOfAlu+SCKYOffsetFromALU, 19,     dataLabelH);

// Status bit C, CCk and its control
const QRect CCkCheckBox             = QRect(ctrlInputX,  BottomOfAlu+CCkYOffsetFromALU, checkW, checkH);
const QRect cBitLabel               = QRect(statusBitsX, BottomOfAlu+CCkYOffsetFromALU -1, 19,     dataLabelH);

// Status bit V, VCk and its control
const QRect VCkCheckBox             = QRect(ctrlInputX,  BottomOfAlu+VCkYOffsetFromALU, checkW, checkH);
const QRect vBitLabel               = QRect(statusBitsX, BottomOfAlu+VCkYOffsetFromALU, 19,     dataLabelH);

// AndZ and its control
const QRect AndZLabel               = QRect(ctrlLabelX,  BottomOfAlu+ANDZYOffsetFromALU, 45,     20);
const QRect AndZTristateLabel       = QRect(ctrlInputX, BottomOfAlu+ANDZYOffsetFromALU, labelTriW,labelTriH);
const QRect AndZMuxLabel            = QRect(416 + controlOffsetX, BottomOfAlu+ANDZYOffsetFromALU+27, 41,21);

// Status bit Z, ZCk and its control
const QRect ZCkCheckBox             = QRect(ctrlInputX, BottomOfAlu+ZCkYOffsetFromALU, 60, 20);
const QRect zBitLabel               = QRect(statusBitsX, BottomOfAlu+ZCkYOffsetFromALU, 19, dataLabelH);

// Status bit N, NCk and its control
const QRect NCkCheckBox             = QRect(ctrlInputX, BottomOfAlu+NCkYOffsetFromALU, checkW, checkH);
const QRect nBitLabel               = QRect(statusBitsX, BottomOfAlu+NCkYOffsetFromALU, 19, dataLabelH);

// MemWrite and its control
const QRect MemWriteLabel           = QRect(ctrlLabelX, BottomOfAlu+MemWriteYOffsetFromALU, check2W, check2H);
const QRect MemWriteTristateLabel   = QRect(ctrlInputX, BottomOfAlu+MemWriteYOffsetFromALU, labelTriW, labelTriH);

// MemRead and its control
const QRect MemReadLabel            = QRect(ctrlLabelX, BottomOfAlu+MemReadYOffsetFromALU, check2W, check2H);
const QRect MemReadTristateLabel    = QRect(ctrlInputX, BottomOfAlu+MemReadYOffsetFromALU, labelTriW, labelTriH);

//const Arrow MDRCk                   = OneByteShapes::MDRCk;
const QPolygon CMuxBus                = OneByteShapes::CMuxBus.translated(controlOffsetX, aluOffsetY);
const QPolygon ALUPoly                = OneByteShapes::ALUPoly.translated(controlOffsetX, aluOffsetY);
const QRect MDRBusOutRect             = OneByteShapes::MDRBusOutRect;
const QPolygon MDRBusOutArrow         = OneByteShapes::MDRBusOutArrow;
const QPolygon MARBus = QPolygon(QVector<QPoint>()
                                 //Top Foot
                                 << QPoint(MARBLabel.x()+MARBLabel.width()/2-5,MARBLabel.bottom()+1) //Top Left Corner
                                 << QPoint(MARBLabel.x()+MARBLabel.width()/2-5,(MARBLabel.bottom()+MARALabel.y())/2-10) //Point Between top right corner and inner top Point.
                                 // arrow:
                                 << QPoint(AddrBus.right()+arrowHDepth,(MARBLabel.bottom()+MARALabel.y())/2-10) //Arrow Inner Top Point
                                 << QPoint(AddrBus.right()+arrowHDepth,(MARBLabel.bottom()+MARALabel.y())/2-15) //Arrow Outer Top Point
                                 << QPoint(AddrBus.right()+arrowHOffset,(MARBLabel.bottom()+MARALabel.y())/2+1) //Arrow Middle Point
                                 << QPoint(AddrBus.right()+arrowHDepth,(MARBLabel.bottom()+MARALabel.y())/2+16) //Arrow Outer bottom Point
                                 << QPoint(AddrBus.right()+arrowHDepth,(MARBLabel.bottom()+MARALabel.y())/2+10) //Arrow Inner Bottom Point
                                 //Bottom Foot
                                 << QPoint(MARALabel.x()+MARBLabel.width()/2-5,(MARBLabel.bottom()+MARALabel.y())/2+10) //Point between bottom right corner and bottom inner point
                                 << QPoint(MARALabel.x()+MARBLabel.width()/2-5,MARALabel.y()) //Bottom left Corner
                                 << QPoint(MARALabel.x()+MARBLabel.width()/2+5,MARALabel.y()) //Bottom Right Corner
                                 //Black Line
                                 << QPoint(MARBLabel.x()+MARBLabel.width()/2+5,(MARBLabel.bottom()+MARALabel.y())/2) //Black line right point
                                 << QPoint(AddrBus.right()+arrowHDepth,(MARBLabel.bottom()+MARALabel.y())/2) //Black line left point
                                 << QPoint(MARBLabel.x()+MARBLabel.width()/2+5,(MARBLabel.bottom()+MARALabel.y())/2) //Black line right point
                                 //Top Foot
                                 << QPoint(MARBLabel.x()+MARBLabel.width()/2+5,MARBLabel.bottom()+1)); //Top Right Corner
                                 // black line in the middle:

const QPolygon NZVCDataPath = OneByteShapes::NZVCDataPath.translated(controlOffsetX, aluOffsetY);

// AMux, its controls, selection lines, and output.
const QRect aMuxerDataLabel         =  QRect(((controlOffsetX+OneByteShapes::ALUUpperLeftLine_LeftPoint)+(controlOffsetX+OneByteShapes::ALUUpperLeftLine_RightPoint))/2-dataLabelW/2,
                                            //Center AMUX's x on the midpoint of the ALUPolygon, which has been shifted by controlOffsetX pixels.
                                            ALUPoly.boundingRect().y()-AMuxYOffsetFromALUPoly, dataLabelW, dataLabelH);//Place AMuxYOffsetFromALUPoly pixels distance between AMux and the ALU

const QRect aMuxTristateLabel       = QRect(ctrlInputX, aMuxerDataLabel.y(), labelTriW, 21);
const QRect aMuxLabel               = QRect(ctrlLabelX, aMuxTristateLabel.y(), labelW, labelH);
const Arrow AMuxSelect              = Arrow(QVector<QPoint>()
                                            //Place the arrowhead slightly off-centered from AMux, otherwise it is visually odd.
                                            << QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()+3,aMuxerDataLabel.y()+aMuxerDataLabel.height()/2-2),
                                            //Draw a line from the aMuxTristateLabel to AMux, and center the line vertically between the two.
                                            //Add one to the calculated y coordinates, otherwise the line and arrow don't appear to be centered.
                                            QVector<QLine>()<<QLine(aMuxTristateLabel.x(),aMuxerDataLabel.y()+aMuxerDataLabel.height()/2+1,
                                                                    //Add 5 to the x coordinate, otherwise the line extends past the arrow.
                                                                    aMuxerDataLabel.x()+aMuxerDataLabel.width()+5,aMuxerDataLabel.y()+aMuxerDataLabel.height()/2+1));

//EOMux bus definition is split from EOMux, because it depends on AMUX, which depends on the ALU
const QPolygon EOMuxOutputBus          = QPolygon(QVector<QPoint>()
                                            //Foot
                                            <<QPoint(EOMuxerDataLabel.x()+EOMuxerDataLabel.width()/2-5,EOMuxerDataLabel.bottom()+1) //Top Left point
                                            <<QPoint(EOMuxerDataLabel.x()+EOMuxerDataLabel.width()/2+5,EOMuxerDataLabel.bottom()+1) //Top Right point
                                            <<QPoint(EOMuxerDataLabel.x()+EOMuxerDataLabel.width()/2+5,EOMuxerDataLabel.bottom()+5) //Point between upper right vertical leg, and upper horizontal leg
                                            <<QPoint(aMuxerDataLabel.x()+15,EOMuxerDataLabel.bottom()+5) //Point between upper horizontal leg, and the right vertical leg
                                            //Arrow
                                            <<QPoint(aMuxerDataLabel.x()+15,aMuxerDataLabel.y()-(arrowHDepth-5)) //Arrow inner right point
                                            <<QPoint(aMuxerDataLabel.x()+20,aMuxerDataLabel.y()-(arrowHDepth-5)) //Arrow outer right point
                                            <<QPoint(aMuxerDataLabel.x()+10,aMuxerDataLabel.y()-arrowHOffset) //Arrow middle  point
                                            <<QPoint(aMuxerDataLabel.x(),aMuxerDataLabel.y()-(arrowHDepth-5)) //Arrow outer left point
                                            <<QPoint(aMuxerDataLabel.x()+5 ,aMuxerDataLabel.y()-(arrowHDepth-5)) //Arrow inner left point
                                            //Remainder of Foot
                                            <<QPoint(aMuxerDataLabel.x()+5,EOMuxerDataLabel.bottom()+15) //Point between arrow left side and lower horizontal leg
                                            <<QPoint(EOMuxerDataLabel.x()+EOMuxerDataLabel.width()/2-5,EOMuxerDataLabel.bottom()+15) //Point between lower horizontal leg and start
                                            );

const QPolygon AMuxBus              = QPolygon(QVector<QPoint>()
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2-5,aMuxerDataLabel.y()+aMuxerDataLabel.height()) //Upper Left Corner
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2+5,aMuxerDataLabel.y()+aMuxerDataLabel.height()) //Upper Right Corner
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2+5,ALUPoly.boundingRect().y()-(arrowHDepth-5)) //Arrow Inner Right Edge
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2+10,ALUPoly.boundingRect().y()-(arrowHDepth-5)) //Arrow Outer Right Edge
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2,ALUPoly.boundingRect().y()-arrowHOffset) //Arrow Outer Right Edge
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2-10,ALUPoly.boundingRect().y()-(arrowHDepth-5)) //Arrow Outer Left Edge
                                               <<QPoint(aMuxerDataLabel.x()+aMuxerDataLabel.width()/2-5,ALUPoly.boundingRect().y()-(arrowHDepth-5)) //Arrow Inner Left Edge
                                               );

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
                                           << QPoint(MDROMuxerDataLabel.x()+15, MDROMuxerDataLabel.bottom()+(arrowHDepth-5)+18) //Point between vertical right leg and lower horizontal leg
                                           << QPoint(80,  MDROMuxerDataLabel.bottom()+(arrowHDepth-5)+18) //Lower left corner on bus
                                           << QPoint(80,  MDROMuxerDataLabel.bottom()+(arrowHDepth-5)+8) //Upper left corner on bus
                                           << QPoint(MDROMuxerDataLabel.x()+5, MDROMuxerDataLabel.bottom()+(arrowHDepth-5)+8) //Point between vertical left leg and upper horizontal leg
                                           // arrowhead:
                                           << QPoint(MDROMuxerDataLabel.x()+5, MDROMuxerDataLabel.bottom()+(arrowHDepth-5)) //arrow inner left edge
                                           << QPoint(MDROMuxerDataLabel.x()+0, MDROMuxerDataLabel.bottom()+(arrowHDepth-5)) //arrow outer left edge
                                           << QPoint(MDROMuxerDataLabel.x()+10, MDROMuxerDataLabel.bottom()+arrowHOffset) //arrow midpoint
                                           << QPoint(MDROMuxerDataLabel.x()+20, MDROMuxerDataLabel.bottom()+(arrowHDepth-5)) //arrow right outer edge
                                           << QPoint(MDROMuxerDataLabel.x()+15, MDROMuxerDataLabel.bottom()+(arrowHDepth-5))); //arrow inner left edge

const QPolygon DataToMDREMuxBus = QPolygon(QVector<QPoint>()
                                           // foot:
                                           << QPoint(MDREMuxerDataLabel.x()+15, MDREMuxerDataLabel.bottom()+(arrowHDepth-5)+18) //Point between vertical right leg and lower horizontal leg
                                           << QPoint(80,  MDREMuxerDataLabel.bottom()+(arrowHDepth-5)+18) //Lower left corner on bus
                                           << QPoint(80,  MDREMuxerDataLabel.bottom()+(arrowHDepth-5)+8) //Upper left corner on bus
                                           << QPoint(MDREMuxerDataLabel.x()+5, MDREMuxerDataLabel.bottom()+(arrowHDepth-5)+8) //Point between vertical left leg and upper horizontal leg
                                           // arrowhead:
                                           << QPoint(MDREMuxerDataLabel.x()+5, MDREMuxerDataLabel.bottom()+(arrowHDepth-5)) //arrow inner left edge
                                           << QPoint(MDREMuxerDataLabel.x()+0, MDREMuxerDataLabel.bottom()+(arrowHDepth-5)) //arrow outer left edge
                                           << QPoint(MDREMuxerDataLabel.x()+10, MDREMuxerDataLabel.bottom()+arrowHOffset) //arrow midpoint
                                           << QPoint(MDREMuxerDataLabel.x()+20, MDREMuxerDataLabel.bottom()+(arrowHDepth-5)) //arrow right outer edge
                                           << QPoint(MDREMuxerDataLabel.x()+15, MDREMuxerDataLabel.bottom()+(arrowHDepth-5))); //arrow inner left edge
//const QPolygon MDRToDataBus;
const QPolygon MDROToDataBus = QPolygon(QVector<QPoint>()
                                        << QPoint(MDROLabel.x(), MDROLabel.y()+MDROLabel.height()/2-5)                      //Top Right Corner
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDROLabel.y()+MDROLabel.height()/2-5)     //Arrow Inner upper point
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDROLabel.y()+MDROLabel.height()/2-10)    //Arrow Outer Upper point
                                        << QPoint(DataBus.x()+DataBus.width()+3, MDROLabel.y()+MDROLabel.height()/2)        //Arrow middle
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDROLabel.y()+MDROLabel.height()/2+10)    //Arrow Outer Lower Point
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDROLabel.y()+MDROLabel.height()/2+5)     //Arrow Inner Lower Point
                                        << QPoint(MDROLabel.x(), MDROLabel.y()+MDROLabel.height()/2+5));                    //Bottom Right Corner

const QPolygon MDREToDataBus = QPolygon(QVector<QPoint>()
                                        << QPoint(MDRELabel.x(), MDRELabel.y()+MDRELabel.height()/2-5)                      //Top Right Corner
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDRELabel.y()+MDRELabel.height()/2-5)     //Arrow Inner upper point
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDRELabel.y()+MDRELabel.height()/2-10)    //Arrow Outer Upper point
                                        << QPoint(DataBus.x()+DataBus.width()+3, MDRELabel.y()+MDRELabel.height()/2)        //Arrow middle
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDRELabel.y()+MDRELabel.height()/2+10)    //Arrow Outer Lower Point
                                        << QPoint(DataBus.x()+DataBus.width()+13, MDRELabel.y()+MDRELabel.height()/2+5)     //Arrow Inner Lower Point
                                        << QPoint(MDRELabel.x(), MDRELabel.y()+MDRELabel.height()/2+5));                    //Bottom Right Corner
const QPolygon MDREToEOMuxBus = QPolygon(QVector<QPoint>()
                                         << QPoint(MDRELabel.right()+1,MDRELabel.y()+MDRELabel.height()/2 - 5) //MDRE Top Point
                                         << QPoint(MDRELabel.right()+1,MDRELabel.y()+MDRELabel.height()/2 + 5) //MDRE Bottom Point
                                         << QPoint(EOMuxerDataLabel.x()+5,MDRELabel.y()+MDRELabel.height()/2 + 5) //Pivot between MDRE bottom and EOMux bottom
                                         << QPoint(EOMuxerDataLabel.x()+5,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Inner Left Edge
                                         << QPoint(EOMuxerDataLabel.x()+0,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Outer Left Edge
                                         << QPoint(EOMuxerDataLabel.x()+10,EOMuxerDataLabel.y()-(arrowHOffset)) //EOMux Arrow Midpoint
                                         << QPoint(EOMuxerDataLabel.x()+20,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Outer Right Edge
                                         << QPoint(EOMuxerDataLabel.x()+15,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Inner Right Edge
                                         << QPoint(EOMuxerDataLabel.x()+15,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Inner Right Edge
                                         << QPoint(EOMuxerDataLabel.x()+20,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Outer Right Edge
                                         << QPoint(EOMuxerDataLabel.x()+10,MARMuxerDataLabel.bottom()+(arrowHOffset)) //MARMux Arrow Midpoint
                                         << QPoint(EOMuxerDataLabel.x()+0,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Outer Left Edge
                                         << QPoint(EOMuxerDataLabel.x()+5,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Inner Left Edge
                                         << QPoint(EOMuxerDataLabel.x()+5,MDRELabel.y()+MDRELabel.height()/2 -5) //Pivot between MARMux bottom and MDRE top
                                         );
const QPolygon MDROToEOMuxBus = QPolygon(QVector<QPoint>()
                                         << QPoint(MDROLabel.right()+1,MDROLabel.y()+MDROLabel.height()/2 - 5) //MDRE Top Point
                                         << QPoint(MDROLabel.right()+1,MDROLabel.y()+MDROLabel.height()/2 + 5) //MDRE Bottom Point
                                         << QPoint(EOMuxerDataLabel.right()-15,MDROLabel.y()+MDROLabel.height()/2 + 5) //Pivot between MDRE bottom and EOMux bottom
                                         << QPoint(EOMuxerDataLabel.right()-15,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Inner Left Edge
                                         << QPoint(EOMuxerDataLabel.right()-20,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Outer Left Edge
                                         << QPoint(EOMuxerDataLabel.right()-10,EOMuxerDataLabel.y()-(arrowHOffset)) //EOMux Arrow Midpoint
                                         << QPoint(EOMuxerDataLabel.right()-0,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Outer Right Edge
                                         << QPoint(EOMuxerDataLabel.right()-5,EOMuxerDataLabel.y()-(arrowHDepth-5)) //EOMux Arrow Inner Right Edge
                                         << QPoint(EOMuxerDataLabel.right()-5,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Inner Right Edge
                                         << QPoint(EOMuxerDataLabel.right()+0,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Outer Right Edge
                                         << QPoint(EOMuxerDataLabel.right()-10,MARMuxerDataLabel.bottom()+(arrowHOffset)) //MARMux Arrow Midpoint
                                         << QPoint(EOMuxerDataLabel.right()-20,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Outer Left Edge
                                         << QPoint(EOMuxerDataLabel.right()-15,MARMuxerDataLabel.bottom()+(arrowHDepth-5)) //MARMux Arrow Inner Left Edge
                                         << QPoint(EOMuxerDataLabel.right()-15,MDROLabel.y()+MDROLabel.height()/2 -5) //Pivot between MARMux bottom and MDRE top
                                         );
//const QPolygon MDRMuxOutBus;
const QPolygon MDROMuxOutBus = QPolygon(QVector<QPoint>()
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2-5, MDROMuxerDataLabel.y()) //Bottom Left Corner
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2-5, MDROMuxerDataLabel.y() - 7) //Arrow Inner Left Edge
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2-10, MDROMuxerDataLabel.y() - 7) //Arrow  Outer left Edge
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2, MDROMuxerDataLabel.y() - 17) //Arrow Point
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2+10, MDROMuxerDataLabel.y() - 7) //Arrow Outer Right Edge
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2+5, MDROMuxerDataLabel.y() - 7) //Arrow Inner Right Edge
                                        << QPoint(MDROMuxerDataLabel.x()+MDROMuxerDataLabel.width()/2+5, MDROMuxerDataLabel.y())); //Bottom Right Corner

const QPolygon MDREMuxOutBus = QPolygon(QVector<QPoint>()
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2-5, MDREMuxerDataLabel.y()) //Bottom Left Corner
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2-5, MDREMuxerDataLabel.y() - 7) //Arrow Inner Left Edge
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2-10, MDREMuxerDataLabel.y() - 7) //Arrow  Outer left Edge
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2, MDREMuxerDataLabel.y() - 17) //Arrow Point
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2+10, MDREMuxerDataLabel.y() - 7) //Arrow Outer Right Edge
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2+5, MDREMuxerDataLabel.y() - 7) //Arrow Inner Right Edge
                                        << QPoint(MDREMuxerDataLabel.x()+MDREMuxerDataLabel.width()/2+5, MDREMuxerDataLabel.y())); //Bottom Right Corner



const QPolygon ALUOutBus = OneByteShapes::ALUOutBus.translated(controlOffsetX, aluOffsetY);

const Arrow ALUSelect      = Arrow(QVector<QPoint>() <<
                                   QPoint(ALUPoly.boundingRect().right() - 13,
                                          ALUPoly.boundingRect().bottom() - 21),
                                   QVector<QLine>() << QLine(ALUPoly.boundingRect().right() - 13,
                                                             ALULineEdit.y() + selectYOffset - 1,
                                                             ctrlInputX - 7,
                                                             ALULineEdit.y() + selectYOffset - 1)
                                   << QLine(ctrlInputX - 17,
                                            ALULineEdit.y() + 13,
                                            ctrlInputX - 27,
                                            ALULineEdit.y() + 3)); // diagonal line

const Arrow ALUSelectOut = OneByteShapes::ALUSelectOut.translated(controlOffsetX, aluOffsetY);

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
const QLine NZVCDataLine   = QLine(OneByteShapes::NZVCDataLine).translated(controlOffsetX, aluOffsetY);
const Arrow CBitToNZVC     = OneByteShapes::CBitToNZVC.translated(controlOffsetX, aluOffsetY);
const Arrow CBitToCSMux    = OneByteShapes::CBitToCSMux.translated(controlOffsetX, aluOffsetY);
const Arrow CInToALU       = OneByteShapes::CInToALU.translated(controlOffsetX, aluOffsetY);
const Arrow SBitToCSMux    = OneByteShapes::SBitToCSMux.translated(controlOffsetX, aluOffsetY);
const Arrow ZBitOut        = OneByteShapes::ZBitOut.translated(controlOffsetX, aluOffsetY);
const Arrow VBitOut        = OneByteShapes::VBitOut.translated(controlOffsetX, aluOffsetY);
const Arrow NBitOut        = OneByteShapes::NBitOut.translated(controlOffsetX, aluOffsetY);

const Arrow AndZOut        = OneByteShapes::AndZOut.translated(controlOffsetX, aluOffsetY);

const QLine MemReadSelect  = QLine(DataBus.right()   + arrowHOffset,
                                  MemReadLabel.y() + selectYOffset,
                                  ctrlInputX - 7,
                                  MemReadLabel.y() + selectYOffset);
const QLine MemWriteSelect = QLine(DataBus.right()   + arrowHOffset,
                                  MemWriteLabel.y() + selectYOffset,
                                  ctrlInputX - 7,
                                  MemWriteLabel.y() + selectYOffset); //Doesn't draw vertical lines

}



#endif // SHAPES_TWO_BYTE_DATA_BUS_H

