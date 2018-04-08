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
#pragma message "Todo"
    //Return the n'th line of microcode;
}

