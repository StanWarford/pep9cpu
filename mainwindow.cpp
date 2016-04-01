// File: mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialize the read-only registers to the correct values
    Sim::initMRegs();

    mainMemory = new MainMemory(ui->mainSplitter);
    delete ui->memoryFrame;
    microcodePane = new MicrocodePane(ui->codeSplitter);
    delete ui->microcodeFrame;
    objectCodePane = new ObjectCodePane(ui->codeSplitter);
    delete ui->objectCodeFrame;
    cpuPaneOneByteDataBus = new CpuPane(Enu::OneByteDataBus, ui->mainSplitter);
    ui->mainSplitter->insertWidget(1, cpuPaneOneByteDataBus);
    cpuPane = cpuPaneOneByteDataBus;

    cpuPaneTwoByteDataBus = new CpuPane(Enu::TwoByteDataBus, ui->mainSplitter);
    cpuPaneTwoByteDataBus->hide();
    ui->mainSplitter->insertWidget(1, cpuPaneTwoByteDataBus);

    delete ui->cpuFrame;

    QList<int> list;
    list.append(3000);
    list.append(1);
    ui->codeSplitter->setSizes(list);

    ui->mainSplitter->insertWidget(0, mainMemory);
    ui->mainSplitter->insertWidget(1, cpuPane);

    helpDialog = new HelpDialog(this);
    aboutPepDialog = new AboutPep(this);

    // Byte converter setup
    byteConverterDec = new ByteConverterDec(this);
    ui->byteConverterToolBar->addWidget(byteConverterDec);
    byteConverterHex = new ByteConverterHex(this);
    ui->byteConverterToolBar->addWidget(byteConverterHex);
    byteConverterBin = new ByteConverterBin(this);
    ui->byteConverterToolBar->addWidget(byteConverterBin);
    byteConverterChar = new ByteConverterChar(this);
    ui->byteConverterToolBar->addWidget(byteConverterChar);
    ui->byteConverterToolBar->setWindowTitle("Byte Converter");
    connect(byteConverterDec, SIGNAL(textEdited(const QString &)), this, SLOT(slotByteConverterDecEdited(const QString &)));
    connect(byteConverterHex, SIGNAL(textEdited(const QString &)), this, SLOT(slotByteConverterHexEdited(const QString &)));
    connect(byteConverterBin, SIGNAL(textEdited(const QString &)), this, SLOT(slotByteConverterBinEdited(const QString &)));
    connect(byteConverterChar, SIGNAL(textEdited(const QString &)), this, SLOT(slotByteConverterCharEdited(const QString &)));

    connect(helpDialog, SIGNAL(copyToMicrocodeClicked()), this, SLOT(helpCopyToMicrocodeButtonClicked()));

    connect(qApp->instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(focusChanged(QWidget*, QWidget*)));
    connect(microcodePane, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoability(bool)));
    connect(microcodePane, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoability(bool)));

    connect(cpuPane, SIGNAL(updateSimulation()), this, SLOT(updateSimulation()));
    connect(cpuPane, SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));
    connect(cpuPane, SIGNAL(stopSimulation()), this, SLOT(stopSimulation()));
    connect(cpuPane, SIGNAL(writeByte(int)), this, SLOT(updateMemAddress(int)));

    //Pep::initEnumMnemonMaps();

    readSettings();

    qApp->installEventFilter(this);

    connect(cpuPane, SIGNAL(appendMicrocodeLine(QString)), this, SLOT(appendMicrocodeLine(QString)));

    on_actionOne_Byte_Data_Bus_Model_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// Protected closeEvent
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)) {
            if (cpuPane->hasFocus()) {
                // single step or clock, depending
                if (ui->actionSystem_Stop_Debugging->isEnabled()) {
                    // single step
                    cpuPane->singleStep();
                }
                else {
                    // clock
                    cpuPane->clock();
                }
                return true;
            }
            else if (ui->actionSystem_Stop_Debugging->isEnabled() &&
                     (microcodePane->hasFocus() || objectCodePane->hasFocus())) {
                cpuPane->giveFocus();
            }
        }
    }
    else if (event->type() == QEvent::FileOpen) {
        if (ui->actionSystem_Stop_Debugging->isEnabled()) {
            ui->statusBar->showMessage("Open failed, currently debugging.", 4000);
            return false;
        }
        loadFile(static_cast<QFileOpenEvent *>(event)->file());
        return true;
    }
    return false;
}

