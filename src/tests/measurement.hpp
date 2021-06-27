#pragma once

TEST(Measurement, Collapse) {
  for (int i = 0; i < 100; i++) {
    std::srand(i);

    qs::Circuit circuit(1);
    circuit.apply(qs::Gates::H, 0);
    EXPECT_EQ(circuit.measure(), circuit.measure());
  }
}

TEST(Measurement, Partial) {
  for (int i = 0; i < 100; i++) {
    std::srand(i);

    qs::Circuit circuit(3);
    circuit.apply(qs::Gates::H, 0);
    circuit.apply(qs::Gates::H, 2);
    circuit.apply(qs::Gates::CNOT, 0);
    unsigned long first = circuit.measure(2, 1);
    EXPECT_EQ(first, (circuit.measure(0, 2) & 0b10) >> 1);
  }
}