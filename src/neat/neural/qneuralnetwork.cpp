#include "qneuralnetwork.h"
#include "../qneatsettings.h"
#include "../core/qneatcore.h"
#include <QDebug>
#include "qneuronvalue.h"

QSet<int> QNeuralNetwork::expandLayerPaths(QSet<int> layer)
{
    QSet<int> expandedLayer;

    for(int neuronID: layer) {
        int neuronType = m_neurons[neuronID]->type();

        if(neuronType != QNeuron::Output) {
            for(int nextNeuronID: m_neurons[neuronID]->outputs())
                expandedLayer.insert(nextNeuronID);
        }
    }

    return expandedLayer;
}

QSet<int> QNeuralNetwork::removeOutputNeuronsFromLayer(QSet<int> layer)
{
    QSet<int> needToRemove;

    for(int neuronID: layer) {
        if(m_neurons[neuronID]->type() == QNeuron::Output)
            needToRemove.insert(neuronID);
    }

    return layer.subtract(needToRemove);
}

int QNeuralNetwork::lastNeuronID()
{
    QList<int> IDs = m_neurons.keys();
    qSort(IDs);

    return IDs.last();
}

QNeuralNetwork::QNeuralNetwork(QObject *parent) : QObject(parent)
{
    m_neuronsCount = 0;

    for(int i = 0; i < QNeatSettings::InputsCount; i++)
        this->addNeuron(new QNeuron(QNeuron::Input));
    // add one extra for bias
    this->addNeuron(new QNeuron(QNeuron::Bias));

    for(int i = 0; i < QNeatSettings::OutputsCount; i++)
        this->addNeuron(new QNeuron(QNeuron::Output));

    // generate connections
    for(int inID = 0; inID < QNeatSettings::InputsCount + 1; inID++) {
        for(int outID = QNeatSettings::InputsCount + 1; outID < QNeatSettings::InputsCount + QNeatSettings::OutputsCount + 1; outID++) {
            QNeuralConnection* connection = new QNeuralConnection(inID, outID, QNeatCore::randomNumber() * 2. - 1.);
            Q_ASSERT_X(connection->input() != connection->output(), "io", "init");
            this->addConnection(connection);
        }
    }
}

QNeuralNetwork::~QNeuralNetwork()
{
    //qDebug() << "Deleting network";
}

QList<int> QNeuralNetwork::neuronsIndexes()
{
    return m_neurons.keys();
}

QMap<int, QNeuralConnection*> QNeuralNetwork::connections() const
{
    return m_connections;
}

bool QNeuralNetwork::hasNeuron(int neuronID) const
{
    if(m_neurons.keys().contains(neuronID))
        return true;
    return false;
}

QNeuron *QNeuralNetwork::neuron(int neuronID) const
{
    return m_neurons[neuronID];
}

QList<QNeuron *> QNeuralNetwork::neurons() const
{
    return m_neurons.values();
}

QList<QNeuronValue> QNeuralNetwork::evaluate(QList<double> inputs)
{
    Q_ASSERT_X(inputs.length() == QNeatSettings::InputsCount, "inputs", "Inputs size does not match QNeatSettings::InputsCount");

    /*for(int key: m_neurons.keys()) {
        qDebug() << m_neurons[key]->strType() << " " << key << "=" << m_neurons[key]->ID();
    }*/

    QList<QNeuronValue> results;

    // clear neurons weights and neurons
    for(QNeuron* neuron: m_neurons.values())
        neuron->clearAll();

    // fill with updated weights and neurons
    for(QNeuralConnection* connection: m_connections.values()) {
        if(connection->enabled()) {
            m_neurons[connection->output()]->addInputNeuron(m_neurons[connection->input()]);
            m_neurons[connection->output()]->addInputWeight(connection->input(), connection->weight());
        }
    }

    // set inputs to input neurons
    for(int i = 0; i < QNeatSettings::InputsCount; i++)
        m_neurons[i]->setValue(inputs[i]);

    // output neurons indexes starts at InputsCount + 1 and end at InputsCount + OutputsCount
    for(int i = 0; i < QNeatSettings::OutputsCount; i++)
        results.append(m_neurons[QNeatSettings::InputsCount + i + 1]->value());

    return results;
}

