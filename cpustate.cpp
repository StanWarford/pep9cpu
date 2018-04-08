#include "cpustate.h"
CPUDataSection* CPUDataSection::_instance = nullptr;
CPUControlSection *CPUControlSection::_instance = nullptr;
CPUTester *CPUTester::_instance = nullptr;
CPUDataSection::CPUDataSection(QObject *parent): QObject(parent),cpuFeatures(Enu::OneByteDataBus),mainBusState(Enu::None),
    memoryRegisters(6),registerBank(32),memory(0XFFFF),controlSignals(20),clockSignals(10),hadDataError(false),errorMessage("")
{
    presetStaticRegisters();
}

bool CPUDataSection::aluFnIsUnary() const
{
    return controlSignals[Enu::ALU]==0||controlSignals[Enu::ALU]>=10;
}

bool CPUDataSection::getAMuxOutput(quint8& result) const
{
    switch(cpuFeatures)
    {
    case Enu::OneByteDataBus:
        if(controlSignals[Enu::AMux]==0)
        {
            result = memoryRegisters[Enu::MEM_MDR];
            return true;
        }
        else if(controlSignals[Enu::AMux]==1)
        {
            return valueOnABus(result);
        }
        else return false;
    case Enu::TwoByteDataBus:
#pragma message "TODO: implement AMux for two byte bus"
        break;
    default:
        break;
    }
}

bool CPUDataSection::calculateCSMuxOutput(bool &result) const
{
    if(controlSignals[Enu::CSMux]==0)
    {
        result=NZVCSbits&Enu::CMask;
        return true;
    }
    else if(controlSignals[Enu::CSMux]==1)
    {
        result = NZVCSbits&Enu::SMask;
        return true;
    }
    else return false;
}

bool CPUDataSection::calculatALUOutput(quint8 &res, quint8 &NZVC) const
{
    quint8 a,b;
    bool carryIn;
    bool hasA=getAMuxOutput(a), hasB=valueOnBBus(b),hasCin=calculateCSMuxOutput(carryIn);
    if(!((aluFnIsUnary()&&hasA)||(hasA&&hasB)))
    {
#pragma message "TODO: Warning when ALU has bad inputs"
        return false;
    }
    switch(controlSignals[Enu::ALU])
    {
    case Enu::A_func: //A
        if(hasA) res=a;
        else return false;
        break;
    case Enu::ApB_func: //A plus B
        res=a+b;
        NZVC|= Enu::CMask*(((res &0x1ff)>>8) &0x1);
        NZVC|= Enu::NMask*((((a & 0x7f) + (b & 0x7f)) >> 7) & 0x1) ^ (NZVC&Enu::CMask);
        break;
    case Enu::ApnBp1_func: //A plus ~B plus 1
        carryIn=1;
        //Intentional fallthrough
    case Enu::ApnBpCin_func: //A plus ~B plus Cin
        b=~b;
        //Intentional fallthrough
    case Enu::ApBpCin_func: //A plus B plus Cin
        res=a+b+(int)carryIn;
        NZVC|= Enu::CMask*(((res &0x1ff)>>8) &0x1);
        NZVC|= Enu::NMask*((((a & 0x7f) + (b & 0x7f)) >> 7) & 0x1) ^ (NZVC&Enu::CMask);
        break;
    case Enu::AandB_func: //A*B
        res=a&b;
        break;
    case Enu::nAandB_func: //~(A*B)
        res=~(a&b);
        break;
    case Enu::AorB_func: //A+B
        res=a|b;
        break;
    case Enu::nAorB_func: //~(A+B)
        res=~(a|b);
        break;
    case Enu::AxorB_func: //A xor B
        res=a^b;
        break;
    case Enu::nA_func: //~A
        res=~a;
        break;
    case Enu::ASLA_func: //ASL A
        res=a<<1;
        NZVC|=Enu::CMask*((a & 0x80) >> 7);
        NZVC|=Enu::VMask*(((a & 0x40) >> 6) ^ (NZVC&Enu::CMask));
        break;
    case Enu::ROLA_func: //ROL A
        res=a<<1 | ((int) carryIn);
        NZVC|=Enu::CMask*((a & 0x80) >> 7);
        NZVC|=Enu::VMask*(((a & 0x40) >> 6) ^ (NZVC&Enu::CMask));
        break;
    case Enu::ASRA_func: //ASR A
        res = (a>>1)|(a&0x80);
        NZVC|=Enu::CMask*(a&1);
        break;
    case Enu::RORA_func: //ROR a
        res = (a>>1)|((int)carryIn<<7);
        NZVC|=Enu::CMask*(a&1);
        break;
    case Enu::NZVCA_func: //Move A to NZVC
        res=0;
        NZVC|=Enu::NMask&a;
        NZVC|=Enu::ZMask&a;
        NZVC|=Enu::VMask&a;
        NZVC|=Enu::CMask&a;
        return true; //Must return early to avoid NZ calculation
    default:
        break;

    }
    //N bit is always set the same
    NZVC|=(Enu::NMask*(res>127));
    //Z bit is always set the same
    NZVC|=(Enu::ZMask*(res==0));
    return true;

}

