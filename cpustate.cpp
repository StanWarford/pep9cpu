#include "cpustate.h"
#include "cpudatasection.h"
CPUControlSection *CPUControlSection::_instance = nullptr;
CPUTester *CPUTester::_instance = nullptr;


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
    //This object should last the lifetime of the  program, it does not need to be cleaned up
}

void CPUControlSection::setMicrocodeProgram(MicrocodeProgram *program)
{
    this->program=program;
    microprogramCounter=0;
}

bool CPUControlSection::hadErrorOnStep()
{
    return hadControlError || data->hadErrorOnStep();
}

void CPUControlSection::onSimulationStarted()
{
#pragma message "todo"
}

void CPUControlSection::onSimulationFinished()
{
#pragma message "todo"
}

void CPUControlSection::onDebuggingStarted()
{
#pragma message "todo"
}

void CPUControlSection::onDebuggingFinished()
{
#pragma message "todo"
}

void CPUControlSection::onStep(quint8 mode) noexcept
{
    //Do step logic
    const MicroCode* prog = program->getCodeLine(microprogramCounter);
    data->setSignalsFromMicrocode(prog);
    microprogramCounter++; //prog->branchAddress();
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
        if(prog==nullptr)
        {
            return;
        }
        //
        onStep(-1);
        //If there was a logical error on data operation
        if(data->hadErrorOnStep())
        {
            //Pass up the error
            qDebug()<<"The data section died";
            qDebug()<<data->errorMessage;
            break;
        }
        //If there was an error on the control flow
        else if(this->hadErrorOnStep())
        {
            qDebug()<<"The control section died";
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

void CPUControlSection::onClearMemory() noexcept
{
    data->onClearMemory();
}

CPUControlSection::CPUControlSection(CPUDataSection * data): QObject(nullptr),data(data),microprogramCounter(0)
{

}

void CPUControlSection::initCPUStateFromPreconditions()
{
    onClearCPU();
    QList<UnitPreCode*> preCode;
    microprogramCounter=0;
    if(program == nullptr)
    {
        qDebug()<<"Can't init from null program";
    }
    for(Code* x : program->getObjectCode())
    {
        if(x->hasUnitPre())preCode.append((UnitPreCode*)x);
    }
    for(auto x : preCode)
    {
        x->setUnitPre(data);
    }
    //Handle any control section logic
    //None at the moment
    //Handle data section logic


}

bool CPUControlSection::testPost()
{
    QList<UnitPreCode*> preCode;
    if(program == nullptr)
    {
        qDebug()<<"Can't init from null program";
    }
    for(Code* x : program->getObjectCode())
    {
        if(x->hasUnitPost())preCode.append((UnitPreCode*)x);
    }
    QString err;
    bool t=false;
    for(auto x : preCode)
    {
       ((UnitPostCode*) x)->testPostcondition(data,err);
        if(err!="")t=true;
    }
    qDebug()<<"The postcondtions were:"<<!t;
    return !t;
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

