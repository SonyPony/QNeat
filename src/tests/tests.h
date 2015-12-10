#ifndef TESTS_H
#define TESTS_H

#include <QObject>
#include <QDebug>
#include <QTest>
#include "neat/qchromosomeanalyzertests.h"

namespace Tests {
    template<typename sT, typename... T>
    int runTests(T... args)
    {
        int result = 0;

        sT a[] = {args...};
        for(sT b: a)
            result |= QTest::qExec(b);
        return result;
    }

    int run()
    {
        QChromosomeAnalyzerTests ChromAnTests;

        return Tests::runTests<QObject*>(
                    &ChromAnTests);
        return 0;
    }
}

#endif // TESTS_H
