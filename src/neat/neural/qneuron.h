#ifndef QNEURON_H
#define QNEURON_H

#include <QObject>
#include <QMap>
#include "qneuronvalue.h"
#include <QSet>

class QNeuron : public QObject
{
        Q_OBJECT
    private:
        int m_type;
        QMap<int, double> m_inputWeights;
        QMap<int, QNeuron*> m_inputNeurons;
        int m_ID;
        double m_value;
        QSet<int> m_inputs;
        QSet<int> m_outputs;

    public:
        QNeuron(int type, int id = -1, QObject *parent = 0);
        ~QNeuron();

        enum NeuronType {
            Output = 1,
            Input = 2,
            Bias = 4,
            Hidden = 8
        };

        QNeuronValue value();
        int type() const;
        QString strType() const;
        QMap<int, double> inputWeights() const;
        QMap<int, QNeuron *> inputNeurons() const;
        int ID() const;
        QSet<int> inputs() const;
        QSet<int> outputs() const;

    public Q_SLOTS:
        void setValue(double v);
        void clearAll();
        void setID(int ID);
        void addInput(int inputNeuronID);
        void addOutput(int outputNeurontID);
        void removeInput(int inputNeuronID);
        void removeOutput(int outputNeurontID);
        void addInputNeuron(QNeuron* neuron);
        void addInputWeight(int neuronID, double weight);
};

#endif // QNEURON_H
