#include <QCoreApplication>
#include "neat/genetic/containers/qspeciespool.h"
#include "neat/core/qneatcore.h"
#include <QTime>
#include <qmath.h>
#include <stdlib.h>
#include <QList>
#include <QTextStream>
#include <QDebug>

int calcFitness(QList<double> intputs, QList<QNeuronValue> results)
{
    if(!results.first().valid())
        return 0;

    int result = qRound(results[0].value());
    bool state = (qRound(intputs[0]) ^ qRound(intputs[1])) == result;

    if(state) {
        return 1;
    }

    return 0;
}

bool checkMax(int fitness)
{
    if(fitness == 4)
        return true;
    return false;
}

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msec());

    //QCoreApplication a(argc, argv);
    int generation = 0;
    QTextStream out(stdout);
    QSpeciesPool *pool = new QSpeciesPool();
    QList<double> inp1 = {0., 0.};
    QList<double> inp2 = {0., 1.};
    QList<double> inp3 = {1., 0.};
    QList<double> inp4 = {1., 1.};

    QChromosome* chromosome = NULL;

    for(int i = 0; i < 200; i++) {
        // clear alle fitness
        pool->clearFitness();

        // eval all 4 states of XOR
        pool->evaluateAll(inp1, &calcFitness);
        pool->evaluateAll(inp2, &calcFitness);
        pool->evaluateAll(inp3, &calcFitness);
        pool->evaluateAll(inp4, &calcFitness);
        out << "Next generation " << generation++ << endl;

        // check wheter optimal network has been found
        chromosome = pool->resultFound(&checkMax);

        if(chromosome != NULL) {
            out << "My chromosome" << "\n" <<QNeatCore::formatGenes(chromosome) << "\n" << "++++++++++++++++++" << endl;

            QNeuralNetwork* n = chromosome->toNetwork();
            // show all neurons
            for(QNeuron* neuron: n->neurons())
                out << neuron->ID() << neuron->strType() << endl;

            // show all states
            out << "Eval 1" << n->evaluate({0, 0})[0].value() << calcFitness({0, 0}, n->evaluate({0, 0})) << endl;
            out << "Eval 2" << n->evaluate({0, 1})[0].value() << calcFitness({0, 1}, n->evaluate({0, 1})) << endl;
            out << "Eval 3" << n->evaluate({1, 0})[0].value() << calcFitness({1, 0}, n->evaluate({1, 0})) << endl;
            out << "Eval 4" << n->evaluate({1, 1})[0].value() << calcFitness({1, 1}, n->evaluate({1, 1})) << endl;
            out << ((1 ^ 0) == 1) << qRound(-0.649392) << chromosome->fitness() << endl;
            n->deleteLater();
            break;
        }
        else {
            out << "Creating generation" << generation << endl;
            pool->nextGeneration();
            pool->clearFitness();
        }
    }

    return 0;//a.exec();
}
