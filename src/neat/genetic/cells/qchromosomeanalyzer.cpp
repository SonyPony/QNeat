#include "qchromosomeanalyzer.h"
#include <QPair>
#include <QDebug>
#include "../../qneatsettings.h"

QPair<QChromosome *, QChromosome *>
QChromosomeAnalyzer::compareLenOfChromosomes(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    if(chromosome->genes().size() > otherChromosome->genes().size())
        return qMakePair(chromosome, otherChromosome);
    else
        return qMakePair(otherChromosome, chromosome);
}

QPair<QChromosome *, int> QChromosomeAnalyzer::chromosomeWithLowerInnovation(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    QList<int> fsChromosomeInnovations = chromosome->innovations().toList();
    QList<int> ndChromosomeInnovations = otherChromosome->innovations().toList();

    qSort(fsChromosomeInnovations);
    qSort(ndChromosomeInnovations);

    if(fsChromosomeInnovations.last() > ndChromosomeInnovations.last())
        return qMakePair(chromosome, fsChromosomeInnovations.last());
    else
        return qMakePair(otherChromosome, ndChromosomeInnovations.last());
}

QPair<QChromosome *, QChromosome *>
QChromosomeAnalyzer::compareChromosomesFitness(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    if(chromosome->fitness() > otherChromosome->fitness())
        return qMakePair(chromosome, otherChromosome);
    else
        return qMakePair(otherChromosome, chromosome);
}

QPair<QGeneSet, QGeneSet>
QChromosomeAnalyzer::commonGenes(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    QPair<QChromosome*, QChromosome*> fitSortedChromosomes = QChromosomeAnalyzer::compareChromosomesFitness(chromosome, otherChromosome);
    QChromosome* moreFitChromosome = fitSortedChromosomes.first;
    QChromosome* lessFitChromosome = fitSortedChromosomes.second;

    QInnovationSet commonGenesInnovations = chromosome->innovations().intersect(
                otherChromosome->innovations());
    QGeneSet commonGenes;
    QGeneSet lessFitCommonGenes;

    for(int innovation: commonGenesInnovations) {
        commonGenes.insert(moreFitChromosome->gene(innovation));
        lessFitCommonGenes.insert(lessFitChromosome->gene(innovation));
    }

    return qMakePair(commonGenes, lessFitCommonGenes);
}

QPair<QGeneSet, QGeneSet>
QChromosomeAnalyzer::disjointAndExcessGenes(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    int highestInnovationOfLowerChromosome = QChromosomeAnalyzer::chromosomeWithLowerInnovation(
                chromosome, otherChromosome).second;

    QInnovationSet disjointAndExcessInnovations = chromosome->innovations()
            .unite(otherChromosome->innovations())
            .subtract(chromosome->innovations().intersect(otherChromosome->innovations()));
    QInnovationSet disjointInnovations;
    QInnovationSet excessInnovations;

    for(int innovation: disjointAndExcessInnovations) {
        if(innovation > highestInnovationOfLowerChromosome)
            excessInnovations.insert(innovation);
        else
            disjointInnovations.insert(innovation);
    }

    // fill disjointGenes and excessGenes
    QGeneSet disjointGenes;
    QGeneSet excessGenes;

    // fill disjoint genes
    for(int innovation: disjointInnovations) {
        // has gene
        if(chromosome->innovations().contains(innovation))
            disjointGenes.insert(chromosome->gene(innovation));
        else
            disjointGenes.insert(otherChromosome->gene(innovation));
    }

    // fill excess genes
    for(int innovation: excessInnovations) {
        // has gene
        if(chromosome->innovations().contains(innovation))
            excessGenes.insert(chromosome->gene(innovation));
        else
            excessGenes.insert(otherChromosome->gene(innovation));
    }

    return qMakePair(disjointGenes, excessGenes);
}

bool QChromosomeAnalyzer::isSameSpecies(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    QPair<QGeneSet, QGeneSet> disjointAndExcess = QChromosomeAnalyzer::disjointAndExcessGenes(
                chromosome, otherChromosome);
    QGeneSet disjoint = disjointAndExcess.first;
    QGeneSet excess = disjointAndExcess.second;

    double dExcess = QNeatSettings::ExcessCoefficient * excess.size();
    double dDisjoint = QNeatSettings::DisjointCoefficient * disjoint.size();
    double dWeights = QNeatSettings::WeightCoefficient * QChromosomeAnalyzer::averageWeightDifferences(chromosome, otherChromosome);

    return (dExcess + dDisjoint + dWeights) < QNeatSettings::DeltaThreshold;
}

double QChromosomeAnalyzer::averageWeightDifferences(QChromosome * const chromosome, QChromosome * const otherChromosome)
{
    QGeneSet moreFitCommonGenes = QChromosomeAnalyzer::commonGenes(chromosome, otherChromosome).first;
    QChromosome* lessFitChromosome = QChromosomeAnalyzer::compareChromosomesFitness(chromosome, otherChromosome).second;

    double avgWeightDiffs = 0;

    for(QGene* gene: moreFitCommonGenes) {
        avgWeightDiffs += qAbs(
                    qAbs(gene->weight()) -
                    qAbs(lessFitChromosome->gene(gene->innovation())->weight())
                    );
    }

    return avgWeightDiffs / (double)moreFitCommonGenes.size();
}
