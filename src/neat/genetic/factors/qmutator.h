#ifndef QMUTATOR_H
#define QMUTATOR_H

#include <QObject>
#include "../cells/qchromosome.h"
#include "../cells/qgene.h"

namespace QMutator
{
    void mutateChromosome(QChromosome* chromosome);
    void mutateGeneWeight(QGene* gene);
    void mutateDisabledInheritedGenes(QChromosome* child);
    void mutateChromosomeGenesEnable(QChromosome* chromosome);
    void newNode(QChromosome* chromosome);
    void newLink(QChromosome* chromosome);
    QChromosome* breedChild(QChromosome* parent1, QChromosome* parent2);
}

#endif // QMUTATOR_H
