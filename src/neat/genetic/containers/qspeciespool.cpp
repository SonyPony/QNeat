#include "qspeciespool.h"
#include <qmath.h>
#include <QDebug>
#include "../../core/qneatcore.h"
#include "../../neural/qneuronvalue.h"
#include "../../qneatsettings.h"
#include "../cells/qchromosomeanalyzer.h"
#include "../../neural/qneuralnetwork.h"

void QSpeciesPool::generateInitialChromosomes()
{
    QNeuralNetwork* newNetwork = NULL;

    for(int i = 0; i < QNeatSettings::Population; i++) {
        newNetwork = new QNeuralNetwork;
        if(newNetwork == NULL)
            qDebug() << "NULL";
        QChromosome* chromosome = new QChromosome(newNetwork);
        if(chromosome == NULL)
            qDebug() << "NULL";
        this->addChromosomeToPool(chromosome);
        newNetwork->deleteLater();
    }
}

QSpeciesPool::QSpeciesPool(QObject *parent) : QObject(parent)
{
    this->generateInitialChromosomes();
}

QList<QSpecies *> QSpeciesPool::species() const
{
    return m_species;
}

double QSpeciesPool::totalFitness() const
{
    double sumFitness = 0;

    for(QSpecies* species: m_species)
        sumFitness += species->averageFitness();

    return sumFitness;
}

void QSpeciesPool::clearFitness()
{
    for(QSpecies* species: m_species) {
        for(QChromosome* chromosome: species->chromosomes())
            chromosome->setFitness(0);
    }
}

void QSpeciesPool::evaluateAll(QList<double> inputs, std::function<int(QList<double>, QList<QNeuronValue>)> fitnessFunc)
{
    QNeuralNetwork* tempNetwork = NULL;
    QList<QNeuronValue> results;

    for(QSpecies* species: m_species) {
        for(QChromosome* chromosome: species->chromosomes()) {
            tempNetwork = chromosome->toNetwork();
            results = tempNetwork->evaluate(inputs);
            tempNetwork->deleteLater();

            chromosome->setFitness(chromosome->fitness() + fitnessFunc(inputs, results));
        }
    }
}

void QSpeciesPool::nextGeneration()
{
    this->removeWeakSpecies();

    QList<QChromosome*> children;
    int numberOfChildren;

    for(QSpecies* species: m_species) {
        //qDebug() << "---------------------";
        //qDebug() << species << " contains " << species->chromosomes().size() << " and have sf " << species->averageFitness();
        if(this->totalFitness() <= 0)
            numberOfChildren = QNeatSettings::Population;
        else
            numberOfChildren = qFloor(species->averageFitness() / this->totalFitness() * (double)QNeatSettings::Population);

        species->removeWorstHalf();

        // breed
        for(int i = 0; i < numberOfChildren; i++) {
            children.append(species->breedChild());
        }

        species->removeAllExceptBest();
    }
    for(QChromosome* child: children) {
        this->addChromosomeToPool(child, false);
    }

    for(QSpecies* species: m_species) {
        species->mutate();
    }
}

QSpecies *QSpeciesPool::newSpecies()
{
    QSpecies* species = new QSpecies;
    if(species == NULL)
        qDebug() << "NULL";
    m_species.append(species);
    return species;
}

void QSpeciesPool::removeWeakSpecies()
{
    if(this->totalFitness() <= 0)
        return;

    int numberOfNeededChildren;

    for(QSpecies* species: m_species) {
        numberOfNeededChildren = species->averageFitness() / this->totalFitness() * (double)QNeatSettings::Population;
        // if it would not reproduce any child, than remove it
        if(numberOfNeededChildren < 1) {
            m_species.removeAll(species);
            species->deleteLater();
        }
    }
}

void QSpeciesPool::addChromosomeToPool(QChromosome *chromosome, bool on)
{
    bool adddedToSpecies = false;

    for(QSpecies* species: m_species) {
        if(QChromosomeAnalyzer::isSameSpecies(chromosome, species->chromosomes()[0])) {
            adddedToSpecies = true;
            species->addChromosome(chromosome);
            break;
        }
    }

    if(!adddedToSpecies) {
        QSpecies* species = this->newSpecies();
        species->addChromosome(chromosome);
    }
}

