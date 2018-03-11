// File: cpugraphicsitems.cpp
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

#include "cpugraphicsitems.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QGraphicsScene>
#include <QPainter>

#include <QGraphicsItem>

#include <QDebug>
#include "sim.h"
#include "pep.h"

#include "shapes_two_byte_data_bus.h"

CpuGraphicsItems::CpuGraphicsItems(Enu::CPUType type, QWidget *widgetParent,
                                                   QGraphicsItem *itemParent,
                                                   QGraphicsScene *scene)
    : QGraphicsItem(itemParent),
      parent(widgetParent)
{    
    // http://colrd.com/image-dna/23448/
    seqCircuitColor = QColor(0x3B3630).lighter(370);
    combCircuitRed = QColor(0xD92405).lighter(140);
    combCircuitBlue = QColor(0x3563EB).lighter(120);
    combCircuitYellow = QColor(0xEAC124).lighter(120);
    combCircuitGreen = QColor(0x739211).lighter(130);
    muxCircuitRed = combCircuitRed.lighter(140);
    muxCircuitBlue = combCircuitBlue.lighter(140);
    muxCircuitYellow = combCircuitYellow.lighter(140);
    muxCircuitGreen = combCircuitGreen.lighter(140);

    // set up arrow heads:
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

    // save our current model for this set of items;
    model = type;

    // ************************************
    // two byte exclusive stuff
    // ************************************
    MARMuxerDataLabel = new QLabel("MARMux");
    MARMuxerDataLabel->setGeometry(TwoByteShapes::MARMuxerDataLabel);
    MARMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARMuxerDataLabel->setPalette(QPalette(Qt::white));
    MARMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MARMuxerDataLabel);

    MDROCk = new QCheckBox("MDROCk");
    MDROCk->setGeometry(TwoByteShapes::MDROCkCheckbox);
    MDROCk->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    MDROCk->setPalette(QPalette(Qt::white));
    scene->addWidget(MDROCk);

    MDRECk = new QCheckBox("MDRECk");
    MDRECk->setGeometry(TwoByteShapes::MDRECkCheckbox);
    MDRECk->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    MDRECk->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRECk);

    MDROMuxLabel = new QLabel("MDROMux");
    MDROMuxLabel->setGeometry(TwoByteShapes::MDROMuxLabel);
    MDROMuxLabel->setPalette(QPalette(Qt::white));
    MDROMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDROMuxLabel);
    MDROMuxerDataLabel = new QLabel("MDROMux");
    MDROMuxerDataLabel->setGeometry(TwoByteShapes::MDROMuxerDataLabel);
    MDROMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDROMuxerDataLabel->setPalette(QPalette(Qt::white));
    MDROMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDROMuxerDataLabel);
    MDROMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    MDROMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDROMuxTristateLabel->setGeometry(TwoByteShapes::MDROMuxTristateLabel);
    MDROMuxTristateLabel->setPalette(QPalette(Qt::white));
    MDROMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDROMuxTristateLabel);

    MDREMuxLabel = new QLabel("MDREMux");
    MDREMuxLabel->setGeometry(TwoByteShapes::MDREMuxLabel);
    MDREMuxLabel->setPalette(QPalette(Qt::white));
    MDREMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDREMuxLabel);
    MDREMuxerDataLabel = new QLabel("MDREMux");
    MDREMuxerDataLabel->setGeometry(TwoByteShapes::MDREMuxerDataLabel);
    MDREMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDREMuxerDataLabel->setPalette(QPalette(Qt::white));
    MDREMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDREMuxerDataLabel);
    MDREMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    MDREMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDREMuxTristateLabel->setGeometry(TwoByteShapes::MDREMuxTristateLabel);
    MDREMuxTristateLabel->setPalette(QPalette(Qt::white));
    MDREMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDREMuxTristateLabel);

    EOMuxLabel = new QLabel("EOMux");
    EOMuxLabel->setGeometry(TwoByteShapes::EOMuxLabel);
    EOMuxLabel->setPalette(QPalette(Qt::white));
    EOMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(EOMuxLabel);

    EOMuxerDataLabel = new QLabel("EOMux");
    EOMuxerDataLabel->setGeometry(TwoByteShapes::EOMuxerDataLabel);
    EOMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    EOMuxerDataLabel->setPalette(QPalette(Qt::white));
    EOMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(EOMuxerDataLabel);

    EOMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    EOMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    EOMuxTristateLabel->setGeometry(TwoByteShapes::EOMuxTristateLabel);
    EOMuxTristateLabel->setPalette(QPalette(Qt::white));
    EOMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(EOMuxTristateLabel);

    MDRELabel = new QLabel("0x00");
    MDRELabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRELabel->setGeometry(TwoByteShapes::MDRELabel);
    MDRELabel->setPalette(QPalette(seqCircuitColor));
    MDRELabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDRELabel);

    MDROLabel = new QLabel("0x00");
    MDROLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDROLabel->setGeometry(TwoByteShapes::MDROLabel);
    MDROLabel->setPalette(QPalette(seqCircuitColor));
    MDROLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDROLabel);

    MARMuxLabel = new QLabel("MARMux");
    MARMuxLabel->setGeometry(TwoByteShapes::MARMuxLabel);
    MARMuxLabel->setPalette(QPalette(Qt::white));
    MARMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MARMuxLabel);
    MARMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    MARMuxTristateLabel->setGeometry(TwoByteShapes::MARMuxTristateLabel);
    MARMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARMuxTristateLabel->setPalette(QPalette(Qt::white));
    MARMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MARMuxTristateLabel);


    // ************************************
    // end two byte exclusive stuff
    // ************************************


    // MARA & MARB
    MARALabel = new QLabel("0x00");
    MARALabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARALabel->setAutoFillBackground(false);
    MARALabel->setPalette(QPalette(seqCircuitColor));
    MARALabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MARALabel);
    MARBLabel = new QLabel("0x00");
    MARBLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MARBLabel->setPalette(QPalette(seqCircuitColor));
    MARBLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MARBLabel);

    // LoadCk
    loadCk = new QCheckBox("LoadCk");
    loadCk->setGeometry(OneByteShapes::loadCkCheckbox);
    loadCk->setPalette(QPalette(Qt::white));
    loadCk->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(loadCk);

    // C
    // Note: the line edits must be added first, otherwise they cover the
    //  labels that go with them.
    QRegExp cbaRegExp("^((3[0-1])|([0-2][0-9])|([0-9]))$");
    cLineEdit = new QLineEdit();
    cLineEdit->setAlignment(Qt::AlignCenter);
    cLineEdit->setGeometry(OneByteShapes::cLineEdit);
    cLineEdit->setValidator(new QRegExpValidator(cbaRegExp, cLineEdit));
    cLineEdit->setPalette(QPalette(Qt::white));
    cLineEdit->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(cLineEdit);
    cLabel = new QLabel("C");
    cLabel->setGeometry(OneByteShapes::cLabel);
    cLabel->setPalette(QPalette(Qt::white));
    cLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(cLabel);

    // B
    bLineEdit = new QLineEdit();
    bLineEdit->setAlignment(Qt::AlignCenter);
    bLineEdit->setGeometry(OneByteShapes::bLineEdit);
    bLineEdit->setValidator(new QRegExpValidator(cbaRegExp, bLineEdit));
    bLineEdit->setPalette(QPalette(Qt::white));
    bLineEdit->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(bLineEdit);
    bLabel = new QLabel("B");
    bLabel->setGeometry(OneByteShapes::bLabel);
    bLabel->setPalette(QPalette(Qt::white));
    bLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(bLabel);

    // A
    aLineEdit = new QLineEdit();
    aLineEdit->setAlignment(Qt::AlignCenter);
    aLineEdit->setGeometry(OneByteShapes::aLineEdit);
    aLineEdit->setValidator(new QRegExpValidator(cbaRegExp, aLineEdit));
    aLineEdit->setPalette(QPalette(Qt::white));
    aLineEdit->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(aLineEdit);
    aLabel = new QLabel("A");
    aLabel->setGeometry(OneByteShapes::aLabel);
    aLabel->setPalette(QPalette(Qt::white));
    aLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(aLabel);

    // MARCk
    MARCk = new QCheckBox("MARCk");
    MARCk->setGeometry(OneByteShapes::MARCkCheckbox);
    MARCk->setPalette(QPalette(Qt::white));
    MARCk->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MARCk);

    // MDRCk
    MDRCk = new QCheckBox("MDRCk");
    MDRCk->setGeometry(OneByteShapes::MDRCkCheckbox);
    MDRCk->setPalette(QPalette(Qt::white));
    MDRCk->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDRCk);

    // AMux
    aMuxLabel = new QLabel("AMux");
    aMuxLabel->setGeometry(OneByteShapes::aMuxLabel);
    aMuxLabel->setPalette(QPalette(Qt::white));
    aMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(aMuxLabel);
    aMuxerDataLabel = new QLabel("AMux");
    aMuxerDataLabel->setGeometry(OneByteShapes::aMuxerDataLabel);
    aMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    aMuxerDataLabel->setPalette(QPalette(Qt::white));
    aMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(aMuxerDataLabel);
    aMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    aMuxTristateLabel->setGeometry(OneByteShapes::aMuxTristateLabel);
    aMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    aMuxTristateLabel->setPalette(QPalette(Qt::white));
    aMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(aMuxTristateLabel);

    // MDRMux
    MDRMuxLabel = new QLabel("MDRMux");
    MDRMuxLabel->setGeometry(OneByteShapes::MDRMuxLabel);
    MDRMuxLabel->setPalette(QPalette(Qt::white));
    MDRMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDRMuxLabel);
    MDRMuxerDataLabel = new QLabel("MDRMux");
    MDRMuxerDataLabel->setGeometry(OneByteShapes::MDRMuxerDataLabel);
    MDRMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
    MDRMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDRMuxerDataLabel);
    MDRMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    MDRMuxTristateLabel->setGeometry(OneByteShapes::MDRMuxTristateLabel);
    MDRMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRMuxTristateLabel->setPalette(QPalette(Qt::white));
    MDRMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDRMuxTristateLabel);
    MDRLabel = new QLabel("0x00");
    MDRLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MDRLabel->setGeometry(OneByteShapes::MDRLabel);
    MDRLabel->setPalette(QPalette(seqCircuitColor));
    MDRLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MDRLabel);


    // CMux
    cMuxLabel = new QLabel("CMux");
    cMuxLabel->setGeometry(OneByteShapes::cMuxLabel);
    cMuxLabel->setPalette(QPalette(Qt::white));
    cMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(cMuxLabel);
    cMuxerLabel = new QLabel("CMux");
    cMuxerLabel->setGeometry(OneByteShapes::cMuxerLabel);
    cMuxerLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cMuxerLabel->setPalette(QPalette(Qt::white));
    cMuxerLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(cMuxerLabel);
    cMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    cMuxTristateLabel->setGeometry(OneByteShapes::cMuxTristateLabel);
    cMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cMuxTristateLabel->setPalette(QPalette(Qt::white));
    cMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(cMuxTristateLabel);

    // ALU
    // keep this before the label that goes with it, or the line edit
    //  appears on top of the label
    ALULineEdit = new QLineEdit();
    ALULineEdit->setAlignment(Qt::AlignCenter);
    ALULineEdit->setGeometry(OneByteShapes::ALULineEdit);
    ALULineEdit->setValidator(new QRegExpValidator(
                                  QRegExp("^((1[0-5])|(0[0-9])|[0-9])$"),
                                  ALULineEdit));
    ALULineEdit->setPalette(QPalette(Qt::white));
    ALULineEdit->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(ALULineEdit);
    ALULabel = new QLabel("ALU");
    ALULabel->setGeometry(OneByteShapes::ALULabel);
    ALULabel->setPalette(QPalette(Qt::white));
    ALULabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(ALULabel);

    ALUFunctionLabel = new QLabel("");
    ALUFunctionLabel->setGeometry(OneByteShapes::ALUFunctionLabel);
    ALUFunctionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ALUFunctionLabel->setPalette(QPalette(combCircuitBlue.lighter(150)));
    ALUFunctionLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSizeSmall));
    scene->addWidget(ALUFunctionLabel);

    // ALU shape
    ALUPoly = scene->addPolygon(OneByteShapes::ALUPoly,
                                QPen(QBrush(combCircuitBlue),
                                     2, Qt::SolidLine,
                                     Qt::SquareCap,
                                     Qt::MiterJoin),
                                QBrush(combCircuitBlue.lighter(150)));
    ALUPoly->setZValue(-1);

    // CSMux
    CSMuxLabel = new QLabel("CSMux");
    CSMuxLabel->setGeometry(OneByteShapes::CSMuxLabel);
    CSMuxLabel->setPalette(QPalette(Qt::white));
    CSMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(CSMuxLabel);
    CSMuxerDataLabel = new QLabel("CSMux");
    CSMuxerDataLabel->setGeometry(OneByteShapes::CSMuxerDataLabel);
    CSMuxerDataLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    CSMuxerDataLabel->setPalette(QPalette(Qt::white));
    CSMuxerDataLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(CSMuxerDataLabel);
    CSMuxTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    CSMuxTristateLabel->setGeometry(OneByteShapes::CSMuxTristateLabel);
    CSMuxTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    CSMuxTristateLabel->setPalette(QPalette(Qt::white));
    CSMuxTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(CSMuxTristateLabel);

    // SCk
    SCkCheckBox = new QCheckBox ("SCk");
    SCkCheckBox->setGeometry(OneByteShapes::SCkCheckBox);
    SCkCheckBox->setPalette(QPalette(Qt::white));
    SCkCheckBox->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(SCkCheckBox);
    sBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    sBitLabel->setText("0");
    sBitLabel->setGeometry(OneByteShapes::sBitLabel);
    sBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    sBitLabel->setPalette(QPalette(seqCircuitColor));
    sBitLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(sBitLabel);

    // CCk
    CCkCheckBox = new QCheckBox ("CCk");
    CCkCheckBox->setGeometry(OneByteShapes::CCkCheckBox);
    CCkCheckBox->setPalette(QPalette(Qt::white));
    CCkCheckBox->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(CCkCheckBox);
    cBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    cBitLabel->setText("0");
    cBitLabel->setGeometry(OneByteShapes::cBitLabel);
    cBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cBitLabel->setPalette(QPalette(seqCircuitColor));
    cBitLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(cBitLabel);

    // VCk
    VCkCheckBox = new QCheckBox("VCk");
    VCkCheckBox->setGeometry(OneByteShapes::VCkCheckBox);
    VCkCheckBox->setPalette(QPalette(Qt::white));
    VCkCheckBox->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(VCkCheckBox);
    vBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    vBitLabel->setText("0");
    vBitLabel->setGeometry(OneByteShapes::vBitLabel);
    vBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    vBitLabel->setPalette(QPalette(seqCircuitColor));
    vBitLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(vBitLabel);

    // AndZ
    AndZLabel = new QLabel("AndZ");
    AndZLabel->setGeometry(OneByteShapes::AndZLabel);
    AndZLabel->setPalette(QPalette(Qt::white));
    AndZLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(AndZLabel);
    AndZTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    AndZTristateLabel->setGeometry(OneByteShapes::AndZTristateLabel);
    AndZTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    AndZTristateLabel->setPalette(QPalette(Qt::white));
    AndZTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(AndZTristateLabel);

    AndZMuxLabel = new QLabel("AndZ");
    AndZMuxLabel->setGeometry(OneByteShapes::AndZMuxLabel);
    AndZMuxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    AndZMuxLabel->setPalette(QPalette(Qt::white));
    AndZMuxLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(AndZMuxLabel);

    // ZCk
    ZCkCheckBox = new QCheckBox("ZCk");
    ZCkCheckBox->setGeometry(OneByteShapes::ZCkCheckBox);
    ZCkCheckBox->setPalette(QPalette(Qt::white));
    ZCkCheckBox->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(ZCkCheckBox);
    zBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    zBitLabel->setText("0");
    zBitLabel->setGeometry(OneByteShapes::zBitLabel);
    zBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    zBitLabel->setPalette(QPalette(seqCircuitColor));
    zBitLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(zBitLabel);

    // NCk
    NCkCheckBox = new QCheckBox ("NCk");
    NCkCheckBox->setGeometry(OneByteShapes::NCkCheckBox);
    NCkCheckBox->setPalette(QPalette(Qt::white));
    NCkCheckBox->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(NCkCheckBox);
    nBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    nBitLabel->setText("0");
    nBitLabel->setGeometry(OneByteShapes::nBitLabel);
    nBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    nBitLabel->setPalette(QPalette(seqCircuitColor));
    nBitLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(nBitLabel);

    // MemRead/Write
    MemWriteLabel = new QLabel("MemWrite");
    MemWriteLabel->setPalette(QPalette(Qt::white));
    MemWriteLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MemWriteLabel);
    MemWriteTristateLabel = new TristateLabel(0, TristateLabel::OneUndefined);
    MemWriteTristateLabel->setGeometry(OneByteShapes::MemWriteTristateLabel);
    MemWriteTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemWriteTristateLabel->setPalette(QPalette(Qt::white));
    MemWriteTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MemWriteTristateLabel);

    MemReadLabel = new QLabel("MemRead");
    MemReadLabel->setPalette(QPalette(Qt::white));
    MemReadLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MemReadLabel);
    MemReadTristateLabel = new TristateLabel(0, TristateLabel::OneUndefined);
    MemReadTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemReadTristateLabel->setPalette(QPalette(Qt::white));
    MemReadTristateLabel->setFont (QFont(Pep::labelFont, Pep::labelFontSize));
    scene->addWidget(MemReadTristateLabel);


    // Registers
    scene->addRect(OneByteShapes::RegBank, QPen(QBrush(QColor(Qt::red), Qt::SolidPattern),
                                        2, Qt::DotLine,
                                        Qt::SquareCap,
                                        Qt::MiterJoin), QBrush(seqCircuitColor));

    QLabel *ph;
    ph = new QLabel("0,1");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(1, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("A");
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(1, 1));
    scene->addWidget(ph);
    aRegLineEdit = new QLineEdit("0x0000");
    aRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    aRegLineEdit->setGeometry(OneByteShapes::aRegLineEdit);
    aRegLineEdit->setValidator(new QRegExpValidator(
                                   QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                   aRegLineEdit));
    aRegLineEdit->setPalette(QPalette(seqCircuitColor));
    aRegLineEdit->setFrame(false);
    aRegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(aRegLineEdit);
    //    QObject::connect(A, SIGNAL(valueChanged()),
                           //this, SLOT(slotRegisterChanged()));

    ph = new QLabel("2,3");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(1, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("X");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(1, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    xRegLineEdit = new QLineEdit("0x0000");
    xRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    xRegLineEdit->setGeometry(OneByteShapes::xRegLineEdit);
    xRegLineEdit->setValidator(new QRegExpValidator(
                                   QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                   xRegLineEdit));
    xRegLineEdit->setPalette(QPalette(seqCircuitColor));
    xRegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    xRegLineEdit->setFrame(false);
    scene->addWidget(xRegLineEdit);

    ph = new QLabel("4,5");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(1, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("SP");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(1, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    spRegLineEdit = new QLineEdit("0x0000");
    spRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    spRegLineEdit->setGeometry(OneByteShapes::spRegLineEdit);
    spRegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}[0-9a-fA-F]{0,4}"),
                                    spRegLineEdit));
    spRegLineEdit->setPalette(QPalette(seqCircuitColor));
    spRegLineEdit->setFrame(false);
    spRegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(spRegLineEdit);

    ph = new QLabel("6,7");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(1, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("PC");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(1, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    pcRegLineEdit = new QLineEdit("0x0000");
    pcRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pcRegLineEdit->setGeometry(OneByteShapes::pcRegLineEdit);
    pcRegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    pcRegLineEdit));
    pcRegLineEdit->setPalette(QPalette(seqCircuitColor));
    pcRegLineEdit->setFrame(false);
    pcRegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(pcRegLineEdit);
    //    QObject::connect(X, SIGNAL(valueChanged()),
                           //this, SLOT(slotRegisterChanged()));

    ph = new QLabel("8-10");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(2, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("IR");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(2, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    irRegLineEdit = new QLineEdit("0x000000");
    irRegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    irRegLineEdit->setGeometry(OneByteShapes::irRegLineEdit);
    irRegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,6}"),
                                    irRegLineEdit));
    irRegLineEdit->setPalette(QPalette(seqCircuitColor));
    irRegLineEdit->setFrame(false);
    irRegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(irRegLineEdit);

    ph = new QLabel("11");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(2, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("T1");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(2, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t1RegLineEdit = new QLineEdit("0x00");
    t1RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t1RegLineEdit->setGeometry(OneByteShapes::t1RegLineEdit);
    t1RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,2}"),
                                    t1RegLineEdit));
    t1RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t1RegLineEdit->setFrame(false);
    t1RegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(t1RegLineEdit);

    ph = new QLabel("12,13");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(2, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("T2");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(2, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t2RegLineEdit = new QLineEdit("0x0000");
    t2RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t2RegLineEdit->setGeometry(OneByteShapes::t2RegLineEdit);
    t2RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t2RegLineEdit));
    t2RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t2RegLineEdit->setFrame(false);
    t2RegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(t2RegLineEdit);

    ph = new QLabel("14,15");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(2, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("T3");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(2, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t3RegLineEdit = new QLineEdit("0x0000");
    t3RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t3RegLineEdit->setGeometry(OneByteShapes::t3RegLineEdit);
    t3RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t3RegLineEdit));
    t3RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t3RegLineEdit->setFrame(false);
    t3RegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(t3RegLineEdit);

    ph = new QLabel("16,17");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(3, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("T4");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(3, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t4RegLineEdit = new QLineEdit("0x0000");
    t4RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t4RegLineEdit->setGeometry(OneByteShapes::t4RegLineEdit);
    t4RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t4RegLineEdit));
    t4RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t4RegLineEdit->setFrame(false);
    t4RegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(t4RegLineEdit);

    ph = new QLabel("18,19");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(3, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("T5");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(3, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t5RegLineEdit = new QLineEdit("0x0000");
    t5RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t5RegLineEdit->setGeometry(OneByteShapes::t5RegLineEdit);
    t5RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t5RegLineEdit));
    t5RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t5RegLineEdit->setFrame(false);
    t5RegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(t5RegLineEdit);

    ph = new QLabel("20,21");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(3, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("T6");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(3, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    t6RegLineEdit = new QLineEdit("0x0000");
    t6RegLineEdit->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    t6RegLineEdit->setGeometry(OneByteShapes::t6RegLineEdit);
    t6RegLineEdit->setValidator(new QRegExpValidator(
                                    QRegExp("(0x){0,1}([0-9a-fA-F]){0,4}"),
                                    t6RegLineEdit));
    t6RegLineEdit->setPalette(QPalette(seqCircuitColor));
    t6RegLineEdit->setFrame(false);
    t6RegLineEdit->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(t6RegLineEdit);

    ph = new QLabel("22,23");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(3, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("M1");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(3, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0001");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m1RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(ph);

    ph = new QLabel("24,25");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(4, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("M2");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(4, 1));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0203");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m2RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(ph);

    ph = new QLabel("26,27");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(4, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("M3");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(4, 2));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0x0408");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m3RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(ph);

    ph = new QLabel("28,29");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(4, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("M4");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(4, 3));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0xF0F6");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m4RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(ph);

    ph = new QLabel("30,31");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegNoRect(4, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeSmall));
    scene->addWidget(ph);
    ph = new QLabel("M5");
    ph->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setGeometry(OneByteShapes::getRegLabelRect(4, 4));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSizeLarge));
    ph->setFont(QFont(ph->font().family(), ph->font().pointSize(), QFont::Bold));
    scene->addWidget(ph);
    ph = new QLabel("0xFEFF");
    ph->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    ph->setGeometry(OneByteShapes::m5RegLabel);
    ph->setPalette(QPalette(seqCircuitColor));
    ph->setFont (QFont(Pep::codeFont, Pep::codeFontSize));
    scene->addWidget(ph);

    //outline around register bank
    scene->addRect(OneByteShapes::RegBank, QPen(QBrush(QColor(Qt::black),
                                                       Qt::SolidPattern),
                                                2, Qt::SolidLine, Qt::SquareCap,
                                                Qt::MiterJoin));

    // do stuff based on the current model:
    if (model == Enu::OneByteDataBus) {
        // hide 2 byte bus stuff:
        MDROCk->hide();
        MDRECk->hide();
        MDROMuxTristateLabel->hide();
        MDREMuxTristateLabel->hide();
        EOMuxTristateLabel->hide();
        MARMuxerDataLabel->hide();
        MARMuxTristateLabel->hide();
        MDROMuxerDataLabel->hide();
        MDREMuxerDataLabel->hide();
        MDROMuxLabel->hide();
        MDREMuxLabel->hide();
        EOMuxLabel->hide();
        EOMuxerDataLabel->hide();
        MDRELabel->hide();
        MDROLabel->hide();
        MARMuxLabel->hide();

        // MARBus (MARA/MARB output bus)
        scene->addPolygon(OneByteShapes::MARBus,
                          QPen(QBrush(Qt::black), 1), QBrush(combCircuitYellow));
        // MAR
        MARALabel->setGeometry(OneByteShapes::MARALabel);
        MARBLabel->setGeometry(OneByteShapes::MARBLabel);
        scene->addRect(OneByteShapes::MARBLabel);
        scene->addRect(OneByteShapes::MARALabel);

        // MDR
        scene->addRect(OneByteShapes::MDRLabel);
        scene->addRect(OneByteShapes::MDRMuxTristateLabel, QPen(Qt::gray));
        //scene->addRect(OneByteShapes::MDRLabel);
        // MDR data section
        scene->addRect(OneByteShapes::MDRMuxerDataLabel);

        // MDRBus (output from MDR, right arrow):
        scene->addRect(OneByteShapes::MDRBusOutRect,
                       QPen(Qt::black), QBrush(combCircuitYellow));
        scene->addPolygon(OneByteShapes::MDRBusOutArrow,
                          QPen(Qt::black), QBrush(combCircuitYellow));
        // note: left arrow gets drawn in repaintMemWrite

        // MemRead/Write
        MemWriteLabel->setGeometry(OneByteShapes::MemWriteLabel);
        MemReadLabel->setGeometry(OneByteShapes::MemReadLabel);
        MemWriteTristateLabel->setGeometry(OneByteShapes::MemWriteTristateLabel);
        MemReadTristateLabel->setGeometry(OneByteShapes::MemReadTristateLabel);
        scene->addRect(QRectF(MemReadTristateLabel->pos(),
                              MemReadTristateLabel->size()), QPen(Qt::gray));
        scene->addRect(QRectF(MemWriteTristateLabel->pos(),
                              MemWriteTristateLabel->size()), QPen(Qt::gray));

        scene->addRect(OneByteShapes::aMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(OneByteShapes::cMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(OneByteShapes::cMuxerLabel);
        aMuxerBorder = scene->addRect(OneByteShapes::aMuxerDataLabel);
        scene->addRect(QRectF(CSMuxTristateLabel->pos(), CSMuxTristateLabel->size()),QPen(Qt::gray));
        scene->addRect(QRectF(CSMuxerDataLabel->pos(), CSMuxerDataLabel->size()));
        scene->addRect(QRectF(AndZTristateLabel->pos(), AndZTristateLabel->size()),
                       QPen(Qt::gray));
        scene->addRect(OneByteShapes::AndZMuxLabel);

        // NZVC data path to CMux, vertical black line
        scene->addPolygon(OneByteShapes::NZVCDataPath,
                          QPen(QBrush(Qt::black), 1), QBrush(combCircuitYellow));
        scene->addLine(OneByteShapes::NZVCDataLine);

    }
    else if (model == Enu::TwoByteDataBus) {
        // hide 1 byte bus stuff:
        MDRCk->hide();
        MDRLabel->hide();
        MDRMuxerDataLabel->hide();
        MDRMuxLabel->hide();
        MDRMuxTristateLabel->hide();

        // MARBus (MARA/MARB output bus)
        scene->addPolygon(TwoByteShapes::MARBus,
                          QPen(QBrush(Qt::black), 1), QBrush(combCircuitYellow));

        // ALU drawing:
        ALUPoly->moveBy(TwoByteShapes::controlOffsetX, TwoByteShapes::aluOffsetY);

        // ***************************************
        // fix geometry for the two byte bus
        // ***************************************
        // MAR
        MARALabel->setGeometry(TwoByteShapes::MARALabel);
        MARBLabel->setGeometry(TwoByteShapes::MARBLabel);
        scene->addRect(TwoByteShapes::MARBLabel);
        scene->addRect(TwoByteShapes::MARALabel);

        // register signals
        loadCk->setGeometry(TwoByteShapes::loadCkCheckbox);
        cLineEdit->setGeometry(TwoByteShapes::cLineEdit);
        cLabel->setGeometry(TwoByteShapes::cLabel);
        bLineEdit->setGeometry(TwoByteShapes::bLineEdit);
        bLabel->setGeometry(TwoByteShapes::bLabel);
        aLineEdit->setGeometry(TwoByteShapes::aLineEdit);
        aLabel->setGeometry(TwoByteShapes::aLabel);

        // misc control signals
        MARCk->setGeometry(TwoByteShapes::MARCkCheckbox);
        aMuxLabel->setGeometry(TwoByteShapes::aMuxLabel);
        aMuxerDataLabel->setGeometry(TwoByteShapes::aMuxerDataLabel);
        aMuxTristateLabel->setGeometry(TwoByteShapes::aMuxTristateLabel);
        cMuxLabel->setGeometry(TwoByteShapes::cMuxLabel);
        cMuxerLabel->setGeometry(TwoByteShapes::cMuxerLabel);
        cMuxTristateLabel->setGeometry(TwoByteShapes::cMuxTristateLabel);
        ALULineEdit->setGeometry(TwoByteShapes::ALULineEdit);
        ALULabel->setGeometry(TwoByteShapes::ALULabel);
        ALUFunctionLabel->setGeometry(TwoByteShapes::ALUFunctionLabel);

        // status bit control signals
        CSMuxLabel->setGeometry(TwoByteShapes::CSMuxLabel);
        CSMuxerDataLabel->setGeometry(TwoByteShapes::CSMuxerDataLabel);
        CSMuxTristateLabel->setGeometry(TwoByteShapes::CSMuxTristateLabel);
        SCkCheckBox->setGeometry(TwoByteShapes::SCkCheckBox);
        sBitLabel->setGeometry(TwoByteShapes::sBitLabel);
        CCkCheckBox->setGeometry(TwoByteShapes::CCkCheckBox);
        cBitLabel->setGeometry(TwoByteShapes::cBitLabel);
        VCkCheckBox->setGeometry(TwoByteShapes::VCkCheckBox);
        vBitLabel->setGeometry(TwoByteShapes::vBitLabel);
        AndZLabel->setGeometry(TwoByteShapes::AndZLabel);
        AndZTristateLabel->setGeometry(TwoByteShapes::AndZTristateLabel);
        AndZMuxLabel->setGeometry(TwoByteShapes::AndZMuxLabel);
        ZCkCheckBox->setGeometry(TwoByteShapes::ZCkCheckBox);
        zBitLabel->setGeometry(TwoByteShapes::zBitLabel);
        NCkCheckBox->setGeometry(TwoByteShapes::NCkCheckBox);
        nBitLabel->setGeometry(TwoByteShapes::nBitLabel);



        // Status Bits
        CSMuxerDataLabel->setGeometry(TwoByteShapes::CSMuxerDataLabel);
        sBitLabel->setGeometry(TwoByteShapes::sBitLabel);
        cBitLabel->setGeometry(TwoByteShapes::cBitLabel);
        vBitLabel->setGeometry(TwoByteShapes::vBitLabel);
        AndZMuxLabel->setGeometry(TwoByteShapes::AndZMuxLabel);
        zBitLabel->setGeometry(TwoByteShapes::zBitLabel);
        nBitLabel->setGeometry(TwoByteShapes::nBitLabel);


        // MemRead/Write
        MemWriteLabel->setGeometry(TwoByteShapes::MemWriteLabel);
        MemReadLabel->setGeometry(TwoByteShapes::MemReadLabel);
        MemWriteTristateLabel->setGeometry(TwoByteShapes::MemWriteTristateLabel);
        MemReadTristateLabel->setGeometry(TwoByteShapes::MemReadTristateLabel);
        scene->addRect(QRectF(MemWriteTristateLabel->pos(),
                              MemWriteTristateLabel->size()), QPen(Qt::gray));
        scene->addRect(QRectF(MemReadTristateLabel->pos(),
                              MemReadTristateLabel->size()), QPen(Qt::gray));

        scene->addRect(TwoByteShapes::aMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(TwoByteShapes::cMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(TwoByteShapes::cMuxerLabel);
        aMuxerBorder = scene->addRect(TwoByteShapes::aMuxerDataLabel);
        scene->addRect(QRectF(CSMuxTristateLabel->pos(), CSMuxTristateLabel->size()),QPen(Qt::gray));
        scene->addRect(QRectF(CSMuxerDataLabel->pos(), CSMuxerDataLabel->size()));
        scene->addRect(QRectF(AndZTristateLabel->pos(), AndZTristateLabel->size()),
                       QPen(Qt::gray));
        scene->addRect(TwoByteShapes::AndZMuxLabel);

        scene->addRect(TwoByteShapes::MDROMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(TwoByteShapes::MDREMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(TwoByteShapes::EOMuxTristateLabel, QPen(Qt::gray));
        scene->addRect(TwoByteShapes::MARMuxTristateLabel, QPen(Qt::gray));

        scene->addRect(TwoByteShapes::MARMuxerDataLabel);
        scene->addRect(TwoByteShapes::MDROMuxerDataLabel);
        scene->addRect(TwoByteShapes::MDREMuxerDataLabel);
        scene->addRect(TwoByteShapes::EOMuxerDataLabel);

        //scene->addRect(TwoByteShapes::MDREMuxLabel);
        //scene->addRect(TwoByteShapes::MDROMuxLabel);

        scene->addRect(TwoByteShapes::MDROLabel);
        scene->addRect(TwoByteShapes::MDRELabel);

        // NZVC data path to CMux, vertical black line
        scene->addPolygon(TwoByteShapes::NZVCDataPath,
                          QPen(QBrush(Qt::black), 1), QBrush(combCircuitYellow));
        scene->addLine(QLine(TwoByteShapes::NZVCDataLine));

    }

    // Status Bits
    scene->addRect(QRectF(cBitLabel->pos(), cBitLabel->size())); // C
    scene->addRect(QRectF(vBitLabel->pos(), vBitLabel->size())); // V
    scene->addRect(QRectF(zBitLabel->pos(), zBitLabel->size())); // Z
    scene->addRect(QRectF(nBitLabel->pos(), nBitLabel->size())); // N
    scene->addRect(QRectF(sBitLabel->pos(), sBitLabel->size())); // S

}

CpuGraphicsItems::~CpuGraphicsItems()
{
    delete loadCk;
    delete cLabel;
    delete cLineEdit;
    delete bLabel;
    delete bLineEdit;
    delete aLabel;
    delete aLineEdit;
    delete MARCk;
    delete MARALabel;
    delete MARBLabel;
    delete MDRCk;
    delete aMuxLabel;
    delete aMuxerDataLabel;
    delete aMuxerBorder;
    delete aMuxTristateLabel;
    delete MDRMuxLabel;
    delete MDRMuxerDataLabel;
    delete MDRMuxTristateLabel;
    delete MDRLabel;
    delete cMuxLabel;
    delete cMuxTristateLabel;
    delete cMuxerLabel;
    delete ALULabel;
    delete ALULineEdit;
    delete ALUFunctionLabel;
    delete ALUPoly;
    delete CSMuxLabel;
    delete CSMuxerDataLabel;
    delete CSMuxTristateLabel;
    delete SCkCheckBox;
    delete CCkCheckBox;
    delete VCkCheckBox;
    delete AndZLabel;
    delete AndZTristateLabel;
    delete AndZMuxLabel;
    delete ZCkCheckBox;
    delete NCkCheckBox;
    delete nBitLabel;
    delete zBitLabel;
    delete vBitLabel;
    delete cBitLabel;
    delete sBitLabel;
    delete MemReadLabel;
    delete MemReadTristateLabel;
    delete MemWriteLabel;
    delete MemWriteTristateLabel;

    delete aRegLineEdit;
    delete xRegLineEdit;
    delete spRegLineEdit;
    delete pcRegLineEdit;
    delete irRegLineEdit;
    delete t1RegLineEdit;
    delete t2RegLineEdit;
    delete t3RegLineEdit;
    delete t4RegLineEdit;
    delete t5RegLineEdit;
    delete t6RegLineEdit;

    delete MDROCk;
    delete MDRECk;
    delete MDROMuxTristateLabel;
    delete MDREMuxTristateLabel;
    delete EOMuxTristateLabel;
    delete MARMuxTristateLabel;
    delete MDRELabel;
    delete MDROLabel;
}

QRectF CpuGraphicsItems::boundingRect() const
{
    return QRectF(0,0, 650, 620);


    if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        return QRectF(0,0, 650, 620);
    }
    else if (Pep::cpuFeatures == Enu::TwoByteDataBus) {
        return QRectF(0,0, 650, 620);
    }
}

bool CpuGraphicsItems::aluHasCorrectOutput()
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

void CpuGraphicsItems::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(Qt::black);
    drawDiagramFreeText(painter);

    // c,b,a select line text
    repaintLoadCk(painter);
    repaintCSelect(painter);
    repaintBSelect(painter);
    repaintASelect(painter);
    repaintMARCk(painter);
    repaintAMuxSelect(painter); // Needs to be painted before buses
    repaintCMuxSelect(painter);
    painter->setPen(Qt::black);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:

        //Paint the buses in the correct order for the One Byte Bus
        /*
         * In the one byte bus, the buses must be drawn in the folling order C, B, A.
         * The B bus overlaps with the A bus, and the C bus overlaps with the B bus.
         * So, this rendering order prevents graphical issues.
         */
        repaintCBusOneByteModel(painter);
        repaintBBusOneByteModel(painter);
        repaintABusOneByteModel(painter);

        repaintMDRMuxSelect(painter);

        repaintMDRCk(painter);
        break;
    case Enu::TwoByteDataBus:
        repaintMDROCk(painter);
        repaintMDRECk(painter);
        repaintEOMuxSelect(painter);
        repaintMDROSelect(painter);
        repaintMDRESelect(painter);
        repaintMARMUXToMARBuses(painter);
        repaintMDRMuxOutputBuses(painter);
        //Every select line above ALU firt first
        repaintMARMuxSelect(painter);
        repaintMDREToEOMuxBus(painter);
        repaintMDROToEOMuxBus(painter);
        repaintEOMuxOutpusBus(painter);
        repaintABusTwoByteModel(painter);
        repaintBBusTwoByteModel(painter);
        repaintCBusTwoByteModel(painter);

        break;
    default:
        break;
    }



    repaintCSMuxSelect(painter);

    repaintSCk(painter);
    repaintCCk(painter);
    repaintVCk(painter);
    repaintZCk(painter);
    repaintNCk(painter);
    repaintMemCommon(painter);
    repaintMemRead(painter);
    repaintMemWrite(painter);
    repaintSBitOut(painter);
    repaintCBitOut(painter);
    repaintVBitOut(painter);
    repaintZBitOut(painter);
    repaintNBitOut(painter);

    repaintAndZSelect(painter);
    repaintALUSelect(painter);

}

void CpuGraphicsItems::drawDiagramFreeText(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(Qt::black);
    painter->drawText(7, 320, "Addr");
    painter->drawText(528,92, "5");
    painter->drawText(528,70, "5");
    painter->drawText(528,48, "5");
    painter->save();
    painter->rotate(-90);
    auto font= painter->font();
    font.setPointSize(font.pointSize()*1.3);
    painter->setFont(font);
    painter->drawText(-260,35, "System Bus");
    painter->restore();
    switch(Pep::cpuFeatures)
    {
    case Enu::CPUType::OneByteDataBus:
        painter->drawText(7, 395, "Data");
        painter->drawText(372,132, "ABus");
        painter->drawText(433,132, "BBus");
        painter->drawText(300,132, "CBus");
        // alu select line text
        painter->drawText(OneByteShapes::ctrlInputX - 23, ALULineEdit->y() + 5, "4");

        painter->drawText(368,388, "ALU");

        // NZVC data path text
        painter->drawText(314,531, "0");
        painter->drawText(314,541, "0");
        painter->drawText(314,551, "0");
        painter->drawText(314,561, "0");

        painter->drawText(OneByteShapes::MARALabel.x() - 37, OneByteShapes::MARALabel.y() + 13, "MARA");
        painter->drawText(OneByteShapes::MARBLabel.x() - 37, OneByteShapes::MARBLabel.y() + 13, "MARB");

        break;
    case Enu::CPUType::TwoByteDataBus:
        painter->drawText(7, TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowOuterYSpread+10, "Data");
        painter->drawText(427,132, "ABus");
        painter->drawText(483,132, "BBus");
        painter->drawText(350,132, "CBus");
        // alu select line text
        painter->drawText(TwoByteShapes::ctrlInputX - 23, ALULineEdit->y() + 5, "4");
        painter->drawText(368 + TwoByteShapes::controlOffsetX,
                          388 + TwoByteShapes::aluOffsetY, "ALU");

        // NZVC data path text
        painter->drawText(314 + TwoByteShapes::controlOffsetX,
                          531 + TwoByteShapes::aluOffsetY, "0");
        painter->drawText(314 + TwoByteShapes::controlOffsetX,
                          541 + TwoByteShapes::aluOffsetY, "0");
        painter->drawText(314 + TwoByteShapes::controlOffsetX,
                          551 + TwoByteShapes::aluOffsetY, "0");
        painter->drawText(314 + TwoByteShapes::controlOffsetX,
                          561 + TwoByteShapes::aluOffsetY, "0");
        painter->drawText(TwoByteShapes::MARALabel.x() - 37, TwoByteShapes::MARALabel.y() + 13, "MARA");
        painter->drawText(TwoByteShapes::MARBLabel.x() - 37, TwoByteShapes::MARBLabel.y() + 13, "MARB");
        break;
    }
}

void CpuGraphicsItems::repaintLoadCk(QPainter *painter)
{
    QColor color;

    color = loadCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::loadCkSelect._lines);

        for (int i = 0; i < OneByteShapes::loadCkSelect._arrowheads.length(); i++) {
            painter->drawImage(OneByteShapes::loadCkSelect._arrowheads.at(i),
                               color == Qt::gray ? arrowLeftGray : arrowLeft);
        }
        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::loadCkSelect._lines);

        for (int i = 0; i < TwoByteShapes::loadCkSelect._arrowheads.length(); i++) {
            painter->drawImage(TwoByteShapes::loadCkSelect._arrowheads.at(i),
                               color == Qt::gray ? arrowLeftGray : arrowLeft);
        }
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintCSelect(QPainter *painter)
{
    bool ok;
    QColor color;

    cLineEdit->text().toInt(&ok, 10);
    ok ? color = Qt::black : color = Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // Draw select lines
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::CSelect._lines);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::CSelect._lines);
        break;
    default:
        break;
    }

    painter->drawImage(QPoint(499,47),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
}