void CPUDataSection::setMemoryRegister(Enu::EMemoryRegisters, quint8 value)
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

quint8 CPUDataSection::getRegisterBankByte(quint8 registerNumber) const
{
    if(registerNumber>Enu::maxRegisterNumber) return 0;
    else return memory[registerNumber];

}

quint16 CPUDataSection::getRegisterBankWord(quint8 registerNumber) const
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

bool CPUDataSection::valueOnABus(quint8 &result) const
{
    if(controlSignals[Enu::A]==Enu::signalDisabled) return false;
    result=registerBank[controlSignals[Enu::A]];
    return true;
}

bool CPUDataSection::valueOnBBus(quint8 &result) const
{
    if(controlSignals[Enu::B]==Enu::signalDisabled) return false;
    result=registerBank[controlSignals[Enu::B]];
    return true;
}

bool CPUDataSection::valueOnCBus(quint8 &result) const
{
    if(controlSignals[Enu::CMux]==0)
    {
        result=0;
        result= (NZVCSbits&~(Enu::SMask)); //Mask out S bit
        return true;
    }
    else if(controlSignals[Enu::CMux]==1)
    {
        quint8 temp;
        return calculatALUOutput(result,temp);
    }
    else return false;



}

Enu::MainBusState CPUDataSection::getMainBusState() const
{
    return mainBusState;
}

quint8 CPUDataSection::getMemoryByte(quint16 address) const
{
    return memory[address];
}

MicroCode* CPUDataSection::getMicrocodeFromSignals() const
{
#pragma message "todo"
    return nullptr;
}

void CPUDataSection::setStatusBitPre(Enu::EStatusBit statusBit, bool val)
{
    qDebug()<<statusBit<<","<<val;
    switch(statusBit)
    {
    case Enu::STATUS_N:
        //Mask out the original N bit, and then or it with the properly shifted value
        NZVCSbits=(NZVCSbits&~Enu::NMask)|(val?1:0)*Enu::NMask;
        break;
    case Enu::STATUS_Z:
        NZVCSbits=(NZVCSbits&~Enu::ZMask)|(val?1:0)*Enu::ZMask;
        break;
    case Enu::STATUS_V:
        NZVCSbits=(NZVCSbits&~Enu::VMask)|(val?1:0)*Enu::VMask;
        break;
    case Enu::STATUS_C:
        NZVCSbits=(NZVCSbits&~Enu::CMask)|(val?1:0)*Enu::CMask;
        break;
    case Enu::STATUS_S:
        NZVCSbits=(NZVCSbits&~Enu::SMask)|(val?1:0)*Enu::SMask;
        break;
    }
}

void CPUDataSection::setMemoryBytePre(quint16 address, quint8 val)
{
    qDebug()<<address<<"--"<<val;
    memory[address]=val;
}

void CPUDataSection::setMemoryWordPre(quint16 address, quint16 val)
{
    qDebug()<<address<<"--"<<val;
    address=address-address%2;
    memory[address]=val/256;
    memory[address+1]=val%256;
}

void CPUDataSection::setRegisterBytePre(quint8 reg, quint8 val)
{
    if(reg>21) return;
    qDebug()<<reg<<"|"<<val;
    registerBank[reg]=val;
}

void CPUDataSection::setRegisterWordPre(quint8 reg, quint16 val)
{
   if(reg>21) return;
   qDebug()<<reg<<"||"<<val;
   registerBank[reg]=val/256;
   registerBank[reg+1]=val%256;
}

bool CPUDataSection::setSignalsFromMicrocode(const MicroCode *line)
{
    quint32 ctrlChanged=0;
    quint16 clockChanged=0;
    quint8 sig;
    for(int it=0;it<controlSignals.length();it++)
    {
        sig=line->getControlSignal((Enu::EControlSignals)it);
        if(controlSignals[it]!=sig)
        {
            ctrlChanged=(ctrlChanged<<1)+1;
            controlSignals[it]=sig;
        }
    }
    emit controlSignalChanged(ctrlChanged);
    bool val;
    for(int it=0;it<clockSignals.length();it++)
    {
        val=line->getClockSignal((Enu::EClockSignals)it);
        if(clockSignals[it]!=val)
        {
            clockChanged=(clockChanged<<1)+1;
            clockSignals[it]=val;
        }
    }
    emit clockSignalsChanged(clockChanged);
    return ctrlChanged|clockChanged;
}

