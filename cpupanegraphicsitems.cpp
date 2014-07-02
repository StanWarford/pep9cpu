// File: cpupanegraphicsitems.cpp
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

#include "cpupanegraphicsitems.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QGraphicsScene>
#include <QPainter>

#include <QDebug>
#include "sim.h"

CpuPaneGraphicsItems::CpuPaneGraphicsItems(QWidget *widgetParent, QGraphicsItem *itemParent, QGraphicsScene *scene)
    : QGraphicsItem(itemParent),
      parent(widgetParent)
{

    seqCircuitColor = QColor(0xffdd77);// 0xffee99); //QColor(Qt::gray).lighter(140);
    combCircuitRed = QColor(Qt::red).lighter(190);
    combCircuitBlue = QColor(Qt::blue).lighter(190);
    combCircuitYellow = QColor(Qt::yellow).lighter(170);
    combCircuitGreen = QColor(Qt::green).lighter(190);

    loadCk = new QCheckBox("LoadCk");
    loadCk->setPalette(QPalette(Qt::white));
    loadCk->setGeometry(550, 18, 80, 20);
    scene->addWidget(loadCk);

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

    MDRCk = new QCheckBox("MDRCk");
    MDRCk->setGeometry(550, 225, 80, 20);
    MDRCk->setPalette(QPalette(Qt::white));
    scene->addWidget(MDRCk);

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

    cMuxLabel = new QLabel("CMux");
    cMuxLabel->setGeometry(QRect(579, 349, 61, 20));
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

    CCkCheckBox = new QCheckBox ("CCk");
    CCkCheckBox->setGeometry(550, 407, 60, 20);
    CCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(CCkCheckBox);
    cBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    cBitLabel->setText("0");
    cBitLabel->setGeometry(476,406, 19, 19);
    cBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(cBitLabel);

    VCkCheckBox = new QCheckBox("VCk");
    VCkCheckBox->setGeometry(550, 444, 60, 20);
    VCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(VCkCheckBox);
    vBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    vBitLabel->setText("0");
    vBitLabel->setGeometry(476,442, 19, 19);
    vBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    vBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(vBitLabel);

    ANDZLabel = new QLabel("ANDZ");
    ANDZLabel->setGeometry(579, 470, 45, 20);
    ANDZLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ANDZLabel);
    ANDZTristateLabel = new TristateLabel(0, TristateLabel::Tristate);
    ANDZTristateLabel->setGeometry(550, 468, 25, 21);
    ANDZTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ANDZTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ANDZTristateLabel);
    scene->addRect(550, 468, 25, 21, QPen(Qt::gray));

    ANDZMuxLabel = new QLabel("ANDZ");
    ANDZMuxLabel->setGeometry(415,495, 41,21);
    ANDZMuxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ANDZMuxLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(ANDZMuxLabel);

    ZCkCheckBox = new QCheckBox("ZCk");
    ZCkCheckBox->setGeometry(550, 499, 60, 20);
    ZCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(ZCkCheckBox);
    zBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    zBitLabel->setText("0");
    zBitLabel->setGeometry(476, 498, 19, 19);
    zBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    zBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(zBitLabel);

    NCkCheckBox = new QCheckBox ("NCk");
    NCkCheckBox->setGeometry(550, 550, 60, 20);
    NCkCheckBox->setPalette(QPalette(Qt::white));
    scene->addWidget(NCkCheckBox);
    nBitLabel = new TristateLabel(0, TristateLabel::ZeroOne);
    nBitLabel->setText("0");
    nBitLabel->setGeometry(476,549, 19, 19);
    nBitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    nBitLabel->setPalette(QPalette(seqCircuitColor));
    scene->addWidget(nBitLabel);

    MemReadLabel = new QLabel("MemRead");
    MemReadLabel->setGeometry(579, 577, 80, 20);
    MemReadLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemReadLabel);
    MemReadTristateLabel = new TristateLabel(0, TristateLabel::OneUndefined);
    MemReadTristateLabel->setGeometry(550, 576, 25, 20);
    MemReadTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemReadTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemReadTristateLabel);
    scene->addRect(550, 576, 25, 20, QPen(Qt::gray));

    MemWriteLabel = new QLabel("MemWrite");
    MemWriteLabel->setGeometry(579, 598, 80, 20);
    MemWriteLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemWriteLabel);
    MemWriteTristateLabel = new TristateLabel(0, TristateLabel::OneUndefined);
    MemWriteTristateLabel->setGeometry(550, 596, 25, 20);
    MemWriteTristateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MemWriteTristateLabel->setPalette(QPalette(Qt::white));
    scene->addWidget(MemWriteTristateLabel);
    scene->addRect(550, 596, 25, 20, QPen(Qt::gray));

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

    // MARA & MARB
    scene->addRect(175, 132, 69, 19);
    scene->addRect(175, 202, 69, 19);

    // MARBus
    QPolygonF poly;
    poly << QPoint(205,151) << QPoint(205,167) << QPoint(173-70,167) << QPoint(173-70,162)
         << QPoint(158-70,177) << QPoint(173-70,192) << QPoint(173-70,187) << QPoint(205,187)
         << QPoint(205,202) << QPoint(215,202) << QPoint(215,151);

    scene->addPolygon(poly, QPen(QBrush(Qt::black), 1), QBrush(Qt::yellow));
    QGraphicsLineItem *lineItem = scene->addLine(173-70, 177, 215, 177);
    lineItem->setZValue(1); // make sure this line appears above the bus

    // MDR
    scene->addRect(175, 254, 69, 19);

    // MDRBus
    scene->addRect(244,258, 36,10, QPen(Qt::black), QBrush(Qt::yellow));
    poly.clear();
    poly << QPoint(290,258) << QPoint(326,258) << QPoint(326,280) << QPoint(331,280)
         << QPoint(321,290) << QPoint(311,280) << QPoint(316,280) << QPoint(316,268) << QPoint(290,268);
    scene->addPolygon(poly, QPen(Qt::black), QBrush(Qt::yellow));
    poly.clear();
    poly << QPoint(175,258) << QPoint(168-70,258) << QPoint(168-70,253) << QPoint(158-70,263)
         << QPoint(168-70,273) << QPoint(168-70,268) << QPoint(175,268);
    scene->addPolygon(poly, QPen(Qt::black), QBrush(Qt::yellow));

    // MDRMux
    scene->addRect(175, 293, 69, 19);

    // AMux
    aMuxerBorder = scene->addRect(306,293, 69,19);

    // CMux
    scene->addRect(250,374, 69,19);

    // ANDZ
    scene->addRect(415,495, 41,21);

    // ALU
    poly.clear();
    poly << QPoint(314,342) << QPoint(366,342) << QPoint(370,353) << QPoint(390,353) << QPoint(394,342)
         << QPoint(447,342) << QPoint(421,394) << QPoint(340,394);
    ALUPoly = scene->addPolygon(poly, QPen(QBrush(QColor(Qt::blue)), 2, Qt::SolidLine, Qt::SquareCap,
                                           Qt::BevelJoin), QBrush(QColor(Qt::blue).lighter(190)));
    ALUPoly->setZValue(-1);

    // Status Bits
    scene->addRect(476,406, 19,19);
    scene->addRect(476,442, 19,19);
    scene->addRect(476,498, 19,19);
    scene->addRect(476,549, 19,19);

    poly.clear();
    poly << QPoint(310,513) << QPoint(269,513) << QPoint(269,407) << QPoint(274,407) << QPoint(264,397)
         << QPoint(254,407) << QPoint(259,407) << QPoint(259,523) << QPoint(310,523);
    scene->addPolygon(poly, QPen(QBrush(Qt::black), 1), QBrush(Qt::yellow));
    scene->addLine(310, 477, 310, 559);
}

