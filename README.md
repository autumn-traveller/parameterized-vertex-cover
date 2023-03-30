# parameterized-vertex-cover
Simple demonstration of a basic parameterization for vertex cover with respect to solution size

# instructions

Build with `cmake -S . -B build && cmake --build build`

Run tests with `cd build && ctest`

Profiling with `build/param_vc && gprof build/param_vc build/gmon.out`

Build release version (more performant) with the cmake option `-DCMAKE_BUILD_TYPE=Release`

# still to do

visualise solutions with Qt
verify functionality
gprof optimization
logic to parse in graphs from a file
