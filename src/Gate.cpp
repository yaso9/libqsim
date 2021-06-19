#include <assert.h>
#include <limits>

#include "Gate.hpp"

Gate::Gate(size_t size, af::array mat) : size(size), mat(mat) {
  // Assert mat is unitary

  // Check the size of the matrix and make sure it's square
  assert(mat.dims(0) == pow(2, size));
  assert(mat.dims(0) == mat.dims(1));

  // Check if mat dagger * mat = I
  // This comparison is annoying cause of floats and imaginary numbers
  const af::array should_be_ident = af::matmul(mat, mat, AF_MAT_CTRANS);
  const dim_t n_amps = (dim_t)pow(2, size);
  const af::array ident = af::identity(n_amps, n_amps);
  const af::array re_diffs = af::abs(af::real(should_be_ident) - ident);
  assert(
      af::allTrue<bool>(re_diffs < std::numeric_limits<float>::epsilon() * 10));
  assert(af::allTrue<bool>(af::abs(af::imag(should_be_ident)) <
                           std::numeric_limits<float>::epsilon() * 10));
}
