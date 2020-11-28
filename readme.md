# Overview

This is a templated constexpr library which will:
* Perform mathematical operations and transforms on matrices
* Compute the QR decomposition of a square matrix
* **Compute the eigenvalues for a square diagonal matrix**(Not yet)

It does not rely on the standard C++ library.

# Important Notes
* Does not work with complex numbers

# TODO

* Remove dependency on gcem and any std library implementations
* Verify numerical stability of MGS implementation
  [Reference](https://www.math.tamu.edu/~yvorobet/MATH304-503/Lect3-07web.pdf)
* Add more robust test cases
* Add in -wall etc...
