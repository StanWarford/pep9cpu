// File: cpupanebasegraphicsitems.cpp
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

#include "cpupanebasegraphicsitems.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QGraphicsScene>
#include <QPainter>

#include <QDebug>
#include "sim.h"

#include "shapes_one_byte_data_bus.h"

CpuPaneBaseGraphicsItems::CpuPaneBaseGraphicsItems(QWidget *widgetParent,
                                                   QGraphicsItem *itemParent,
                                                   QGraphicsScene *scene)
    : QGraphicsItem(itemParent),
      parent(widgetParent)
{

    MDROCk = new QCheckBox("MDROCk", parent);
    MDROCk->hide();
    MDRECk = new QCheckBox("MDRECk", parent);
    MDRECk->hide();
    MDROMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    MDROMuxTristateLabel->hide();
    MDREMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    MDREMuxTristateLabel->hide();
    EOMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    EOMuxTristateLabel->hide();
    MARMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    MARMuxTristateLabel->hide();


    seqCircuitColor = QColor(0xffdd77);
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
    loadCk = new QCheckBox("LoadCk", parent);
    loadCk->setPalette(QPalette(Qt::white));
    loadCk->setGeometry(OneByteShapes::loadCkCheckbox);
    scene->addWidget(loadCk);

    // C
    // Note: the line edits must be added first, otherwise they cover the
    //  labels that go with them.
    QRegExp cbaRegExp("^((3[0-1])|([0-2][0-9])|([0-9]))$");
    cLineEdit = new QLineEdit(parent);
    cLineEdit->setAlignment(Qt::AlignCenter);
    cLineEdit->setGeometry(OneByteShapes::cLineEdit);
    cLineEdit->setValidator(new QRegExpValidator(cbaRegExp, cLineEdit));
    cLineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(cLineEdit);
    cLabel = new QLabel("C", parent);
    cLabel->setPalette(QPalette(Qt::white));
    cLabel->setGeometry(OneByteShapes::cLabel);
    scene->addWidget(cLabel);

    // B
    bLineEdit = new QLineEdit(parent);
    bLineEdit->setAlignment(Qt::AlignCenter);
    bLineEdit->setGeometry(OneByteShapes::bLineEdit);
    bLineEdit->setValidator(new QRegExpValidator(cbaRegExp, bLineEdit));
    bLineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(bLineEdit);
    bLabel = new QLabel("B", parent);
    bLabel->setGeometry(OneByteShapes::bLabel);
    bLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(bLabel);

    // A
    aLineEdit = new QLineEdit(parent);
    aLineEdit->setAlignment(Qt::AlignCenter);
    aLineEdit->setGeometry(OneByteShapes::aLineEdit);
    aLineEdit->setValidator(new QRegExpValidator(cbaRegExp, aLineEdit));
    aLineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(aLineEdit);
    aLabel = new QLabel("A", parent);
    aLabel->setGeometry(OneByteShapes::aLabel);
    aLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aLabel);

    // MARCk
    MARCk = new QCheckBox("MARCk", parent);
    MARCk->setGeometry(OneByteShapes::MARCkCheckbox);
    MARCk->setPalette(QPalette(Qt::white));
    scene->addWidget(MARCk);
    MARALabel = new QLabel("0x00", parent);
    MARALabel->setGeometry(OneByteShapes::MARALabel);
    MARALabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARALabel->setAutoFillBackground(false);
    MARALabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(MARALabel);
    MARBLabel = new QLabel("0x00", parent);
    MARBLabel->setGeometry(OneByteShapes::MARBLabel);
    MARBLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARBLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(MARBLabel);
    // MARA & MARB
    scene->addRect(OneByteShapes::MARBLabel);
    scene->addRect(OneByteShapes::MARALabel);
    // MARBus (MARA/MARB output bus)
    scene->addPolygon(OneByteShapes::MARBus,
                      QPen(QBrush(Qt::black), 1), QBrush(Qt::yellow));

    // MDRCk
    MDRCk = new QCheckBox("MDRCk", parent);
    MDRCk->setGeometry(OneByteShapes::MDRCkCheckbox);
    MDRCk->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRCk);
    // MDR
    scene->addRect(OneByteShapes::MDRLabel);

    aMuxLabel = new QLabel("AMux", parent);
    aMuxLabel->setGeometry(OneByteShapes::aMuxLabel);
    aMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aMuxLabel);
    aMuxerDataLabel = new QLabel("AMux", parent);
    aMuxerDataLabel->setGeometry(OneByteShapes::aMuxerDataLabel);
    aMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    aMuxerDataLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aMuxerDataLabel);
    aMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    aMuxTristateLabel->setGeometry(OneByteShapes::aMuxTristateLabel);
    aMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    aMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(aMuxTristateLabel);
    scene->addRect(OneByteShapes::aMuxTristateLabel, QPen(Qt::gray));
    // AMux
    aMuxerBorder = scene->addRect(OneByteShapes::aMuxerDataLabel);

    // MDRMux
    MDRMuxLabel = new QLabel("MDRMux", parent);
    MDRMuxLabel->setGeometry(OneByteShapes::MDRMuxLabel);
    MDRMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRMuxLabel);
    MDRMuxerDataLabel = new QLabel("MDRMux", parent);
    MDRMuxerDataLabel->setGeometry(OneByteShapes::MDRMuxerDataLabel);
    MDRMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRMuxerDataLabel);
    MDRMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    MDRMuxTristateLabel->setGeometry(OneByteShapes::MDRMuxTristateLabel);
    MDRMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRMuxTristateLabel);
    scene->addRect(OneByteShapes::MDRMuxTristateLabel, QPen(Qt::gray));
    MDRLabel = new QLabel("0x00", parent);
    MDRLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRLabel->setGeometry(OneByteShapes::MDRLabel);
    MDRLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(MDRLabel);
    scene->addRect(OneByteShapes::MDRLabel);
    // MDR data section
    scene->addRect(OneByteShapes::MDRMuxerDataLabel);
    // MDRBus (output from MDR, right arrow):
    scene->addRect(OneByteShapes::MDRBusOutRect,
                   QPen(Qt::black), QBrush(Qt::yellow));
    scene->addPolygon(OneByteShapes::MDRBusOutArrow,
                      QPen(Qt::black), QBrush(Qt::yellow));
    // note: left arrow gets drawn in repaintMemWrite


    // CMux
    cMuxLabel = new QLabel("CMux", parent);
    cMuxLabel->setGeometry(OneByteShapes::cMuxLabel);
    cMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(cMuxLabel);
    cMuxerLabel = new QLabel("CMux", parent);
    cMuxerLabel->setGeometry(OneByteShapes::cMuxerLabel);
    cMuxerLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cMuxerLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(cMuxerLabel);
    cMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    cMuxTristateLabel->setGeometry(OneByteShapes::cMuxTristateLabel);
    cMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(cMuxTristateLabel);
    scene->addRect(OneByteShapes::cMuxTristateLabel, QPen(Qt::gray));
    scene->addRect(OneByteShapes::cMuxerLabel);

    // ALU
    // keep this before the label that goes with it, or the line edit
    //  appears on top of the label
    ALULineEdit = new QLineEdit(parent);
    ALULineEdit->setAlignment(Qt::AlignCenter);
    ALULineEdit->setGeometry(OneByteShapes::ALULineEdit);
    ALULineEdit->setValidator(new QRegExpValidator(
                                  QRegExp("^((1[0-5])|(0[0-9])|[0-9])$"),
                                  ALULineEdit));
    ALULineEdit->setPalette(QPalette(Qt::white));
    scene->addWidget(ALULineEdit);
    ALULabel = new QLabel("ALU", parent);
    ALULabel->setGeometry(OneByteShapes::ALULabel);
    ALULabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ALULabel);

    ALUFunctionLabel = new QLabel("", parent);
    ALUFunctionLabel->setGeometry(OneByteShapes::ALUFunctionLabel);
    ALUFunctionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ALUFunctionLabel->setPalette(QPalette(QColor(Qt::blue).lighter(190)));
    ALUFunctionLabel->setFont(QFont(ALUFunctionLabel->font().family(),
                                    ALUFunctionLabel->font().pointSize() - 2));
    scene->addWidget(ALUFunctionLabel);

    // ALU shape
    ALUPoly = scene->addPolygon(OneByteShapes::ALUPoly,
                                QPen(QBrush(QColor(Qt::blue)),
                                     2, Qt::SolidLine,
                                     Qt::SquareCap,
                                     Qt::BevelJoin),
                                QBrush(QColor(Qt::blue).lighter(190)));
    ALUPoly->setZValue(-1);

    // CSMux
    CSMuxLabel = new QLabel("CSMux", parent);
    CSMuxLabel->setGeometry(OneByteShapes::CSMuxLabel);
    CSMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(CSMuxLabel);
    CSMuxerDataLabel = new QLabel("CSMux", parent);
    CSMuxerDataLabel->setGeometry(OneByteShapes::CSMuxerDataLabel);
    CSMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    CSMuxerDataLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(CSMuxerDataLabel);
    CSMuxTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    CSMuxTristateLabel->setGeometry(OneByteShapes::CSMuxTristateLabel);
    CSMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    CSMuxTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(CSMuxTristateLabel);
    scene->addRect(QRectF(CSMuxTristateLabel->pos(),
                          CSMuxTristateLabel->size()),QPen(Qt::gray));
    scene->addRect(QRectF(CSMuxerDataLabel->pos(),
                          CSMuxerDataLabel->size()));

    // SCk
    SCkCheckBox = new QCheckBox ("SCk", parent);
    SCkCheckBox->setGeometry(OneByteShapes::SCkCheckBox);
    SCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(SCkCheckBox);
    sBitLabel = new TristateLabel(parent, TristateLabel::ZeroOne);
    sBitLabel->setText("0");
    sBitLabel->setGeometry(OneByteShapes::sBitLabel);
    sBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    sBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(sBitLabel);

    // CCk
    CCkCheckBox = new QCheckBox ("CCk", parent);
    CCkCheckBox->setGeometry(OneByteShapes::CCkCheckBox);
    CCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(CCkCheckBox);
    cBitLabel = new TristateLabel(parent, TristateLabel::ZeroOne);
    cBitLabel->setText("0");
    cBitLabel->setGeometry(OneByteShapes::cBitLabel);
    cBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(cBitLabel);

    // VCk
    VCkCheckBox = new QCheckBox("VCk", parent);
    VCkCheckBox->setGeometry(OneByteShapes::VCkCheckBox);
    VCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(VCkCheckBox);
    vBitLabel = new TristateLabel(parent, TristateLabel::ZeroOne);
    vBitLabel->setText("0");
    vBitLabel->setGeometry(OneByteShapes::vBitLabel);
    vBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    vBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(vBitLabel);

    // AndZ
    AndZLabel = new QLabel("AndZ", parent);
    AndZLabel->setGeometry(OneByteShapes::AndZLabel);
    AndZLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(AndZLabel);
    AndZTristateLabel = new TristateLabel(parent, TristateLabel::Tristate);
    AndZTristateLabel->setGeometry(OneByteShapes::AndZTristateLabel);
    AndZTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    AndZTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(AndZTristateLabel);
    scene->addRect(QRectF(AndZTristateLabel->pos(), AndZTristateLabel->size()),
                   QPen(Qt::gray));

    AndZMuxLabel = new QLabel("AndZ", parent);
    AndZMuxLabel->setGeometry(OneByteShapes::AndZMuxLabel);
    AndZMuxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    AndZMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(AndZMuxLabel);
    scene->addRect(OneByteShapes::AndZMuxLabel);

    // ZCk
    ZCkCheckBox = new QCheckBox("ZCk", parent);
    ZCkCheckBox->setGeometry(OneByteShapes::ZCkCheckBox);
    ZCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(ZCkCheckBox);
    zBitLabel = new TristateLabel(parent, TristateLabel::ZeroOne);
    zBitLabel->setText("0");
    zBitLabel->setGeometry(OneByteShapes::zBitLabel);
    zBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    zBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(zBitLabel);

    // NCk
    NCkCheckBox = new QCheckBox ("NCk", parent);
    NCkCheckBox->setGeometry(OneByteShapes::NCkCheckBox); //582+4
    NCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(NCkCheckBox);
    nBitLabel = new TristateLabel(parent, TristateLabel::ZeroOne);
    nBitLabel->setText("0");
    nBitLabel->setGeometry(OneByteShapes::nBitLabel); //582+4
    nBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    nBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(nBitLabel);

    // Status Bits
    scene->addRect(QRectF(cBitLabel->pos(), cBitLabel->size())); // C
    scene->addRect(QRectF(vBitLabel->pos(), vBitLabel->size())); // V
    scene->addRect(QRectF(zBitLabel->pos(), zBitLabel->size())); // Z
    scene->addRect(QRectF(nBitLabel->pos(), nBitLabel->size())); // N
    scene->addRect(QRectF(sBitLabel->pos(), sBitLabel->size())); // S

    MemWriteLabel = new QLabel("MemWrite", parent);
    MemWriteLabel->setGeometry(OneByteShapes::MemWriteLabel);
    MemWriteLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemWriteLabel);
    MemWriteTristateLabel = new TristateLabel(parent, TristateLabel::OneUndefined);
    MemWriteTristateLabel->setGeometry(OneByteShapes::MemWriteTristateLabel);
    MemWriteTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemWriteTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemWriteTristateLabel);
    scene->addRect(QRectF(MemWriteTristateLabel->pos(),
                          MemWriteTristateLabel->size()), QPen(Qt::gray));

    MemReadLabel = new QLabel("MemRead", parent);
    MemReadLabel->setGeometry(OneByteShapes::MemReadLabel);
    MemReadLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemReadLabel);
    MemReadTristateLabel = new TristateLabel(parent, TristateLabel::OneUndefined);
    MemReadTristateLabel->setGeometry(OneByteShapes::MemReadTristateLabel);
    MemReadTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemReadTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemReadTristateLabel);
    scene->addRect(QRectF(MemReadTristateLabel->pos(),
                          MemReadTristateLabel->size()), QPen(Qt::gray));


    // Registers
    scene->addRect(OneByteShapes::RegBank, QPen(QBrush(QColor(Qt::red), Qt::SolidPattern),
                                        2, Qt::DotLine,
                                        Qt::SquareCap,
                                        Qt::BevelJoin), QBrush(seqCircuitColor));

    QLabel *ph;
    ph = new QLabel("0,1", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("A", parent);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,10, 20,22);
    scene->addWidget(ph);
    aRegLineEdit = new QLineEdit("0x0000", parent);
    aRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    aRegLineEdit->setGeometry(OneByteShapes::aRegLineEdit);
    aRegLineEdit->setValidator(new QRegExpValidator(
                                   QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                   aRegLineEdit));
    aRegLineEdit->setPalette(QPalette(seqCircuitColor));
    aRegLineEdit->setFrame(false);
    scene->addWidget(aRegLineEdit);
    //    QObject::connect(A, SIGNAL(valueChanged()),
                           //this, SLOT(slotRegisterChanged()));

    ph = new QLabel("2,3", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("X", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    xRegLineEdit = new QLineEdit("0x0000", parent);
    xRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    xRegLineEdit->setGeometry(OneByteShapes::xRegLineEdit);
    xRegLineEdit->setValidator(new QRegExpValidator(
                                   QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                   xRegLineEdit));
    xRegLineEdit->setPalette(QPalette(seqCircuitColor));
    xRegLineEdit->setFrame(false);
    scene->addWidget(xRegLineEdit);

    ph = new QLabel("4,5", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("SP", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    spRegLineEdit = new QLineEdit("0x0000", parent);
    spRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    spRegLineEdit->setGeometry(OneByteShapes::spRegLineEdit);
    spRegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}[0-9a-fA-F]{0,4}"),
                                    spRegLineEdit));
    spRegLineEdit->setPalette(QPalette(seqCircuitColor));
    spRegLineEdit->setFrame(false);
    scene->addWidget(spRegLineEdit);

    ph = new QLabel("6,7", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(8,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("PC", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(30,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    pcRegLineEdit = new QLineEdit("0x0000", parent);
    pcRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pcRegLineEdit->setGeometry(OneByteShapes::pcRegLineEdit);
    pcRegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    pcRegLineEdit));
    pcRegLineEdit->setPalette(QPalette(seqCircuitColor));
    pcRegLineEdit->setFrame(false);
    scene->addWidget(pcRegLineEdit);
    //    QObject::connect(X, SIGNAL(valueChanged()),
                           //this, SLOT(slotRegisterChanged()));

    ph = new QLabel("8-10", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,10, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("IR", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    irRegLineEdit = new QLineEdit("0x000000", parent);
    irRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    irRegLineEdit->setGeometry(OneByteShapes::irRegLineEdit);
    irRegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,6}"),
                                    irRegLineEdit));
    irRegLineEdit->setPalette(QPalette(seqCircuitColor));
    irRegLineEdit->setFrame(false);
    scene->addWidget(irRegLineEdit);

    ph = new QLabel("11", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,38, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T1", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t1RegLineEdit = new QLineEdit("0x00", parent);
    t1RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t1RegLineEdit->setGeometry(OneByteShapes::t1RegLineEdit);
    t1RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,2}"),
                                    t1RegLineEdit));
    t1RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t1RegLineEdit->setFrame(false);
    scene->addWidget(t1RegLineEdit);

    ph = new QLabel("12,13", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,66, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T2", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t2RegLineEdit = new QLineEdit("0x0000", parent);
    t2RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t2RegLineEdit->setGeometry(OneByteShapes::t2RegLineEdit);
    t2RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t2RegLineEdit));
    t2RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t2RegLineEdit->setFrame(false);
    scene->addWidget(t2RegLineEdit);

    ph = new QLabel("14,15", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(128,93, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T3", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(160,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t3RegLineEdit = new QLineEdit("0x0000", parent);
    t3RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t3RegLineEdit->setGeometry(OneByteShapes::t3RegLineEdit);
    t3RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t3RegLineEdit));
    t3RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t3RegLineEdit->setFrame(false);
    scene->addWidget(t3RegLineEdit);

    ph = new QLabel("16,17", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,10, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T4", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t4RegLineEdit = new QLineEdit("0x0000", parent);
    t4RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t4RegLineEdit->setGeometry(OneByteShapes::t4RegLineEdit);
    t4RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t4RegLineEdit));
    t4RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t4RegLineEdit->setFrame(false);
    scene->addWidget(t4RegLineEdit);

    ph = new QLabel("18,19", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,38, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T5", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t5RegLineEdit = new QLineEdit("0x0000", parent);
    t5RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t5RegLineEdit->setGeometry(OneByteShapes::t5RegLineEdit);
    t5RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t5RegLineEdit));
    t5RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t5RegLineEdit->setFrame(false);
    scene->addWidget(t5RegLineEdit);

    ph = new QLabel("20,21", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,66, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("T6", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t6RegLineEdit = new QLineEdit("0x0000", parent);
    t6RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t6RegLineEdit->setGeometry(OneByteShapes::t6RegLineEdit);
    t6RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t6RegLineEdit));
    t6RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t6RegLineEdit->setFrame(false);
    scene->addWidget(t6RegLineEdit);

    ph = new QLabel("22,23", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(257,93, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M1", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(289,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0001", parent);
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m1RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("24,25", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,10, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M2", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,10, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0203", parent);
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m2RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("26,27", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,38, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M3", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,38, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0408", parent);
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m3RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("28,29", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,66, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M4", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,66, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0xF0F6", parent);
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m4RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    ph = new QLabel("30,31", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(386,93, 30,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize() - 2));
    scene->addWidget(ph);
    ph = new QLabel("M5", parent);
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(418,93, 20,22);
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0xFEFF", parent);
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m5RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(ph);

    scene->addRect(OneByteShapes::RegBank, QPen(QBrush(QColor(Qt::red),
                                               Qt::SolidPattern),
                                        2, Qt::SolidLine, Qt::SquareCap,
                                        Qt::BevelJoin));
    // NZVC data path, vertical black line
    scene->addPolygon(OneByteShapes::NZVCDataPath,
                      QPen(QBrush(Qt::black), 1), QBrush(Qt::yellow));
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
        qDebug() << "ALU text to int conversion failed - non-number in the ALU";
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
        else if (aMuxTristateLabel->text() == "1" && aLineEdit->text() != ""
                 && bLineEdit->text() != "") {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

void CpuPaneBaseGraphicsItems::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(Qt::black);

    // c,b,a select line text
    painter->drawText(528,92, "5");
    painter->drawText(528,70, "5");
    painter->drawText(528,48, "5");
    // alu select line text
    painter->drawText(528,374, "4");

    // NZVC data path text
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

    painter->drawText(23,145, "System Bus");

    painter->drawText(7, 320, "Addr");
    painter->drawText(7, 395, "Data");

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

    repaintAndZSelect(painter);
    repaintALUSelect(painter);
    repaintMDRMuxSelect(painter);

}

void CpuPaneBaseGraphicsItems::repaintLoadCk(QPainter *painter)
{
    QColor color;

    color = loadCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLines(OneByteShapes::loadCkSelect._lines);

    for (int i = 0; i < OneByteShapes::loadCkSelect._arrowheads.length(); i++) {
        painter->drawImage(OneByteShapes::loadCkSelect._arrowheads.at(i),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
    }
}

void CpuPaneBaseGraphicsItems::repaintCSelect(QPainter *painter)
{
    bool ok;
    QColor color;

    cLineEdit->text().toInt(&ok, 10);
    ok ? color = Qt::black : color = Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    painter->drawLines(OneByteShapes::CSelect._lines);

    painter->drawImage(QPoint(499,47),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintBSelect(QPainter *painter)
{
    bool ok;
    QColor color;

    bLineEdit->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    painter->drawLines(OneByteShapes::BSelect._lines);

    // arrow
    painter->drawImage(QPoint(499,69),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    color = ok ? Qt::red : Qt::white;
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // BBus
    painter->drawPolygon(OneByteShapes::BBus1);
    painter->drawPolygon(OneByteShapes::BBus2);
    painter->drawRect(OneByteShapes::BBusRect);
}

void CpuPaneBaseGraphicsItems::repaintASelect(QPainter *painter)
{

    bool ok;
    QColor color;

    aLineEdit->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    painter->drawLines(OneByteShapes::ASelect._lines);

    painter->drawImage(QPoint(499,91),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    color = ok ? Qt::red : Qt::white;

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);
    // ABus
    painter->drawPolygon(OneByteShapes::ABus1);
    painter->drawPolygon(OneByteShapes::ABus2);

}

void CpuPaneBaseGraphicsItems::repaintMARCk(QPainter *painter)
{
    QColor color;

    color = MARCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MARCk
    painter->drawLines(OneByteShapes::MARCk._lines);

    painter->drawEllipse(QPoint(235,177), 2, 2);

    painter->drawImage(QPoint(232,155),
                       color == Qt::gray ? arrowUpGray : arrowUp);
    painter->drawImage(QPoint(232,191),
                       color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuPaneBaseGraphicsItems::repaintMDRCk(QPainter *painter)
{
    QColor color;

    color = MDRCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MDRCk
    painter->drawLines(OneByteShapes::MDRCk._lines);

    painter->drawImage(QPoint(207,241),
                       color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuPaneBaseGraphicsItems::repaintAMuxSelect(QPainter *painter)
{
    QColor color;
    bool ok;
    int aMux = aMuxTristateLabel->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // AMux Select
    painter->drawLines(OneByteShapes::AMuxSelect._lines);

    painter->drawImage(QPoint(380,300),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

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
    painter->drawPolygon(OneByteShapes::AMuxBus);
}

void CpuPaneBaseGraphicsItems::repaintCMuxSelect(QPainter *painter)
{
    QColor color;

    color = cMuxTristateLabel->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // CMux Select
    painter->drawLines(OneByteShapes::CMuxSelect._lines);

    painter->drawImage(QPoint(257,362),
                       color == Qt::gray ? arrowDownGray : arrowDown);

    if (cMuxTristateLabel->text() == "0") {
        color = Qt::yellow;
        cMuxerLabel->setPalette(QPalette(combCircuitYellow));
    }
    else if (cMuxTristateLabel->text() == "1") {
        if (!aluHasCorrectOutput() || ALULineEdit->text() == "15") {
            // CBus.state == UNDEFINED or NZVC A
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
    painter->drawPolygon(OneByteShapes::CBus);
}

void CpuPaneBaseGraphicsItems::repaintSCk(QPainter *painter)
{
    QColor color;

    color = SCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    painter->drawLine(505,445, 543,445); //437+8

    // arrow
    painter->drawImage(QPoint(497,442), //437+8-3
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintCCk(QPainter *painter)
{
    QColor color;

    color = CCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    painter->drawLine(505,472, 543,472); //464+8

    // arrow
    painter->drawImage(QPoint(497,469),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintVCk(QPainter *painter)
{
    QColor color;

    color = VCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(505,499, 543,499); //491+8

    painter->drawImage(QPoint(497,496),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintZCk(QPainter *painter)
{
    QColor color;

    color = ZCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(505, 552, 543, 552);

    painter->drawImage(QPoint(497,549),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintNCk(QPainter *painter)
{
    QColor color;

    color = NCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(505,594, 543,594); //586+8

    painter->drawImage(QPoint(497,591), //586+8-3
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
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

    // memRead line from label to bus:
    painter->drawLine(81,639, 543,639); //631+8

    painter->drawImage(QPoint(73,636), //631+8-3
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    if (MemWriteTristateLabel->text() == "1") {
        // Do not paint main bus if MemWrite is isHigh
        return;
    }

    // Draw ADDR bus stuff:
    isHigh ? color = Qt::yellow : color = Qt::white;

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // Main Bus
    // ADDR bus:
    painter->drawRect(OneByteShapes::AddrBus);
    // left arrow to addr:
    painter->drawPolygon(OneByteShapes::AddrArrow);

    painter->setBrush(Qt::white);

    // Draw DATA bus stuff:
    if (isHigh && (Sim::mainBusState == Enu::MemReadReady ||
                Sim::mainBusState == Enu::MemReadSecondWait)) {
        color = QColor(16, 150, 24); // green
    }
    else {
        color = Qt::white;
    }
    painter->setBrush(color);

    painter->setBrush(Qt::red);
    // Data bus:
    painter->drawRect(OneByteShapes::DataBus);

    // Mem Data Bus
    poly.clear();
    // arrowhead into the main bus:
    if (color == QColor(16, 150, 24)) {
        // square end (when reading):
        poly << QPoint(3, 365) << QPoint(3, 375);
    }
    else {
        // arrowhead (normally):
        poly << QPoint(13, 365) << QPoint(13, 360) << QPoint(3, 370)
             << QPoint(13, 380) << QPoint(13, 375);
    }
    poly << QPoint(29, 375) << QPoint(29, 380) << QPoint(39, 370)
         << QPoint(29, 360) << QPoint(29, 365);
    painter->drawPolygon(poly);

    // right arrow from Bus to MDRMux:
    painter->drawPolygon(OneByteShapes::DataToMDRMuxBus);

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

    // memWrite line from the label to the bus:
    painter->drawLine(81,619, 543,619); //611+8
    painter->drawImage(QPoint(73,616), //611+8-3
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    // draw line from memWrite to MDR out:
    painter->drawEllipse(QPoint(96,619), 2, 2); //611+8
    painter->drawLine(96,619, 96,345); //611+8
    // memWrite line from the label to the bus:
    painter->drawLine(96,333, 96,280); //268+12
    painter->drawImage(QPoint(93,271), //96-3 //268+12-9
                       color == Qt::gray ? arrowUpGray : arrowUp);

    // repaint the MDR-to-main-bus line, based on if MemWrite is set or not
    // note: it should be lighter (disabled) when MemWrite is not set.
    color = QColor(16, 150, 24); // green
    if (!isHigh) {
        color = color.lighter(150);
    }
    painter->setBrush(color);
    painter->setPen(QPen(QBrush(Qt::black), 1));

    // mdr to data bus
    painter->drawPolygon(OneByteShapes::MDRToDataBus);


    if (MemReadTristateLabel->text() == "1") {
        // Do not paint main bus if MemRead is high
        return;
    }

    // Draw ADDR bus stuff:
    if (isHigh) {
        // qDebug() << "mainBusState: " << Sim::mainBusState;
        // ADDR bus is yellow if the bus is high
        color = Qt::yellow;
    }
    else {
        color = Qt::white;
    }

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // Main Bus
    // Main ADDR bus:
    painter->drawRect(OneByteShapes::AddrBus);
    // left arrow from addr bus to memory:
    painter->drawPolygon(OneByteShapes::AddrArrow);

    // Draw DATA bus stuff:
    // figure out the color:
    if (isHigh && (Sim::mainBusState == Enu::MemWriteReady ||
                Sim::mainBusState == Enu::MemWriteSecondWait)) {
        color = QColor(16, 150, 24); // green
    }
    else {
        color = Qt::white;
    }
    painter->setBrush(color);

    // Main Data bus:
    painter->drawRect(OneByteShapes::DataBus);

    // Mem Data Bus (bidirectional arrow)
    poly.clear();
    // arrowhead:
    poly << QPoint(13, 365) << QPoint(13, 360) << QPoint(3, 370)
         << QPoint(13, 380) << QPoint(13, 375);
    // other end of the bus:
    if (color == QColor(16, 150, 24)) {
        // flat end
        poly << QPoint(40, 375) << QPoint(40, 365);
    }
    else {
        // arrowhead
        poly << QPoint(29, 375) << QPoint(29, 380) << QPoint(39, 370)
             << QPoint(29, 360) << QPoint(29, 365);
    }
    painter->drawPolygon(poly);

    // Main Bus to MDRMux is ALWAYS white on a memWrite:
    painter->setBrush(Qt::white);

    // right arrow from Bus to MDRMux:
    painter->drawPolygon(OneByteShapes::DataToMDRMuxBus);
}

void CpuPaneBaseGraphicsItems::repaintSBitOut(QPainter *painter)
{
    sBitLabel->text() = Sim::sBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from S bit to CSMux
    painter->drawLine(487,437, //476+11
                      487,437-8); //476+11
    // arrow:
    painter->drawImage(QPoint(484,421), arrowUp); //476+11-3 //437-8-8
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
    // arrow to the NZVC data bus
    painter->drawImage(QPoint(314,483), arrowLeft);

    // line from C bit to CSMux
    // bitty bit above C bit:
    painter->drawLine(487,459, 487,463); //463-4
    painter->drawLine(434,459, //426+8 //463-4
                      487,459); //463-4
    painter->drawLine(434, //426+8
                      426, //399+19+8
                      434,459); //426+8 //463-4
    // arrow to the CSMux
    painter->drawImage(
                QPoint(431,  //426+8-3
                       421), //399+19+3
                       arrowUp);

    // CIN line back to the ALU
    painter->drawLine(461,389, 433,389); //426+35
    painter->drawLine(461,399,  //426+35
                      461,389); //426+35
    // CIN arrow to the ALU
    painter->drawImage(QPoint(428,386), arrowLeft);

}

void CpuPaneBaseGraphicsItems::repaintVBitOut(QPainter *painter)
{
    vBitLabel->text() = Sim::vBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    /* V out */
    painter->drawLine(487,510,  //491+19
                      487,514); // bitty bit //491+19+4
    painter->drawLine(487,514,  //491+19+4
                      352,514); //491+19+4
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

    // line up to AndZ
    painter->drawEllipse(QPoint(437,582), 2, 2);
    painter->drawLine(437,582, 437,574);

    painter->drawImage(QPoint(314,503), arrowLeft);
    painter->drawImage(QPoint(434,566), arrowUp);  // AndZ arrow upwards
}

void CpuPaneBaseGraphicsItems::repaintNBitOut(QPainter *painter)
{
    nBitLabel->text() = Sim::nBit ? "1" : "0";

    QPolygon poly;
    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(487,605, 487,609); //605+4
    painter->drawLine(487,609, 330,609); //605+4
    painter->drawLine(330,609, 330,517); //605+4
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
    painter->drawLine(505,409, 543,409); //399+10

    // arrow
    painter->drawImage(QPoint(497,406), //399+10-3
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuPaneBaseGraphicsItems::repaintAndZSelect(QPainter *painter)
{
    QPolygon poly;

    QColor color = Qt::gray;

    if (AndZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // lines coming out of tristate label
    painter->drawLine(437,532, 437,524); // vertical line //517+7+8 //517+7
    painter->drawLine(437,524, 543,524); // horiz line //517+7

    painter->drawImage(QPoint(434,532), //517+7+8
                       color == Qt::gray ? arrowDownGray : arrowDown);

    color = Qt::gray;
    if (ALULineEdit->text() != "" && AndZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // AndZ out
    painter->drawLine(458,552, 465,552);

    painter->drawImage(QPoint(465,549),
                       color == Qt::gray ? arrowRightGray : arrowRight);
}

void CpuPaneBaseGraphicsItems::repaintALUSelect(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = ALULineEdit->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // ALU Select
    painter->drawLine(439,376, 543,376);
    painter->drawLine(523,371, 533,381); // diagonal line

    painter->drawImage(QPoint(433,373),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

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
    painter->drawPolygon(OneByteShapes::ALUOutBus);

    // Draw status bit lines
    painter->setPen(aluHasCorrectOutput() ? Qt::black : Qt::gray);
    painter->setBrush(aluHasCorrectOutput() ? Qt::black : Qt::gray);

    // N
    painter->drawLine(371,395, 371,594); //586+8
    painter->drawLine(371,594, 465,594); //586+8

    painter->drawImage(QPoint(465,591), //586+8-3
                       color == Qt::gray ? arrowRightGray : arrowRight);

    // Z
    painter->drawLine(386,395, 386,552);
    painter->drawLine(386,552, 404,552);

    painter->drawImage(QPoint(404,549),
                       color == Qt::gray ? arrowRightGray : arrowRight);

    // V
    painter->drawLine(401,395, 401,499);
    painter->drawLine(401,499, 466,499);

    painter->drawImage(QPoint(466,496),
                       color == Qt::gray ? arrowRightGray : arrowRight);

    // Cout:
    // C
    painter->drawLine(416,395, 416,472);
    painter->drawLine(416,472, 465,472);
    painter->drawImage(QPoint(465,469),
                       color == Qt::gray ? arrowRightGray : arrowRight);
    // S
    painter->drawLine(416,446, 465,446); //476-11 //437+9
    painter->drawEllipse(QPoint(416,446), 2, 2); //437+9
    painter->drawImage(QPoint(465,443), //476-11 //437+9-3
                       color == Qt::gray ? arrowRightGray : arrowRight);
}

void CpuPaneBaseGraphicsItems::repaintMDRMuxSelect(QPainter *painter)
{
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

    painter->drawImage(QPoint(249,300),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    painter->setPen(Qt::black);

    if (MDRMuxTristateLabel->text() == "0") {
        if ((Sim::mainBusState == Enu::MemReadSecondWait &&
             MemReadTristateLabel->text() == "1") ||
                Sim::mainBusState == Enu::MemReadReady) {
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

    // MDRMuxOutBus (MDRMux to MDR arrow)
    painter->drawPolygon(OneByteShapes::MDRMuxOutBus);


}