void CPUDataSection::setRegisterByte(quint8 reg, quint8 value)
{
    quint8 old = registerBank[reg];
    setRegisterBytePre(reg,value);
    emit registerChanged(reg,old,value);
}

void CPUDataSection::setMemoryByte(quint16 address, quint8 value)
{
#pragma message "todo"
    setMemoryBytePre(address,value);
    //emit memoryChanged(address);
}

void CPUDataSection::setMemoryWord(quint16 address, quint16 value)
{
#pragma message "todo"
    setMemoryWordPre(address,value);
    //emit memoryChanged(address);
}

void CPUDataSection::setStatusBit(Enu::EStatusBit statusBit, bool val)
{
    setStatusBitPre(statusBit,val);
    //emit statusBitChanged(statusBit);
}

bool CPUDataSection::hadErrorOnStep()
{
    return hadDataError;
}

void CPUDataSection::handleMainBusState() noexcept
{
#pragma message "I don't know how the main bus returns to none after ready"
    bool marChanged= false;
    quint8 a,b;
    if(clockSignals[Enu::MARCk]&&valueOnABus(a)&&valueOnBBus(b))
    {
        marChanged=!(a==memoryRegisters[Enu::MEM_MARA]&&b==memoryRegisters[Enu::MEM_MARB]);
    }
    switch(mainBusState)
    {
    case Enu::None:
        //One cannot change MAR contents and initiate a R/W on same cycle
        if(!marChanged)
        {
            if(clockSignals[Enu::MemRead]) mainBusState = Enu::MemReadFirstWait;
            else if(clockSignals[Enu::MemWrite]) mainBusState = Enu::MemWriteFirstWait;
        }
        break;
    case Enu::MemReadFirstWait:
        if(!marChanged&&clockSignals[Enu::MemRead]) mainBusState = Enu::MemReadSecondWait;
        else if(marChanged&&clockSignals[Enu::MemRead]); //Initiating a new read brings us back to first wait
        else if(clockSignals[Enu::MemWrite]) mainBusState = Enu::MemWriteFirstWait; //Switch from read to write.
        else mainBusState = Enu::None; //If neither are check, bus goes back to doing nothing
        break;
    case Enu::MemReadSecondWait:
        if(!marChanged&&clockSignals[Enu::MemRead]) mainBusState = Enu::MemReadReady;
        else if(marChanged&&clockSignals[Enu::MemRead])mainBusState = Enu::MemReadFirstWait;
        else if(clockSignals[Enu::MemWrite]) mainBusState = Enu::MemWriteFirstWait;
        else mainBusState = Enu::None; //If neither are check, bus goes back to doing nothing
        break;
    case Enu::MemReadReady:
        if(!marChanged&&clockSignals[Enu::MemRead]); //Do nothing, we are already ready
        else if(marChanged&&clockSignals[Enu::MemRead])mainBusState = Enu::MemReadFirstWait;
        else if(clockSignals[Enu::MemWrite]) mainBusState = Enu::MemWriteFirstWait;
        else mainBusState = Enu::None; //If neither are check, bus goes back to doing nothing
        break;
    case Enu::MemWriteFirstWait:
        if(!marChanged&&clockSignals[Enu::MemWrite]) mainBusState = Enu::MemWriteSecondWait;
        else if(marChanged&&clockSignals[Enu::MemWrite]); //Initiating a new write brings us back to first wait
        else if(clockSignals[Enu::MemRead]) mainBusState = Enu::MemReadFirstWait; //Switch from write to read.
        else mainBusState = Enu::None; //If neither are check, bus goes back to doing nothing
        break;
    case Enu::MemWriteSecondWait:
        if(!marChanged&&clockSignals[Enu::MemWrite]) mainBusState = Enu::MemWriteReady;
        else if(marChanged&&clockSignals[Enu::MemWrite])mainBusState = Enu::MemWriteFirstWait; //Initiating a new write brings us back to first wait
        else if(clockSignals[Enu::MemRead]) mainBusState = Enu::MemReadFirstWait; //Switch from write to read.
        else mainBusState = Enu::None; //If neither are check, bus goes back to doing nothing
        break;
    case Enu::MemWriteReady:
        if(!marChanged&&clockSignals[Enu::MemWrite]); //Do nothing, MemWrite is already ready
        else if(marChanged&&clockSignals[Enu::MemWrite])mainBusState = Enu::MemWriteFirstWait; //Initiating a new write brings us back to first wait
        else if(clockSignals[Enu::MemRead]) mainBusState = Enu::MemReadFirstWait; //Switch from write to read.
        else mainBusState = Enu::None; //If neither are check, bus goes back to doing nothing
        break;
    default:
        mainBusState=Enu::None;
        break;

    }
    qDebug()<<mainBusState;
}