void MainWindow::readSettings()
{
    QSettings settings("Pep9CPU", "MainWindow");
    QDesktopWidget *desktop = QApplication::desktop();
    int width = static_cast<int>(desktop->width() * 0.80);
    int height = static_cast<int>(desktop->height() * 0.70);
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    QPoint pos = settings.value("pos", QPoint((screenWidth - width) / 2, (screenHeight - height) / 2)).toPoint();
    QSize size = settings.value("size", QSize(width, height)).toSize();
    if (Pep::getSystem() == "Mac") {
        pos.setY(pos.y() + 20); // Every time the app launches, it seems OSX moves the window 20 pixels up the screen, so we compensate here.
    }
    else if (Pep::getSystem() == "Linux") { // Linux has a similar issue, so compensate here.
        pos.setY(pos.y() - 20);
    }
    if (pos.x() > width || pos.x() < 0 || pos.y() > height || pos.y() < 0) {
        pos = QPoint(0, 0);
    }
    resize(size);
    move(pos);
    curPath = settings.value("filePath", QDir::homePath()).toString();
}

void MainWindow::writeSettings()
{
    QSettings settings("Pep9CPU", "MainWindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("filePath", curPath);
}

// Save methods
bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return on_actionFile_Save_As_triggered();
    }
    else {
        return saveFile(curFile);
    }
}

bool MainWindow::maybeSave()
{
    if (microcodePane->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "Pep/9 CPU",
                                   "The microcode has been modified.\n"
                                   "Do you want to save your changes?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Pep/9 CPU"), tr("Cannot read file %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("ISO 8859-1"));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // Set source code pane text
    microcodePane->setMicrocode(in.readAll());
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 4000);
    QApplication::restoreOverrideCursor();
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Pep/9 CPU"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("ISO 8859-1"));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << Pep::addCycleNumbers(microcodePane->getMicrocode());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage("Microcode saved", 4000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    microcodePane->setFilename(QFileInfo(fileName).fileName());
    curFile = fileName;
    microcodePane->setModifiedFalse();

    if (!fileName.isEmpty()) {
        curPath = QFileInfo(fileName).path();
    }
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// File MainWindow triggers
void MainWindow::on_actionFile_New_triggered()
{
    if (maybeSave()) {
        microcodePane->setMicrocode("");
        objectCodePane->setObjectCode("");
        setCurrentFile("");
    }
}

void MainWindow::on_actionFile_Open_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(
                    this,
                    "Open text file",
                    curPath,
                    "Text files (*.pepcpu *.txt)");
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            curPath = QFileInfo(fileName).path();
        }
    }
}

bool MainWindow::on_actionFile_Save_triggered()
{
    if (curFile.isEmpty()) {
        return on_actionFile_Save_As_triggered();
    }
    else {
        return saveFile(curFile);
    }
}

bool MainWindow::on_actionFile_Save_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Save Microcode",
                curFile.isEmpty() ? curPath + "/untitled.pepcpu" : curPath + "/" + strippedName(curFile),
                "Pep9 Source (*.pepcpu *.txt)");
    if (fileName.isEmpty()) {
        return false;
    }
    return saveFile(fileName);
}

// Edit MainWindow triggers
void MainWindow::on_actionEdit_Undo_triggered()
{
    if (microcodePane->hasFocus() && ui->actionSystem_Start_Debugging->isEnabled()) {
        microcodePane->undo();
    }
    // other panes should not be able to undo
}

void MainWindow::on_actionEdit_Redo_triggered()
{
    if (microcodePane->hasFocus() && ui->actionSystem_Start_Debugging->isEnabled()) {
        microcodePane->redo();
    }
    // other panes should not be able to redo
}

void MainWindow::on_actionEdit_Cut_triggered()
{
    if (microcodePane->hasFocus() && ui->actionSystem_Start_Debugging->isEnabled()) {
        microcodePane->cut();
    }
    // other panes should not be able to cut
}

void MainWindow::on_actionEdit_Copy_triggered()
{
    if (microcodePane->hasFocus()) {
        microcodePane->copy();
    }
    else if (objectCodePane->hasFocus()) {
        objectCodePane->copy();
    }
    else if (helpDialog->hasFocus()) {
        helpDialog->copy();
    }
    // other panes should not be able to copy
}

void MainWindow::on_actionEdit_Paste_triggered()
{
    if (microcodePane->hasFocus() && ui->actionSystem_Start_Debugging->isEnabled()) {
        microcodePane->paste();
    }
    // other panes should not be able to paste
}

void MainWindow::on_actionEdit_UnComment_Line_triggered()
{
    if (!ui->actionSystem_Stop_Debugging->isEnabled()) { // we are not debugging
        microcodePane->unCommentSelection();
    }
}

