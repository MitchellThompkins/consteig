---
title: Butterworth Filter Design
---

# Digital Filter Design

Embedded filter design typically requires an offline tool like MATLAB or Python
to compute coefficients, which then get hardcoded into source. With consteig,
the transfer function lives in source and the compiler derives the coefficients
directly. consteig can automatically generate IIR digital filter
coefficients from a time-domain transfer function. They are derived at
compile-time and can be saved for use in the filter step when the actual
filtering takes place. Consider the 2nd-order Butterworth low-pass transfer
function in the continuous time domain:

$$H(s) = \frac{\omega_c^2}{s^2 + \sqrt{2}\,\omega_c\,s + \omega_c^2}$$

Instead of symbolically transforming $H(s)$ to $H(z)$ with the Z-transform, we
use the Matched Z-Transform (matched-pole mapping):

1. Read the denominator coefficients of $H(s)$ directly into the companion-form state-space matrix $A_c$.
2. Use consteig to find the continuous-time poles (eigenvalues of $A_c$).
3. Map these poles directly to the Z-domain using $z = e^{sT}$ (Matched Z-Transform).
4. Reconstruct the digital filter's characteristic polynomial from the mapped poles.

The [`butterworth_core.hpp`](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/butterworth/butterworth_core.hpp)
example generates the following design for a 100Hz cutoff at 1kHz sampling:

```bash
./build/bin/butterworth.main
```

```text
Designing 2nd Order Butterworth Lowpass Filter
Cutoff: 100.0000 Hz, Sampling Rate: 1000.0000 Hz
Continuous System Matrix A:
[ 0.0000, 1.0000 ]
[ -394784.1760, -888.5766 ]

Continuous-time Poles (Eigenvalues of A):
p1 = -444.2883 + j444.2883
p2 = -444.2883 + j-444.2883

Discrete-time Poles (Mapped via z = exp(sT)):
z1 = 0.5790 + j0.2756
z2 = 0.5790 + j-0.2756

Filter Coefficients (Denominator):
a1 = -1.1580
a2 = 0.4112
Gain K = 0.2532

Final Digital Filter Difference Equation:
y[n] = 0.2532 * x[n] - (-1.1580) * y[n-1] - (0.4112) * y[n-2]
```

## Verifying Compile-Time Evaluation

The compile-time nature of this library can be verified directly. The filter
data is compiled into a special `.filter_data` section of the binary, which
can be extracted to confirm that the coefficients are calculated entirely at
compile-time.

```bash
make butterworth.main
objcopy -O binary -j .filter_data build/examples/CMakeFiles/butterworth.main.dir/butterworth/butterworth_values.cpp.o filter_data.bin
xxd -c 8 filter_data.bin
```

```text
00000000: 0000 0000 0040 8f40  .....@.@
00000008: 0000 0000 0000 5940  ......Y@
00000010: 0000 0000 0000 0000  ........
00000018: 0000 0000 0000 f03f  .......?
00000020: 4ac4 44b4 8018 18c1  J.D.....
00000028: 84ea f9d9 9cc4 8bc0  ........
00000030: 84ea f9d9 9cc4 7bc0  ......{.
00000038: 85ea f9d9 9cc4 7b40  ......{@
00000040: 84ea f9d9 9cc4 7bc0  ......{.
00000048: 85ea f9d9 9cc4 7bc0  ......{.
00000050: 76a7 3023 5b87 e23f  v.0#[..?
00000058: 0c13 dc5a f5a3 d13f  ...Z...?
00000060: 76a7 3023 5b87 e23f  v.0#[..?
00000068: 0c13 dc5a f5a3 d1bf  ...Z....
00000070: 76a7 3023 5b87 f2bf  v.0#[...
00000078: afc5 de84 c451 da3f  .....Q.?
00000080: d727 1cf8 5734 d03f  .'..W4.?
```

```bash
python3 examples/butterworth/print_butterworth_binary.py
```

```text
Name         Double (64-bit)      Decimal
----------------------------------------------------------------------
fs           0000 0000 0040 8f40  1000.000000000000000
fc           0000 0000 0000 5940  100.000000000000000
A_c[0][0]    0000 0000 0000 0000  0.000000000000000
A_c[0][1]    0000 0000 0000 f03f  1.000000000000000
A_c[1][0]    4ac4 44b4 8018 18c1  -394784.176043574349023
A_c[1][1]    84ea f9d9 9cc4 8bc0  -888.576587631673192
p1_real      84ea f9d9 9cc4 7bc0  -444.288293815836596
p1_imag      85ea f9d9 9cc4 7b40  444.288293815836653
p2_real      84ea f9d9 9cc4 7bc0  -444.288293815836596
p2_imag      85ea f9d9 9cc4 7b40  444.288293815836653
z1_real      76a7 3023 5b87 e23f  0.579022949915482
z1_imag      0c13 dc5a f5a3 d13f  0.275632227640288
z2_real      76a7 3023 5b87 e23f  0.579022949915482
z2_imag      0c13 dc5a f5a3 d1bf  -0.275632227640288
a1           76a7 3023 5b87 f2bf  -1.158045899830964
a2           afc5 de84 c451 da3f  0.411240701442774
K            d727 1cf8 5734 d03f  0.253194801611810
```
