# parameterized-vertex-cover
Simple demonstration of a basic parameterization for vertex cover with respect to solution size

# instructions

Build with `cmake -S . -B build && cmake --build build`

Run tests with `ctest`

Profiling with `build/param_vc && gprof build/param_vc build/gmon.out`

Build release (performant) version with the cmake option `-DCMAKE_BUILD_TYPE=Release`
