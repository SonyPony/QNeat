#ifndef QNEURONVALUE_H
#define QNEURONVALUE_H

#include <QObject>

class QNeuronValue
{
    private:
        double m_value;
        int m_type;

    public:
        QNeuronValue(int type);
        QNeuronValue(double value);

        enum ValueType {
            None,
            Valid,
        };

        double value() const;
        bool valid() const;
};

#endif // QNEURONVALUE_H
