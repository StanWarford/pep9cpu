#include "microcodeprogram.h"
#include "code.h"

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

