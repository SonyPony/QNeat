TEMPLATE = app
CONFIG += c++11
QT += core testlib
QT -= gui
CONFIG += console
CONFIG -= app_bundle
TARGET = ../QNeat

SOURCES += main.cpp \
    neat/genetic/cells/qchromosome.cpp \
    neat/genetic/cells/qgene.cpp \
    neat/neural/qneuralconnection.cpp \
    neat/core/qinnovation.cpp \
    neat/core/qneatcore.cpp \
    neat/neural/qneuron.cpp \
    neat/core/qextendedmath.cpp \
    neat/neural/qneuronvalue.cpp \
    neat/neural/qneuralnetwork.cpp \
    #tests/neat/qneurontests.cpp \
    #tests/neat/qneuralnetworktests.cpp
    neat/genetic/cells/qchromosomeanalyzer.cpp \
    neat/genetic/factors/qmutator.cpp \
    neat/genetic/factors/qcrossover.cpp \
    neat/genetic/containers/qspecies.cpp \
    neat/genetic/containers/qspeciespool.cpp \
    tests/neat/qchromosomeanalyzertests.cpp


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include("../deployment.pri")

unix {
    QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
    QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0


    LIBS += \
        -lgcov
}

HEADERS += \
    neat/genetic/cells/qchromosome.h \
    neat/genetic/cells/qgene.h \
    neat/neural/qneuralconnection.h \
    neat/qneatsettings.h \
    neat/core/qinnovation.h \
    neat/core/qneatcore.h \
    neat/neural/qneuron.h \
    neat/core/qextendedmath.h \
    neat/neural/qneuronvalue.h \
    neat/neural/qneuralnetwork.h \
    #tests/neat/qneurontests.h \
    #tests/neat/qneuralnetworktests.h
    neat/genetic/cells/qchromosomeanalyzer.h \
    neat/genetic/factors/qmutator.h \
    neat/genetic/factors/qcrossover.h \
    neat/genetic/containers/qspecies.h \
    neat/genetic/containers/qspeciespool.h \
    tests/neat/qchromosomeanalyzertests.h \
    tests/tests.h
