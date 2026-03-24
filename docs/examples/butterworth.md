title: Butterworth Filter Design

# Butterworth Filter Design

This example designs a 2nd-order IIR Butterworth digital filter using compile-time eigenvalue computation, without performing a symbolic Z-transform or bilinear transform. Full source: [`examples/butterworth_core.hpp`](https://github.com/mitchellthompkins/consteig/blob/develop/examples/butterworth_core.hpp).

## Approach

Instead of symbolically transforming H(s) to H(z), the algorithm:

1. Defines the continuous-time state-space matrix A_c
2. Uses `eigenvalues` to find the continuous-time poles (eigenvalues of A_c)
3. Maps poles to the Z-domain via the matched Z-transform: z = e^(sT)
4. Reconstructs the digital filter's characteristic polynomial from the mapped poles

All steps happen at compile time.

## Key Code

```cpp
// Continuous-time state-space matrix for 2nd-order Butterworth at 100Hz / 1kHz
static constexpr consteig::Matrix<double, 2, 2> A_c{{
    {              0.0,          1.0},
    {-omega_n * omega_n, -2*zeta*omega_n}
}};

// Continuous-time poles (complex eigenvalues)
static constexpr auto ct_poles = consteig::eigenvalues(A_c);

// Map to discrete time: z = exp(s * T)
// z1 = exp(ct_poles(0,0) * T_s)
// z2 = exp(ct_poles(1,0) * T_s)

// Digital filter denominator coefficients
// a1 = -(z1.real + z2.real)
// a2 = z1.real * z2.real - z1.imag * z2.imag  (for conjugate pair)
```

## Output for 100Hz cutoff at 1kHz sampling

```
Continuous-time Poles:
p1 = -444.2883 + j444.2883
p2 = -444.2883 + j-444.2883

Discrete-time Poles:
z1 = 0.5790 + j0.2756
z2 = 0.5790 + j-0.2756

Filter Coefficients:
a1 = -1.1580
a2 =  0.4112
K  =  0.2532

Digital Filter: y[n] = 0.2532*x[n] + 1.1580*y[n-1] - 0.4112*y[n-2]
```

## Verifying Compile-Time Evaluation

The filter coefficients are placed in a dedicated `.filter_data` ELF section. You can extract them from the object file to confirm no runtime computation occurred:

```bash
make butterworth.main
objcopy -O binary -j .filter_data build/examples/.../butterworth_values.cpp.o filter_data.bin
python3 examples/print_butterworth_binary.py
```
