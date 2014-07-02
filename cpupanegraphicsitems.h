// File: cpupanegraphicsitems.h
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
#ifndef CPUPANEGRAPHICSITEMS_H
#define CPUPANEGRAPHICSITEMS_H

#include <QGraphicsItem>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>

#include "tristatelabel.h"

class CpuPaneGraphicsItems : public QGraphicsItem
{
public:
    CpuPaneGraphicsItems(QWidget *widgetParent, QGraphicsItem *itemParent = 0, QGraphicsScene *scene = 0);
    QRectF boundingRect() const;

    bool aluHasCorrectOutput();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // OUTSIDE REGISTERS
    QCheckBox *loadCk;
    QLabel *cLabel;
    QLineEdit *cLineEdit;
    QLabel *bLabel;
    QLineEdit *bLineEdit;
    QLabel *aLabel;
    QLineEdit *aLineEdit;

    QCheckBox *MARCk;
    QLabel *MARALabel; // data section
    QLabel *MARBLabel; // data section
    QCheckBox *MDRCk;

    QLabel *aMuxLabel;
    QLabel *aMuxerDataLabel; // data section
    QGraphicsRectItem *aMuxerBorder; // data section
    TristateLabel *aMuxTristateLabel;

    QLabel *MDRMuxLabel;
    QLabel *MDRMuxerDataLabel; // data section
    TristateLabel *MDRMuxTristateLabel;
    QLabel *MDRLabel; // data section

    QLabel *cMuxLabel;
    TristateLabel *cMuxTristateLabel;
    QLabel *cMuxerLabel; // data section

    QLabel *ALULabel;
    QLineEdit *ALULineEdit;
    QLabel *ALUFunctionLabel; // data section
    QGraphicsPolygonItem *ALUPoly; //data section

    QCheckBox *CCkCheckBox;

    QCheckBox *VCkCheckBox;
    QLabel *ANDZLabel;
    TristateLabel *ANDZTristateLabel;
    QLabel *ANDZMuxLabel; // data section

    QCheckBox *ZCkCheckBox;

    QCheckBox *NCkCheckBox;

    TristateLabel *nBitLabel; // data section
    TristateLabel *zBitLabel; // data section
    TristateLabel *vBitLabel; // data section
    TristateLabel *cBitLabel; // data section

    QLabel *MemReadLabel;
    TristateLabel *MemReadTristateLabel;

    QLabel *MemWriteLabel;
    TristateLabel *MemWriteTristateLabel;

    // REGISTER BANK
    QLineEdit *aRegLineEdit;
    QLineEdit *xRegLineEdit;
    QLineEdit *spRegLineEdit;
    QLineEdit *pcRegLineEdit;
    QLineEdit *irRegLineEdit;
    QLineEdit *t1RegLineEdit;
    QLineEdit *t2RegLineEdit;
    QLineEdit *t3RegLineEdit;
    QLineEdit *t4RegLineEdit;
    QLineEdit *t5RegLineEdit;
    QLineEdit *t6RegLineEdit;

private:
    QGraphicsScene *parentScene;
    QWidget *parent;

    QColor seqCircuitColor;
    QColor combCircuitRed;
    QColor combCircuitBlue;
    QColor combCircuitYellow;
    QColor combCircuitGreen;

    void repaintLoadCk(QPainter *painter);
    void repaintCSelect(QPainter *painter);
    void repaintBSelect(QPainter *painter);
    void repaintASelect(QPainter *painter);
    void repaintMARCk(QPainter *painter);
    void repaintMDRCk(QPainter *painter);
    void repaintAMuxSelect(QPainter *painter);

    void repaintCMuxSelect(QPainter *painter);

    void repaintCCk(QPainter *painter);
    void repaintVCk(QPainter *painter);
    void repaintZCk(QPainter *painter);
    void repaintNCk(QPainter *painter);
    void repaintMemRead(QPainter *painter);
    void repaintMemWrite(QPainter *painter);
    void repaintCBitOut(QPainter *painter);
    void repaintVBitOut(QPainter *painter);
    void repaintZBitOut(QPainter *painter);
    void repaintNBitOut(QPainter *painter);

    void repaintANDZSelect(QPainter *painter);
    void repaintALUSelect(QPainter *painter);
    void repaintMDRMuxSelect(QPainter *painter);
};

#endif // CPUPANEGRAPHICSITEMS_H
