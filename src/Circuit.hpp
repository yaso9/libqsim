#pragma once

#include <arrayfire.h>
#include <memory>
#include <optional>
#include <queue>

#include "Gate.hpp"

namespace qs {
class Circuit {
private:
  af::array amps;
  std::vector<std::queue<std::optional<qs::Gate>>> operations;
  size_t get_operations_queue_size(size_t begin = 0, size_t end = 0) const;

public:
  const size_t size;

  Circuit(size_t size);

  void apply(Gate gate, size_t qubit);

  void update();

  af::array get_amps();

  af::array probs();

  unsigned long measure(size_t start = 0, size_t n = 0);
};
} // namespace qs