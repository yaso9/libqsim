#pragma once

#include <arrayfire.h>

#include "Gate.hpp"

class Circuit {
public:
    const size_t size;
    af::array amps;

    Circuit(size_t size);

    void apply(Gate gate, size_t qubit);

    af::array probs() const;

    unsigned long measure(size_t start = 0, size_t n = 0);
};