QRectF CpuPaneGraphicsItems::boundingRect() const
{
    return QRectF(0,0, 650, 620);
}

bool CpuPaneGraphicsItems::aluHasCorrectOutput()
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

void CpuPaneGraphicsItems::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

    repaintCCk(painter);
    repaintVCk(painter);
    repaintZCk(painter);
    repaintNCk(painter);
    repaintMemRead(painter);
    repaintMemWrite(painter);
    repaintCBitOut(painter);
    repaintVBitOut(painter);
    repaintZBitOut(painter);
    repaintNBitOut(painter);

    repaintANDZSelect(painter);
    repaintALUSelect(painter);
    repaintMDRMuxSelect(painter);

}

void CpuPaneGraphicsItems::repaintLoadCk(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = loadCk->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(543, 27, 499, 27);

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(499,27) << QPoint(507,24) << QPoint(507,30);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintCSelect(QPainter *painter)
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

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(499, 50) << QPoint(507, 47) << QPoint(507, 53);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintBSelect(QPainter *painter)
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

    painter->setRenderHint(QPainter::Antialiasing);
    poly.clear();
    poly << QPoint(499, 72) << QPoint(507, 69) << QPoint(507, 75);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

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

void CpuPaneGraphicsItems::repaintASelect(QPainter *painter)
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

    painter->setRenderHint(QPainter::Antialiasing);
    poly.clear();
    poly << QPoint(499, 94) << QPoint(507, 91) << QPoint(507, 97);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

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

void CpuPaneGraphicsItems::repaintMARCk(QPainter *painter)
{
    QPolygon poly;
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

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(238,163) << QPoint(232,163) << QPoint(235,155);
    painter->drawPolygon(poly);

    poly.clear();
    poly << QPoint(238,191) << QPoint(232,191) << QPoint(235,199);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintMDRCk(QPainter *painter)
{
    QPolygon poly;
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

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(213,241) << QPoint(207,241) << QPoint(210,249);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintAMuxSelect(QPainter *painter)
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

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 388, 300, 388, 306, 380, 303);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

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

void CpuPaneGraphicsItems::repaintCMuxSelect(QPainter *painter)
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
    //    painter->drawLine(428,350, 543,350);

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(257,362) << QPoint(263,362) << QPoint(260,370);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

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

    // CMuxBus
    poly.clear();
    poly << QPoint(290,374) << QPoint(290,130) << QPoint(295,130) << QPoint(285,120)
         << QPoint(275,130) << QPoint(280,130) << QPoint(280,334) << QPoint(240,334)
         << QPoint(240,326) << QPoint(245,326) << QPoint(235,316) << QPoint(225,326)
         << QPoint(230,326) << QPoint(230,344) << QPoint(280,344) << QPoint(280,374);
    painter->drawPolygon(poly);
}

void CpuPaneGraphicsItems::repaintCCk(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = CCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510,415, 543,415);

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(510,412) << QPoint(510,418) << QPoint(502,415);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

}

