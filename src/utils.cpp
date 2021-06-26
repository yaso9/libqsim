#include <arrayfire.h>

__attribute__((visibility("hidden"))) af::array kron(af::array &lhs,
                                                     af::array &rhs) {
  // This isn't very readable, but it implements the kronecker product with GPU
  // acceleration
  af::array arr = af::tile(lhs, rhs.dims(1));
  arr = af::moddims(arr, lhs.dims(0), lhs.dims(1) * rhs.dims(1)).T();
  arr = af::tile(arr, rhs.dims(0));
  arr = af::moddims(arr, lhs.dims(1) * rhs.dims(1), lhs.dims(0) * rhs.dims(0))
            .T();
  return arr * af::tile(rhs, lhs.dims());
}