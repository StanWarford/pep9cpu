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
    Implement post conditions
    Implement setting of memory registers from preconditions
    Split this file into CPUDataSection and CPUControlSection
    Implement jumps instead of increment in control section
    Comment The data section

THEN
    Remove all CPUPane stepping code
    Remove all state code from Sim
    Fix format-from microcode
    Have CPU pane listen to data section, and pass on value changes to CPUGraphicsItems

 */
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
    quint8 getRegisterBankByte(quint8 registerNumber) const;
    //Will return the even/odd pair ass
    quint16 getRegisterBankWord(quint8 registerNumber) const;
    bool valueOnABus(quint8& result) const;
    bool valueOnBBus(quint8& result) const;
    bool valueOnCBus(quint8& result) const;
    Enu::MainBusState getMainBusState() const;
    quint8 getMemoryByte(quint16 address) const;
    quint16 getMemoryWord(quint16 address) const; //Uses the same even / odd conventions as Pep9
    const QVector<quint8> getMemory() const;
    quint8 getControlSignals(Enu::EControlSignals controlSignal) const;
    bool getClockSignals(Enu::EClockSignals) const;
    bool getStatusBit(Enu::EStatusBit) const;
    MicroCode* getMicrocodeFromSignals() const;

    /*
     *  Modify CPU state
     */
    //Internally, all set...() methods will call set...Pre() code, but will emit events afterwards
    void setStatusBitPre(Enu::EStatusBit,bool val);
    void setMemoryBytePre(quint16 address,quint8 val);
    void setMemoryWordPre(quint16 address,quint16 val);
    void setRegisterBytePre(quint8 reg,quint8 val);
    void setRegisterWordPre(quint8 reg,quint16 val);
    bool setSignalsFromMicrocode(const MicroCode* line);
    inline bool hadErrorOnStep();
    inline QString getErrorMessage();
private:
    CPUDataSection(QObject* parent=0);
    static CPUDataSection* _instance;
    Enu::CPUType cpuFeatures;
    Enu::MainBusState mainBusState;
    //Data registers
    QVector<quint8> memoryRegisters;
    QVector<quint8> registerBank;
    QVector<quint8> memory;
    quint8 NZVCSbits;
    //Control Signals
    QVector<quint8> controlSignals;
    QVector<bool> clockSignals;
    //Error handling
    bool hadDataError=false;
    QString errorMessage="";

    bool aluFnIsUnary() const;
    bool getAMuxOutput(quint8 &result) const;
    bool calculateCSMuxOutput(bool& result) const;
    bool calculatALUOutput(quint8& res,quint8 &NZVC) const;
    inline void setMemoryRegister(Enu::EMemoryRegisters,quint8 value);
    inline void setRegisterByte(quint8 register,quint8 value);
    inline void setMemoryByte(quint16 address, quint8 value);
    inline void setMemoryWord(quint16 address, quint16 value);
    inline void setStatusBit(Enu::EStatusBit,bool val);

    void handleMainBusState() noexcept;
    void stepOneByte() noexcept;
    void stepTwoByte() noexcept;

    void presetStaticRegisters() noexcept;
    void clearControlSignals() noexcept;
    void clearClockSignals() noexcept;
    void clearRegisters() noexcept;
    void clearMemory() noexcept;
    void clearErrors() noexcept;
public slots:
    void onStep() noexcept;
    void onClock() noexcept;
    void onClearCPU()noexcept;
    void onClearMemory() noexcept;
    void onCPUFeaturesChanged(Enu::CPUType newFeatures) throw(Enu::InvalidCPUMode);
signals:
    void CPUFeaturesChanged(Enu::CPUType newFeatures);
    void registerChanged(quint8 register,quint8 oldVal,quint8 newVal);
    void memoryRegisterChanged(Enu::EMemoryRegisters,quint8 oldVal,quint8 newVal);
    void controlSignalChanged(quint32 ControlSignals);
    void clockSignalsChanged(quint16 clockSignals) ;
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
