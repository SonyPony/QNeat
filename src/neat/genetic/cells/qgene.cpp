#include "qgene.h"
#include "../../core/qinnovation.h"
#include "../../core/qneatcore.h"
#include <QDebug>

QGene::QGene(const QNeuralConnection *connection, QObject *parent):
    QGene(connection->input(), connection->output(), connection->weight(), parent)
{
    //m_innovation = connection->innovation();
    m_enabled = connection->enabled();
}

QGene::QGene(int input, int output, double weight, QObject *parent):
    m_input(input), m_output(output), m_weight(weight), QObject(parent)
{
    m_innovation = QNeatCore::instance().getInnovationNumber(QInnovation(input, output));
    m_enabled = true;
}

QGene::QGene(const QGene *other, QObject *parent): QObject(parent)
{
    m_enabled = other->m_enabled;
    m_input = other->m_input;
    m_output = other->m_output;
    m_weight = other->m_weight;
    m_innovation = other->m_innovation;
}

bool QGene::enabled() const
{
    return m_enabled;
}

int QGene::input() const
{
    return m_input;
}

int QGene::output() const
{
    return m_output;
}

double QGene::weight() const
{
    return m_weight;
}

int QGene::innovation() const
{
    return m_innovation;
}

QString QGene::toString()
{
    QString firstLine = QString("| Gene %1 (%2) |")
            .arg(m_innovation).
            arg(QString::number(m_weight, 'f', 2));
    QString secondLine = QString("| %1 -> %2")
            .arg(m_input)
            .arg(m_output);

    secondLine.append((QString(" ").repeated(firstLine.length() - secondLine.length() - 2))).append(" |");
    return firstLine + "\n" + secondLine;
}

QNeuralConnection* QGene::toConnection()
{
    QNeuralConnection* connection = new QNeuralConnection(m_input, m_output, m_weight);
    connection->setEnabled(m_enabled);

    return connection;
}

void QGene::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void QGene::setInput(int input)
{
    m_input = input;
}

void QGene::setOutput(int output)
{
    m_output = output;
}

void QGene::setWeight(double weight)
{
    m_weight = weight;
}
