#ifndef QNEURALNETWORK_H
#define QNEURALNETWORK_H

#include <QObject>
#include "qneuron.h"
#include "qneuralconnection.h"
#include <QMap>

class QNeuralNetwork : public QObject
{
        Q_OBJECT
    private:
        QMap<int, QNeuron*> m_neurons;
        QMap<int, QNeuralConnection*> m_connections;
        int m_neuronsCount;

        QSet<int> expandLayerPaths(QSet<int> layer);
        // just for checking recurrent connection
        QSet<int> removeOutputNeuronsFromLayer(QSet<int> layer);

    protected:
        int lastNeuronID();

        void removeNeuron(int neuronID);
        void removeConnection(QNeuralConnection* connection);

    public:
        QNeuralNetwork(QObject *parent = 0);
        ~QNeuralNetwork();
        QList<int> neuronsIndexes();
        QMap<int, QNeuralConnection*> connections() const;
        bool hasNeuron(int neuronID) const;
        QNeuron* neuron(int neuronID) const;
        QList<QNeuron*> neurons() const;

        void addNeuron(QNeuron* neuron, int id = -1);
        void addConnection(QNeuralConnection* connection);
        QList<QNeuronValue> evaluate(QList<double> inputs);
        bool checkReccurentConnection(QNeuralConnection* connection);
        int randomNeuron(int excludeTypes);
};

#endif // QNEURALNETWORK_H
