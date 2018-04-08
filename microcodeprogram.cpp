#include "microcodeprogram.h"

MicrocodeProgram::MicrocodeProgram():objectCode()
{

}

MicrocodeProgram::~MicrocodeProgram()
{
    for(int it = 0; it < objectCode.size();it++)
    {
        delete objectCode[it];
    }
}

MicrocodeProgram::MicrocodeProgram(QVector<Code*>objectCode):
    objectCode(objectCode)
{
}

const QVector<Code*> MicrocodeProgram::getObjectCode() const
{
    return this->objectCode;
}

const MicroCode *MicrocodeProgram::getCodeLine(quint16 codeLine) const
{
    int microCodeIt=0;
    for(int it=0;it<objectCode.size();it++)
    {
        if(objectCode[it]->isMicrocode()) ++microCodeIt;
        if(microCodeIt==codeLine+1) return (MicroCode*)objectCode[it];
    }
    return nullptr;
    //Return the n'th line of microcode;
}

