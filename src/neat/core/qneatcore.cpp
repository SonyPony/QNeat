#include "qneatcore.h"
#include <stdlib.h>
#include <QDebug>

int QNeatCore::addNewInnovation(QInnovation innovation)
{
    m_innovations.append(QInnovation(innovation.input(), innovation.output(), m_innovations.length() + 1));

    return m_innovations.last().innovation();
}

int QNeatCore::checkExistingInnovations(QInnovation innovation)
{
    for(QInnovation i: m_innovations) {
        if(i == innovation)
            return i.innovation();
    }

    // means not existing innovation
    return -1;
}

QNeatCore::QNeatCore(QObject *parent) : QObject(parent) {}

QNeatCore &QNeatCore::instance()
{
    static QNeatCore instance;
    return instance;
}

double QNeatCore::randomNumber()
{
    return qrand() / (double)(RAND_MAX);
}

int QNeatCore::getInnovationNumber(QInnovation innovation)
{
    int existingInnovation = QNeatCore::instance().checkExistingInnovations(innovation);

    // new innovation
    if(existingInnovation == -1)
        return QNeatCore::instance().addNewInnovation(innovation);
    else
        return existingInnovation;
}

QList<QGene *> QNeatCore::sortGenes(QList<QGene *> genes)
{
    qSort(genes.begin(), genes.end(), [](QGene* a, QGene* b) -> bool {
        return a->innovation() < b->innovation();
    });

    return genes;
}

QString QNeatCore::addColor(QString text, QString color)
{
    return text.prepend(QStringLiteral("\033[0;") + color).append("\033[0;0m");
}

QString QNeatCore::formatGenes(QList<QGene *> genes) {
    QString firstLine("");
    QString secondLine("");
    QString color;

    for(QGene* gene: QNeatCore::sortGenes(genes)) {
        QStringList temp = gene->toString().split("\n");

        if(!gene->enabled())
            color = "31m";  // lime
        else
            color = "32m";  // red

        firstLine.append(QNeatCore::addColor(temp[0], color));
        secondLine.append(QNeatCore::addColor(temp[1], color));
    }

    return firstLine + "\n" + secondLine;
}

QString QNeatCore::formatGenes(QChromosome *chromosome, bool colored) {
    QString firstLine("");
    QString secondLine("");
    QString color;

    for(QGene* gene: QNeatCore::sortGenes(chromosome->genes().toList())) {
        QStringList temp = gene->toString().split("\n");

        if(colored) {
            if(!gene->enabled())
                color = "31m";
            else
                color = "32m";

            firstLine.append(QNeatCore::addColor(temp[0], color));
            secondLine.append(QNeatCore::addColor(temp[1], color));
        }

        else {
            firstLine.append(temp[0]);
            secondLine.append(temp[1]);
        }
    }

    return firstLine + "\n" + secondLine + "\n";
}

