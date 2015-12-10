#ifndef QGENE_H
#define QGENE_H

#include <QObject>
#include "../../neural/qneuralconnection.h"

class QGene : public QObject
{
        Q_OBJECT

    private:
        bool m_enabled;
        int m_input;
        int m_output;
        double m_weight;
        int m_innovation;

    public:
        QGene(const QNeuralConnection* connection, QObject *parent = 0);
        QGene(int input, int output, double weight, QObject *parent = 0);
        QGene(const QGene* other, QObject *parent = 0);

        bool enabled() const;
        int input() const;
        int output() const;
        double weight() const;
        int innovation() const;

        QString toString();

        QNeuralConnection *toConnection();

    public Q_SLOTS:
        void setEnabled(bool enabled);
        void setInput(int input);
        void setOutput(int output);
        void setWeight(double weight);
};

#endif // QGENE_H
