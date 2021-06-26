#pragma once

TEST(Entanglement, TwoQubit) {
  for (int i = 0; i < 100; i++) {
    std::srand(i);

    qs::Circuit circuit(2);
    circuit.apply(qs::Gates::H, 1);
    circuit.apply(qs::Gates::CNOT, 0);
    unsigned long res = circuit.measure();
    EXPECT_EQ(res & 0b1, (res & 0b10) >> 1);
  }
}

TEST(Entanglement, ThreeQubit) {
  for (int i = 0; i < 100; i++) {
    std::srand(i);

    qs::Circuit circuit(3);
    circuit.apply(qs::Gates::H, 2);
    circuit.apply(qs::Gates::CNOT, 1);
    circuit.apply(qs::Gates::CNOT, 0);
    unsigned long res = circuit.measure();
    EXPECT_EQ(res & 0b1, (res & 0b10) >> 1);
    EXPECT_EQ(res & 0b1, (res & 0b100) >> 2);
  }
}