void CpuPaneGraphicsItems::repaintVCk(QPainter *painter)
{
    QColor color;
    QPolygon poly;

    color = VCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510, 451, 543, 451);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 510, 448, 510, 454, 502, 451);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintZCk(QPainter *painter)
{
    QColor color;
    QPolygon poly;

    color = ZCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510, 507, 543, 507);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 510,504, 510,510, 502,507);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

}

void CpuPaneGraphicsItems::repaintNCk(QPainter *painter)
{
    QColor color;
    QPolygon poly;

    color = NCkCheckBox->isChecked() ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(510, 558, 543, 558);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 510,555, 510,561, 502,558);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintMemRead(QPainter *painter)
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

    painter->drawLine(166-70, 585, 543, 585); // memRead line from the label to the bus

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(166-70, 582) << QPoint(166-70, 588) << QPoint(158-70, 585);// memRead arrowhead
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

    if (MemWriteTristateLabel->text() == "1") {
        // Do not paint main bus if MemWrite is isHigh
        return;
    }

    // Draw main bus
    if (isHigh) {
        if (Sim::mainBusState == Enu::None) {
            // We haven't memRead yet, but are about to
            color = Qt::yellow;
        }
        else if (Sim::mainBusState == Enu::MemReadReady || Sim::mainBusState == Enu::MemReadWait) {
            // We have read once, and are about to again
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
    poly << QPoint(145-70, 132) << QPoint(155-70, 132) << QPoint(155-70, 334) << QPoint(180, 334)
         << QPoint(180, 326) << QPoint(175, 326) << QPoint(185, 316) << QPoint(195, 326) << QPoint(190, 326)
         << QPoint(190, 344) << QPoint(155-70, 344) << QPoint(155-70, 608) << QPoint(145-70, 608)
         << QPoint(145-70, 375) << QPoint(136-123, 375) << QPoint(136-123, 380) << QPoint(126-123, 370)
         << QPoint(136-123, 360) << QPoint(136-123, 365) << QPoint(145-70, 365);
    painter->drawPolygon(poly);

    if (Sim::mainBusState != Enu::MemReadReady) {
        color = Qt::white;
    }
    painter->setBrush(color);

    // MemOutBus
    poly.clear();
    poly << QPoint(0, 350) << QPoint(134-70, 350) << QPoint(134-70, 355) << QPoint(144-70, 345)
         << QPoint(134-70, 335) << QPoint(134-70, 340) << QPoint(0, 340);
    painter->drawPolygon(poly);
}

void CpuPaneGraphicsItems::repaintMemWrite(QPainter *painter)
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

    painter->drawLine(166-70, 605, 543, 605); // memWrite line from label to bus

    painter->setRenderHint(QPainter::Antialiasing);
    poly << QPoint(166-70, 602) << QPoint(166-70, 608) << QPoint(158-70, 605); // memWrite arrowhead
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

    if (MemReadTristateLabel->text() == "1") {
        // Do not paint main bus if MemRead is high
        return;
    }

    // Draw main bus
    if (isHigh) {
        // qDebug() << "mainBusState: " << Sim::mainBusState;
        if (Sim::mainBusState == Enu::None) {
            // We have not yet memWrite'n, but are about to clock for the first time
            color = Qt::yellow;
        }
        else if (Sim::mainBusState == Enu::MemWriteWait && MemWriteTristateLabel->text() == "1") {
            // We have written once, and are about to again
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
         << QPoint(190, 344) << QPoint(155-70, 344) << QPoint(155-70, 608) << QPoint(145-70, 608)
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

void CpuPaneGraphicsItems::repaintCBitOut(QPainter *painter)
{
    cBitLabel->text() = Sim::cBit ? "1" : "0";

    QColor color = Qt::black;
    QPolygon poly;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(487,426, 487,430);
    painter->drawLine(487,430, 330,430);
    painter->drawLine(330,430, 330,486);
    painter->drawLine(330,486, 322,486);

    painter->drawLine(487,404, 487,389);
    painter->drawLine(487,389, 443,389);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 322,483, 322,489, 314,486);
    painter->drawPolygon(poly);
    poly.setPoints(3, 443,386, 443,392, 435,389);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintVBitOut(QPainter *painter)
{
    vBitLabel->text() = Sim::vBit ? "1" : "0";

    QPolygon poly;
    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    /* V out */
    painter->drawLine(487,462, 487,466);
    painter->drawLine(487,466, 345,466);
    painter->drawLine(345,466, 345,496);
    painter->drawLine(345,496, 322,496);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 322,493, 322,499, 314,496);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintZBitOut(QPainter *painter)
{
    zBitLabel->text() = Sim::zBit ? "1" : "0";

    QPolygon poly;
    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(487,518, 487,537);
    painter->drawLine(487,537, 345,537);
    painter->drawLine(345,537, 345,506);
    painter->drawLine(345,506, 322,506);

    painter->drawLine(437,537, 437,529);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 322,503, 322,509, 314,506);
    painter->drawPolygon(poly);
    poly.setPoints(3, 434,529, 440,529, 437,521);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintNBitOut(QPainter *painter)
{
    nBitLabel->text() = Sim::nBit ? "1" : "0";

    QPolygon poly;
    QColor color = Qt::black;

    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    painter->drawLine(487,569, 487,573);
    painter->drawLine(487,573, 330,573);
    painter->drawLine(330,573, 330,517);
    painter->drawLine(330,517, 322,517);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 322,514, 322,520, 314,517);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintANDZSelect(QPainter *painter)
{
    QPolygon poly;

    QColor color = Qt::gray;

    if (ANDZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    painter->drawLine(437,484, 437,476);
    painter->drawLine(437,476, 543,476);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 434,484, 440,484, 437,493);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

    color = Qt::gray;
    if (ALULineEdit->text() != "" && ANDZTristateLabel->text() != "") {
        color = Qt::black;
    }
    painter->setPen(color);
    painter->setBrush(color);

    // ANDZ out
    painter->drawLine(458,507, 465,507);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 465,504, 465,510, 473,507);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintALUSelect(QPainter *painter)
{
    QPolygon poly;
    QColor color;

    color = ALULineEdit->text() != "" ? Qt::black : Qt::gray;
    painter->setPen(QPen(QBrush(color), 1));
    painter->setBrush(color);

    // ALU Select Coordinates
    painter->drawLine(449,376, 543,376);
    painter->drawLine(523,371, 533,381);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 449,373, 449,379, 441,376);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

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
    painter->drawLine(371,395, 371,558);
    painter->drawLine(371,558, 465,558);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 465,555, 465,561, 473,558);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

    // Z
    painter->drawLine(386,395, 386,507);
    painter->drawLine(386,507, 404,507);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 404,504, 404,510, 412,507);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

    // V
    painter->drawLine(401,395, 401,451);
    painter->drawLine(401,451, 466,451);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 466,448, 466,454, 474,451);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);

    // C
    painter->drawLine(416,395, 416,415);
    painter->drawLine(416,415, 465,415);

    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 465,412, 465,418, 473,415);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CpuPaneGraphicsItems::repaintMDRMuxSelect(QPainter *painter)
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
    //painter->drawLine(523,319, 533,329);
    painter->setRenderHint(QPainter::Antialiasing);
    poly.setPoints(3, 257,300, 257,306, 249,303);
    painter->drawPolygon(poly);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(Qt::black);

    if (MDRMuxTristateLabel->text() == "0") {
        if ((Sim::mainBusState == Enu::MemReadWait && MemReadTristateLabel->text() == "1") || Sim::mainBusState == Enu::MemReadReady) {
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

    // MDRMuxBus
    poly.setPoints(7, 205,293, 205,286, 200,286, 210,276, 220,286, 215,286, 215,293); // arrow
    painter->drawPolygon(poly);
}

