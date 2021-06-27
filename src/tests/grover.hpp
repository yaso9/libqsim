#pragma once

// Circuit from https://qiskit.org/textbook/ch-algorithms/grover.html
TEST(Grover, TwoQubit) {
  for (int i = 0; i < 100; i++) {
    std::srand(i);

    qs::Circuit circuit(2);
    circuit.apply(qs::Gates::H, 0);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::CNOT, 0);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::H, 0);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::Z, 0);
    circuit.apply(qs::Gates::Z, 1);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::CNOT, 0);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::H, 0);
    circuit.apply(qs::Gates::H, 1);
    EXPECT_EQ(circuit.measure(), 0b11);
  }
}