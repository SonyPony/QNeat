#ifndef QCHROMOSOMEANALYZERTESTS_H
#define QCHROMOSOMEANALYZERTESTS_H

#include <QObject>
#include <QTest>
#include "../../neat/genetic/cells/qchromosomeanalyzer.h"

class QChromosomeAnalyzerTests : public QObject
{
        Q_OBJECT
    private Q_SLOTS:
        void testDisjointAndExcess();
};

#endif // QCHROMOSOMEANALYZERTESTS_H