void CpuGraphicsItems::repaintBSelect(QPainter *painter)
{
    bool ok;
    QColor color;

    bLineEdit->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // arrow
    painter->drawImage(QPoint(499,69),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    // Draw select lines
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::BSelect._lines);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::BSelect._lines);
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintASelect(QPainter *painter)
{
    bool ok;

    aLineEdit->text().toInt(&ok, 10);
    QColor color;
    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // arrow
    painter->drawImage(QPoint(499,91),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    // Draw select lines
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::ASelect._lines);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::ASelect._lines);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintMARCk(QPainter *painter)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        repaintMARCkOneByteModel(painter);
        break;
    case Enu::TwoByteDataBus:
        repaintMARCkTwoByteModel(painter);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintAMuxSelect(QPainter *painter)
{
    QColor color;
    bool ok;
    int aMux = aMuxTristateLabel->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);
    // Draw AMux select depending on the enabled feature set
    switch(Pep::cpuFeatures)
    {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::AMuxSelect._lines);
        painter->drawImage(QPoint(380,300),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::AMuxSelect._lines);
        painter->drawImage(TwoByteShapes::AMuxSelect._arrowheads.first(), //Should more arrowheads be added, this will need to be a proper for loop.
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    }
    if (ok) {
        switch (aMux) {
        case (0):
            if(Pep::cpuFeatures==Enu::TwoByteDataBus){
                if(EOMuxTristateLabel->text()=="0"){
                    color=combCircuitGreen;
                    aMuxerDataLabel->setPalette(QPalette(muxCircuitGreen));
                }
                else{
                    color=combCircuitYellow;
                    aMuxerDataLabel->setPalette(QPalette(muxCircuitYellow));
                }
            }
            else
            {
                color=combCircuitYellow;
                aMuxerDataLabel->setPalette(QPalette(muxCircuitYellow));
            }
            break;
        case (1):
            if (aLineEdit->text() == "") { // ABus.state == UNDEFINED
                color = Qt::white;
                aMuxerDataLabel->setPalette(QPalette(Qt::white));
            } else {
                color = combCircuitRed;
                aMuxerDataLabel->setPalette(QPalette(muxCircuitRed));
            }
            break;
        }
    } else {
        aMuxerDataLabel->setPalette(QPalette(Qt::white));
        color = Qt::white;
    }
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // Draw AMux bus depending on the enabled feature set
    switch(Pep::cpuFeatures)
    {
    case Enu::OneByteDataBus:
        painter->drawPolygon(OneByteShapes::AMuxBus);
        break;
    case Enu::TwoByteDataBus:
        painter->drawPolygon(TwoByteShapes::AMuxBus);
        break;
    }

}