void CPUDataSection::stepOneByte() noexcept
{
    handleMainBusState();
    if(hadErrorOnStep()) return;
    Enu::EALUFunc aluFunc = (Enu::EALUFunc) controlSignals[Enu::ALU];
    quint8 a,b,c,alu,NZVC;
    bool hasA=valueOnABus(a),hasB=valueOnBBus(b),hasC=valueOnCBus(c),aluOutput=calculatALUOutput(alu,NZVC);
    //Handle write to memory
    if(mainBusState == Enu::MemWriteReady)
    {
        quint16 address = (memoryRegisters[Enu::MEM_MARA]<<8)+memoryRegisters[Enu::MEM_MARB];
        setMemoryByte(address,memoryRegisters[Enu::MEM_MDR]);
    }
    //MARCk
    if(clockSignals[Enu::MARCk]&&hasA&&hasB)
    {
        setMemoryRegister(Enu::MEM_MARA,a);
        setMemoryRegister(Enu::MEM_MARB,b);
    }
    else
    {
        //Handle error
    }
    //LoadCk
    if(clockSignals[Enu::LoadCk])
    {
        if(controlSignals[Enu::C]==Enu::signalDisabled)
        {
            hadDataError=true;
            errorMessage = "No destination register specified for LoadCk.";
        }
        else setRegisterByte(controlSignals[Enu::C],c);
    }
    //MDRCk
    if(clockSignals[Enu::MDRCk])
    {
        switch(controlSignals[Enu::MDRMux])
        {
        case 0: //Pick memory
        {
            quint16 address = (memoryRegisters[Enu::MEM_MARA]<<8)+memoryRegisters[Enu::MEM_MARB];
            if(mainBusState!=Enu::MemReadReady){
                hadDataError=true;
                errorMessage = "No value from data bus to write to MDR";
            }
            else setMemoryRegister(Enu::MEM_MDR,getMemoryByte(address));
            break;
        }
        case 1: //Pick C Bus;
        {
            if(!hasC)
            {
                hadDataError=true;
                errorMessage = "No value on C bus to write to MDR";
            }
            else setMemoryRegister(Enu::MEM_MDR,c);
            break;
        }
        default:
            hadDataError=true;
            errorMessage = "No value to clock into MDR";
            break;
        }

    }
    //NCk
    if(clockSignals[Enu::NCk])
    {
        //And against a instead of ALU output, since ALU output is technically 0
        if(aluFunc!=Enu::UNDEFINED_func) setStatusBit(Enu::STATUS_N,Enu::NMask & NZVC);
    }
    //ZCk
    if(clockSignals[Enu::ZCk])
    {
        if(aluFunc!=Enu::UNDEFINED_func) setStatusBit(Enu::STATUS_Z,Enu::ZMask & NZVC);
    }
    //VCk
    if(clockSignals[Enu::VCk])
    {
        if(aluFunc!=Enu::UNDEFINED_func) setStatusBit(Enu::STATUS_V,Enu::VMask & NZVC);
    }
    //CCk
    if(clockSignals[Enu::CCk])
    {
        if(aluFunc!=Enu::UNDEFINED_func) setStatusBit(Enu::STATUS_C,Enu::CMask & NZVC);
    }
    //SCk
    if(clockSignals[Enu::SCk])
    {
        if(aluFunc!=Enu::UNDEFINED_func) setStatusBit(Enu::STATUS_S,Enu::CMask & NZVC);
    }

}

void CPUDataSection::stepTwoByte() noexcept
{
    //TODO
    handleMainBusState();
    if(hadErrorOnStep()) return;
}

void CPUDataSection::presetStaticRegisters() noexcept
{
    registerBank[23]=0x01;
    registerBank[24]=0x02;
    registerBank[25]=0x03;
    registerBank[26]=0x01;
    registerBank[27]=0x08;
    registerBank[28]=0xF0;
    registerBank[29]=0xF6;
    registerBank[30]=0xFE;
    registerBank[31]=0xFF;
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
    for(int it=0;it<registerBank.length();it++)
    {
        registerBank[it]=0;
    }
    presetStaticRegisters();
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

void CPUControlSection::setMicrocodeProgram(MicrocodeProgram *program)
{
    this->program=program;
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
    qDebug()<<"Was stepped";
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

void CPUControlSection::initCPUStateFromPreconditions()
{
    onClearCPU();
    QList<UnitPreCode*> preCode;
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

