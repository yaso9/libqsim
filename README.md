# libqsim

A GPU accelerated quantum computing simulator library.

## Dependencies
- ArrayFire

## Usage
The `qs::Circuit` class represents a quantum circuit. The constructor takes the number of qubits in the circuit as a parameter. Quantum gates (represented by the `qs::Gate` class) can be added to a circuit using the `qs::Circuit::apply` method, which takes the gate and the index of the qubit to apply it to. Some common gates can be found in the `qs::Gates` namespace. Custom gates can be created by instantiating the `qs::Gate` class and passing the constructor the number of qubits the gate acts on and the complex unitary matrix representing the gate as an ArrayFire `af::array`. The probabilities of different measurement outcomes can be found using the `qs::Circuit::props` method. Measurement, including partial measurement, can be simulated using the `qs::Circuit::measure` method, which optionally takes the start index of the qubits to measure and the number of qubits to measure as parameters. `qs::Circuit::measure` selects an outcome using weighted random selection with `std::rand`, which can be seeded using `std::srand`.

## Examples
Examples can be found in the `src/tests` directory.

## Building
The build process requires CMake and Ninja.

```bash
cmake -S . -B build -G Ninja # For debug builds add "-DCMAKE_BUILD_TYPE=Debug"
ninja -C build
```

## Testing
After building, run `ctest --test-dir build` to run tests.