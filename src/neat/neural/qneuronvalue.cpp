#include "qneuronvalue.h"

QNeuronValue::QNeuronValue(int type)
{
    m_type = type;
}

QNeuronValue::QNeuronValue(double value)
{
    m_type = QNeuronValue::Valid;
    m_value = value;
}

double QNeuronValue::value() const
{
    Q_ASSERT_X(m_type == QNeuronValue::Valid, "QNeuron value", "QNeuron value is not valid");

    return m_value;
}

bool QNeuronValue::valid() const
{
    if(m_type == QNeuronValue::Valid)
        return true;
    return false;
}