bool QNeuralNetwork::checkReccurentConnection(QNeuralConnection *connection)
{
    if(connection->input() == connection->output())
        return true;

    this->addConnection(connection);
    QNeuron* startNeuron = m_neurons[connection->input()];
    QSet<int> currentLayer;
    currentLayer.insert(startNeuron->ID());

    // because link with bias or input neuron, can not cause recurrent connection
    if(startNeuron->type() & (QNeuron::Input | QNeuron::Bias))
        return false;

    for(int neuronID: startNeuron->outputs())
        currentLayer.insert(neuronID);

    QSet<int> expandedLayer;

    //qDebug() << "exanding";
    //int f =0;
    while(currentLayer.size()) {
        /*if(f++ > 10) {
            qDebug() << "++++++++++++++++++++++++++++++++++++++++++++";
            qDebug() << "My con" <<connection->input() << " -> " << connection->output();
            for(QNeuralConnection* c: m_connections)
                qDebug() << c->input() << " -> " << c->output();
            qDebug() << "---------------------------------------";
            for(QNeuron* n: m_neurons)
                qDebug() << n->strType() << n->ID();
            qDebug() << "++++++++++++++++++++++++++++++++++++++++++++";
        }*/
        expandedLayer = this->expandLayerPaths(currentLayer);
        currentLayer = this->removeOutputNeuronsFromLayer(expandedLayer);

        // if startNeuronID is found in expanded layer
        if(currentLayer.contains(startNeuron->ID())) {
            this->removeConnection(connection);
            //qDebug() << "done exanding";
            return true;
        }
    }
    //qDebug() << "done exanding";
    this->removeConnection(connection);
    return false;
}

int QNeuralNetwork::randomNeuron(int excludeTypes)
{
    int randomID;
    QList<int> IDList = m_neurons.keys();

    do {
        randomID = qrand() % (IDList.length());
    } while((excludeTypes & m_neurons[randomID]->type()) != 0);

    return randomID;
}

void QNeuralNetwork::addNeuron(QNeuron *neuron, int id)
{
    int neuronID;

    if(id == -1) {
        neuronID = m_neuronsCount;
        m_neuronsCount++;
    }

    else {
        neuronID = id;
        m_neuronsCount = this->lastNeuronID();
    }

    neuron->setID(neuronID);
    neuron->setParent(this);
    m_neurons.insert(neuronID, neuron);
}

void QNeuralNetwork::addConnection(QNeuralConnection *connection)
{
    Q_ASSERT_X(connection->input() != connection->output(), "io", "add connection");
    // input neuron does not exists
    if(!m_neurons.keys().contains(connection->input()))
        this->addNeuron(new QNeuron(QNeuron::Hidden), connection->input());
    // output neuron does not exists
    if(!m_neurons.keys().contains(connection->output()))
        this->addNeuron(new QNeuron(QNeuron::Hidden), connection->output());

    m_neurons[connection->input()]->addOutput(connection->output());
    m_neurons[connection->output()]->addInput(connection->input());
    connection->setParent(this);
    m_connections.insert(connection->innovation(), connection);
}

void QNeuralNetwork::removeNeuron(int neuronID)
{
    QNeuron* neuron = m_neurons[neuronID];
    m_neurons.remove(neuronID);

    neuron->deleteLater();

    m_neuronsCount = this->lastNeuronID();
}

void QNeuralNetwork::removeConnection(QNeuralConnection *connection)
{
    int inID = connection->input();
    int outID = connection->output();

    if(m_connections.keys().contains(connection->innovation())) {
        /*if(m_neurons[inID]->outputs().size() == 1 && m_neurons[inID]->outputs().contains(outID))
            this->removeNeuron(inID);
        if(m_neurons[outID]->inputs().size() == 1 && m_neurons[outID]->inputs().contains(inID))
            this->removeNeuron(outID);*/
        m_connections.remove(connection->innovation());
        m_neurons[inID]->removeOutput(outID);
        m_neurons[outID]->removeInput(inID);

        connection->deleteLater();
    }
}

























