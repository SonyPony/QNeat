#ifndef QNEATSETTINGS_H
#define QNEATSETTINGS_H

class QNeatSettings
{
    public:
        static constexpr int Population = 300;

        // IO
        static constexpr int InputsCount = 2;
        static constexpr int OutputsCount = 1;

        // Settings to determinate chromosomes compability to divide them into species
        static constexpr double ExcessCoefficient = 1.0;
        static constexpr double DisjointCoefficient = 1.0;
        static constexpr double WeightCoefficient = 1.5;
        static constexpr double DeltaThreshold = 1;

        // mutations chances and
        // chance, that property will inherit parent property, crossover chances etc...
        static constexpr double ChromosomeMutationChance = 0.8;
        static constexpr double PerturbChance = 0.9;
        static constexpr double SetRandomWeightChance = 0.1;
        static constexpr double DisableGeneIfInheritDisable = 0.75;
        static constexpr double ChromosomeCrossoverChance = 0.75;
        static constexpr double NewLinkChance = 0.06;
        static constexpr double newNodeChance = 0.03;
        static constexpr double DisableGeneChance = 0.04;
        static constexpr double EnableGeneChance = 0.02;
        static constexpr double InterSpeciesCrossoverChange = 0.04;

        static constexpr double MutationStep = 0.1;
};

#endif // QNEATSETTINGS_H
