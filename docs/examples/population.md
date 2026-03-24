title: Population Flow Example

# Population Flow

This example demonstrates using `eigenvalues` to find the steady-state distribution of a population model. Full source: [`examples/population.cpp`](https://github.com/mitchellthompkins/consteig/blob/develop/examples/population.cpp).

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

Running the example produces:

```
Eigenvalues (lambda):
1
0.75

Steady-State Population Flow (Total: 1000000):
Seattle:  800000 (80.00%)
Portland: 200000 (20.00%)
```

The steady-state splits the population 80/20 — determined entirely at compile time.
