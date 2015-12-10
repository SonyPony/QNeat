#include "qchromosomeanalyzertests.h"
#include "../../neat/core/qneatcore.h"
#include "../../neat/genetic/cells/qchromosome.h"
#include "../../neat/genetic/factors/qmutator.h"
#include <QTextStream>
#include <QDebug>

void QChromosomeAnalyzerTests::testDisjointAndExcess()
{
    QTextStream out(stdout);

    QNeuralNetwork n1;
    QNeuralNetwork n2;

    QChromosome c1(&n1);
    QChromosome c2(&n2);
    QMutator::newNode(&c2);


    out << QNeatCore::formatGenes(&c2);

    QMutator::newLink(&c2);
    QMutator::newNode(&c2);
    out << QNeatCore::formatGenes(&c2);
    qDebug() << QChromosomeAnalyzer::disjointAndExcessGenes(&c1, &c2);
    out << QNeatCore::formatGenes(QChromosomeAnalyzer::disjointAndExcessGenes(&c1, &c2).second.toList());
}
