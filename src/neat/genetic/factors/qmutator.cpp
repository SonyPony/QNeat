#include "qmutator.h"
#include "../../qneatsettings.h"
#include "../../core/qneatcore.h"
#include "../../neural/qneuralnetwork.h"
#include "../../neural/qneuralconnection.h"
#include "../../neural/qneuron.h"
#include "qcrossover.h"
#include <QDebug>

void QMutator::mutateGeneWeight(QGene *gene)
{
    if(QNeatCore::randomNumber() < QNeatSettings::PerturbChance) {
        double step = QNeatSettings::MutationStep;
        gene->setWeight(gene->weight() + QNeatCore::randomNumber() * 2. * step - step);
    }

    else
        gene->setWeight(QNeatCore::randomNumber() * 2. - 1.);
}

void QMutator::mutateChromosomeGenesEnable(QChromosome *chromosome)
{
    for(QGene* gene: chromosome->genes()) {
        if(QNeatCore::randomNumber() < QNeatSettings::DisableGeneChance)
            gene->setEnabled(false);
        if(QNeatCore::randomNumber() < QNeatSettings::EnableGeneChance)
            gene->setEnabled(true);
    }
}

void QMutator::mutateDisabledInheritedGenes(QChromosome *child)
{
    // apply inherit disable mutation, where it has chance to have
    // disabled gene if parent has that gene disabled as well
    for(QGene* gene: child->genes()) {
        if(QNeatCore::randomNumber() > QNeatSettings::DisableGeneIfInheritDisable) {
            if(!gene->enabled())
                gene->setEnabled(true);
        }
    }
}

QChromosome *QMutator::breedChild(QChromosome *parent1, QChromosome *parent2)
{
    QChromosome* child = NULL;
    if(QNeatCore::randomNumber() < QNeatSettings::ChromosomeCrossoverChance) {
        child = QCrossover::crossover(parent1, parent2);
        QMutator::mutateDisabledInheritedGenes(child);
    }

    // if not crossover return one of the parent
    else {
        QList<QChromosome*> parents = { parent1, parent2 };
        child = new QChromosome(parents[qrand() % 2]);
    }

    return child;
}

void QMutator::newNode(QChromosome *chromosome)
{
    if(QNeatCore::randomNumber() > QNeatSettings::newNodeChance)
        return;

    // get highest neuron index
    int highestNeuronIndex = 1;
    QSet<int> inns = chromosome->innovations();
    for(QGene* gene: chromosome->genes()) {
        if(gene->input() > highestNeuronIndex)
            highestNeuronIndex = gene->input();
        if(gene->output() > highestNeuronIndex)
            highestNeuronIndex = gene->output();
    }

    QList<int> linns = inns.toList();
    // randomly select gene
    int rn = qrand() % linns.length();
    int geneIndex = linns[rn];
    QGene* selectedGene = chromosome->gene(geneIndex);
    selectedGene->setEnabled(false);

    qSort(linns);

    // first gene
    QNeuralConnection* c1 = new QNeuralConnection(selectedGene->input(), highestNeuronIndex + 1, 1.);
    QNeuralConnection* c2 = new QNeuralConnection(highestNeuronIndex + 1, selectedGene->output(), selectedGene->weight());

    chromosome->addGene(new QGene(c1));
    // second gene
    chromosome->addGene(new QGene(c2));

    delete c1;
    delete c2;
}

void QMutator::newLink(QChromosome *chromosome)
{
    if(QNeatCore::randomNumber() > QNeatSettings::NewLinkChance)
        return;

    // need network to determine if new link will cause recurrent network and to get random neurons
    QNeuralNetwork* network = chromosome->toNetwork();
    int inID, outID;
    QNeuralConnection* newConnection = NULL;
    do {
        inID = network->randomNeuron(QNeuron::Output);
        outID = network->randomNeuron(QNeuron::Input | QNeuron::Bias);

        newConnection = new QNeuralConnection(inID, outID, 1);
    } while(network->checkReccurentConnection(newConnection));

    chromosome->addGene(new QGene(inID, outID, QNeatCore::randomNumber() * 2. - 1.));
    delete network;
}

void QMutator::mutateChromosome(QChromosome *chromosome)
{
    if(QNeatCore::randomNumber() > QNeatSettings::ChromosomeMutationChance)
        return;

    // mutate weights
    for(QGene* gene: chromosome->genes())
        QMutator::mutateGeneWeight(gene);

    QMutator::mutateChromosomeGenesEnable(chromosome);
    QMutator::newNode(chromosome);
    QMutator::newLink(chromosome);
}
