#include "qcrossover.h"
#include "../cells/qchromosomeanalyzer.h"
#include <QDebug>

QChromosome *QCrossover::crossover(QChromosome *chromosome, QChromosome *otherChromosome)
{
    QChromosome* childChromosome = new QChromosome;
    if(childChromosome == NULL)
        qDebug() << "NULL";

    QPair<QGeneSet, QGeneSet> commonGenes = QChromosomeAnalyzer::commonGenes(chromosome, otherChromosome);
    QList<QMap<int, QGene*> > sortedCommonGenes = { QMap<int, QGene*>(), QMap<int, QGene*>() };

    // sort common genes
    for(QGene* gene: commonGenes.first)
        sortedCommonGenes[0].insert(gene->innovation(), gene);
    for(QGene* gene: commonGenes.second)
        sortedCommonGenes[1].insert(gene->innovation(), gene);

    // randomly inherit parents common genes
    for(int innovation: sortedCommonGenes[0].keys()) {
        QGene* gene = new QGene(sortedCommonGenes[qrand() % 2][innovation]);
        childChromosome->addGene(gene);
    }

    // inherit excess and disjoint genes from more fit parent
    QChromosome* moreFitParent = QChromosomeAnalyzer::compareChromosomesFitness(chromosome, otherChromosome).first;
    QPair<QGeneSet, QGeneSet> disjointAndExcessGenes = QChromosomeAnalyzer::disjointAndExcessGenes(chromosome, otherChromosome);

    // inherit disjoint genes
    for(QGene* gene: disjointAndExcessGenes.first) {
        if(moreFitParent->hasGene(gene))
            childChromosome->addGene(new QGene(gene));
    }

    // inherit excess genes
    for(QGene* gene: disjointAndExcessGenes.second) {
        if(moreFitParent->hasGene(gene))
            childChromosome->addGene(new QGene(gene));
    }

    return childChromosome;
}