void MainWindow::on_actionEdit_Auto_Format_Microcode_triggered()
{
#pragma message("todo: fix bug with formatting from previous run")
    Sim::setMicrocodeSourceList();
    if (!Sim::microcodeSourceList.isEmpty()) {
        microcodePane->setMicrocode(Sim::microcodeSourceList.join("\n"));
    }
}

void MainWindow::on_actionEdit_Remove_Error_Messages_triggered()
{
    microcodePane->removeErrorMessages();
}

void MainWindow::on_actionEdit_Font_triggered()
{
    microcodePane->setFont();
}

// System MainWindow triggers
void MainWindow::on_actionSystem_Run_triggered()
{
    if (on_actionSystem_Start_Debugging_triggered()) {
        cpuPane->run();
    }
}

bool MainWindow::on_actionSystem_Start_Debugging_triggered()
{
    Sim::cycleCount = 0; // this stores the number of cycles in a simulation, reset before assembling
    if (microcodePane->microAssemble()) {
        ui->statusBar->showMessage("MicroAssembly succeeded", 4000);
        objectCodePane->setObjectCode(microcodePane->codeToString());
        bool hasUnitPre = false;
        for (int i = 0; i < Sim::codeList.size(); i++) {
            hasUnitPre = hasUnitPre || Sim::codeList.at(i)->hasUnitPre();
        }
        // setup preconditions
        if (hasUnitPre) {
            mainMemory->clearMemory();
            cpuPane->clearCpu();
            for (int i = 0; i < Sim::codeList.size(); i++) {
                Sim::codeList.at(i)->setUnitPre(mainMemory, cpuPane);
            }
        }
    }
    else {
        ui->statusBar->showMessage("MicroAssembly failed", 4000);
        return false;
    }

    // prevent simulation from starting if there's nothing to simulate
    bool hasMicrocode = false;
    for (int i = 0; i < Sim::codeList.size(); i++) {
        if (Sim::codeList.at(i)->isMicrocode()) {
            hasMicrocode = true;
        }
    }
    if (!hasMicrocode) {
        return false;
    }

    // enable the actions available while we're debugging
    ui->actionSystem_Stop_Debugging->setEnabled(true);

    // disable actions related to editing/starting debugging
    ui->actionSystem_Run->setEnabled(false);
    ui->actionSystem_Start_Debugging->setEnabled(false);
    microcodePane->setReadOnly(true);

    cpuPane->startDebugging();
    return true;
}

void MainWindow::on_actionSystem_Stop_Debugging_triggered()
{
    microcodePane->clearSimulationView();
    objectCodePane->clearSimulationView();

    // disable the actions available while we're debugging
    ui->actionSystem_Stop_Debugging->setEnabled(false);

    // enable actions related to editing/starting debugging
    ui->actionSystem_Run->setEnabled(true);
    ui->actionSystem_Start_Debugging->setEnabled(true);
    microcodePane->setReadOnly(false);

    cpuPane->stopDebugging();
}

void MainWindow::on_actionSystem_Clear_CPU_triggered()
{
    cpuPane->clearCpu();
}

void MainWindow::on_actionSystem_Clear_Memory_triggered()
{
    mainMemory->clearMemory();
}

void MainWindow::on_actionOne_Byte_Data_Bus_Model_triggered()
{
    Pep::cpuFeatures = Enu::OneByteDataBus;
    Pep::initEnumMnemonMaps();
    Sim::initMRegs();
    Sim::clearMemory();
    Sim::initNZVCS();
    Sim::initCPUState();

    objectCodePane->clearSimulationView();
    mainMemory->clearMemory();

    disconnect(cpuPane, SIGNAL(updateSimulation()), this, SLOT(updateSimulation()));
    disconnect(cpuPane, SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));
    disconnect(cpuPane, SIGNAL(stopSimulation()), this, SLOT(stopSimulation()));
    disconnect(cpuPane, SIGNAL(writeByte(int)), this, SLOT(updateMemAddress(int)));
    disconnect(cpuPane, SIGNAL(appendMicrocodeLine(QString)), this, SLOT(appendMicrocodeLine(QString)));

    cpuPane->hide();
    cpuPane = cpuPaneOneByteDataBus;
    cpuPane->show();

    connect(cpuPane, SIGNAL(updateSimulation()), this, SLOT(updateSimulation()));
    connect(cpuPane, SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));
    connect(cpuPane, SIGNAL(stopSimulation()), this, SLOT(stopSimulation()));
    connect(cpuPane, SIGNAL(writeByte(int)), this, SLOT(updateMemAddress(int)));
    connect(cpuPane, SIGNAL(appendMicrocodeLine(QString)), this, SLOT(appendMicrocodeLine(QString)));

    cpuPane->clearCpu();
    cpuPane->clearCpuControlSignals();

    ui->actionTwo_Byte_Data_Bus_Model->setText("Switch to Two-byte Data Bus");
    ui->actionTwo_Byte_Data_Bus_Model->setEnabled(true);
    ui->actionOne_Byte_Data_Bus_Model->setText("One-byte Data Bus");
    ui->actionOne_Byte_Data_Bus_Model->setEnabled(false);

}

