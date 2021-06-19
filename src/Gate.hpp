#pragma once

#include <arrayfire.h>

class Gate {
public:
    const size_t size;
    af::array mat;

    Gate(size_t size, af::array mat);
};
