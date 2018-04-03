// File: objectcodepane.cpp
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

#include "objectcodepane.h"
#include "ui_objectcodepane.h"
#include "microcodeprogram.h"
#include "sim.h"
#include "pep.h"
#include <QPainter>
#include <QDebug>
#include <QTextEdit>
#include <QTextItem>
#include <QTableWidget>
#include <QStringList>
#include <QKeyEvent>
#include <rotatedheaderview.h>
ObjectCodePane::ObjectCodePane(QWidget *parent) :
    QWidget(parent), rowCount(0),inSimulation(false),
    ui(new Ui::ObjectCodePane)
{
    ui->setupUi(this);
    QFont font(Pep::codeFont);
    font.setPointSize(Pep::codeFontSize);
    font.setStyleHint(QFont::TypeWriter);
    auto x = new RotatedHeaderView(Qt::Horizontal,ui->codeTable);
    x->setModel(ui->codeTable->model());
    ui->codeTable->setHorizontalHeader(x);
    ui->codeTable->setFont(font);
    ui->codeTable->verticalHeader()->setDefaultSectionSize(15);
    ui->codeTable->horizontalHeader()->setDefaultSectionSize(20);
    ui->codeTable->setShowGrid(false);
    ui->codeTable->setRowCount(0);
    initCPUModelState();
    ui->codeTable->installEventFilter(this);
    //ui->codeTabl
}

ObjectCodePane::~ObjectCodePane()
{
    delete ui;
    delete program;
}

void ObjectCodePane::initCPUModelState()
{

    setObjectCode();
    clearSimulationView();
    assignHeaders();
}

void ObjectCodePane::setObjectCode()
{

    setObjectCode(new MicrocodeProgram());
}

void ObjectCodePane::setObjectCode(MicrocodeProgram* program)
{
    assignHeaders();
    if(this->program==nullptr)
    {
        delete this->program;
    }
    this->program = program;
    int rowNum=0,colNum=0;
    ui->codeTable->setRowCount(0);
    QList<Enu::EMnemonic> list = Pep::memControlToMnemonMap.keys();
    list.append(Pep::decControlToMnemonMap.keys());
    list.append(Pep::clockControlToMnemonMap.keys());
    for(Code* row : program->getObjectCode())
    {
        if(!row->isMicrocode())
        {
           continue;
        }
        colNum=0;
        ui->codeTable->insertRow(rowNum);
        for(auto col : list)
        {
            auto x = QString::number(((MicroCode*)row)->get(col));
            if(x!="-1")
            {
                auto y =new QTableWidgetItem(x);
                //Ownership of y is taken by the codeTable, so no need to deal with the pointer ourselves
                ui->codeTable->setItem(rowNum,colNum,y);
            }
            colNum++;
        }
        rowNum++;
    }
    ui->codeTable->resizeColumnsToContents();
}

void ObjectCodePane::highlightCurrentInstruction()
{
    ui->codeTable->selectRow(rowCount++);
    inSimulation=true;
}

void ObjectCodePane::clearSimulationView()
{
    ui->codeTable->clearSelection();
    rowCount=0;
    inSimulation=false;
}

void ObjectCodePane::copy()
{
    //ui->plainTextEdit->copy();
}

void ObjectCodePane::assignHeaders()
{
    QList<Enu::EMnemonic> list = Pep::memControlToMnemonMap.keys();
    list.append(Pep::decControlToMnemonMap.keys());
    list.append(Pep::clockControlToMnemonMap.keys());
    QMetaEnum num = QMetaEnum::fromType<Enu::EMnemonic>();
    QList<QString> headers;
    for(auto x : list)
    {
        headers.append(QString(num.valueToKey(x)));
    }
    ui->codeTable->setColumnCount(list.size());
    ui->codeTable->setHorizontalHeaderLabels(headers);
    for(int x=0;x<list.size();x++)
    {
        ui->codeTable->horizontalHeaderItem(x)->setTextAlignment(Qt::AlignVCenter);
    }
    ui->codeTable->horizontalHeader()->setVisible(true);
    ui->codeTable->resizeColumnsToContents();
}

bool ObjectCodePane::eventFilter(QObject *object, QEvent *event)
{
    if(!inSimulation) return false;
    switch(event->type())
    {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key())
        {
        case Qt::UpArrow:
        case Qt::DownArrow:
            return true;
        default:
            return false;
        }
    }
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
        return true;
    default:
        return false;
    }
}

void ObjectCodePane::onCPUFeatureChange()
{
    clearSimulationView();
    ui->codeTable->setRowCount(0);
    //Clear the columns
    ui->codeTable->setColumnCount(0);
    assignHeaders();
}

void ObjectCodePane::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ObjectCodePane::highlightOnFocus()
{
    if (ui->codeTable->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}
