// File: objectcodelabel.cpp
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

#include "objectcodelabel.h"

#include <QPainter>

ObjectCodeLabel::ObjectCodeLabel(QWidget *parent) :
    QLabel(parent)
{

}

void ObjectCodeLabel::drawRotatedText(QPainter *painter, float degrees, int x, int y, const QString &text)
{
        painter->save();
        painter->translate(x, y);
        painter->rotate(degrees);
        painter->drawText(0, 0, text);
        painter->restore();
}

void ObjectCodeLabel::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QFontMetrics fm(this->font());
    int height = fm.averageCharWidth();

    // Draw the rotated text. Magic numbers are the offsets from the labels
    // to spread them out to correspond to the correct object code numbers.
#pragma message("todo: update to accomodate both one byte and two byte data busses")
    drawRotatedText(&p, 270, height * 2 - 2, this->height() - 2, "LoadCk");
    drawRotatedText(&p, 270, height * 4 - 2, this->height() - 2, "C");
    drawRotatedText(&p, 270, height * 7 - 2, this->height() - 2, "B");
    drawRotatedText(&p, 270, height * 10 - 2, this->height() - 2, "A");
    drawRotatedText(&p, 270, height * 13 - 2, this->height() - 2, "MARCk");
    drawRotatedText(&p, 270, height * 15 - 2, this->height() - 2, "MDRCk");
    drawRotatedText(&p, 270, height * 17 - 2, this->height() - 2, "AMux");
    drawRotatedText(&p, 270, height * 19 - 2, this->height() - 2, "MDRMux");
    drawRotatedText(&p, 270, height * 21 - 2, this->height() - 2, "CMux");
    drawRotatedText(&p, 270, height * 23 - 2, this->height() - 2, "ALU");
    drawRotatedText(&p, 270, height * 26 - 2, this->height() - 2, "CSMux");
    drawRotatedText(&p, 270, height * 28 - 2, this->height() - 2, "SCk");
    drawRotatedText(&p, 270, height * 30 - 2, this->height() - 2, "CCk");
    drawRotatedText(&p, 270, height * 32 - 2, this->height() - 2, "VCk");
    drawRotatedText(&p, 270, height * 34 - 2, this->height() - 2, "AndZ");
    drawRotatedText(&p, 270, height * 36 - 2, this->height() - 2, "ZCk");
    drawRotatedText(&p, 270, height * 38 - 2, this->height() - 2, "NCk");
    drawRotatedText(&p, 270, height * 40 - 2, this->height() - 2, "MemWrite");
    drawRotatedText(&p, 270, height * 42 - 2, this->height() - 2, "MemRead");
}

