---
title: DC Motor Control
---

# Control Theory

When system parameters and controller gains are known at compile-time, consteig
can verify that the closed-loop poles meet performance requirements, turning a
runtime failure into a build failure. At compile-time, consteig can validate
that the chosen gains for a PID loop meet
the performance requirements for a system. Consider the [DC Motor
Position: PID Controller
Design](https://ctms.engin.umich.edu/CTMS/index.php?example=MotorPosition&section=ControlPID),
along with the [state space
model](https://ctms.engin.umich.edu/CTMS/index.php?example=MotorPosition&section=ControlStateSpace)
from the University of Michigan. The information known at compile-time is:

* The motor parameters (and thus state space model)
* Chosen gains
* System performance requirements

When building the system with gains that do not meet the performance requirements,
`static_assert` can validate system gains by preventing compilation.

Full source: [`examples/dc_motor_control.cpp`](https://github.com/mitchellthompkins/consteig/blob/develop/examples/dc_motor_control.cpp).

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

## Bad Gains: Build Error

```bash
make container.make.dc_motor_control.main
```

```text
...
/Users/mitchellthompkins/workspace/consteig/examples/dc_motor_control.cpp:143:19: error: static assertion failed due to requirement 'check_settling_time(eigs_bad, POLE_LIMIT_FOR_SETTLING)': Scenario 2 REJECTED: Settling time less than 0.040 seconds [FAILED]
  143 |     static_assert(check_settling_time(eigs_bad, POLE_LIMIT_FOR_SETTLING),
      |                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/Users/mitchellthompkins/workspace/consteig/examples/dc_motor_control.cpp:145:19: error: static assertion failed due to requirement 'check_overshoot(eigs_bad, ZETA_LIMIT_FOR_OVERSHOOT)': Scenario 2 REJECTED: Overshoot less than 16% [FAILED]
  145 |     static_assert(check_overshoot(eigs_bad, ZETA_LIMIT_FOR_OVERSHOOT),
      |                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```

This aligns with the output produced by octave:

```bash
octave octave/dc_motor_control.m
```

```text
...
State Feedback Gains: K = [21.0000, 0.0500, 0.0000, -200.0000]

Closed-Loop Poles:
  -1454381.2397 (real)
  -77.7191 +192.3253j  ->  zeta=0.3747
  -77.7191 -192.3253j  ->  zeta=0.3747
  -9.8632 (real)

Validation:
  [PASS] Stability check (All poles in LHP)
  [FAIL] Settling time less than 0.040 seconds (Actual: 0.0509 s)
  [FAIL] Overshoot less than 16% (Actual: 33.40 %)
  [PASS] No steady-state error (Enforced by integral action)
```

## Good Gains: Build Succeeds

With properly tuned gains, the build completes and the system is guaranteed to
meet requirements. The step response plot below shows the validated system
behavior:

![Motor Control Step Response](../step_response.png)
