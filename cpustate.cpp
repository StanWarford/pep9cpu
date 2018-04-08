#include "cpustate.h"
CPUDataSection* CPUDataSection::_instance = nullptr;
CPUControlSection *CPUControlSection::_instance = nullptr;
CPUTester *CPUTester::_instance = nullptr;
CPUDataSection::CPUDataSection(QObject *parent): QObject(parent)
{

}

CPUDataSection* CPUDataSection::getInstance()
{
    if(CPUDataSection::_instance==nullptr)
    {
        CPUDataSection::_instance = new CPUDataSection();
    }
    return CPUDataSection::_instance;
}

CPUDataSection::~CPUDataSection()
{

}

quint8 CPUDataSection::getRegisterByte(quint8 registerNumber) const
{
    if(registerNumber>Enu::maxRegisterNumber) return 0;
    else return memory[registerNumber];

}

quint16 CPUDataSection::getRegisterWord(quint8 registerNumber) const
{
    quint16 returnValue;
    registerNumber= registerNumber-registerNumber%2;
    if(registerNumber>Enu::maxRegisterNumber) returnValue=0;
    else
    {
        returnValue = ((quint16)memory[registerNumber])<<16;
        returnValue+=memory[registerNumber+1];
    }
    return returnValue;

}

MicroCode* CPUDataSection::getMicrocodeFromSignals() const
{
#pragma message "todo"
    return nullptr;
}

bool CPUDataSection::setSignalsFromMicrocode(const MicroCode *line)
{
    #pragma message "todo"
    return true;
}

bool CPUDataSection::hadErrorOnStep()
{
    return hadDataError;
}

void CPUDataSection::handleMainBusState() noexcept
{

}

void CPUDataSection::stepOneByte() noexcept
{
    //TODO
}

void CPUDataSection::stepTwoByte() noexcept
{
    //TODO
}

void CPUDataSection::clearControlSignals() noexcept
{
    for(int it=0; it<controlSignals.length();it++)
    {
        controlSignals[it]=Enu::signalDisabled;
    }
}

void CPUDataSection::clearClockSignals() noexcept
{
    for(int it=0;it<clockSignals.length();it++)
    {
        clockSignals[it]=false;
    }
}

void CPUDataSection::clearRegisters() noexcept
{
    for(int it=0;it<registers.length();it++)
    {
        registers[it]=0;
    }
}

void CPUDataSection::clearMemory() noexcept
{
    for(int it=0;it<memory.length();it++)
    {
        memory[it]=0;
    }
}

void CPUDataSection::clearErrors() noexcept
{
    hadDataError=false;
    errorMessage="";
}

void CPUDataSection::onStep() noexcept
{
    if(cpuFeatures == Enu::OneByteDataBus) stepOneByte();
    else if(cpuFeatures == Enu::TwoByteDataBus) stepTwoByte();
}

void CPUDataSection::onClock() noexcept
{
    onStep();
    clearClockSignals();
    clearControlSignals();
}

void CPUDataSection::onClearCPU() noexcept
{
    mainBusState = Enu::None;
    clearErrors();
    clearRegisters();
    clearClockSignals();
    clearControlSignals();
    clearMemory();
}

void CPUDataSection::onCPUFeaturesChanged(Enu::CPUType newFeatures) throw(Enu::InvalidCPUMode)
{
    if(!(newFeatures!=Enu::OneByteDataBus)&&!(newFeatures!=Enu::TwoByteDataBus))
    {
        throw Enu::InvalidCPUMode();
    }
    cpuFeatures=newFeatures;
    onClearCPU();
    emit CPUFeaturesChanged(newFeatures);
}

CPUControlSection *CPUControlSection::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new CPUControlSection(CPUDataSection::getInstance());
    }
    return _instance;
}

CPUControlSection::~CPUControlSection()
{

}

bool CPUControlSection::hadErrorOnStep()
{
    return hadControlError || data->hadErrorOnStep();
}

void CPUControlSection::onSimulationStarted()
{

}

void CPUControlSection::onSimulationFinished()
{

}

void CPUControlSection::onDebuggingStarted()
{

}

void CPUControlSection::onDebuggingFinished()
{

}

void CPUControlSection::onStep(quint8 mode) noexcept
{
    //Do step logic
    const MicroCode* prog = program->getCodeLine(microprogramCounter);
    data->setSignalsFromMicrocode(prog);
    microprogramCounter = microprogramCounter++; //prog->branchAddress();
    data->onStep();
}

void CPUControlSection::onClock()noexcept
{
    //Do clock logic
    if(!inSimulation)
    {
        data->onClock();
    }
    else
    {
        //One should not get here, otherwise that would mean that we clocked in a simulation
    }
}

void CPUControlSection::onRun()noexcept
{
    const MicroCode* prog = program->getCodeLine(microprogramCounter);
    while(true /*prog->branchFunction!=Enu::Stop*/)
    {
        /*
         * Handle address decoding of next instruction
         */
        //
        onStep(-1);
        //If there was a logical error on data operation
        if(data->hadErrorOnStep())
        {
            //Pass up the error
            break;
        }
        //If there was an error on the control flow
        else if(this->hadErrorOnStep())
        {
            break;
        }
        prog = program->getCodeLine(microprogramCounter);

    }
}

void CPUControlSection::onClearCPU()noexcept
{
    data->onClearCPU();
    inSimulation=false;
    microprogramCounter=0;
    hadControlError=false;
    errorMessage="";
}

CPUControlSection::CPUControlSection(CPUDataSection * data): QObject(nullptr),data(data),microprogramCounter(0)
{

}

CPUTester *CPUTester::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new CPUTester(CPUControlSection::getInstance(),CPUDataSection::getInstance());
    }
    return _instance;
}

CPUTester::CPUTester(CPUControlSection *control, CPUDataSection *data): QObject(nullptr),control(control),data(data)
{

}

CPUTester::~CPUTester()
{

}
