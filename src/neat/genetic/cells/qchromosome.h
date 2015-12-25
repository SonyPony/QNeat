#ifndef QCHROMOSOME_H
#define QCHROMOSOME_H

#include <QObject>
#include "qgene.h"
#include <QMap>
#include "../../neural/qneuralnetwork.h"

typedef QSet<QGene*> QGeneSet;
typedef QSet<int> QInnovationSet;

class QChromosome : public QObject
{
        Q_OBJECT
    private:
        QMap<int, QGene*> m_genes;
        int m_fitness;

    public:
        QChromosome(QObject* parent = 0);
        QChromosome(QNeuralNetwork* network, QObject *parent = 0);
        QChromosome(QChromosome *other, QObject *parent = 0);
        ~QChromosome();

        int fitness() const;
        QGeneSet genes() const;
        QGene* gene(int innovation) const;

        void setFitness(int fitness);
        void addToFitness(int fitness);
        void addGene(QGene* gene);
        bool hasGene(QGene* gene) const;
        QNeuralNetwork* toNetwork();
        QInnovationSet innovations() const;
};

#endif // QCHROMOSOME_H
