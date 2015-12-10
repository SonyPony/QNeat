#include "qchromosome.h"
#include <QDebug>

QChromosome::QChromosome(QObject *parent): QObject(parent)
{
    m_fitness = 0;
}

QChromosome::QChromosome(QNeuralNetwork *network, QObject *parent) : QObject(parent)
{
    m_fitness = 0;

    for(QNeuralConnection* connection: network->connections())
        this->addGene(new QGene(connection, this));
}

QChromosome::QChromosome(QChromosome *other, QObject *parent): QObject(parent)
{
    m_fitness = 0;

    for(QGene* gene: other->genes())
        this->addGene(new QGene(gene));
}

int QChromosome::fitness() const
{
    return m_fitness;
}

QGeneSet QChromosome::genes() const
{
    return m_genes.values().toSet();
}

QGene *QChromosome::gene(int innovation) const
{
    return m_genes[innovation];
}

void QChromosome::setFitness(int fitness)
{
    m_fitness = fitness;
}

void QChromosome::addGene(QGene *gene)
{
    gene->setParent(this);
    m_genes.insert(gene->innovation(), gene);
}

bool QChromosome::hasGene(QGene *gene) const
{
    return m_genes.keys().contains(gene->innovation());
}

QNeuralNetwork* QChromosome::toNetwork()
{
    QNeuralNetwork* network = new QNeuralNetwork;
    QNeuralConnection* connection = NULL;

    for(QGene* gene: m_genes.values()) {
        connection = gene->toConnection();

        // generate neurons if they do not exist
        for(int neuronID: {connection->input(), connection->output()}) {
            // if network does not have neuron
            if(!network->hasNeuron(neuronID)) {
                // because input, bias and output neurons already exists
                network->addNeuron(new QNeuron(QNeuron::Hidden), neuronID);
            }
        }

        QNeuron* startNeuron = network->neuron(connection->input());
        QNeuron* endNeuron = network->neuron(connection->output());

        startNeuron->addOutput(connection->output());
        endNeuron->addInput(connection->input());
        Q_ASSERT_X(connection->input() != connection->output(), "io", "conversion");
        //network->addConnection(connection);
    }

    return network;
}

QInnovationSet QChromosome::innovations() const
{
    return m_genes.keys().toSet();
}

