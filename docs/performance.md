---
title: Performance
---

# Performance

Compile times and peak compiler memory are measured by compiling individual
`static constexpr auto result = eigenvalues(mat)` expressions across matrix sizes
and categories. Each data point is the mean over 10 randomly generated matrices
of that type. Exact numbers will vary by CPU, compiler version, and system load,
but one should expect the trend to be consistent.

The shaded region indicates where `consteig_raise_compiler_limits` was needed.
Below that boundary the default compiler constexpr budget was sufficient.

![Compilation Times](profiling/results/compile_times_gcc_15.2.0.png "compilation_times")
![Compilation Memory](profiling/results/compile_times_gcc_15.2.0_memory.png "compilation_memory")

See [profiling/README.md](https://github.com/mitchellthompkins/consteig/blob/develop/profiling/README.md)
for a discussion on generation of these plots.
