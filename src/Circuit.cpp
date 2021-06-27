#include <arrayfire.h>
#include <assert.h>
#include <functional>
#include <memory>
#include <numeric>

#include "Circuit.hpp"
#include "Gates.hpp"
#include "utils.hpp"

namespace qs {
Circuit::Circuit(size_t size)
    : size(size), amps(af::complex(af::constant(0, (dim_t)pow(2, size)))),
      operations(size, std::queue<std::optional<qs::Gate>>()) {
  this->amps(0) = 1;
}

size_t Circuit::get_operations_queue_size(size_t begin, size_t end) const {
  if (end == 0)
    end = operations.size();
  assert(end > begin);

  size_t size = 0;
  for (int idx = begin; idx < end; idx++) {
    size = std::max(size, operations[idx].size());
  }
  return size;
}

void Circuit::apply(Gate gate, size_t qubit) {
  assert(this->size - qubit >= gate.size);

  // Pad the queues for all the qubits this gate affects to the same length
  const size_t length =
      this->get_operations_queue_size(qubit, qubit + gate.size);
  for (int idx = qubit; idx < qubit + gate.size; idx++) {
    while (operations[idx].size() < length) {
      operations[idx].push(qs::Gates::I);
    }
  }

  // Add a blank space in all the additional qubits the gate uses
  for (int idx = qubit + 1; idx < qubit + gate.size; idx++) {
    operations[idx].push(std::nullopt);
  }

  // Add the gate to the operations queue
  operations[qubit].push(gate);
}

void Circuit::update() {
  // Clear the operations queue and update the amplitudes

  const size_t operations_queue_size = this->get_operations_queue_size();
  for (size_t i = 0; i < operations_queue_size; i++) {
    af::array timestep = af::constant(1, 1, 1);

    for (std::queue<std::optional<qs::Gate>> &queue : operations) {
      if (queue.empty()) {
        af::array I(qs::Gates::I.mat);
        timestep = kron(timestep, I);
        continue;
      }

      std::optional<qs::Gate> operation = queue.front();
      queue.pop();
      if (operation.has_value()) {
        timestep = kron(timestep, operation->mat);
      }
    }

    this->amps = af::matmul(timestep, this->amps);
  }
}

af::array Circuit::get_amps() {
  this->update();
  return this->amps;
}

af::array Circuit::probs() {
  this->update();
  return af::abs(af::pow(this->amps, 2));
}

unsigned long Circuit::measure(size_t start, size_t n) {
  // This algorithm isn't very pretty but it works
  // A bitmask is used to find the sum of the probability of measuring the
  // qubits being measured in each state they can be in Then one of these states
  // is chosen as the measurement result at random weighted by the probabilities
  // Then all the states in which the measured bits are not equal to the state
  // we measured have their amplitudes set to 0 and the state vector is
  // renormalized

  if (n == 0)
    n = this->size - start;
  assert(this->size - start >= n);

  // Create the bitmask and get the probabilities for measuring each state the
  // circuit can be in
  const size_t nProbs = pow(2, n);
  const unsigned long bitmask = (nProbs - 1) << start;
  std::vector<float> allProbs(this->amps.elements());
  this->probs().host(allProbs.data());

  // Determine the probability of measuring each state the bits that are being
  // measured can be in
  std::vector<float> measureProbs(nProbs, 0);
  for (unsigned long i = 0; i < pow(2, this->size); i++) {
    measureProbs[(i & bitmask) >> start] += allProbs[i];
  }

  // Pick one of the states at random weighted by their probability
  std::vector<float> measureProbsCumSum(nProbs, 0);
  std::partial_sum(measureProbs.begin(), measureProbs.end(),
                   measureProbsCumSum.begin(), std::plus<float>());
  float rand = ((float)std::rand() / RAND_MAX) * measureProbsCumSum.back();
  unsigned long res = 0;
  for (int i = 0; i < measureProbsCumSum.size(); i++) {
    if (measureProbsCumSum[i] > rand) {
      res = i;
      break;
    }
  }

  // Set the amplitudes for all states the measured bits are not in the state we
  // measured them to be in to 0
  this->amps(((af::range(af::dim4(this->amps.elements()), -1, u64) & bitmask) >>
              start) != res) = 0;
  this->amps /=
      af::norm(this->amps, AF_NORM_VECTOR_2); // Renormalize the state vector

  return res;
}
} // namespace qs