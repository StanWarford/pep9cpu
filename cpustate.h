#ifndef CPUSTATE_H
#define CPUSTATE_H

#include <QObject>
#include <QVector>
#include <QException>
#include <QString>
#include "enu.h"
#include "code.h"
#include "microcodeprogram.h"
class CPUDataSection: public QObject
{
    Q_OBJECT
    friend class CPUControlSection;
public:
    static CPUDataSection* getInstance();
    virtual ~CPUDataSection();

    /*
     *  Access CPU State
     */
    quint8 getRegisterByte(quint8 registerNumber) const;
    //Will return the even/odd pair ass
    quint16 getRegisterWord(quint8 registerNumber) const;
    quint8 getMemoryByte(quint16 address) const;
    //Uses the same even / odd conventions as Pep9
    quint16 getMemoryWord(quint16 address) const;
    const QVector<quint8> getMemory() const;
    quint8 getControlSignals(Enu::EControlSignals controlSignal) const;
    bool getClockSignals(Enu::EClockSignals) const;
    MicroCode* getMicrocodeFromSignals() const;
    /*
     *  Modify CPU state
     */
     bool setSignalsFromMicrocode(const MicroCode* line);
     inline bool hadErrorOnStep();
     inline QString getErrorMessage();
private:
    CPUDataSection(QObject* parent=0);
    static CPUDataSection* _instance;
    Enu::CPUType cpuFeatures;
    Enu::MainBusState mainBusState;
    //Control Signals and Data
    QVector<quint8> registers;
    QVector<quint8> memory;
    QVector<quint8> controlSignals;
    QVector<bool> clockSignals;
    //Error handling
    bool hadDataError=false;
    QString errorMessage="";

    void handleMainBusState() noexcept;
    void stepOneByte() noexcept;
    void stepTwoByte() noexcept;
    void clearControlSignals() noexcept;
    void clearClockSignals() noexcept;
    void clearRegisters() noexcept;
    void clearMemory() noexcept;
    void clearErrors() noexcept;
public slots:
    void onStep() noexcept;
    void onClock() noexcept;
    void onClearCPU()noexcept;
    void onCPUFeaturesChanged(Enu::CPUType newFeatures) throw(Enu::InvalidCPUMode);
signals:
    void CPUFeaturesChanged(Enu::CPUType newFeatures);
    void registerChanged(quint8 register,quint8 oldVal,quint8 newVal);
    void controlSignalChanged(quint64 ControlSignals);
    void ramChanged(quint16 address,quint8 oldVal, quint8 newVal);

};

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
    bool hadErrorOnStep();
    QString getError();
public slots:
    void onSimulationStarted();
    void onSimulationFinished();
    void onDebuggingStarted();
    void onDebuggingFinished();
    void onStep(quint8 mode)noexcept;
    void onClock()noexcept;
    void onRun()noexcept;
    void onClearCPU() noexcept; //This event is propogated to the DataSection
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
    void initCPUStateFromPreconditions();
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
