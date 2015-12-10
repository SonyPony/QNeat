#ifndef QCHROMOSOMEANALYZER_H
#define QCHROMOSOMEANALYZER_H

#include <QObject>
#include <QPair>
#include "qgene.h"
#include "qchromosome.h"

namespace QChromosomeAnalyzer
{
    // return genes from both chromosomes, because genes does not have to match in weight
    // first set is from more fit parent, second is from less fit parent
    QPair<QGeneSet, QGeneSet> commonGenes(QChromosome* const chromosome, QChromosome* const otherChromosome);

    bool isSameSpecies(QChromosome* const chromosome, QChromosome* const otherChromosome);

    // first is longer chromosome, second is shorter one
    QPair<QChromosome*, QChromosome*> compareLenOfChromosomes(QChromosome* const chromosome, QChromosome* const otherChromosome);

    // first are disjoint genes, second are excess genes
    QPair<QGeneSet, QGeneSet> disjointAndExcessGenes(QChromosome* const chromosome, QChromosome* const otherChromosome);

    // fisr is more fit chromosome, second is less fit chromosome
    QPair<QChromosome*, QChromosome*> compareChromosomesFitness(QChromosome* const chromosome, QChromosome* const otherChromosome);

    // first is chromosome, second is the highest innovation of that chromosome
    QPair<QChromosome*, int> chromosomeWithLowerInnovation(QChromosome* const chromosome, QChromosome* const otherChromosome);

    // calculating average weights differences
    double averageWeightDifferences(QChromosome* const chromosome, QChromosome* const otherChromosome);
}

#endif // QCHROMOSOMEANALYZER_H
