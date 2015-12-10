#include "qextendedmath.h"
#include <qmath.h>

double QExtendedMath::qSigmoid(double value)
{
    return 2. / (1. + qExp(-4.9 * value)) - 1.;
}
