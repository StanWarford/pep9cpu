#ifndef MICROCODEPROGRAM_H
#define MICROCODEPROGRAM_H
#include "enu.h"
class Code;
class MicroCode;
class MicrocodeProgram
{
private:
    QVector<Code*> objectCode;
public:
    MicrocodeProgram();
    ~MicrocodeProgram();
    MicrocodeProgram(QVector<Code*>objectCode);
    const QVector<Code*> getObjectCode() const;
    const MicroCode* getCodeLine(quint16 codeLine) const;
};

#endif // MICROCODEPROGRAM_H
