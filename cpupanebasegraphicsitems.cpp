// File: cpupanebasegraphicsitems.cpp
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

#include "cpupanebasegraphicsitems.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QGraphicsScene>
#include <QPainter>

#include <QDebug>
#include "sim.h"

CpuPaneBaseGraphicsItems::CpuPaneBaseGraphicsItems(QWidget *widgetParent, QGraphicsItem *itemParent, QGraphicsScene *scene)
    : QGraphicsItem(itemParent),
      parent(widgetParent)
{

    seqCircuitColor = QColor(0xffdd77);// 0xffee99); //QColor(Qt::gray).lighter(140);
    combCircuitRed = QColor(Qt::red).lighter(190);
    combCircuitBlue = QColor(Qt::blue).lighter(190);
    combCircuitYellow = QColor(Qt::yellow).lighter(170);
    combCircuitGreen = QColor(Qt::green).lighter(190);

    // setup arrow heads:
    arrowLeft = QImage(":/images/arrowhead.png");
    arrowRight = arrowLeft.mirrored(true, false);
    QTransform t;
    t.rotate(90);
    arrowUp = arrowLeft.transformed(t);
    t.rotate(-180);
    arrowDown = arrowLeft.transformed(t);

    arrowLeftGray = QImage(":/images/arrowhead_gray.png");
    arrowRightGray = arrowLeftGray.mirrored(true, false);
    QTransform t_gray;
    t_gray.rotate(90);
    arrowUpGray = arrowLeftGray.transformed(t_gray);
    t_gray.rotate(-180);
    arrowDownGray = arrowLeftGray.transformed(t_gray);

    // LoadCk
    loadCk = new QCheckBox("LoadCk");
    loadCk->setPalette(QPalette(Qt::white));
    loadCk->setGeometry(550, 18, 80, 20);
    scene->addWidget(loadCk);

    // C
    // Note: the line edits must be added first, otherwise they cover the labels that go with them.
    QRegExp cbaRegExp("^((3[0-1])|([0-2][0-9])|([0-9]))$");
    cLineEdit = new QLineEdit();
    cLineEdit->setAlignment(Qt::AlignCenter);
    cLineEdit->setGeometry(550, 39, 25, 21);
    cLineEdit->setValidator(new QRegExpValidator(cbaRegExp, cLineEdit));
    cLineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(cLineEdit);
    cLabel = new QLabel("C");
    cLabel->setPalette(QPalette(Qt::white));
    cLabel->setGeometry(579, 41, 20, 21);
    scene->addWidget(cLabel);

    // B
    bLineEdit = new QLineEdit();
    bLineEdit->setAlignment(Qt::AlignCenter);
    bLineEdit->setGeometry(550, 61, 25, 21);
    bLineEdit->setValidator(new QRegExpValidator(cbaRegExp, bLineEdit));
    bLineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(bLineEdit);
    bLabel = new QLabel("B");
    bLabel->setGeometry(579, 63, 20, 21);
    bLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(bLabel);

    // A
    aLineEdit = new QLineEdit();
    aLineEdit->setAlignment(Qt::AlignCenter);
    aLineEdit->setGeometry(550,83, 25, 21);
    aLineEdit->setValidator(new QRegExpValidator(cbaRegExp, aLineEdit));
    aLineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(aLineEdit);
    aLabel = new QLabel("A");
    aLabel->setGeometry(579, 85, 20, 21);
    aLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aLabel);

    // MARCk
    MARCk = new QCheckBox("MARCk");
    MARCk->setGeometry(550, 169, 80, 20);
    MARCk->setPalette(QPalette(Qt::white));
    scene->addWidget(MARCk);
    MARALabel = new QLabel("0x00");
    MARALabel->setGeometry(175,202, 69,19);
    MARALabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARALabel->setAutoFillBackground(false);
    MARALabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(MARALabel);
    MARBLabel = new QLabel("0x00");
    MARBLabel->setGeometry(175,132, 69, 19);
    MARBLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARBLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(MARBLabel);
    // MARA & MARB
    scene->addRect(175, 132, 69, 19);
    scene->addRect(175, 202, 69, 19);
    // MARBus (MARA/MARB output bus)
    QPolygonF poly;
    poly << QPoint(205,151) << QPoint(205,167) << QPoint(173-70,167) << QPoint(173-70,162)
         << QPoint(158-70,177) << QPoint(173-70,192) << QPoint(173-70,187) << QPoint(205,187)
         << QPoint(205,202) << QPoint(215,202) << QPoint(215,151);

    scene->addPolygon(poly, QPen(QBrush(Qt::black), 1), QBrush(Qt::yellow));
    QGraphicsLineItem *lineItem = scene->addLine(173-70, 177, 215, 177);
    lineItem->setZValue(1); // make sure this line appears above the bus


    // MDRCk
    MDRCk = new QCheckBox("MDRCk");
    MDRCk->setGeometry(550, 225, 80, 20);
    MDRCk->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRCk);
    // MDR
    scene->addRect(175, 254, 69, 19);

    aMuxLabel = new QLabel("AMux");
    aMuxLabel->setGeometry(579, 297, 42, 20);
    aMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aMuxLabel);
    aMuxerDataLabel = new QLabel("AMux");
    aMuxerDataLabel->setGeometry(306,293, 69,19);
    aMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    aMuxerDataLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aMuxerDataLabel);
    aMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    aMuxTristateLabel->setGeometry(550,295, 25,21);
    aMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    aMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aMuxTristateLabel);
    scene->addRect(550,295, 25,21, QPen(Qt::gray));
    // AMux
    aMuxerBorder = scene->addRect(306,293, 69,19);

    // MDRMux
    MDRMuxLabel = new QLabel("MDRMux");
    MDRMuxLabel->setGeometry(579, 318, 61, 20);
    MDRMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRMuxLabel);
    MDRMuxerDataLabel = new QLabel("MDRMux");
    MDRMuxerDataLabel->setGeometry(175,293, 69,19);
    MDRMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRMuxerDataLabel);
    MDRMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    MDRMuxTristateLabel->setGeometry(550, 316, 25, 20);
    MDRMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRMuxTristateLabel);
    scene->addRect(550, 316, 25, 20, QPen(Qt::gray));
    MDRLabel = new QLabel("0x00");
    MDRLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRLabel->setGeometry(175,254, 69,19);
    MDRLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(MDRLabel);
    scene->addRect(175, 293, 69, 19);
    // MDR data section
    scene->addRect(175, 254, 69, 19);
    // MDRBus (output from MDR)
    scene->addRect(244,258, 36,10, QPen(Qt::black), QBrush(Qt::yellow));
    poly.clear();
    poly << QPoint(290,258) << QPoint(326,258) << QPoint(326,280) << QPoint(331,280)
         << QPoint(321,290) << QPoint(311,280) << QPoint(316,280) << QPoint(316,268) << QPoint(290,268);
    scene->addPolygon(poly, QPen(Qt::black), QBrush(Qt::yellow));
    poly.clear();
    poly << QPoint(175,258) << QPoint(168-70,258) << QPoint(168-70,253) << QPoint(158-70,263)
         << QPoint(168-70,273) << QPoint(168-70,268) << QPoint(175,268);
    scene->addPolygon(poly, QPen(Qt::black), QBrush(Qt::yellow));


    // CMux
    cMuxLabel = new QLabel("CMux");
    cMuxLabel->setGeometry(QRect(579,349, 61,20));
    cMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(cMuxLabel);
    cMuxerLabel = new QLabel("CMux");
    cMuxerLabel->setGeometry(250, 374, 69, 19);
    cMuxerLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cMuxerLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(cMuxerLabel);
    cMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    cMuxTristateLabel->setGeometry(550, 348, 25, 20);
    cMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(cMuxTristateLabel);
    scene->addRect(550, 348, 25, 20, QPen(Qt::gray));
    scene->addRect(250,374, 69,19);

    // ALU
    // keep this before the label that goes with it, or the line edit appears on top of the label
    ALULineEdit = new QLineEdit();
    ALULineEdit->setAlignment(Qt::AlignCenter);
    ALULineEdit->setGeometry(550, 368, 26, 20);
    ALULineEdit->setValidator(new QRegExpValidator(QRegExp("^((1[0-5])|(0[0-9])|[0-9])$"), ALULineEdit));
    ALULineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(ALULineEdit);
    ALULabel = new QLabel("ALU");
    ALULabel->setGeometry(579, 370, 31, 20);
    ALULabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ALULabel);

    ALUFunctionLabel = new QLabel("");
    ALUFunctionLabel->setGeometry(332, 355, 98, 20);
    ALUFunctionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ALUFunctionLabel->setPalette(QPalette(QColor(Qt::blue).lighter(190)));
    ALUFunctionLabel->setFont(QFont(ALUFunctionLabel->font().family(), ALUFunctionLabel->font().pointSize() - 2));
    scene->addWidget(ALUFunctionLabel);

    // ALU shape
    poly.clear();
    poly << QPoint(314,342) << QPoint(366,342) << QPoint(370,353) << QPoint(390,353) << QPoint(394,342)
         << QPoint(447,342) << QPoint(421,394) << QPoint(340,394);
    ALUPoly = scene->addPolygon(poly, QPen(QBrush(QColor(Qt::blue)), 2, Qt::SolidLine, Qt::SquareCap,
                                           Qt::BevelJoin), QBrush(QColor(Qt::blue).lighter(190)));
    ALUPoly->setZValue(-1);

    // CSMux
    CSMuxLabel = new QLabel("CSMux");
    CSMuxLabel->setGeometry(579, 399, 42, 20); // y: 397?
    CSMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(CSMuxLabel);
    CSMuxerDataLabel = new QLabel("CSMux");
    CSMuxerDataLabel->setGeometry(476+19-69,CSMuxLabel->y(), 69,19);
    CSMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    CSMuxerDataLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(CSMuxerDataLabel);
    CSMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    CSMuxTristateLabel->setGeometry(550,CSMuxLabel->y(), 25,21);
    CSMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    CSMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(CSMuxTristateLabel);
    scene->addRect(QRectF(CSMuxTristateLabel->pos(), CSMuxTristateLabel->size()), QPen(Qt::gray));
    scene->addRect(QRectF(CSMuxerDataLabel->pos(), CSMuxerDataLabel->size()));

    // SCk
    SCkCheckBox = new QCheckBox ("SCk");
    SCkCheckBox->setGeometry(550, 437, 60, 20);
    SCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(SCkCheckBox);
    sBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    sBitLabel->setText("0");
    sBitLabel->setGeometry(476,437, 19, 19);
    sBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    sBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(sBitLabel);

    // CCk
    CCkCheckBox = new QCheckBox ("CCk");
    CCkCheckBox->setGeometry(550, 464, 60, 20);
    CCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(CCkCheckBox);
    cBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    cBitLabel->setText("0");
    cBitLabel->setGeometry(476,463, 19, 19);
    cBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(cBitLabel);

    // VCk
    VCkCheckBox = new QCheckBox("VCk");
    VCkCheckBox->setGeometry(550, 491, 60, 20);
    VCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(VCkCheckBox);
    vBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    vBitLabel->setText("0");
    vBitLabel->setGeometry(476,491, 19, 19);
    vBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    vBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(vBitLabel);

    // ANDZ
    ANDZLabel = new QLabel("ANDZ");
    ANDZLabel->setGeometry(579, 517, 45, 20);
    ANDZLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ANDZLabel);
    ANDZTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    ANDZTristateLabel->setGeometry(550, 517, 25, 21);
    ANDZTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ANDZTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ANDZTristateLabel);
    scene->addRect(QRectF(ANDZTristateLabel->pos(), ANDZTristateLabel->size()), QPen(Qt::gray));

    ANDZMuxLabel = new QLabel("ANDZ");
    ANDZMuxLabel->setGeometry(415,542, 41,21);
    ANDZMuxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ANDZMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ANDZMuxLabel);
    scene->addRect(QRectF(ANDZMuxLabel->pos(), ANDZMuxLabel->size()));

    // ZCk
    ZCkCheckBox = new QCheckBox("ZCk");
    ZCkCheckBox->setGeometry(550, 544, 60, 20);
    ZCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(ZCkCheckBox);
    zBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    zBitLabel->setText("0");
    zBitLabel->setGeometry(476, 544, 19, 19);
    zBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    zBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(zBitLabel);

    // NCk
    NCkCheckBox = new QCheckBox ("NCk");
    NCkCheckBox->setGeometry(550, 582+4, 60, 20);
    NCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(NCkCheckBox);
    nBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    nBitLabel->setText("0");
    nBitLabel->setGeometry(476,582+4, 19, 19);
    nBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    nBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(nBitLabel);

    // Status Bits
    scene->addRect(QRectF(cBitLabel->pos(), cBitLabel->size())); // C
    scene->addRect(QRectF(vBitLabel->pos(), vBitLabel->size())); // V
    scene->addRect(QRectF(zBitLabel->pos(), zBitLabel->size())); // Z
    scene->addRect(QRectF(nBitLabel->pos(), nBitLabel->size())); // N
    scene->addRect(QRectF(sBitLabel->pos(), sBitLabel->size())); // S

    MemReadLabel = new QLabel("MemRead");
    MemReadLabel->setGeometry(579, 611, 80, 20);
    MemReadLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemReadLabel);
    MemReadTristateLabel = new TristateLabel(0, TristateLabel::OneUndefined);
    MemReadTristateLabel->setGeometry(550, 611, 25, 20);
    MemReadTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemReadTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemReadTristateLabel);
    scene->addRect(QRectF(MemReadTristateLabel->pos(), MemReadTristateLabel->size()), QPen(Qt::gray));

    MemWriteLabel = new QLabel("MemWrite");
    MemWriteLabel->setGeometry(579, 631, 80, 20);
    MemWriteLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemWriteLabel);
    MemWriteTristateLabel = new TristateLabel(0, TristateLabel::OneUndefined);
    MemWriteTristateLabel->setGeometry(550, 631, 25, 20);
    MemWriteTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemWriteTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemWriteTristateLabel);
    scene->addRect(QRectF(MemWriteTristateLabel->pos(), MemWriteTristateLabel->size()), QPen(Qt::gray));

    // Registers
    scene->addRect(5, 5, 491, 113, QPen(QBrush(QColor(Qt::red), Qt::SolidPattern), 2, Qt::DotLine, Qt::SquareCap, Qt::BevelJoin),
                   QBrush(seqCircuitColor));

    QLabel *ph;
    ph = new QLabel("0,1");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("A");
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,10, 20,22);
    scene->addWidget(ph);
    aRegLineEdit = new QLineEdit();
    aRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    aRegLineEdit->setGeometry(52,10, 60,19);
    aRegLineEdit->setText("0x0000");
    aRegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), aRegLineEdit));
    aRegLineEdit->setPalette(QPalette(seqCircuitColor));
    aRegLineEdit->setFrame(false);
    scene->addWidget(aRegLineEdit);
    //    QObject::connect(A, SIGNAL(valueChanged()), this, SLOT(slotRegisterChanged()));

    ph = new QLabel("2,3");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("X");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    xRegLineEdit = new QLineEdit();
    xRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    xRegLineEdit->setGeometry(53,38, 60,19);
    xRegLineEdit->setText("0x0000");
    xRegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), xRegLineEdit));
    xRegLineEdit->setPalette(QPalette(seqCircuitColor));
    xRegLineEdit->setFrame(false);
    scene->addWidget(xRegLineEdit);

    ph = new QLabel("4,5");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("SP");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    spRegLineEdit = new QLineEdit();
    spRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    spRegLineEdit->setGeometry(52,66, 60,19);
    spRegLineEdit->setText("0x0000");
    spRegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}[0-9a-fA-F]{0,4}"), spRegLineEdit));
    spRegLineEdit->setPalette(QPalette(seqCircuitColor));
    spRegLineEdit->setFrame(false);
    scene->addWidget(spRegLineEdit);

    ph = new QLabel("6,7");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("PC");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    pcRegLineEdit = new QLineEdit();
    pcRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pcRegLineEdit->setGeometry(52,94, 60,19);
    pcRegLineEdit->setText("0x0000");
    pcRegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), pcRegLineEdit));
    pcRegLineEdit->setPalette(QPalette(seqCircuitColor));
    pcRegLineEdit->setFrame(false);
    scene->addWidget(pcRegLineEdit);
    //    QObject::connect(X, SIGNAL(valueChanged()), this, SLOT(slotRegisterChanged()));

    ph = new QLabel("8-10");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,10, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("IR");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    irRegLineEdit = new QLineEdit();
    irRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    irRegLineEdit->setGeometry(182,10, 76,19);
    irRegLineEdit->setText("0x000000");
    irRegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,6}"), irRegLineEdit));
    irRegLineEdit->setPalette(QPalette(seqCircuitColor));
    irRegLineEdit->setFrame(false);
    scene->addWidget(irRegLineEdit);

    ph = new QLabel("11");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,38, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T1");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t1RegLineEdit = new QLineEdit();
    t1RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t1RegLineEdit->setGeometry(182,38, 45,19);
    t1RegLineEdit->setText("0x00");
    t1RegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,2}"), t1RegLineEdit));
    t1RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t1RegLineEdit->setFrame(false);
    scene->addWidget(t1RegLineEdit);

    ph = new QLabel("12,13");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,66, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T2");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t2RegLineEdit = new QLineEdit();
    t2RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t2RegLineEdit->setGeometry(182,66, 60,19);
    t2RegLineEdit->setText("0x0000");
    t2RegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), t2RegLineEdit));
    t2RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t2RegLineEdit->setFrame(false);
    scene->addWidget(t2RegLineEdit);

    ph = new QLabel("14,15");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,93, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T3");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t3RegLineEdit = new QLineEdit();
    t3RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t3RegLineEdit->setGeometry(182,94, 60,19);
    t3RegLineEdit->setText("0x0000");
    t3RegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), t3RegLineEdit));
    t3RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t3RegLineEdit->setFrame(false);
    scene->addWidget(t3RegLineEdit);

    ph = new QLabel("16,17");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,10, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T4");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t4RegLineEdit = new QLineEdit();
    t4RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t4RegLineEdit->setGeometry(311,10, 60,19);
    t4RegLineEdit->setText("0x0000");
    t4RegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), t4RegLineEdit));
    t4RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t4RegLineEdit->setFrame(false);
    scene->addWidget(t4RegLineEdit);

    ph = new QLabel("18,19");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,38, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T5");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t5RegLineEdit = new QLineEdit();
    t5RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t5RegLineEdit->setGeometry(311,38, 60,19);
    t5RegLineEdit->setText("0x0000");
    t5RegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), t5RegLineEdit));
    t5RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t5RegLineEdit->setFrame(false);
    scene->addWidget(t5RegLineEdit);

    ph = new QLabel("20,21");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,66, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T6");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t6RegLineEdit = new QLineEdit();
    t6RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t6RegLineEdit->setGeometry(311,66, 60,19);
    t6RegLineEdit->setText("0x0000");
    t6RegLineEdit->setValidator(new QRegExpValidator(QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"), t6RegLineEdit));
    t6RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t6RegLineEdit->setFrame(false);
    scene->addWidget(t6RegLineEdit);

    ph = new QLabel("22,23");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,93, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M1");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0001");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(313,94, 55,19);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("24,25");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,10, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M2");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0203");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(440,11, 55,19);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("26,27");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,38, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M3");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0408");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(440,39, 55,19);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("28,29");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,66, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M4");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0xFAFC");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(440,67, 55,19);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("30,31");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,93, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M5");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0xFEFF");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(440,94, 55,19);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    scene->addRect(5, 5, 491, 113, QPen(QBrush(QColor(Qt::red), Qt::SolidPattern),
                                        2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    // NZVC data path
    poly.clear();
    poly << QPoint(310,513) << QPoint(269,513) << QPoint(269,407) << QPoint(274,407) << QPoint(264,397)
         << QPoint(254,407) << QPoint(259,407) << QPoint(259,523) << QPoint(310,523);
    scene->addPolygon(poly, QPen(QBrush(Qt::black), 1), QBrush(Qt::yellow));
    scene->addLine(310, 477, 310, 559);
}

QRectF CpuPaneBaseGraphicsItems::boundingRect() const
{
    return QRectF(0,0, 650, 620);
}

bool CpuPaneBaseGraphicsItems::aluHasCorrectOutput()
{
    if (ALULineEdit->text() == "") {
        return false;
    }
    bool ok;
    int alu = ALULineEdit->text().toInt(&ok);
    if (!ok) {
        qDebug() << "ALU text to int conversion failed - non-number in the ALU line edit";
        return false;
    }

    if (Sim::aluFnIsUnary(alu)) { // unary
        if (aMuxTristateLabel->text() == "0") {
            return true;
        }
        else if (aMuxTristateLabel->text() == "1" && aLineEdit->text() != "") {
            return true;
        }
        else {
            return false;
        }
    }
    else { // nonunary
        if (aMuxTristateLabel->text() == "0" && bLineEdit->text() != "") {
            return true;
        }
        else if (aMuxTristateLabel->text() == "1" && aLineEdit->text() != "" && bLineEdit->text() != "") {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

void CpuPaneBaseGraphicsItems::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(Qt::black);

    // select line text
    painter->drawText(528,92, "5");
    painter->drawText(528,70, "5");
    painter->drawText(528,48, "5");
    painter->drawText(528,374, "4");

    painter->drawText(314,531, "0");
    painter->drawText(314,541, "0");
    painter->drawText(314,551, "0");
    painter->drawText(314,561, "0");

    painter->drawText(368,388, "ALU");

    painter->drawText(372,132, "ABus");
    painter->drawText(433,132, "BBus");
    painter->drawText(300,132, "CBus");
    painter->drawText(138,215, "MARA");
    painter->drawText(138,145, "MARB");

    repaintLoadCk(painter);
    repaintCSelect(painter);
    repaintBSelect(painter);
    repaintASelect(painter);
    repaintMARCk(painter);
    repaintMDRCk(painter);
    repaintAMuxSelect(painter);

    repaintCMuxSelect(painter);

    repaintCSMuxSelect(painter);

    repaintSCk(painter);
    repaintCCk(painter);
    repaintVCk(painter);
    repaintZCk(painter);
    repaintNCk(painter);
    repaintMemRead(painter);
    repaintMemWrite(painter);
    repaintSBitOut(painter);
    repaintCBitOut(painter);
    repaintVBitOut(painter);
    repaintZBitOut(painter);
    repaintNBitOut(painter);

    repaintANDZSelect(painter);
    repaintALUSelect(painter);
    repaintMDRMuxSelect(painter);

}

void CpuPaneBaseGraphicsItems::repaintLoadCk(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = loadCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(543, 27, 499, 27);

    painter->drawImage(QPoint(499,24), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintCSelect(QPainter *painter)
{
    bool ok;
    QPolygon poly;
    QColor color;

    cLineEdit->text().toInt(&ok, 10);
    ok ? color = Qt::black : color = Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    painter->drawLine(543, 50, 499, 50);
    painter->drawLine(523, 45, 533, 55);

    painter->drawImage(QPoint(499,47), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintBSelect(QPainter *painter)
{
    bool ok;
    QPolygon poly;
    QColor color;

    bLineEdit->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    painter->drawLine(543, 72, 499, 72);
    painter->drawLine(523, 67, 533, 77);

    // arrow
    painter->drawImage(QPoint(499,69), color == Qt::gray ? arrowLeftGray : arrowLeft);

    color = ok ? Qt::red : Qt::white;
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    painter->drawRect(290, 136, 67, 11);

    // BBus
    poly.clear();
    poly << QPoint(417, 118) << QPoint(417, 136) << QPoint(366, 136) << QPoint(366, 146)
         << QPoint(417, 146) << QPoint(417, 331) << QPoint(412, 331) << QPoint(422, 341)
         << QPoint(432, 331) << QPoint(427, 331) << QPoint(427, 118);
    painter->drawPolygon(poly);

    poly.clear();
    poly << QPoint(280, 136) << QPoint(258, 136) << QPoint(258, 131) << QPoint(248, 141)
         << QPoint(258, 151)<< QPoint(258, 146) << QPoint(280, 146) << QPoint(280, 136);
    painter->drawPolygon(poly);
}

void CpuPaneBaseGraphicsItems::repaintASelect(QPainter *painter)
{

    bool ok;
    QColor color;
    QPolygon poly;

    aLineEdit->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    painter->drawLine(543,94, 499,94);
    painter->drawLine(523,89, 533,99);

    painter->drawImage(QPoint(499,91), color == Qt::gray ? arrowLeftGray : arrowLeft);

    color = ok ? Qt::red : Qt::white;

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);
    // ABus
    poly.clear();
    poly << QPoint(356,118) << QPoint(356,207) << QPoint(290,207) << QPoint(290,217) << QPoint(356,217) << QPoint(356,280)
         << QPoint(351,280) << QPoint(361,290) << QPoint(371,280) << QPoint(366,280) << QPoint(366,118);
    painter->drawPolygon(poly);
    poly.setPoints(8, 280,207, 258,207, 258,202, 248,212, 258,222, 258,217, 280,217, 280,207);
    painter->drawPolygon(poly);

}

void CpuPaneBaseGraphicsItems::repaintMARCk(QPainter *painter)
{
    QColor color;

    color = MARCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MARCk
    painter->drawLine(428,177, 543,177);
    painter->drawLine(367,177, 416,177);
    painter->drawLine(291,177, 355,177);
    painter->drawLine(235,177, 279,177);
    painter->drawLine(235,163, 235,191);

    painter->drawImage(QPoint(232,155), color == Qt::gray ? arrowUpGray : arrowUp);
    painter->drawImage(QPoint(232,191), color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuPaneBaseGraphicsItems::repaintMDRCk(QPainter *painter)
{
    QColor color;

    color = MDRCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MDRCk
    painter->drawLine(210,233, 279,233);
    painter->drawLine(291,233, 355,233);
    painter->drawLine(367,233, 416,233);
    painter->drawLine(428,233, 543,233);
    painter->drawLine(210,233, 210,241);

    painter->drawImage(QPoint(207,241), color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuPaneBaseGraphicsItems::repaintAMuxSelect(QPainter *painter)
{
    QColor color;
    QPolygon poly;
    bool ok;
    int aMux = aMuxTristateLabel->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // AMux Select
    painter->drawLine(388, 303, 416, 303);
    painter->drawLine(428, 303, 543, 303);

    painter->drawImage(QPoint(380,300), color == Qt::gray ? arrowLeftGray : arrowLeft);

    if (ok) {
        switch (aMux) {
        case (0):
            color = Qt::yellow;
            aMuxerDataLabel->setPalette(QPalette(combCircuitYellow));
            break;
        case (1):
            if (aLineEdit->text() == "") { // ABus.state == UNDEFINED
                color = Qt::white;
                aMuxerDataLabel->setPalette(QPalette(Qt::white));
            } else {
                color = Qt::red;
                aMuxerDataLabel->setPalette(QPalette(combCircuitRed));
            }
            break;
        }
    } else {
        aMuxerDataLabel->setPalette(QPalette(Qt::white));
        color = Qt::white;
    }
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // AMux bus
    poly.setPoints(7, 336,312, 336,331, 331,331, 341,341, 351,331, 346,331, 346,312);
    painter->drawPolygon(poly);
}

void CpuPaneBaseGraphicsItems::repaintCMuxSelect(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = cMuxTristateLabel->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // CMux Select
    painter->drawLine(449,355, 543,355);
    painter->drawLine(315,355, 290,355);
    painter->drawLine(280,355, 260,355);
    painter->drawLine(260,355, 260,365);

    painter->drawImage(QPoint(257,362), color == Qt::gray ? arrowDownGray : arrowDown);

    if (cMuxTristateLabel->text() == "0") {
        color = Qt::yellow;
        cMuxerLabel->setPalette(QPalette(combCircuitYellow));
    }
    else if (cMuxTristateLabel->text() == "1") {
        if (!aluHasCorrectOutput() || ALULineEdit->text() == "15") { // CBus.state == UNDEFINED or NZVC A
            qDebug() << "WARNING: CMux select: There is no ALU output";
            cMuxerLabel->setPalette(QPalette(Qt::white));
            color = Qt::white;
        }
        else {
            cMuxerLabel->setPalette(QPalette(combCircuitBlue));
            color = Qt::blue;
        }
    }
    else {
        cMuxerLabel->setPalette(QPalette(Qt::white));
        color = Qt::white;
    }
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // CMuxBus (output)
    poly.clear();
    poly << QPoint(290,374) << QPoint(290,130) << QPoint(295,130) << QPoint(285,120)
         << QPoint(275,130) << QPoint(280,130) << QPoint(280,334) << QPoint(240,334)
         << QPoint(240,326) << QPoint(245,326) << QPoint(235,316) << QPoint(225,326)
         << QPoint(230,326) << QPoint(230,344) << QPoint(280,344) << QPoint(280,374);
    painter->drawPolygon(poly);
}

void CpuPaneBaseGraphicsItems::repaintSCk(QPainter *painter)
{
    QColor color;

    color = SCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    painter->drawLine(510,SCkCheckBox->y()+8, 543,SCkCheckBox->y()+8);

    // arrow
    painter->drawImage(QPoint(502,SCkCheckBox->y()+8-3), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintCCk(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = CCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    painter->drawLine(510,CCkCheckBox->y()+8, 543,CCkCheckBox->y()+8);

    // arrow
    painter->drawImage(QPoint(502,469), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintVCk(QPainter *painter)
{
    QColor color;
    QPolygon poly;

    color = VCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510,VCkCheckBox->y()+8, 543,VCkCheckBox->y()+8);

    painter->drawImage(QPoint(502,496), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintZCk(QPainter *painter)
{
    QColor color;
    QPolygon poly;

    color = ZCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510, 552, 543, 552);

    painter->drawImage(QPoint(502,549), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintNCk(QPainter *painter)
{
    QColor color;
    QPolygon poly;

    color = NCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510, 586+8, 543, 586+8);

    painter->drawImage(QPoint(502,586+8-3), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintMemRead(QPainter *painter)
{
    QPolygon poly;
    QColor color;
    bool isHigh = MemReadTristateLabel->text() == "1";

    // Draw memread select line
    if (isHigh) {
        MemWriteTristateLabel->setDisabled(true);
        color = Qt::black;
    }
    else {
        MemWriteTristateLabel->setDisabled(false);
        color = Qt::gray;
    }
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(166-70,611+8, 543,611+8); // memRead line from the label to the bus

    painter->drawImage(QPoint(158-70,611+8-3), color == Qt::gray ? arrowLeftGray : arrowLeft);

    if (MemWriteTristateLabel->text() == "1") {
        // Do not paint main bus if MemWrite is isHigh
        return;
    }

    // Draw main bus
    if (isHigh) {
        if (Sim::mainBusState == Enu::None || Sim::mainBusState == Enu::MemReadFirstWait) {
            // We haven't memRead yet, but are about to
            color = Qt::yellow;
        }
        else if ((Sim::mainBusState == Enu::MemReadReady || Sim::mainBusState == Enu::MemReadSecondWait) && MemReadTristateLabel->text() == "1") {
            // We have read twice, and are about to again
            color = QColor(16, 150, 24); // green
        }
        else {
            color = Qt::white;
        }
    }
    else {
        // Only repaint white if MemWrite is not set isHigh
        color = Qt::white;
    }

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // MainBus
    poly.clear();
    // oy.
    poly << QPoint(75, 132) << QPoint(85, 132) << QPoint(85, 334) << QPoint(180, 334)
         << QPoint(180, 326) << QPoint(175, 326) << QPoint(185, 316) << QPoint(195, 326) << QPoint(190, 326)
         << QPoint(190, 344) << QPoint(85, 344) << QPoint(85, 650) << QPoint(75, 650)
            // left arrow:
         << QPoint(75, 375)
         << QPoint(13, 375) << QPoint(13, 380) << QPoint(3, 370) << QPoint(13, 360) << QPoint(13, 365)
         << QPoint(75, 365);
    painter->drawPolygon(poly);

    if (Sim::mainBusState != Enu::MemReadReady) {
        color = Qt::white;
    }
    painter->setBrush(color);

    // MemOutBus
    poly.clear();
    poly << QPoint(0, 350) << QPoint(64, 350) << QPoint(64, 355) << QPoint(74, 345)
         << QPoint(64, 335) << QPoint(64, 340) << QPoint(0, 340);
    painter->drawPolygon(poly);
}

void CpuPaneBaseGraphicsItems::repaintMemWrite(QPainter *painter)
{
    QPolygon poly;
    QColor color;
    bool isHigh = MemWriteTristateLabel->text() == "1";

    // Draw memwrite select line
    if (isHigh) {
        MemReadTristateLabel->setDisabled(true);
        color = Qt::black;
    }
    else {
        MemReadTristateLabel->setDisabled(false);
        color = Qt::gray;
    }

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(166-70, 631+8, 543, 631+8); // memWrite line from label to bus

    painter->drawImage(QPoint(158-70,631+8-3), color == Qt::gray ? arrowLeftGray : arrowLeft);

    if (MemReadTristateLabel->text() == "1") {
        // Do not paint main bus if MemRead is high
        return;
    }

    // Draw main bus
    if (isHigh) {
        // qDebug() << "mainBusState: " << Sim::mainBusState;
        if (Sim::mainBusState == Enu::None || Sim::mainBusState == Enu::MemWriteFirstWait) {
            // We have not yet memWrite'n, but are about to clock for the first time
            color = Qt::yellow;
        }
        else if ((Sim::mainBusState == Enu::MemWriteSecondWait || Sim::mainBusState == Enu::MemWriteReady) && MemWriteTristateLabel->text() == "1") {
            // We have written twice, and are about to again
            color = QColor(16, 150, 24); // green
        }
        else {
            color = Qt::white;
        }
    }
    else {
        color = Qt::white;
    }

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // Main Bus
    poly.clear();
    poly << QPoint(145-70, 132) << QPoint(155-70, 132) << QPoint(155-70, 334) << QPoint(180, 334);
    poly << QPoint(180, 326) << QPoint(175, 326) << QPoint(185, 316) << QPoint(195, 326) << QPoint(190, 326)
         << QPoint(190, 344) << QPoint(155-70, 344) << QPoint(155-70, 631+19) << QPoint(145-70, 631+19)
         << QPoint(145-70, 375) << QPoint(136-123, 375) << QPoint(136-123, 380) << QPoint(126-123, 370)
         << QPoint(136-123, 360) << QPoint(136-123, 365) << QPoint(145-70, 365);
    painter->drawPolygon(poly);

    painter->setBrush(Qt::white);

    // MemOutBus
    poly.clear();
    poly << QPoint(0, 350) << QPoint(134-70, 350) << QPoint(134-70, 355) << QPoint(144-70, 345) << QPoint(134-70, 335)
         << QPoint(134-70, 340) << QPoint(0, 340);
    painter->drawPolygon(poly);
}

void CpuPaneBaseGraphicsItems::repaintSBitOut(QPainter *painter)
{
    sBitLabel->text() = Sim::sBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from S bit to CSMux
    painter->drawLine(sBitLabel->x()+11,sBitLabel->y(), sBitLabel->x()+11,sBitLabel->y()-8);
    // arrow:
    painter->drawImage(QPoint(sBitLabel->x()+11-3,sBitLabel->y()-8-8), arrowUp);
}

void CpuPaneBaseGraphicsItems::repaintCBitOut(QPainter *painter)
{
    cBitLabel->text() = Sim::cBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from C bit to bus
    painter->drawLine(487,482, 487,486); // bitty bit under C bit
    painter->drawLine(330,486, 322,486);
    painter->drawLine(330,486, 487,486);

    // line from C bit to CSMux
    painter->drawLine(487,cBitLabel->pos().y()-4, 487,cBitLabel->pos().y()); // bitty bit above C bit
    painter->drawLine(CSMuxerDataLabel->pos().x()+8,cBitLabel->pos().y()-4, 487,cBitLabel->pos().y()-4);
    painter->drawLine(CSMuxerDataLabel->pos().x()+8,CSMuxerDataLabel->pos().y() + CSMuxerDataLabel->height()+8, CSMuxerDataLabel->pos().x()+8,cBitLabel->pos().y()-4);

    // CIN line back to the ALU
    painter->drawLine(CSMuxerDataLabel->x()+35,389, 443,389);
    painter->drawLine(CSMuxerDataLabel->x()+35,CSMuxerDataLabel->pos().y(), CSMuxerDataLabel->x()+35,389);

    // arrow to the NZVC data bus
    painter->drawImage(QPoint(314,483), arrowLeft);
    // CIN arrow to the ALU
    painter->drawImage(QPoint(435,386), arrowLeft);
    // arrow to the CSMux
    painter->drawImage(QPoint(CSMuxerDataLabel->x()+8-3,CSMuxerDataLabel->y()+CSMuxerDataLabel->height()+3), arrowUp);
}

void CpuPaneBaseGraphicsItems::repaintVBitOut(QPainter *painter)
{
    vBitLabel->text() = Sim::vBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    /* V out */
    painter->drawLine(487,vBitLabel->y()+vBitLabel->height(), 487,vBitLabel->y()+vBitLabel->height()+4); // bitty bit
    painter->drawLine(487,vBitLabel->y()+vBitLabel->height()+4, 352,vBitLabel->y()+vBitLabel->height()+4);
    painter->drawLine(352,513, 352,496);
    painter->drawLine(352,496, 322,496); // short line from the arrow

    painter->drawImage(QPoint(314,493), arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintZBitOut(QPainter *painter)
{
    zBitLabel->text() = Sim::zBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(487,563, 487,582); // vertical line to Z bit
    painter->drawLine(487,582, 341,582); // long horizontal line
    painter->drawLine(341,582, 341,506); // vertical line closest to arrowhead
    painter->drawLine(341,506, 322,506); // line from arrowhead on left

    // line up to ANDZ
    painter->drawLine(437,582, 437,574);

    painter->drawImage(QPoint(314,503), arrowLeft);
    painter->drawImage(QPoint(434,566), arrowUp);  // ANDZ arrow upwards
}

void CpuPaneBaseGraphicsItems::repaintNBitOut(QPainter *painter)
{
    nBitLabel->text() = Sim::nBit ? "1" : "0";

    QPolygon poly;
    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(487,605, 487,605+4);
    painter->drawLine(487,605+4, 330,605+4);
    painter->drawLine(330,605+4, 330,517);
    painter->drawLine(330,517, 322,517);

    painter->drawImage(QPoint(314,514), arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintCSMuxSelect(QPainter *painter)
{
    QColor color;

    color = CSMuxTristateLabel->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    painter->drawLine(510,CSMuxerDataLabel->y()+10, 543,CSMuxerDataLabel->y()+10);

    // arrow
    painter->drawImage(QPoint(502,CSMuxerDataLabel->y()+10-3), color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintANDZSelect(QPainter *painter)
{
    QPolygon poly;

    QColor color = Qt::gray;

    if (ANDZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // lines coming out of tristate label
    painter->drawLine(437,517+7+8, 437,517+7); // vertical line
    painter->drawLine(437,517+7, 543,517+7); // horiz line

    painter->drawImage(QPoint(434,517+7+8), color == Qt::gray ? arrowDownGray : arrowDown);

    color = Qt::gray;
    if (ALULineEdit->text() != "" && ANDZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // ANDZ out
    painter->drawLine(458,552, 465,552);

    painter->drawImage(QPoint(465,549), color == Qt::gray ? arrowRightGray : arrowRight);
}

void CpuPaneBaseGraphicsItems::repaintALUSelect(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = ALULineEdit->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // ALU Select
    painter->drawLine(449,376, 543,376);
    painter->drawLine(523,371, 533,381);

    painter->drawImage(QPoint(441,373), color == Qt::gray ? arrowLeftGray : arrowLeft);

    painter->setPen(Qt::black);

    if (ALULineEdit->text() != "" && ALULineEdit->text() != "15") {
        int aluFn = ALULineEdit->text().toInt();
        if (aMuxTristateLabel->text() == "0" && Sim::aluFnIsUnary(aluFn)) {
            painter->setBrush(Qt::blue);
        }
        else if (aMuxTristateLabel->text() == "0" && bLineEdit->text() != "") {
            painter->setBrush(Qt::blue);
        }
        else if (aMuxTristateLabel->text() == "1") {
            if (aLineEdit->text() != "" && Sim::aluFnIsUnary(aluFn)) {
                painter->setBrush(Qt::blue);
            }
            else if (aLineEdit->text() != "" && bLineEdit->text() != "") {
                painter->setBrush(Qt::blue);
            }
            else {
                painter->setBrush(Qt::white);
            }
        }
        else {
            painter->setBrush(Qt::white);
        }
    }
    else {
        painter->setBrush(Qt::white);
    }

    // CBus
    poly.setPoints(11, 346,394, 346,414, 314,414, 314,407,
                   319,407, 309,397, 299,407, 304,407, 304,424,
                   356,424, 356,394);
    painter->drawPolygon(poly);

    // Draw status bit lines
    painter->setPen(aluHasCorrectOutput() ? Qt::black : Qt::gray);
    painter->setBrush(aluHasCorrectOutput() ? Qt::black : Qt::gray);

    // N
    painter->drawLine(371,395, 371,586+8);
    painter->drawLine(371,586+8, 465,586+8);

    painter->drawImage(QPoint(465,586+8-3), color == Qt::gray ? arrowRightGray : arrowRight);

    // Z
    painter->drawLine(386,395, 386,552);
    painter->drawLine(386,552, 404,552);

    painter->drawImage(QPoint(404,549), color == Qt::gray ? arrowRightGray : arrowRight);

    // V
    painter->drawLine(401,395, 401,499);
    painter->drawLine(401,499, 466,499);

    painter->drawImage(QPoint(466,496), color == Qt::gray ? arrowRightGray : arrowRight);

    // C
    painter->drawLine(416,395, 416,472);
    painter->drawLine(416,472, 465,472);

    // S
    painter->drawLine(416,sBitLabel->y()+9, sBitLabel->x()-11,sBitLabel->y()+9);

    // arrow to SCk
    painter->drawImage(QPoint(sBitLabel->x()-11,sBitLabel->y()+9-3), color == Qt::gray ? arrowRightGray : arrowRight);

    // ANDZ
    painter->drawImage(QPoint(465,469), color == Qt::gray ? arrowRightGray : arrowRight);

}

void CpuPaneBaseGraphicsItems::repaintMDRMuxSelect(QPainter *painter)
{
    QPolygon poly;
    QColor color = Qt::gray;

    if (MDRMuxTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // MDRMux Select
    painter->drawLine(257,303, 265,303); painter->drawLine(265,303, 265,324);
    painter->drawLine(265,324, 279,324); painter->drawLine(291,324, 335,324);
    painter->drawLine(347,324, 416,324); painter->drawLine(428,324, 543,324);

    painter->drawImage(QPoint(249,300), color == Qt::gray ? arrowLeftGray : arrowLeft);

    painter->setPen(Qt::black);

    if (MDRMuxTristateLabel->text() == "0") {
        if ((Sim::mainBusState == Enu::MemReadSecondWait && MemReadTristateLabel->text() == "1") || Sim::mainBusState == Enu::MemReadReady) {
            MDRMuxerDataLabel->setPalette(QPalette(combCircuitGreen));
            painter->setBrush(QBrush(QColor(16, 150, 24))); // green
        }
        else {
            MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
            painter->setBrush(Qt::white);
        }
    }
    else if (MDRMuxTristateLabel->text() == "1") {
        if (cMuxTristateLabel->text() == "") {
            MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
            painter->setBrush(Qt::white);
        }
        else if (cMuxTristateLabel->text() == "0") {
            MDRMuxerDataLabel->setPalette(QPalette(combCircuitYellow));
            painter->setBrush(Qt::yellow);
        }
        else if (aluHasCorrectOutput()) {
            MDRMuxerDataLabel->setPalette(QPalette(combCircuitBlue));
            painter->setBrush(Qt::blue);
        }
    }
    else {
        MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
        painter->setBrush(Qt::white);
    }

    // MDRMuxBus (MDRMux to MDR arrow)
    poly.setPoints(7, 205,293, 205,286, 200,286, 210,276, 220,286, 215,286, 215,293); // arrow
    painter->drawPolygon(poly);
}

