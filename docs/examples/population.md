title: Population Flow Example

# Population Flow

If a system's transition matrix is fixed at compile-time, its steady-state
behavior never needs to be recomputed at runtime. Take the example from [Using
Eigenvectors to Find Steady State Population
Flows](https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764)
and apply it using consteig. The transition matrix is fixed at compile-time, so
the steady-state fractions can be stored as `static constexpr` values. At
runtime, computing the distribution for any total population is then a single
multiply. There is no need to iterate the transition matrix until it converges.

The [`population.cpp`](https://github.com/mitchellthompkins/consteig/blob/develop/examples/population.cpp)
example demonstrates this:

## Problem

Consider a simple two-city model where each year:
- 95% of Seattle's population stays, 5% moves to Portland
- 20% of Portland's population moves to Seattle, 80% stays

This gives a transition matrix:

```
A = [0.95  0.20]
    [0.05  0.80]
```

The steady-state distribution is the eigenvector corresponding to eigenvalue 1.

## Solution with consteig

```cpp
#include "consteig.hpp"

static constexpr consteig::Matrix<double, 2, 2> A{{
    {0.95, 0.20},
    {0.05, 0.80}
}};

// Eigenvalues computed at compile time
static constexpr auto eigs = consteig::eigenvalues(A);
// eigs(0,0).real ≈ 1.0   ← steady state
// eigs(1,0).real ≈ 0.75  ← transient mode

// Verify at compile time
static_assert(consteig::checkEigenValues(A, eigs, 1e-9), "Bad eigenvalues");
```

## Output

```text
Population Transition Matrix (A)
0.95 0.2
0.05 0.8

Initial Population Vector (u0)
500000
500000

Eigenvalues (lambda):
1
0.75

To find the steady-state population distribution, we need to find the eigenvector 'v'
that corresponds to the eigenvalue of 1 by solving (A - lambda*I)v = 0.

Steady-State Population Flow (Total: 1000000):
Seattle:  800000 (80.00%)
Portland: 200000 (20.00%)

Steady-State Population Flow (Total: 2500000):
Seattle:  2000000 (80.00%)
Portland: 500000 (20.00%)
```

The same compile-time eigenvectors drive both outputs; no matrix iteration happens at runtime.