void MainWindow::on_actionTwo_Byte_Data_Bus_Model_triggered()
{
    Pep::cpuFeatures = Enu::TwoByteDataBus;
    Pep::initEnumMnemonMaps();
    Sim::initMRegs();
    Sim::clearMemory();
    Sim::initNZVCS();
    Sim::initCPUState();

    objectCodePane->clearSimulationView();
    mainMemory->clearMemory();

    disconnect(cpuPane, SIGNAL(updateSimulation()), this, SLOT(updateSimulation()));
    disconnect(cpuPane, SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));
    disconnect(cpuPane, SIGNAL(stopSimulation()), this, SLOT(stopSimulation()));
    disconnect(cpuPane, SIGNAL(writeByte(int)), this, SLOT(updateMemAddress(int)));
    disconnect(cpuPane, SIGNAL(appendMicrocodeLine(QString)), this, SLOT(appendMicrocodeLine(QString)));

    cpuPane->hide();
    cpuPane = cpuPaneTwoByteDataBus;
    cpuPane->show();

    connect(cpuPane, SIGNAL(updateSimulation()), this, SLOT(updateSimulation()));
    connect(cpuPane, SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));
    connect(cpuPane, SIGNAL(stopSimulation()), this, SLOT(stopSimulation()));
    connect(cpuPane, SIGNAL(writeByte(int)), this, SLOT(updateMemAddress(int)));
    connect(cpuPane, SIGNAL(appendMicrocodeLine(QString)), this, SLOT(appendMicrocodeLine(QString)));

    cpuPane->clearCpu();
    cpuPane->clearCpuControlSignals();

    ui->actionTwo_Byte_Data_Bus_Model->setText("Two-byte Data Bus");
    ui->actionTwo_Byte_Data_Bus_Model->setEnabled(false);
    ui->actionOne_Byte_Data_Bus_Model->setText("Switch to One-byte Data Bus");
    ui->actionOne_Byte_Data_Bus_Model->setEnabled(true);

}


// help:
void MainWindow::on_actionHelp_UsingPep9CPU_triggered()
{
    helpDialog->show();
    helpDialog->selectItem("Using Pep/9 CPU");
}

void MainWindow::on_actionHelp_InteractiveUse_triggered()
{
    helpDialog->show();
    helpDialog->selectItem("Interactive Use");
}

void MainWindow::on_actionHelp_MicrocodeUse_triggered()
{
    helpDialog->show();
    helpDialog->selectItem("Microcode Use");
}

void MainWindow::on_actionHelp_DebuggingUse_triggered()
{
    helpDialog->show();
    helpDialog->selectItem("Debugging Use");
}

void MainWindow::on_actionHelp_Pep9Reference_triggered()
{
    helpDialog->show();
    helpDialog->selectItem("Pep/9 Reference");
}

void MainWindow::on_actionHelp_Examples_triggered()
{
    helpDialog->show();
    helpDialog->selectItem("Examples");
}

void MainWindow::on_actionHelp_triggered()
{
    if (!helpDialog->isHidden()) {
        // give it focus again:
        helpDialog->hide();
        helpDialog->show();
    }
    else {
        helpDialog->show();
    }
}

void MainWindow::on_actionHelp_About_Pep9CPU_triggered()
{
    aboutPepDialog->show();
}

void MainWindow::on_actionHelp_About_Qt_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.qt.io/"));
}

// Byte Converter slots
void MainWindow::slotByteConverterDecEdited(const QString &str)
{
    if (str.length() > 0) {
        bool ok;
        int data = str.toInt(&ok, 10);
        byteConverterHex->setValue(data);
        byteConverterBin->setValue(data);
        byteConverterChar->setValue(data);
    }
}

