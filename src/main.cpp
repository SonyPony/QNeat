#include <QCoreApplication>
#include <QSet>
#include "neat/core/qinnovation.h"
#include "neat/genetic/containers/qspeciespool.h"
#include "neat/genetic/cells/qchromosomeanalyzer.h"
#include "neat/core/qneatcore.h"
#include <QTime>
#include <stdlib.h>
#include <QList>
#include <QTextStream>
#include <QDebug>
#include "neat/genetic/cells/qchromosomeanalyzer.h"
#include "tests/tests.h"

int calcFitness(QList<double> intputs, QList<QNeuronValue> results)
{
    if(!results.first().valid())
        return 0;

    int result = floor(results[0].value());

    if((int)intputs[0] ^ (int)intputs[1] == result)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msec());

    int result = Tests::run();


    //QCoreApplication a(argc, argv);

    //QQmlApplicationEngine engine;
    int generation = 0;
    //QSpeciesPool *pool = new QSpeciesPool();
    /*QList<double> inp1 = {0, 0};
    QList<double> inp2 = {0, 1};
    QList<double> inp3 = {1, 0};
    QList<double> inp4 = {1, 1};*/



    for(int i = 0; i < 0; i++) {
        //pool->clearFitness();

        /*pool->evaluateAll(inp1, &calcFitness);
        pool->evaluateAll(inp2, &calcFitness);
        pool->evaluateAll(inp3, &calcFitness);
        pool->evaluateAll(inp4, &calcFitness);*/
        //pool->randomFitness();
        //qDebug() << "Next generation " << generation++;
        //pool->nextGeneration();
    }


    return 0;//a.exec();
}
