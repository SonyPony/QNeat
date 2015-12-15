#ifndef QNEATCORE_H
#define QNEATCORE_H

#include <QObject>
#include <QList>
#include "qinnovation.h"
#include "../genetic/cells/qchromosome.h"

class QNeatCore : public QObject
{
        Q_OBJECT
    private:
        QList<QInnovation> m_innovations;

        int addNewInnovation(QInnovation innovation);
        int checkExistingInnovations(QInnovation innovation);

    public:
        QNeatCore(QObject *parent = 0);

        static QNeatCore& instance();
        static double randomNumber();
        int getInnovationNumber(QInnovation innovation);

        static QList<QGene*> sortGenes(QList<QGene*> genes);
        static QString addColor(QString text, QString color);
        static QString formatGenes(QList<QGene*> genes);
        static QString formatGenes(QChromosome* chromosome, bool colored = true);
};

#endif // QNEATCORE_H
