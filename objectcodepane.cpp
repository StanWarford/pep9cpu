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
    QWidget(parent), rowCount(0),model(new QStandardItemModel()),inSimulation(false),
    ui(new Ui::ObjectCodePane)
{
    ui->setupUi(this);
    QFont font(Pep::codeFont);
    font.setPointSize(Pep::codeFontSize);
    font.setStyleHint(QFont::TypeWriter);
    ui->codeTable->setModel(model);
    rotatedHeaderView= new RotatedHeaderView(Qt::Horizontal,ui->codeTable);
    rotatedHeaderView->setModel(ui->codeTable->model());
    selectionModel= new DisableSelectionModel(ui->codeTable->model());
    ui->codeTable->setSelectionModel(selectionModel);
    ui->codeTable->setHorizontalHeader(rotatedHeaderView);
    ui->codeTable->setFont(font);
    ui->codeTable->verticalHeader()->setDefaultSectionSize(12);
    ui->codeTable->horizontalHeader()->setDefaultSectionSize(15);
    ui->codeTable->setShowGrid(false);
    model->setRowCount(0);
    initCPUModelState();
    //Connect to disabler
    connect(this,SIGNAL(beginSimulation()),selectionModel,SLOT(onBeginSimulation()));
    connect(this,SIGNAL(endSimulation()),selectionModel,SLOT(onEndSimulation()));

    //ui->codeTabl
}

ObjectCodePane::~ObjectCodePane()
{
    delete ui;
    delete program;
    delete model;
    delete rotatedHeaderView;
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
    model->setRowCount(0);
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
        model->insertRow(rowNum);
        for(auto col : list)
        {
            auto x = QString::number(((MicroCode*)row)->get(col));
            if(x!="-1")
            {
                auto y =new QStandardItem(x);
                y->setTextAlignment(Qt::AlignCenter);
                //Ownership of y is taken by the codeTable, so no need to deal with the pointer ourselves
                model->setItem(rowNum,colNum,y);
            }
            colNum++;
        }
        rowNum++;
    }
    ui->codeTable->resizeColumnsToContents();
}

void ObjectCodePane::highlightCurrentInstruction()
{
    selectionModel->forceSelectRow(rowCount++);
    ui->codeTable->setCurrentIndex(model->index(rowCount,0));
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
    model->setColumnCount(list.size());
    model->setHorizontalHeaderLabels(headers);
    for(int x=0;x<list.size();x++)
    {
        model->horizontalHeaderItem(x)->setTextAlignment(Qt::AlignVCenter);
    }
    ui->codeTable->horizontalHeader()->setVisible(true);
    ui->codeTable->resizeColumnsToContents();
}

void ObjectCodePane::onCPUFeatureChange()
{
    clearSimulationView();
    model->setRowCount(0);
    //Clear the columns
    model->setColumnCount(0);
    assignHeaders();
}

void ObjectCodePane::onBeginSimulation()
{
    emit beginSimulation();
}

void ObjectCodePane::onEndSimulation()
{
    emit endSimulation();
}

void ObjectCodePane::onDarkModeChanged(bool)
{
    ui->codeTable->verticalHeader()->setFont(QFont(Pep::codeFont,Pep::codeFontSize));
    //ui->codeTable->resizeRowsToContents();
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
