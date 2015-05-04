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

#include "sim.h"
#include "pep.h"
#include <QPainter>
#include <QDebug>

ObjectCodePane::ObjectCodePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectCodePane)
{
    ui->setupUi(this);

    QFont font(Pep::codeFont);
    font.setPointSize(Pep::codeFontSize);
    font.setStyleHint(QFont::TypeWriter);

    ui->plainTextEdit->setFont(font);

    cpuLabel = NULL;
    initCPUModelState();
}

ObjectCodePane::~ObjectCodePane()
{
    delete ui;
}

void ObjectCodePane::initCPUModelState()
{
    if (cpuLabel != NULL) {
        delete cpuLabel;
    }

    //cpuLabel = new ObjectCodeLabel(this);
    //ui->verticalLayout->insertWidget(1, cpuLabel);
    //cpuLabel->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    //cpuLabel->setMinimumHeight(QFontMetrics(cpuLabel->font()).averageCharWidth() * 8 + 3); // +3 for padding

    setObjectCode("");
    clearSimulationView();
}

void ObjectCodePane::setObjectCode(QString string)
{
    ui->plainTextEdit->setPlainText(string);
}

void ObjectCodePane::highlightCurrentInstruction()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(QColor(56, 117, 215)); // dark blue
    selection.format.setForeground(Qt::white);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    QTextCursor cursor = QTextCursor(ui->plainTextEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < Sim::microProgramCounter; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }

    // this chunk moves the cursor down and scrolls the text edit to it:
    ui->plainTextEdit->setTextCursor(cursor);
    ui->plainTextEdit->ensureCursorVisible();

    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    ui->plainTextEdit->setExtraSelections(extraSelections);
}

void ObjectCodePane::clearSimulationView()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    ui->plainTextEdit->setExtraSelections(extraSelections);
}

void ObjectCodePane::copy()
{
    ui->plainTextEdit->copy();
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
    if (ui->plainTextEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}
