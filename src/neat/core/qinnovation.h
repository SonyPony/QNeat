#ifndef QINNOVATION_H
#define QINNOVATION_H

class QInnovation
{
    private:
        int m_input;
        int m_output;
        int m_innovationNumber;

    public:
        QInnovation(int input, int output, int innovationNumber = -1);

        int input() const;
        int output() const;
        int innovation() const;

        bool operator==(const QInnovation other);
};

#endif // QINNOVATION_H
