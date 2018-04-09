#ifndef CPUDATASECTION_H
#define CPUDATASECTION_H

#include <QObject>
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


    //Access CPU registers
    quint8 getRegisterBankByte(quint8 registerNumber) const;
    quint16 getRegisterBankWord(quint8 registerNumber) const; //Follows even/odd conventions of pep/9
    quint8 getMemoryRegister(Enu::EMemoryRegisters)const;

    //Access register & Memory Buses
    bool valueOnABus(quint8& result) const;
    bool valueOnBBus(quint8& result) const;
    bool valueOnCBus(quint8& result) const;
    Enu::MainBusState getMainBusState() const;

    //Fetch Values from Memory
    quint8 getMemoryByte(quint16 address) const;
    quint16 getMemoryWord(quint16 address) const; //Uses the same even / odd conventions as Pep9
    const QVector<quint8> getMemory() const; //Returns a shared copy of the memory vector

    //Test for Signals and Registers
    quint8 getControlSignals(Enu::EControlSignals controlSignal) const;
    bool getClockSignals(Enu::EClockSignals) const;
    bool getStatusBit(Enu::EStatusBit) const;

    MicroCode* getMicrocodeFromSignals() const; //Convert the currently enabled signals to a microcode line

    /*
     *  Preset CPU state. These are not meant to be called once the simulation has started.
     */
    //Internally, all set...() methods will call set...Pre() code, but will emit events afterwards
    void setStatusBitPre(Enu::EStatusBit,bool val);
    void setMemoryBytePre(quint16 address,quint8 val);
    void setMemoryWordPre(quint16 address,quint16 val);
    void setRegisterBytePre(quint8 reg,quint8 val);
    void setRegisterWordPre(quint8 reg,quint16 val);
    bool setSignalsFromMicrocode(const MicroCode* line);

    //Return information about errors on the last step
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

    //Set the values values of the sequential data registers (numbers 22-31)
    void presetStaticRegisters() noexcept;

    //Is the CPU function Unary?
    bool aluFnIsUnary() const;
    //Return true if AMux has output, and set result equal to the value of the output.
    //Works for one and two byte buses
    bool getAMuxOutput(quint8 &result) const;
    //Return  true if CSMux has an ouput, and set result equal to the output if present
    bool calculateCSMuxOutput(bool& result) const;
    //Return if the ALU has an ouput, and set result & NZVC bits according to the ALU function
    bool calculatALUOutput(quint8& result,quint8 &NZVC) const;

    inline void setMemoryRegister(Enu::EMemoryRegisters,quint8 value);
    inline void setRegisterByte(quint8 register,quint8 value);
    inline void setMemoryByte(quint16 address, quint8 value);
    inline void setMemoryWord(quint16 address, quint16 value);
    inline void setStatusBit(Enu::EStatusBit,bool val);

    void handleMainBusState() noexcept;
    void stepOneByte() noexcept;
    void stepTwoByte() noexcept;

    //Helper functions to clear different aspects of the data section
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

#endif // CPUDATASECTION_H
