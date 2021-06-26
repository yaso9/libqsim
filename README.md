# libqsim

A GPU accelerated quantum computing simulator library.

## Dependencies
- ArrayFire

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