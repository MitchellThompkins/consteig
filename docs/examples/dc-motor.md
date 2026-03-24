title: DC Motor Control

# DC Motor Control

This example demonstrates using `eigenvalues` and `static_assert` to validate PID controller gains for a DC motor position control system at compile time. Full source: [`examples/dc_motor_control.cpp`](https://github.com/mitchellthompkins/consteig/blob/develop/examples/dc_motor_control.cpp).

## Problem

Given a DC motor state-space model and a set of PID gains, verify at compile time that the closed-loop system meets performance requirements:

- All poles in the left half-plane (stable)
- Settling time < 40ms
- Overshoot < 16%
- No steady-state error

If the gains do not meet these requirements, the build fails with a descriptive error.

## How it Works

```cpp
// Closed-loop state matrix (formed from motor model + PID gains)
static constexpr consteig::Matrix<double, 4, 4> A_cl = /* ... */;

// Compute closed-loop poles at compile time
static constexpr auto poles = consteig::eigenvalues(A_cl);

// All poles must be in the left half-plane (negative real part)
static_assert(check_stability(poles), "System is unstable");

// Settling time check: all poles must have real part < POLE_LIMIT
static_assert(check_settling_time(poles, POLE_LIMIT_FOR_SETTLING),
              "Settling time less than 0.040 seconds [FAILED]");

// Overshoot check: damping ratio must exceed ZETA_LIMIT
static_assert(check_overshoot(poles, ZETA_LIMIT_FOR_OVERSHOOT),
              "Overshoot less than 16% [FAILED]");
```

## Bad Gains → Build Error

With gains that violate the performance requirements, the compiler produces:

```
error: static assertion failed: Settling time less than 0.040 seconds [FAILED]
error: static assertion failed: Overshoot less than 16% [FAILED]
```

This is the key value proposition: performance validation moved from runtime testing (or offline MATLAB analysis) to compile time. Invalid gains cannot ship.

## Good Gains → Build Succeeds

With properly tuned gains, the build completes and the system is guaranteed to meet requirements. The step response plot below shows the validated system behavior:

![Motor Control Step Response](../imgs/step_response.png)

## Octave Reference

The equivalent Octave validation script `octave/dc_motor_control.m` produces matching output:

```
Validation:
  [PASS] Stability check (All poles in LHP)
  [PASS] Settling time less than 0.040 seconds
  [PASS] Overshoot less than 16%
  [PASS] No steady-state error (Enforced by integral action)
```