void MainWindow::slotByteConverterHexEdited(const QString &str)
{
    if (str.length() >= 2) {
        if (str.startsWith("0x")) {
            QString hexPart = str;
            hexPart.remove(0, 2);
            if (hexPart.length() > 0) {
                bool ok;
                int data = hexPart.toInt(&ok, 16);
                byteConverterDec->setValue(data);
                byteConverterBin->setValue(data);
                byteConverterChar->setValue(data);
            }
            else {
                // Exactly "0x" remains, so do nothing
            }
        }
        else {
            // Prefix "0x" was mangled
            byteConverterHex->setValue(-1);
        }
    }
    else {
        // Prefix "0x" was shortened
        byteConverterHex->setValue(-1);
    }
}

void MainWindow::slotByteConverterBinEdited(const QString &str)
{
    if (str.length() > 0) {
        bool ok;
        int data = str.toInt(&ok, 2);
        byteConverterDec->setValue(data);
        byteConverterHex->setValue(data);
        byteConverterChar->setValue(data);
    }
}

void MainWindow::slotByteConverterCharEdited(const QString &str)
{
    if (str.length() > 0) {
        int data = (int)str[0].toLatin1();
        byteConverterDec->setValue(data);
        byteConverterHex->setValue(data);
        byteConverterBin->setValue(data);
    }
}

// Focus Coloring. Activates and deactivates undo/redo/cut/copy/paste actions contextually
void MainWindow::focusChanged(QWidget *, QWidget *)
{
    microcodePane->highlightOnFocus();
    mainMemory->highlightOnFocus();
    objectCodePane->highlightOnFocus();
    cpuPane->highlightOnFocus();

    if (microcodePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(!microcodePane->isUndoable());
        ui->actionEdit_Redo->setDisabled(!microcodePane->isRedoable());
        ui->actionEdit_Cut->setDisabled(false);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(false);
    }
    else if (mainMemory->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(true);
        ui->actionEdit_Paste->setDisabled(true);
    }
    else if (cpuPane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(true);
        ui->actionEdit_Paste->setDisabled(true);
    }
    else if (objectCodePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(true);
        ui->actionEdit_Redo->setDisabled(true);
        ui->actionEdit_Cut->setDisabled(true);
        ui->actionEdit_Copy->setDisabled(false);
        ui->actionEdit_Paste->setDisabled(true);
    }
}

void MainWindow::setUndoability(bool b)
{
    if (microcodePane->hasFocus()) {
        ui->actionEdit_Undo->setDisabled(!b);
    }
    else if (mainMemory->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(true);
    }
    else if (cpuPane->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(true);
    }
    else if (objectCodePane->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(true);
    }
}

void MainWindow::setRedoability(bool b)
{
    if (microcodePane->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(!b);
    }
    else if (mainMemory->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(true);
    }
    else if (cpuPane->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(true);
    }
    else if (objectCodePane->hasFocus()) {
        ui->actionEdit_Redo->setDisabled(true);
    }
}

void MainWindow::updateSimulation()
{
    microcodePane->updateSimulationView();
    objectCodePane->highlightCurrentInstruction();
}

void MainWindow::stopSimulation()
{
    on_actionSystem_Stop_Debugging_triggered();
}

void MainWindow::simulationFinished()
{
    QString errorString;

    on_actionSystem_Stop_Debugging_triggered();

    for (int i = 0; i < Sim::codeList.size(); i++) {
        if (!Sim::codeList.at(i)->testPostcondition(mainMemory, cpuPane, errorString)) {
            microcodePane->appendMessageInSourceCodePaneAt(-1, errorString);
            QMessageBox::warning(this, "Pep9CPU", "Failed unit test");
            return;
        }
    }
    // feature, not a bug: we will display the "passed unit test" even
    // on the empty case - no postconditions

    ui->statusBar->showMessage("Passed unit test", 4000);
}

void MainWindow::appendMicrocodeLine(QString line)
{
    microcodePane->appendMessageInSourceCodePaneAt(-2, line);
}

void MainWindow::helpCopyToMicrocodeButtonClicked()
{
    if (maybeSave()) {
        if (helpDialog->getExamplesModel() == Enu::OneByteDataBus) {
            on_actionOne_Byte_Data_Bus_Model_triggered();
        }
        else if (helpDialog->getExamplesModel() == Enu::TwoByteDataBus) {
            on_actionTwo_Byte_Data_Bus_Model_triggered();
        }

        microcodePane->setMicrocode(helpDialog->getExampleText());
        objectCodePane->setObjectCode("");
        helpDialog->hide();
        statusBar()->showMessage("Copied to microcode", 4000);
    }
}

void MainWindow::updateMemAddress(int address)
{
    mainMemory->setMemAddress(address, Sim::readByte(address));
    mainMemory->showMemEdited(address);
}






