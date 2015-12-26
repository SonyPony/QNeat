#ifndef QSPECIES_H
#define QSPECIES_H

#include <QObject>
#include <QList>
#include "../cells/qchromosome.h"
#include "../factors/qmutator.h"

class QSpecies : public QObject
{
        Q_OBJECT
    private:
        QList<QChromosome*> m_chromosomes;

    protected:
        void sortChromosomes();

    public:
        QSpecies(QObject *parent = 0);
        ~QSpecies();

        QList<QChromosome*> chromosomes() const;
        double averageFitness() const;

        QChromosome* breedChild() const;
        QChromosome* randomChromosome() const;
        void mutate();
        void removeWorstHalf();
        void removeAllExceptBest();
        void addChromosome(QChromosome* chromosome);
};

#endif // QSPECIES_H
