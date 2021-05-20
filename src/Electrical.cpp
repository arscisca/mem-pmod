#include "Electrical.h"

DataVector vectorize(const PULParameters &parameters) {
    return {parameters.Rsdc, parameters.Rsdc, parameters.Ls,
            parameters.Rpdc, parameters.Rpac, parameters.Lp, parameters.Cp};
}

PULParameters devectorize(const DataVector &data) {
    return {data[0], data[1], data[2], data[3], data[4], data[5], data[6]};
}