#include <arrayfire.h>
#include <iostream>

#include "Circuit.hpp"
#include "Gates.hpp"

int main(int argc, char **argv) {
  for (int i = 0; i < 10; i++) {
    std::srand(i);
    Circuit c(3);
    c.apply(Gates::H, 0);
    c.apply(Gates::H, 2);
    c.apply(Gates::CNOT, 1);
    std::cout << c.measure(1) << std::endl;
    std::cout << c.measure() << std::endl;
    std::cout << std::endl;
  }

  return 0;
}