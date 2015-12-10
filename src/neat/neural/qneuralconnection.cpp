#include "qneuralconnection.h"
#include "../core/qneatcore.h"
#include "../core/qinnovation.h"
#define DEBUG
#ifdef DEBUG
#include <QDebug>
#endif

QNeuralConnection::QNeuralConnection(int input, int output, double weight, QObject *parent) : QObject(parent)
{
    m_input = input;
    m_output = output;
    m_weight = weight;
    m_enabled = true;
    m_innovation = QNeatCore::instance().getInnovationNumber(QInnovation(input, output));
    //qDebug() << "Connection - " << m_input << " to " << m_output << " - " << m_innovation;
}

QNeuralConnection::~QNeuralConnection()
{
#ifdef DEBUG
    //qDebug() << "Deleting connection - " << m_input << " to " << m_output;
#endif
}

bool QNeuralConnection::enabled() const
{
    return m_enabled;
}

int QNeuralConnection::input() const
{
    return m_input;
}

int QNeuralConnection::output() const
{
    return m_output;
}

double QNeuralConnection::weight() const
{
    return m_weight;
}

int QNeuralConnection::innovation() const
{
    return m_innovation;
}

void QNeuralConnection::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void QNeuralConnection::setInput(int input)
{
    m_input = input;
}

void QNeuralConnection::setOutput(int output)
{
    m_output = output;
}

void QNeuralConnection::setWeight(double weight)
{
    m_weight = weight;
}
