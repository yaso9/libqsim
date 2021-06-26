#pragma once

#include <arrayfire.h>

namespace qs {
class Gate {
public:
  const size_t size;
  af::array mat;

  Gate(size_t size, af::array mat);
};
} // namespace qs
