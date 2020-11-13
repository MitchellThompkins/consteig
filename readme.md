# Overview

This is a templated constexpr library which will:
* Perform mathematical operations and transforms on matrices
* Compute the QR decomposition of a square matrix
* **Compute the eigenvalues for a square diagonal matrix**(Not yet)

# TODO

* Remove dependency on qr decomposition only solving for square matrices
* Add matrix operation to solve determinant
* Verify numerical stability of MGS implementation
  [Reference](https://www.math.tamu.edu/~yvorobet/MATH304-503/Lect3-07web.pdf)
* Add more robust test cases
