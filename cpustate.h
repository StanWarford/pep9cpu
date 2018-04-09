#ifndef CPUSTATE_H
#define CPUSTATE_H

#include <QObject>
#include <QVector>
#include <QException>
#include <QString>
#include "enu.h"
#include "code.h"
#include "microcodeprogram.h"
/*
Still left to do:
    Implement setting of memory registers from preconditions
    Split this file into CPUDataSection and CPUControlSection
    Implement jumps instead of increment in control section
    Comment the data section
    Fix VC bits of data section

THEN
    Remove all CPUPane stepping code
    Remove all state code from Sim
    Fix format-from microcode
    Have CPU pane listen to data section, and pass on value changes to CPUGraphicsItems

 */

/*
 * Class that feeds microcode lines to the CPUDataSection
 * To trigger execution, a onSumulationStarted/onDebuggingStarted is called.
 * This will typically be done by some button on the main window.
 */
class CPUControlSection: public QObject
{
    Q_OBJECT
public:
    static CPUControlSection* getInstance();
    virtual ~CPUControlSection();
    void initCPUStateFromPreconditions();
    bool testPost();
    void setMicrocodeProgram(MicrocodeProgram* program);
    bool hadErrorOnStep();
    QString getError();
public slots:
    void onSimulationStarted();
    void onSimulationFinished();
    void onDebuggingStarted();
    void onDebuggingFinished();
    void onStep(quint8 mode) noexcept;
    void onClock() noexcept;
    void onRun() noexcept;
    void onClearCPU() noexcept; //This event is propogated to the DataSection
    void onClearMemory() noexcept; //This event is propogated to the DataSection
signals:
    void simulationStarted();
    void simulationStepped();
    void simulationFinished();
private:
    static CPUControlSection *_instance;
    CPUDataSection* data;
    MicrocodeProgram* program;
    int microprogramCounter;
    bool inSimulation,hadControlError;
    QString errorMessage;
    CPUControlSection(CPUDataSection* dataSection);
};

class CPUTester: public QObject
{
    Q_OBJECT
public:
    static CPUTester* getInstance();
    virtual ~CPUTester();
private:
    static CPUTester* _instance;
    CPUTester(CPUControlSection *control, CPUDataSection *data);
    CPUControlSection* control;
    CPUDataSection* data;
};

#endif // CPUSTATE_H
