#include "qspecies.h"
#include "../factors/qmutator.h"
#include <QDebug>

void QSpecies::sortChromosomes()
{
    qSort(m_chromosomes.begin(), m_chromosomes.end(), [](QChromosome* const c1, QChromosome* const c2) -> bool {
        return c1->fitness() > c2->fitness();
    });
}

QSpecies::QSpecies(QObject *parent) : QObject(parent)
{

}

QList<QChromosome *> QSpecies::chromosomes() const
{
    return m_chromosomes;
}

double QSpecies::averageFitness() const
{
    double sumFitness = 0.;

    for(QChromosome* chromosome: m_chromosomes)
        sumFitness += chromosome->fitness();

    return sumFitness / (double)m_chromosomes.length();
}

QChromosome *QSpecies::breedChild() const
{
    QChromosome* parent1 = m_chromosomes[qrand() % m_chromosomes.length()];
    QChromosome* parent2 = m_chromosomes[qrand() % m_chromosomes.length()];

    return QMutator::breedChild(parent1, parent2);
}

void QSpecies::mutate()
{
    for(QChromosome* chromosome: m_chromosomes) {
        //QMutator::mutateChromosome(chromosome);
        QMutator::newNode(chromosome);
        QMutator::newLink(chromosome);
        /*for(QGene* gene: chromosome->genes())
            QMutator::mutateGeneWeight(gene);

        QMutator::mutateChromosomeGenesEnable(chromosome);
        QMutator::newNode(chromosome);*/
    }
}

void QSpecies::removeWorstHalf()
{
    this->sortChromosomes();

    for(int i = 0; i < m_chromosomes.length() / 2; i++) {
        QChromosome* chromosome = m_chromosomes.last();
        m_chromosomes.removeLast();
        chromosome->deleteLater();
    }
}

void QSpecies::removeAllExceptBest()
{
    this->sortChromosomes();

    for(int i = 1; i < m_chromosomes.length(); i++) {
        QChromosome* chromosome = m_chromosomes.last();
        m_chromosomes.removeLast();
        chromosome->deleteLater();
    }
}

void QSpecies::addChromosome(QChromosome *chromosome)
{
    chromosome->setParent(this);
    m_chromosomes.append(chromosome);
}
