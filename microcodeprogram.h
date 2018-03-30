#ifndef MICROCODEPROGRAM_H
#define MICROCODEPROGRAM_H
#include "enu.h"
class Code;
class MicrocodeProgram
{
private:
    QVector<Code*> objectCode;
public:
    MicrocodeProgram();
    ~MicrocodeProgram();
    MicrocodeProgram(QVector<Code*>objectCode);
    const QVector<Code*> getObjectCode() const;
};

#endif // MICROCODEPROGRAM_H
