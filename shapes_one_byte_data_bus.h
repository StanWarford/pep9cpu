// File: shapes_one_byte_data_bus.h
/*
    Pep9CPU is a CPU simulator for executing microcode sequences to
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
#ifndef SHAPES_ONE_BYTE_DATA_BUS_H
#define SHAPES_ONE_BYTE_DATA_BUS_H

#include <QPair>
#include <QPoint>
#include <QLine>
#include <QVector>
#include <QPolygon>
#include <QImage>

#include <QList>

enum Direction {
    Up,
    Down,
    Left,
    Right,
};

class ArrowHead {
public:
    ArrowHead();
    ArrowHead(QPoint pos, Direction dir) {
        _pos = pos;
        _dir = dir;
    }
    QPoint _pos;
    Direction _dir;
};

class Arrow {
public:
    Arrow (QList<ArrowHead > arrowheads, QList<QLine > lines) {
        _arrowheads = arrowheads;
        _lines = lines;
    }
    QList<ArrowHead > _arrowheads;
    QList<QLine > _lines;
};

namespace OneByteShapes
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

};

enum CommonPositions {
    controlV = 579,

};

//    const QPolygon checkbox  = QPolygon(QRect(0,0, 60, 20));
//    const QPolygon checkbox2 = QPolygon(QRect(0,0, 80, 20));
//    const QPolygon label     = QPolygon(QRect(0,0, 42, 20));
//    const QPolygon dataLabel = QPolygon(QRect(0,0, 69, 19));
//    const QPolygon triLabel  = QPolygon(QRect(0,0, 25, 20));
//    const QPolygon lineEdit  = QPolygon(QRect(0,0, 25, 21));

    // input/label/control section:
    const QRect loadCkCheckbox =     QRect(550, 18, check2W, check2H);
    const QRect cLineEdit =          QRect(550, 39, lineEditW, lineEditH);
    // todo: check on '21':
    const QRect cLabel =             QRect(579, 41, labelW, 21);
    const QRect bLineEdit =          QRect(550, 61, lineEditW, lineEditH);
    const QRect bLabel =             QRect(579, 63, labelW, 21);
    const QRect aLineEdit =          QRect(550, 83, lineEditW, lineEditH);
    const QRect aLabel =             QRect(579, 85, labelW, 21);
    const QRect MARCkCheckbox =      QRect(550, 169, check2W, check2H);
    const QRect MARALabel =          QRect(175, 202, dataLabelW, dataLabelH);
    const QRect MARBLabel =          QRect(175, 132, dataLabelW, dataLabelH);
    const QRect MDRCkCheckbox =      QRect(550, 225, check2W, check2H);
    const QRect aMuxLabel =          QRect(550, 225, labelW, labelH);
    const QRect aMuxerDataLabel =    QRect(306, 293, dataLabelW, dataLabelH);
    const QRect aMuxTristateLabel =  QRect(550, 295, labelTriW, 21);
    const QRect MDRMuxLabel =        QRect(579, 318, 61, 20);
    const QRect MDRMuxerDataLabel =  QRect(175,293, dataLabelW, dataLabelH);
    const QRect MDRMuxTristateLabel = QRect(550, 316, labelTriW, labelTriH);
    const QRect MDRLabel =           QRect(175, 254, dataLabelW, dataLabelH);
    const QRect cMuxLabel =          QRect(579, 349, 61,20);
    const QRect cMuxerLabel =        QRect(250, 374, dataLabelW, dataLabelH);
    const QRect cMuxTristateLabel =  QRect(550, 348, labelTriW, labelTriH);
    const QRect ALULineEdit  =       QRect(550, 368, 26, 20);
    const QRect ALULabel =           QRect(579, 370, 31, 20);
    const QRect ALUFunctionLabel =   QRect(332, 355, 98, 20);
    //const QPolygon ALUPoly = QPolygon(QRect);
    const QRect CSMuxLabel =         QRect(579, 399, labelW, labelH);
    const QRect CSMuxerDataLabel =   QRect(476+19-69, 399, dataLabelW, dataLabelH);
    const QRect CSMuxTristateLabel = QRect(550, 399, 25,21);
    const QRect SCkCheckBox =        QRect(550, 437, checkW, checkH);
    const QRect sBitLabel =          QRect(476, 437, 19, 19);
    const QRect CCkCheckBox =        QRect(550, 464, checkW, checkH);
    const QRect cBitLabel =          QRect(476, 463, 19, 19);
    const QRect VCkCheckBox =        QRect(550, 491, checkW, checkH);
    const QRect vBitLabel =          QRect(476, 491, 19, 19);
    const QRect AndZLabel =          QRect(579, 517, 45, 20);
    const QRect AndZTristateLabel =  QRect(415,542, 41,21);
    const QRect AndZMuxLabel =       QRect(550, 544, checkW, checkH);
    const QRect ZCkCheckBox =        QRect(476, 544, 19, 19);
    const QRect NCkCheckBox =        QRect(550, 582+4, checkW, checkH);
    const QRect nBitLabel =          QRect(476, 582+4, 19, 19);
    const QRect MemWriteLabel =      QRect(579, 611, check2W, check2H);
    const QRect MemWriteTristateLabel =  QRect(550, 611, labelTriW, labelTriH);
    const QRect MemReadLabel =       QRect(579, 631, check2W, check2H);
    const QRect MemReadTristateLabel =   QRect(550, 631, labelTriW, labelTriH);

    // registers

    // lines and shapes:
    const Arrow loadCkSelect = Arrow(QList<ArrowHead>()
                                     << ArrowHead(QPoint(499, 24), Left),
                                     QList<QLine>() << QLine(543, 27, 499, 27));
    const Arrow CSelect = Arrow(QList<ArrowHead>() << ArrowHead(QPoint(499, 47), Left),
                                QList<QLine>() << QLine(543, 50, 499, 50)
                                               << QLine(543, 50, 499, 50));
    const Arrow BSelect = Arrow(QList<ArrowHead>() << ArrowHead(QPoint(499, 69), Left),
                                QList<QLine>() << QLine(543, 72, 499, 72)
                                               << QLine(523, 67, 533, 77));
    const Arrow ASelect = Arrow(QList<ArrowHead>() << ArrowHead(QPoint(499, 91), Left),
                                QList<QLine>() << QLine(543, 94, 499, 94)
                                               << QLine(523, 89, 533, 99));
    const Arrow MARCk = Arrow(QList<ArrowHead>()
                                            << ArrowHead(QPoint(232,155), Up)
                                            << ArrowHead(QPoint(232,191), Down),
                              QList<QLine > () << QLine(428,177, 543,177)
                                            << QLine(367,177, 416,177)
                                            << QLine(291,177, 355,177)
                                            << QLine(235,177, 279,177)
                                            << QLine(235,163, 235,191));
    const Arrow MDRCk = Arrow(QList<ArrowHead>()
                                            << ArrowHead(QPoint(207,241), Down),
                              QList<QLine>() << QLine(210,233, 279,233)
                              << QLine(291,233, 355,233)
                              << QLine(367,233, 416,233)
                              << QLine(428,233, 543,233)
                              << QLine(210,233, 210,241));
    const Arrow AMuxSelect = Arrow(QList<ArrowHead>()
                                                 << ArrowHead(QPoint(380,300), Left),
                                   QList<QLine>() << QLine(388, 303, 416, 303)
                                   << QLine(428, 303, 543, 303));
    const QPolygon AMuxBus = QPolygon(QVector<QPoint>() << QPoint(336,312)
                                      << QPoint(336,331)
                                      << QPoint(331,331)
                                      << QPoint(341,341)
                                      << QPoint(351,331)
                                      << QPoint(346,331)
                                      << QPoint(346,312));
    const Arrow CMuxSelect = Arrow(QList<ArrowHead>()
                                                 << ArrowHead(QPoint(257,362), Down),
                                   QList<QLine>() << QLine(449,355, 543,355)
                                   << QLine(315,355, 290,355)
                                   << QLine(280,355, 260,355)
                                   << QLine(260,355, 260,365));
    const QPolygon CMuxBus = QPolygon(QVector<QPoint>() << QPoint(290,374)
                                      << QPoint(290,130) << QPoint(295,130)
                                      << QPoint(285,120) << QPoint(275,130)
                                      << QPoint(280,130) << QPoint(280,334)
                                      << QPoint(240,334) << QPoint(240,326)
                                      << QPoint(245,326) << QPoint(235,316)
                                      << QPoint(225,326) << QPoint(230,326)
                                      << QPoint(230,344) << QPoint(280,344)
                                      << QPoint(280,374));



}

#endif // SHAPES_ONE_BYTE_DATA_BUS_H