void CpuGraphicsItems::repaintMARMuxSelect(QPainter *painter)
{
    QColor color;
    bool ok;
    MARMuxTristateLabel->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // AMux Select
    painter->drawLines(TwoByteShapes::MARMuxSelect._lines);
    painter->drawImage(TwoByteShapes::MARMuxSelect._arrowheads.first(),
                       color == Qt::gray ? arrowDownGray : arrowDown);

}

void CpuGraphicsItems::repaintEOMuxSelect(QPainter *painter)
{
    QColor color;
    bool ok;
    EOMuxTristateLabel->text().toInt(&ok, 10);

    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // AMux Select
    painter->drawLines(TwoByteShapes::EOMuxSelect._lines);

    painter->drawImage(TwoByteShapes::EOMuxSelect._arrowheads.first(),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

}

void CpuGraphicsItems::repaintCMuxSelect(QPainter *painter)
{
    QColor color;

    color = cMuxTristateLabel->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // CMux Select
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::CMuxSelect._lines);
        painter->drawImage(OneByteShapes::CMuxSelect._arrowheads.first(),
                           color == Qt::gray ? arrowDownGray : arrowDown);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::CMuxSelect._lines);
        painter->drawImage(TwoByteShapes::CMuxSelect._arrowheads.first(),
                           color == Qt::gray ? arrowDownGray : arrowDown);
        break;
    default:
        break;
    }
    if (cMuxTristateLabel->text() == "0") {
        color = combCircuitYellow;
        cMuxerLabel->setPalette(QPalette(muxCircuitYellow));
    }
    else if (cMuxTristateLabel->text() == "1") {
        if (!aluHasCorrectOutput() || ALULineEdit->text() == "15") {
            // CBus.state == UNDEFINED or NZVC A
            qDebug() << "WARNING: CMux select: There is no ALU output";
            cMuxerLabel->setPalette(QPalette(Qt::white));
            color = Qt::white;
        }
        else {
            cMuxerLabel->setPalette(muxCircuitBlue);
            color = combCircuitBlue;
        }
    }
    else {
        cMuxerLabel->setPalette(QPalette(Qt::white));
        color = Qt::white;
    }
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // CMuxBus (output)
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        break;
    case Enu::TwoByteDataBus:
        painter->drawPolygon(TwoByteShapes::CBus);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintSCk(QPainter *painter)
{
    QColor color;

    color = SCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::SBitSelect);
        // arrow
        painter->drawImage(QPoint(OneByteShapes::sBitLabel.right() + OneByteShapes::arrowHOffset,
                                  OneByteShapes::SBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);

        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::SBitSelect);
        // arrow
        painter->drawImage(QPoint(TwoByteShapes::sBitLabel.right() + TwoByteShapes::arrowHOffset,
                                  TwoByteShapes::SBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);

        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintCCk(QPainter *painter)
{
    QColor color;

    color = CCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // line from checkbox to data
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::CBitSelect);
        // arrow
        painter->drawImage(QPoint(OneByteShapes::cBitLabel.right() + OneByteShapes::arrowHOffset,
                                  OneByteShapes::CBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::CBitSelect);
        // arrow
        painter->drawImage(QPoint(TwoByteShapes::cBitLabel.right() + TwoByteShapes::arrowHOffset,
                                  TwoByteShapes::CBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintVCk(QPainter *painter)
{
    QColor color;

    color = VCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::VBitSelect);
        painter->drawImage(QPoint(OneByteShapes::vBitLabel.right() + OneByteShapes::arrowHOffset,
                                  OneByteShapes::VBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::VBitSelect);
        painter->drawImage(QPoint(TwoByteShapes::vBitLabel.right() + TwoByteShapes::arrowHOffset,
                                  TwoByteShapes::VBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintZCk(QPainter *painter)
{
    QColor color;

    color = ZCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::ZBitSelect);
        painter->drawImage(QPoint(OneByteShapes::zBitLabel.right() + OneByteShapes::arrowHOffset,
                                  OneByteShapes::ZBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::ZBitSelect);
        painter->drawImage(QPoint(TwoByteShapes::zBitLabel.right() + TwoByteShapes::arrowHOffset,
                                  TwoByteShapes::ZBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintNCk(QPainter *painter)
{
    QColor color;

    color = NCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::NBitSelect);
        painter->drawImage(QPoint(OneByteShapes::nBitLabel.right() + OneByteShapes::arrowHOffset,
                                  OneByteShapes::NBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::NBitSelect);
        painter->drawImage(QPoint(TwoByteShapes::nBitLabel.right() + TwoByteShapes::arrowHOffset,
                                  TwoByteShapes::NBitSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintMemCommon(QPainter *painter)
{
    switch(Pep::cpuFeatures)
    {
    case Enu::OneByteDataBus:
        break;
    case Enu::TwoByteDataBus:
        repaintMemCommonTwoByte(painter);
        break;
    }
}

void CpuGraphicsItems::repaintMemRead(QPainter *painter)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        repaintMemReadOneByteModel(painter);
        break;
    case Enu::TwoByteDataBus:
        repaintMemReadTwoByteModel(painter);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintMemWrite(QPainter *painter)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        repaintMemWriteOneByteModel(painter);
        break;
    case Enu::TwoByteDataBus:
        repaintMemWriteTwoByteModel(painter);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintSBitOut(QPainter *painter)
{
    sBitLabel->text() = Sim::sBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch(Pep::cpuFeatures)
    {
    case Enu::OneByteDataBus:
        // line from S bit to CSMux
        painter->drawLines(OneByteShapes::SBitToCSMux._lines);
        // arrow:
        painter->drawImage(OneByteShapes::SBitToCSMux._arrowheads.first(), arrowUp);
        break;
    case Enu::TwoByteDataBus:
        // line from S bit to CSMux
        painter->drawLines(TwoByteShapes::SBitToCSMux._lines);
        // arrow:
        painter->drawImage(TwoByteShapes::SBitToCSMux._arrowheads.first(), arrowUp);
        break;
    }
}

void CpuGraphicsItems::repaintCBitOut(QPainter *painter)
{
    cBitLabel->text() = Sim::cBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        // line from C bit to NZVC bus
        painter->drawLines(OneByteShapes::CBitToNZVC._lines);
        // arrow to the NZVC data bus
        painter->drawImage(OneByteShapes::CBitToNZVC._arrowheads.first(), arrowLeft);

        // line from C bit to CSMux
        painter->drawLines(OneByteShapes::CBitToCSMux._lines);
        // arrow to the CSMux
        painter->drawImage(QPoint(OneByteShapes::CBitToCSMux._arrowheads.first()), arrowUp);

        // CIN line back to the ALU
        painter->drawLines(OneByteShapes::CInToALU._lines);
        // CIN arrow to the ALU
        painter->drawImage(OneByteShapes::CInToALU._arrowheads.first(), arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        // line from C bit to NZVC bus
        painter->drawLines(TwoByteShapes::CBitToNZVC._lines);
        // arrow to the NZVC data bus
        painter->drawImage(TwoByteShapes::CBitToNZVC._arrowheads.first(), arrowLeft);

        // line from C bit to CSMux
        painter->drawLines(TwoByteShapes::CBitToCSMux._lines);
        // arrow to the CSMux
        painter->drawImage(QPoint(TwoByteShapes::CBitToCSMux._arrowheads.first()), arrowUp);

        // CIN line back to the ALU
        painter->drawLines(TwoByteShapes::CInToALU._lines);
        // CIN arrow to the ALU
        painter->drawImage(TwoByteShapes::CInToALU._arrowheads.first(), arrowLeft);
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintVBitOut(QPainter *painter)
{
    vBitLabel->text() = Sim::vBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::VBitOut._lines);

        painter->drawImage(OneByteShapes::VBitOut._arrowheads.first(), arrowLeft);

        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::VBitOut._lines);

        painter->drawImage(TwoByteShapes::VBitOut._arrowheads.first(), arrowLeft);

        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintZBitOut(QPainter *painter)
{
    zBitLabel->text() = Sim::zBit ? "1" : "0";

    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    QPoint point = QPoint(437,582);
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawEllipse(point, 2, 2);
        painter->drawLines(OneByteShapes::ZBitOut._lines);

        painter->drawImage(OneByteShapes::ZBitOut._arrowheads.first(), arrowLeft);
        painter->drawImage(OneByteShapes::ZBitOut._arrowheads.last(), arrowUp);  // AndZ arrow upwards
        break;
    case Enu::TwoByteDataBus:
        point.setX(point.x() + TwoByteShapes::controlOffsetX);
        point.setY(point.y() + TwoByteShapes::aluOffsetY);
        painter->drawEllipse(point, 2, 2);
        painter->drawLines(TwoByteShapes::ZBitOut._lines);

        painter->drawImage(TwoByteShapes::ZBitOut._arrowheads.first(), arrowLeft);
        painter->drawImage(TwoByteShapes::ZBitOut._arrowheads.last(), arrowUp);  // AndZ arrow upwards
        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintNBitOut(QPainter *painter)
{
    nBitLabel->text() = Sim::nBit ? "1" : "0";

    QPolygon poly;
    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLines(OneByteShapes::NBitOut._lines);

        painter->drawImage(OneByteShapes::NBitOut._arrowheads.first(), arrowLeft);

        break;
    case Enu::TwoByteDataBus:
        painter->drawLines(TwoByteShapes::NBitOut._lines);

        painter->drawImage(TwoByteShapes::NBitOut._arrowheads.first(), arrowLeft);

        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintCSMuxSelect(QPainter *painter)
{
    QColor color;

    color = CSMuxTristateLabel->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        // line from checkbox to data
        painter->drawLine(OneByteShapes::CSMuxSelect);
        // arrow
        painter->drawImage(QPoint(OneByteShapes::CSMuxerDataLabel.right() + OneByteShapes::arrowHOffset,
                                  OneByteShapes::CSMuxSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    case Enu::TwoByteDataBus:
        // line from checkbox to data
        painter->drawLine(TwoByteShapes::CSMuxSelect);
        // arrow
        painter->drawImage(QPoint(TwoByteShapes::CSMuxerDataLabel.right() + TwoByteShapes::arrowHOffset,
                                  TwoByteShapes::CSMuxSelect.y1() - 3),
                           color == Qt::gray ? arrowLeftGray : arrowLeft);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintAndZSelect(QPainter *painter)
{
    QPolygon poly;

    QColor color = Qt::gray;

    if (AndZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // lines coming out of tristate label
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::AndZOut._lines[0]);
        painter->drawLine(OneByteShapes::AndZOut._lines[1]);

        painter->drawImage(OneByteShapes::AndZOut._arrowheads.first(),
                           color == Qt::gray ? arrowDownGray : arrowDown);

        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::AndZOut._lines[0]);
        painter->drawLine(TwoByteShapes::AndZOut._lines[1]);
        painter->drawImage(TwoByteShapes::AndZOut._arrowheads.first(),
                           color == Qt::gray ? arrowDownGray : arrowDown);

        break;
    default:
        break;
    }

    color = Qt::gray;
    if (aluHasCorrectOutput() && AndZTristateLabel->text() != "") {
        color = Qt::black;
    }
    else{
        color = Qt::gray;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // AndZ out
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        painter->drawLine(OneByteShapes::AndZOut._lines[2]);
        painter->drawImage(QPoint(OneByteShapes::zBitLabel.x()-13,OneByteShapes::AndZMuxLabel.y()+OneByteShapes::AndZMuxLabel.height()/2-4),
                           color == Qt::gray ? arrowRightGray : arrowRight);

        break;
    case Enu::TwoByteDataBus:
        painter->drawLine(TwoByteShapes::AndZOut._lines[2]);
        //The arrow is ~10 pixels long, and another 3 are needed for it to fit comfortably next to the box
        //The arrow is 8 pixels high, align the the center of the arrow with the middle of the box.
        painter->drawImage(QPoint(TwoByteShapes::zBitLabel.x()-13,TwoByteShapes::AndZMuxLabel.y()+TwoByteShapes::AndZMuxLabel.height()/2-4),
                           color == Qt::gray ? arrowRightGray : arrowRight);

        break;
    default:
        break;
    }


}

void CpuGraphicsItems::repaintALUSelect(QPainter *painter)
{
    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:
        repaintALUSelectOneByteModel(painter);
        break;
    case Enu::TwoByteDataBus:
        repaintALUSelectTwoByteModel(painter);
        break;
    default:
        break;
    }
}

void CpuGraphicsItems::repaintMDRMuxSelect(QPainter *painter)
{
    QColor color;
    painter->setPen(Qt::black);
    if(MDRCk->isChecked()){
        if(MDRMuxTristateLabel->text()=="0"&&Sim::mainBusState==Enu::MemReadSecondWait){
            MDRMuxerDataLabel->setPalette(muxCircuitRed);
            color = combCircuitRed;
        }
        else if(MDRMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="0"){
            MDRMuxerDataLabel->setPalette(muxCircuitYellow);
            color = combCircuitYellow;
        }
        else if(MDRMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="1"&&aluHasCorrectOutput()){
            MDRMuxerDataLabel->setPalette(muxCircuitBlue);
            color = combCircuitBlue;
        }
        else{
            MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
            color = Qt::white;
        }

    }
    else{
        MDRMuxerDataLabel->setPalette(QPalette(Qt::white));
        color = Qt::white;
    }

    painter->setBrush(color);
    // MDRMuxOutBus (MDRMux to MDR arrow)
    painter->drawPolygon(OneByteShapes::MDRMuxOutBus);

    // finish up by drawing select lines:
    color = Qt::gray;
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
}


// ***************************************************************************
// One byte model-specific functionality:
// ***************************************************************************

void CpuGraphicsItems::repaintMARCkOneByteModel(QPainter *painter)
{
    QColor color;

    color = MARCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MARCk
    painter->drawLines(OneByteShapes::MARCk._lines);

    painter->drawEllipse(QPoint(235,177), 2, 2);

    painter->drawImage(OneByteShapes::MARCk._arrowheads.first(),
                       color == Qt::gray ? arrowUpGray : arrowUp);
    painter->drawImage(OneByteShapes::MARCk._arrowheads.last(),
                       color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuGraphicsItems::repaintMDRCk(QPainter *painter)
{
    QColor color;

    switch (Pep::cpuFeatures) {
    case Enu::OneByteDataBus:

        color = MDRCk->isChecked() ? Qt::black : Qt::gray;
        painter->setPen(QPen(QBrush(color), 1));
        painter->setBrush(color);

        // MDRCk
        painter->drawLines(OneByteShapes::MDRCk._lines);

        painter->drawImage(OneByteShapes::MDRCk._arrowheads.first(),
                           color == Qt::gray ? arrowDownGray : arrowDown);
        break;
    case Enu::TwoByteDataBus:

        break;
    default:
        break;
    }

}

void CpuGraphicsItems::repaintALUSelectOneByteModel(QPainter *painter)
{
    QColor color;

    color = ALULineEdit->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // ALU Select
    painter->drawLines(OneByteShapes::ALUSelect._lines);

    painter->drawImage(OneByteShapes::ALUSelect._arrowheads.first(),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    painter->setPen(Qt::black);

    if (ALULineEdit->text() != "" && ALULineEdit->text() != "15") {
        int aluFn = ALULineEdit->text().toInt();
        if (aMuxTristateLabel->text() == "0" && Sim::aluFnIsUnary(aluFn)) {
            painter->setBrush(combCircuitBlue);
        }
        else if (aMuxTristateLabel->text() == "0" && bLineEdit->text() != "") {
            painter->setBrush(combCircuitBlue);
        }
        else if (aMuxTristateLabel->text() == "1") {
            if (aLineEdit->text() != "" && Sim::aluFnIsUnary(aluFn)) {
                painter->setBrush(combCircuitBlue);
            }
            else if (aLineEdit->text() != "" && bLineEdit->text() != "") {
                painter->setBrush(combCircuitBlue);
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
    color = aluHasCorrectOutput() ? Qt::black : Qt::gray;
    painter->setPen(color);
    painter->setBrush(color);

    painter->drawLines(OneByteShapes::ALUSelectOut._lines);

    for (int i = 0; i < OneByteShapes::ALUSelectOut._arrowheads.length(); i++) {
        painter->drawImage(OneByteShapes::ALUSelectOut._arrowheads.at(i),
                           color == Qt::gray ? arrowRightGray : arrowRight);
    }

    // S ellipse
    painter->drawEllipse(QPoint(416,446), 2, 2); //437+9
}

void CpuGraphicsItems::repaintMemReadOneByteModel(QPainter *painter)
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
    painter->drawLine(OneByteShapes::MemReadSelect);

    painter->drawImage(QPoint(OneByteShapes::DataBus.right()+5,
                              OneByteShapes::MemReadSelect.y1() - 3),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    if (MemWriteTristateLabel->text() == "1") {
        // Do not paint main bus if MemWrite is isHigh
        return;
    }

    // Draw ADDR bus stuff:
    isHigh ? color = combCircuitYellow : color = Qt::white;

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
        color = combCircuitGreen;
    }
    else {
        color = Qt::white;
    }
    painter->setBrush(color);

    painter->setBrush(combCircuitRed);
    // Data bus:
    painter->drawRect(OneByteShapes::DataBus);

    // Mem Data Bus
    poly.clear();
    // arrowhead into the main bus:
    if (color == combCircuitGreen) {
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

void CpuGraphicsItems::repaintMemWriteOneByteModel(QPainter *painter)
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
    painter->drawLine(OneByteShapes::MemWriteSelect);
    painter->drawImage(QPoint(OneByteShapes::DataBus.right()+5,
                              OneByteShapes::MemWriteSelect.y1() - 3),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    // draw line from memWrite to MDR out:
    painter->drawEllipse(QPoint(OneByteShapes::DataBus.right()+25,
                                OneByteShapes::MemWriteSelect.y1()),
                         2, 2);
    painter->drawLine(OneByteShapes::DataBus.right()+25, OneByteShapes::MemWriteSelect.y1() - 3,
                      OneByteShapes::DataBus.right()+25,345);
    // memWrite line from the label to the bus:
    painter->drawLine(OneByteShapes::DataBus.right()+25,333, OneByteShapes::DataBus.right()+25,280); //268+12
    painter->drawImage(QPoint(OneByteShapes::DataBus.right()+22,271), //96-3 //268+12-9
                       color == Qt::gray ? arrowUpGray : arrowUp);

    // repaint the MDR-to-main-bus line, based on if MemWrite is set or not
    // note: it should be lighter (disabled) when MemWrite is not set.
    color = combCircuitGreen;
    if (!isHigh) {
        color = color.lighter(120);
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
        color = combCircuitYellow;
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
        color = combCircuitGreen;
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

void CpuGraphicsItems::repaintABusOneByteModel(QPainter *painter)
{
    bool ok;
    aLineEdit->text().toInt(&ok, 10);
    QColor color;
    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    color = ok ? combCircuitRed : Qt::white;

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);
    // ABus
    painter->drawPolygon(OneByteShapes::ABus1);
    painter->drawPolygon(OneByteShapes::ABus2);
}

void CpuGraphicsItems::repaintBBusOneByteModel(QPainter *painter)
{
    bool ok;
    bLineEdit->text().toInt(&ok, 10);;
    QColor color;
    color = ok ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    color = ok ? combCircuitRed : Qt::white;

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);
    // ABus
    painter->drawPolygon(OneByteShapes::BBus1);
    painter->drawPolygon(OneByteShapes::BBus2);
    painter->drawPolygon(OneByteShapes::BBusRect);
}

void CpuGraphicsItems::repaintCBusOneByteModel(QPainter *painter)
{ QColor color;
    if (cMuxTristateLabel->text() == "0") {
        color = combCircuitYellow;
    }
    else if (cMuxTristateLabel->text() == "1") {
        if (!aluHasCorrectOutput() || ALULineEdit->text() == "15") {
            color = Qt::white;
        }
        else {
            color = combCircuitBlue;
        }
    }
    else {
        color = Qt::white;
    }
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);
    painter->drawPolygon(OneByteShapes::CBus);
}



// ***************************************************************************
// Two byte model-specific functionality:
// ***************************************************************************

void CpuGraphicsItems::repaintMARCkTwoByteModel(QPainter *painter)
{
    QColor color;

    color = MARCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MARCk
    painter->drawLines(TwoByteShapes::MARCk._lines);


    painter->drawEllipse(QPoint(TwoByteShapes::MARCk._lines.last().x1(),TwoByteShapes::MARMuxerDataLabel.y()+TwoByteShapes::MARMuxerDataLabel.height()/2), 2, 2);
    painter->drawImage(TwoByteShapes::MARCk._arrowheads[0],
            color == Qt::gray ? arrowUpGray : arrowUp);
    painter->drawImage(TwoByteShapes::MARCk._arrowheads[1],
            color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuGraphicsItems::repaintMDROCk(QPainter *painter)
{
    QColor color;
    color = MDROCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MDROdd Clock
    painter->drawLines(TwoByteShapes::MDROck._lines);
    painter->drawImage(TwoByteShapes::MDROck._arrowheads.first(),
                       color == Qt::gray ? arrowDownGray : arrowDown);
}

void CpuGraphicsItems::repaintMDRECk(QPainter *painter)
{
    QColor color;
    color = MDRECk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // MDREven Clock
    painter->drawLines(TwoByteShapes::MDREck._lines);
    painter->drawImage(TwoByteShapes::MDREck._arrowheads.first(),
                       color == Qt::gray ? arrowDownGray : arrowDown);

}

void CpuGraphicsItems::repaintEOMuxOutpusBus(QPainter *painter)
{
    QColor color = Qt::white;
    if(EOMuxTristateLabel->text()=="1"){
        color=combCircuitYellow;
        EOMuxerDataLabel->setPalette(muxCircuitYellow);
    }
    else if(EOMuxTristateLabel->text()=="0"){
        color=combCircuitGreen;
        EOMuxerDataLabel->setPalette(muxCircuitGreen);
    }
    else
    {
        EOMuxerDataLabel->setPalette(Qt::white);
    }
    painter->setPen(Qt::black);
    painter->setBrush(color);
    painter->drawPolygon(TwoByteShapes::EOMuxOutputBus);
}

void CpuGraphicsItems::repaintALUSelectTwoByteModel(QPainter *painter)
{
    QColor color;

    color = ALULineEdit->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // ALU Select
    painter->drawLines(TwoByteShapes::ALUSelect._lines);

    painter->drawImage(TwoByteShapes::ALUSelect._arrowheads.first(),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    painter->setPen(Qt::black);

    if (ALULineEdit->text() != "" && ALULineEdit->text() != "15") {
        int aluFn = ALULineEdit->text().toInt();
        if (aMuxTristateLabel->text() == "0" && Sim::aluFnIsUnary(aluFn)) {
            painter->setBrush(combCircuitBlue);
        }
        else if (aMuxTristateLabel->text() == "0" && bLineEdit->text() != "") {
            painter->setBrush(combCircuitBlue);
        }
        else if (aMuxTristateLabel->text() == "1") {
            if (aLineEdit->text() != "" && Sim::aluFnIsUnary(aluFn)) {
                painter->setBrush(combCircuitBlue);
            }
            else if (aLineEdit->text() != "" && bLineEdit->text() != "") {
                painter->setBrush(combCircuitBlue);
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
    painter->drawPolygon(TwoByteShapes::ALUOutBus);

    // Draw status bit lines
    painter->setPen(aluHasCorrectOutput() ? Qt::black : Qt::gray);
    painter->setBrush(aluHasCorrectOutput() ? Qt::black : Qt::gray);

    painter->drawLines(TwoByteShapes::ALUSelectOut._lines);

    for (int i = 0; i < TwoByteShapes::ALUSelectOut._arrowheads.length(); i++) {
        painter->drawImage(TwoByteShapes::ALUSelectOut._arrowheads.at(i),
                           color == Qt::gray ? arrowRightGray : arrowRight);
    }

    // S ellipse
    painter->drawEllipse(QPoint(416 + TwoByteShapes::controlOffsetX,
                                TwoByteShapes::sBitLabel.y() + TwoByteShapes::selectYOffset),
                         2, 2);

}

void CpuGraphicsItems::repaintMemCommonTwoByte(QPainter *painter)
{
    QColor color;
    bool readIsHigh = MemReadTristateLabel->text()=="1",writeIsHigh = MemWriteTristateLabel->text()=="1";

    if (readIsHigh||writeIsHigh) {
        // qDebug() << "mainBusState: " << Sim::mainBusState;
        // ADDR bus is yellow if the bus is high
        color = combCircuitYellow;
    }
    else {
        color = Qt::white;
    }

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // Main Bus
    // Main ADDR bus:
    painter->drawRect(TwoByteShapes::AddrBus);
    // left arrow from addr bus to memory:
    painter->drawPolygon(TwoByteShapes::AddrArrow);
    // Draw memwrite select line
    if (writeIsHigh) {
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
    painter->drawLine(TwoByteShapes::MemWriteSelect); //611+8
    painter->drawImage(QPoint(TwoByteShapes::DataBus.right()+5,
                              TwoByteShapes::MemWriteSelect.y1() - 3),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);

    // draw line from memWrite to MDRO out:
    painter->drawEllipse(QPoint(TwoByteShapes::DataBus.right()+20,
                                TwoByteShapes::MemWriteSelect.y1()),
                         2, 2);
    painter->drawLine(TwoByteShapes::DataBus.right()+20,TwoByteShapes::MemWriteSelect.y1(), TwoByteShapes::DataBus.right()+20,TwoByteShapes::MDROLabel.bottom()); //611+8
    // memWrite line from the label to the bus:
    painter->drawImage(QPoint(TwoByteShapes::DataBus.right()+17, //96-3
                              //The bottom of the bus is 5 pixels below the label's midpoint, and add 3 pixels for visual comfort.
                              TwoByteShapes::MDROLabel.y()+TwoByteShapes::MDROLabel.height()/2+5+3),
                       color == Qt::gray ? arrowUpGray : arrowUp);
    painter->drawImage(QPoint(TwoByteShapes::DataBus.right()+17,//96-3
                              //The bottom of the bus is 5 pixels below the label's midpoint, and add 3 pixels for visual comfort.
                              TwoByteShapes::MDRELabel.y()+TwoByteShapes::MDRELabel.height()/2+5+3),
                       color == Qt::gray ? arrowUpGray : arrowUp);

    // Draw memread select line
    if (readIsHigh) {
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
    painter->drawLine(TwoByteShapes::MemReadSelect);

    painter->drawImage(QPoint(TwoByteShapes::DataBus.right()+5,
                              TwoByteShapes::MemReadSelect.y1() - 3),
                       color == Qt::gray ? arrowLeftGray : arrowLeft);
    QPolygon poly;

    //Pick data bus and data arrow color
    if(MemReadTristateLabel->text()=="1" && (Sim::mainBusState == Enu::MemReadReady ||
                                             Sim::mainBusState == Enu::MemReadSecondWait)){
        color=combCircuitRed;
    }
    else if(MemWriteTristateLabel->text()=="1" && (Sim::mainBusState == Enu::MemWriteReady ||
                                                   Sim::mainBusState == Enu::MemWriteSecondWait)){
        color = combCircuitGreen;
    }
    else{
        color =Qt::white;
    }
    painter->setPen(Qt::black);
    painter->setBrush(color);
    // Left end points
    if (MemReadTristateLabel->text()=="1" && (Sim::mainBusState == Enu::MemReadReady ||
                   Sim::mainBusState == Enu::MemReadSecondWait)) {
        // square end (when reading):
        poly << QPoint(TwoByteShapes::DataArrowLeftX+TwoByteShapes::arrowHOffset,
                       TwoByteShapes::DataArrowMidpointY-TwoByteShapes::DataArrowInnerYSpread) // Arrow Top Inner point
             << QPoint(TwoByteShapes::DataArrowLeftX+TwoByteShapes::arrowHOffset,
                       TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowInnerYSpread); // Arrow Bottom Inner point
    }
    else {
        poly << QPoint(TwoByteShapes::DataArrowLeftX+TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY-TwoByteShapes::DataArrowInnerYSpread)  // Arrow Top Inner point
             << QPoint(TwoByteShapes::DataArrowLeftX+TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY-TwoByteShapes::DataArrowOuterYSpread)  // Arrow Top Outer point
             << QPoint(TwoByteShapes::DataArrowLeftX, TwoByteShapes::DataArrowMidpointY)   // Arrow Middle point
             << QPoint(TwoByteShapes::DataArrowLeftX+TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowOuterYSpread)  // Arrow Bottom Outer point
             << QPoint(TwoByteShapes::DataArrowLeftX+TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowInnerYSpread); // Arrow Bottom Inner point
    }
    // Right end points
    if (MemWriteTristateLabel->text()=="1" && (Sim::mainBusState == Enu::MemWriteReady ||
                                               Sim::mainBusState == Enu::MemWriteSecondWait)) {
        // square end (when writing):
        poly << QPoint(TwoByteShapes::DataArrowRightX+1,
                       TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowInnerYSpread) // Arrow Bottom Inner point
             << QPoint(TwoByteShapes::DataArrowRightX+1,
                       TwoByteShapes::DataArrowMidpointY-TwoByteShapes::DataArrowInnerYSpread); // Arrow Top Inner point
    }
    else {
        poly << QPoint(TwoByteShapes::DataArrowRightX-TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowInnerYSpread) // Arrow Bottom Inner point
             << QPoint(TwoByteShapes::DataArrowRightX-TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY+TwoByteShapes::DataArrowOuterYSpread) // Arrow Bottom Outer point
             << QPoint(TwoByteShapes::DataArrowRightX, TwoByteShapes::DataArrowMidpointY) // Arrow Middle point
             << QPoint(TwoByteShapes::DataArrowRightX-TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY-TwoByteShapes::DataArrowOuterYSpread) // Arrow Top Outer point
             << QPoint(TwoByteShapes::DataArrowRightX-TwoByteShapes::DataArrowDepth,
                       TwoByteShapes::DataArrowMidpointY-TwoByteShapes::DataArrowInnerYSpread); // Arrow Top Inner point
    }
    painter->drawPolygon(poly);

    //Do right end or arrow


}

void CpuGraphicsItems::repaintMemReadTwoByteModel(QPainter *painter)
{
    QColor color;
    bool isHigh = MemReadTristateLabel->text() == "1";

    // right arrow from Bus to MDR{O,E}Mux:
    if (MemWriteTristateLabel->text() == "1") {
        // Do not paint main bus if MemWrite is isHigh

        return;
    }
    if (isHigh && (Sim::mainBusState == Enu::MemReadReady ||
                Sim::mainBusState == Enu::MemReadSecondWait)) {
        color = combCircuitRed;
    }
    else {
        color = Qt::white;
    }
    painter->setBrush(color);
    // Data bus:
    painter->drawRect(TwoByteShapes::DataBus);

    // Mem Data Bus
    painter->drawPolygon(TwoByteShapes::DataToMDROMuxBus);
    painter->drawPolygon(TwoByteShapes::DataToMDREMuxBus);


}

void CpuGraphicsItems::repaintMemWriteTwoByteModel(QPainter *painter)
{
    QColor color;
    bool isHigh = MemWriteTristateLabel->text() == "1";

    // repaint the MDR-to-main-bus line, based on if MemWrite is set or not
    // note: it should be lighter (disabled) when MemWrite is not set.
    color = combCircuitGreen;
    if (!isHigh) {
        color = color.lighter(120);
    }
    painter->setBrush(color);
    painter->setPen(QPen(QBrush(Qt::black), 1));
    // mdr to data bus
    painter->drawPolygon(TwoByteShapes::MDROToDataBus);
    painter->drawPolygon(TwoByteShapes::MDREToDataBus);

    if (MemReadTristateLabel->text() == "1") {
        // Do not paint main bus if MemRead is high
        return;
    }

    // Draw ADDR bus stuff:
    // Draw DATA bus stuff:
    // figure out the color:
    if (isHigh && (Sim::mainBusState == Enu::MemWriteReady ||
                Sim::mainBusState == Enu::MemWriteSecondWait)) {
        color = combCircuitGreen;
    }
    else {
        color = Qt::white;
    }
    painter->setPen(Qt::black);
    painter->setBrush(color);

    // Main Data bus:
    painter->drawRect(TwoByteShapes::DataBus);

    // Mem Data Bus (bidirectional arrow)
    // Main Bus to MDRMux is ALWAYS white on a memWrite:
    painter->setBrush(Qt::white);

    // right arrow from Bus to MDR{O,E}Mux:
    painter->drawPolygon(TwoByteShapes::DataToMDROMuxBus);
    painter->drawPolygon(TwoByteShapes::DataToMDREMuxBus);

}

void CpuGraphicsItems::repaintMARMUXToMARBuses(QPainter *painter)
{
    //Needs conditional painting based on the state of the bus.
    bool marckIsHigh = MARCk->isChecked();
    QColor colorTop=Qt::white,colorBottom=Qt::white;
    if(marckIsHigh && MARMuxTristateLabel->text()=="0"){
        colorTop= combCircuitYellow;
        colorBottom = combCircuitGreen;
    }
    else if(marckIsHigh && MARMuxTristateLabel->text()=="1")
    {
        colorTop = colorBottom = combCircuitRed;
    }
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(colorBottom);
    painter->drawPolygon(TwoByteShapes::MARMuxToMARABus);
    painter->setBrush(colorTop);
    painter->drawPolygon(TwoByteShapes::MARMuxToMARBBus);
}

void CpuGraphicsItems::repaintMDRESelect(QPainter *painter)
{
    //Determine if control line is high, so that the color may be set accordingly
    bool MDREIsHigh=MDREMuxTristateLabel->text()=="1"||MDREMuxTristateLabel->text()=="0";
    QColor MDREColor=MDREIsHigh?Qt::black:Qt::gray;

    //Paint MDRESelect's lines and arrow in the appropriate color
    painter->setPen(MDREColor);
    painter->drawLines(TwoByteShapes::MDREMuxSelect._lines);
    painter->drawImage(TwoByteShapes::MDREMuxSelect._arrowheads.first(),
                       MDREColor == Qt::gray ? arrowLeftGray : arrowLeft);
    if(MDRECk->isChecked()){
        if(MDREMuxTristateLabel->text()=="0"&&Sim::mainBusState==Enu::MemReadSecondWait){
            MDREMuxerDataLabel->setPalette(muxCircuitRed);
        }
        else if(MDREMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="0"){
            MDREMuxerDataLabel->setPalette(muxCircuitYellow);
        }
        else if(MDREMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="1"&&aluHasCorrectOutput()){
            MDREMuxerDataLabel->setPalette(muxCircuitBlue);
        }
        else{
            MDREMuxerDataLabel->setPalette(QPalette(Qt::white));
        }

    }
    else{
        MDREMuxerDataLabel->setPalette(QPalette(Qt::white));
    }
}

void CpuGraphicsItems::repaintMDROSelect(QPainter *painter)
{
    //Determine if control line is high, so that the color may be set accordingly
    bool MDROIsHigh=MDROMuxTristateLabel->text()=="1"||MDROMuxTristateLabel->text()=="0";
    QColor MDROColor=MDROIsHigh?Qt::black:Qt::gray;

    //Paint MDROSelect's lines and arrow in the appropriate color
    painter->setPen(MDROColor);
    painter->drawLines(TwoByteShapes::MDROMuxSelect._lines);
    painter->drawImage(TwoByteShapes::MDROMuxSelect._arrowheads.first(),
                       MDROColor == Qt::gray ? arrowLeftGray : arrowLeft);
    if(MDROCk->isChecked()){
        if(MDROMuxTristateLabel->text()=="0"&&Sim::mainBusState==Enu::MemReadSecondWait){
            MDROMuxerDataLabel->setPalette(muxCircuitRed);
        }
        else if(MDROMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="0"){
            MDROMuxerDataLabel->setPalette(muxCircuitYellow);
        }
        else if(MDROMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="1"&&aluHasCorrectOutput()){
            MDROMuxerDataLabel->setPalette(muxCircuitBlue);
        }
        else{
            MDROMuxerDataLabel->setPalette(QPalette(Qt::white));
        }

    }
    else{
        MDROMuxerDataLabel->setPalette(QPalette(Qt::white));
    }
}

void CpuGraphicsItems::repaintMDRMuxOutputBuses(QPainter *painter)
{
    // MDRMuxOutBus (MDRMux to MDR arrow)
    QColor colorMDRE = Qt::white, colorMDRO = Qt::white;
    // Depending on which input is selected on the MDRMuxes, the color might need to change.
    // For now red seems to be the most logical color to use all the time.
    QString MDREText = MDREMuxTristateLabel->text(), MDROText = MDROMuxTristateLabel->text();
    if(MDRECk->isChecked()&&(MDREText=="1"||MDREText=="0")){
         //If the muxer is enabled, and data can be clocked in to the register, pick an appropriate color
        if(MDREMuxTristateLabel->text()=="0"&&Sim::mainBusState==Enu::MemReadSecondWait){
            colorMDRE=combCircuitRed;
        }
        else if(MDREMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="0"){
            colorMDRE=combCircuitYellow;
        }
        else if(MDREMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="1"&&aluHasCorrectOutput()){
            colorMDRE=combCircuitBlue;
        }
        else{
            colorMDRE = Qt::white;
        }
    }
    if(MDROCk->isChecked()&&(MDROText=="1"||MDROText=="0")){
         //If the muxer is enabled, and data can be clocked in to the register, pick an appropriate color
        if(MDROMuxTristateLabel->text()=="0"&&Sim::mainBusState==Enu::MemReadSecondWait){
            colorMDRO=combCircuitRed;
        }
        else if(MDROMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="0"){
            colorMDRO=combCircuitYellow;
        }
        else if(MDROMuxTristateLabel->text()=="1"&&cMuxTristateLabel->text()=="1"&&aluHasCorrectOutput()){
            colorMDRO=combCircuitBlue;
        }
        else{
            colorMDRO = Qt::white;
        }
    }
    painter->setPen(Qt::black);
    painter->setBrush(colorMDRE);
    painter->drawPolygon(TwoByteShapes::MDREMuxOutBus);
    painter->setBrush(colorMDRO);
    painter->drawPolygon(TwoByteShapes::MDROMuxOutBus);
}

void CpuGraphicsItems::repaintMDREToEOMuxBus(QPainter *painter){
    QColor color = Qt::white;
    if(MARMuxTristateLabel->text()=="0"||EOMuxTristateLabel->text()=="1"||EOMuxTristateLabel->text()=="0"){
        color=combCircuitGreen;
    }
    painter->setPen(Qt::black);
    painter->setBrush(color);
    painter->drawPolygon(TwoByteShapes::MDREToEOMuxBus);
}

void CpuGraphicsItems::repaintMDROToEOMuxBus(QPainter *painter){
    QColor color=Qt::white;
    if(MARMuxTristateLabel->text()=="0"||EOMuxTristateLabel->text()=="1"||EOMuxTristateLabel->text()=="0"){
        color=combCircuitYellow;
    }
    painter->setPen(Qt::black);
    painter->setBrush(color);
    painter->drawPolygon(TwoByteShapes::MDROToEOMuxBus);
}

void CpuGraphicsItems::repaintABusTwoByteModel(QPainter *painter)
{
    bool ok;
    aLineEdit->text().toInt(&ok, 10);
    QColor color;
    color = ok ? combCircuitRed : Qt::white;
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // ABus
    painter->drawPolygon(TwoByteShapes::ABus);
}

void CpuGraphicsItems::repaintBBusTwoByteModel(QPainter *painter)
{
    bool ok;
    bLineEdit->text().toInt(&ok, 10);
    QColor color;
    color = ok ? combCircuitRed : Qt::white;
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // BBus
    painter->drawPolygon(TwoByteShapes::BBus);
}

void CpuGraphicsItems::repaintCBusTwoByteModel(QPainter *painter)
{
    bool ok;
    cLineEdit->text().toInt(&ok, 10);
    QColor color;
    color = ok ? combCircuitRed : Qt::white;
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(color);

    // CBus
    painter->drawPolygon(TwoByteShapes::CBus);
}



