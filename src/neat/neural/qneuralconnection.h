#ifndef QNEURALCONNECTION_H
#define QNEURALCONNECTION_H

#include <QObject>

class QNeuralConnection : public QObject
{
        Q_OBJECT
    private:
        bool m_enabled;
        int m_input;
        int m_output;
        double m_weight;
        int m_innovation;

    public:
        QNeuralConnection(int input, int output, double weight, QObject *parent = 0);
        ~QNeuralConnection();

        bool enabled() const;
        int input() const;
        int output() const;
        double weight() const;
        int innovation() const;

    public Q_SLOTS:
        void setEnabled(bool enabled);
        void setInput(int input);
        void setOutput(int output);
        void setWeight(double weight);
};

#endif // QNEURALCONNECTION_H
