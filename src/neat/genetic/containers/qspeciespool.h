#ifndef QSPECIESPOOL_H
#define QSPECIESPOOL_H

#include <functional>
#include <QObject>
#include <QList>
#include "qspecies.h"
#include "../cells/qchromosome.h"

class QSpeciesPool : public QObject
{
        Q_OBJECT
    private:
        QList<QSpecies*> m_species;

    protected:
        void generateInitialChromosomes();

    public:
        QSpeciesPool(QObject *parent = 0);

        QList<QSpecies*> species() const;
        double totalFitness() const;
        void clearFitness();
        void randomFitness() const;

        void evaluateAll(QList<double> inputs, std::function<int(QList<double>, QList<QNeuronValue>)> fitnessFunc);
        void nextGeneration();
        QSpecies* newSpecies();
        void removeWeakSpecies();
        void addChromosomeToPool(QChromosome* chromosome, bool on = true);
};

#endif // QSPECIESPOOL_H
