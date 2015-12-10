#include "qneuron.h"
#include "../core/qextendedmath.h"

#ifdef DEBUG
#include <QDebug>
#endif

QNeuron::QNeuron(int type, int id, QObject *parent): QObject(parent)
{
    m_type = type;
    m_ID = id;
}

QNeuron::~QNeuron()
{
#ifdef DEBUG
    qDebug() << "Deleting neuron - " << m_ID;
#endif
}

void QNeuron::setValue(double v)
{
    Q_ASSERT_X(m_type == QNeuron::Input, "QNeuron type", "Expected Input neuron type");

    m_value = v;
}

QNeuronValue QNeuron::value()
{
    if(m_type == QNeuron::Bias)
        return QNeuronValue(1.);
    else if(m_type == QNeuron::Input)
        return QNeuronValue(m_value);
    else if(m_inputNeurons.size() == 0)
        return QNeuronValue(QNeuronValue::None);
    else {
        double sum = 0;

        for(int key: m_inputNeurons.keys()) {
            QNeuronValue neuronValue(m_inputNeurons[key]->value());

            if(neuronValue.valid())
                sum += neuronValue.value() * m_inputWeights[key];
        }

        return QNeuronValue(QExtendedMath::qSigmoid(sum));
    }
}


QSet<int> QNeuron::inputs() const
{
    Q_ASSERT_X((m_type & (QNeuron::Input | QNeuron::Bias)) == 0, "QNeuron type", "Input and bias can not have inputs");

    return m_inputs;
}

QSet<int> QNeuron::outputs() const
{
    Q_ASSERT_X(m_type != QNeuron::Output, "QNeuron type", "Output neuron can not have outputs");

    return m_outputs;
}

int QNeuron::type() const
{
    return m_type;
}

QString QNeuron::strType() const
{
    if(m_type == QNeuron::Input)
        return QStringLiteral("Input");
    else if(m_type == QNeuron::Output)
        return QStringLiteral("Output");
    else if(m_type == QNeuron::Hidden)
        return QStringLiteral("Hidden");
    else if(m_type == QNeuron::Bias)
        return QStringLiteral("Bias");
}

QMap<int, double> QNeuron::inputWeights() const
{
    return m_inputWeights;
}

QMap<int, QNeuron*> QNeuron::inputNeurons() const
{
    return m_inputNeurons;
}

int QNeuron::ID() const
{
    return m_ID;
}

void QNeuron::setID(int ID)
{
    m_ID = ID;
}

void QNeuron::addInput(int inputNeuronID)
{
    m_inputs.insert(inputNeuronID);
}

void QNeuron::addOutput(int outputNeurontID)
{
    m_outputs.insert(outputNeurontID);
}

void QNeuron::removeInput(int inputNeuronID)
{
    m_inputs.remove(inputNeuronID);
}

void QNeuron::removeOutput(int outputNeurontID)
{
    m_outputs.remove(outputNeurontID);
}

void QNeuron::addInputNeuron(QNeuron *neuron)
{
    m_inputNeurons.insert(neuron->ID(), neuron);
}

void QNeuron::addInputWeight(int ID, double weight)
{
    m_inputWeights.insert(ID, weight);
}

void QNeuron::clearAll()
{
    m_inputWeights.clear();
    m_inputNeurons.clear();
}

