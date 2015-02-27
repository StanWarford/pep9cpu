// File: cpupanetwobytedatabus.h
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
#ifndef CPUPANETWOBYTEDATABUS_H
#define CPUPANETWOBYTEDATABUS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>

#include "enu.h"

#include "cpupanebasegraphicsitems.h"
#include "cpupane.h"

namespace Ui {
    class CpuPane;
}

class CpuPaneTwoByteDataBus : public CpuPane
{
    Q_OBJECT
public:
    explicit CpuPaneTwoByteDataBus(QWidget *parent = 0);

    void initModel();

    void clearCpu();
    void clearCpuControlSignals();

protected:
    //simulation helper
    void updateMainBusState();

    // called by the push buttons to simulate a single step; returns true if there were no issues
    bool step(QString& errorString);

private:
    Ui::CpuPane *ui;

protected slots:
    void on_copyToMicrocodePushButton_clicked();

    bool getAMuxOut(quint8& out, QString& errorString);
    bool getMARMuxOut(quint8& mara, quint8& marb, QString& errorString);
    bool getMDROMuxOut(quint8& out, QString& errorString);
    bool getMDREMuxOut(quint8& out, QString& errorString);
    bool getEOMuxOut(quint8& out, QString& errorString);

signals:
    void updateSimulation();
    void stopSimulation();
    void simulationFinished();
    void appendMicrocodeLine(QString line);
    void readByte(int address);
    void writeByte(int address);

};

#endif // CPUPANETWOBYTEDATABUS_H
