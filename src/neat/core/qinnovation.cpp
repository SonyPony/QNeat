#include "qinnovation.h"

QInnovation::QInnovation(int input, int output, int innovationNumber):
    m_input(input), m_output(output), m_innovationNumber(innovationNumber)
{

}

int QInnovation::input() const
{
    return m_input;
}

int QInnovation::output() const
{
    return m_output;
}

int QInnovation::innovation() const
{
    return m_innovationNumber;
}

bool QInnovation::operator==(const QInnovation other)
{
    if(m_input == other.input() && m_output == other.output())
        return true;
    else
        return false;
}

