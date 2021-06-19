#include <arrayfire.h>
#include <assert.h>
#include <functional>
#include <memory>
#include <numeric>

#include "Circuit.hpp"
#include "utils.hpp"

Circuit::Circuit(size_t size) : size(size) {
  this->amps = af::complex(af::constant(0, (dim_t)pow(2, size)));
  this->amps(0) = 1;
}

void Circuit::apply(Gate gate, size_t qubit) {
  assert(this->size - qubit >= gate.size);

  // Resize the gate matrix so it fits the circuit
  af::array sized_gate = af::constant(1, 1, 1);
  af::array identity2x2 = af::identity(2, 2);
  for (unsigned long i = 0; i < this->size - qubit - gate.size; i++) {
    sized_gate = kron(sized_gate, identity2x2);
  }
  sized_gate = kron(sized_gate, gate.mat);
  for (unsigned long i = 0; i < qubit; i++) {
    sized_gate = kron(sized_gate, identity2x2);
  }

  // Apply the gate
  this->amps = af::matmul(sized_gate, this->amps);
}

af::array Circuit::probs() const { return af::abs(af::pow(this->amps, 2)); }

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
  const std::unique_ptr<std::vector<float>> allProbs =
      std::make_unique<std::vector<float>>(this->amps.elements());
  this->probs().host(allProbs->data());

  // Determine the probability of measuring each state the bits that are being
  // measured can be in
  const std::unique_ptr<std::vector<float>> measureProbs =
      std::make_unique<std::vector<float>>(nProbs, 0);
  for (unsigned long i = 0; i < pow(2, this->size); i++) {
    (*measureProbs)[(i & bitmask) >> start] += (*allProbs)[i];
  }

  // Pick one of the states at random weighted by their probability
  const std::unique_ptr<std::vector<float>> measureProbsCumSum =
      std::make_unique<std::vector<float>>(nProbs, 0);
  std::partial_sum(measureProbs->begin(), measureProbs->end(),
                   measureProbsCumSum->begin(), std::plus<float>());
  float rand = ((float)std::rand() / RAND_MAX) * measureProbsCumSum->back();
  unsigned long res = 0;
  for (int i = 0; i < measureProbsCumSum->size(); i++) {
    if ((*measureProbsCumSum)[i] > rand) {
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