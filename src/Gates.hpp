#pragma once

#include <arrayfire.h>

#include "Gate.hpp"

namespace Gates {
    namespace {
        const float H_mat[] = {1/sqrt(2), 1/sqrt(2), 1/sqrt(2), -1/sqrt(2)};
        const float X_mat[] = {0, 1, 1, 0};
        const float CNOT_mat[] = {1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0};
    }

    const Gate H = Gate(1, af::complex(af::array(2,2, H_mat)));
    const Gate X = Gate(1, af::complex(af::array(2,2, X_mat)));
    const Gate CNOT = Gate(2, af::complex(af::array(4, 4, CNOT_mat)));
}