#ifndef QCROSSOVER_H
#define QCROSSOVER_H

#include "../cells/qchromosome.h"
#include <QObject>

namespace QCrossover
{
    QChromosome* crossover(QChromosome* chromosome, QChromosome* otherChromosome);
}

#endif // QCROSSOVER_H
