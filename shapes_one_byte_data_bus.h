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

#include <QList>

class Arrow {
    Arrow(QPoint headPos, QLine line, QLine aux = NULL) {
        this->headPos = headPos;
        this->line = line;
        this->aux = aux;
    }
public:
    QPoint headPos;
    QLine line;
    QLine aux;
};

class Arrow {
    Arrow(QList<QPoint > points, QList<QLine > lines) {
        this->points = points;
        this->lines = lines;
    }
public:
    QList<QPoint > points;
    QList<QLine > lines;
};

namespace OneByteDataBusShapes
{

    const Arrow loadCk = Arrow(QPoint(499,24), QLine(543, 27, 499, 27));
    const Arrow CSelect = Arrow(QPoint(499,47), QLine(543, 50, 499, 50),
                                QLine(543, 50, 499, 50));
    const Arrow BSelect = Arrow(QPoint(499,69), QLine(543, 72, 499, 72),
                                QLine(523, 67, 533, 77));
    const Arrow ASelect = Arrow(QPoint(499,91), QLine(543,94, 499,94),
                                QLine(523,89, 533,99));
    const Arrow MARCk = Arrow(QList<QPoint>() << QPoint(232,155)
                              << QPoint(232,191),
                              QList<Qline>() << QLine(428,177, 543,177)
                              << QLine(367,177, 416,177)
                              << QLine(291,177, 355,177)
                              << QLine(235,177, 279,177)
                              << QLine(235,163, 235,191));
    const Arrow MDRCk = Arrow(QList<QPoint>({QPoint(207,241)}),
                              QList<QLine>() << QLine(210,233, 279,233)
                              << QLine(291,233, 355,233)
                              << QLine(367,233, 416,233)
                              << QLine(428,233, 543,233)
                              << QLine(210,233, 210,241));
    const Arrow AMuxSelect = Arrow(QList<QPoint>() << QPoint(380,300),
                                   QList<QLine>() << QLine(388, 303, 416, 303)
                                   << QLine(428, 303, 543, 303));
    const QPolygon AMuxBus = QPolygon(QVector<QPoint>() << QPoint(336,312)
                                      << QPoint(336,331)
                                      << QPoint(331,331)
                                      << QPoint(341,341)
                                      << QPoint(351,331)
                                      << QPoint(346,331)
                                      << QPoint(346,312));
    const Arrow CMuxSelect = Arrow(QList<QPoint() << QPoint(257,362),
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











