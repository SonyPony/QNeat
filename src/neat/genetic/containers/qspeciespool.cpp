#include "qspeciespool.h"
#include <qmath.h>
#include <QDebug>
#include <QTextStream>
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
        QChromosome* chromosome = new QChromosome(newNetwork);
        this->addChromosomeToPool(chromosome);
        delete newNetwork;
    }
}

QSpeciesPool::QSpeciesPool(QObject *parent) : QObject(parent)
{
    this->generateInitialChromosomes();
}

QSpeciesPool::~QSpeciesPool()
{
    for(QSpecies* species: m_species)
        delete species;
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
            chromosome->addToFitness(fitnessFunc(inputs, results));

            delete tempNetwork;
        }
    }
}

QChromosome *QSpeciesPool::interspeciesCrossover()
{
    QSet<int> speciesKeys;

    while(speciesKeys.size() < 2)
        speciesKeys.insert(qrand() % m_species.length());
    QList<int> speciesKeysList = speciesKeys.toList();
    QSpecies* species1 = m_species[speciesKeysList.at(0)];
    QSpecies* species2 = m_species[speciesKeysList.at(1)];

    return QMutator::breedChild(species1->randomChromosome(), species2->randomChromosome());
}

QChromosome* QSpeciesPool::resultFound(std::function<bool (int)> checkMaxFunc)
{
    for(QSpecies* species: m_species) {
        for(QChromosome* chromosome: species->chromosomes()) {
            if(checkMaxFunc(chromosome->fitness()))
                return chromosome;
        }
    }

    return NULL;
}

void QSpeciesPool::nextGeneration()
{
    this->removeWeakSpecies();

    QList<QChromosome*> children;
    int numberOfChildren;

    if(QNeatCore::randomNumber() < QNeatSettings::InterSpeciesCrossoverChange)
        children.append(this->interspeciesCrossover());

    for(QSpecies* species: m_species) {
        qDebug() << species << " contains " << species->chromosomes().size() << " and has shared fitness " << species->averageFitness();
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
            delete species;
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

