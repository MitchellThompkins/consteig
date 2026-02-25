% DC Motor Position Control
% Based on UofM CTMS model: https://ctms.engin.umich.edu/CTMS/index.php?example=MotorSpeed&section=SystemModeling
% Extended from velocity to position control by adding position state
%
% States: x = [position; velocity; current]
% Input:  u = voltage
% Output: y = position

clear all;
close all;
pkg load control;

% ── Motor Parameters (from UofM example) ─────────────────────────
J = 0.01;    % Moment of inertia [kg⋅m²]
b = 0.1;     % Viscous friction [N⋅m⋅s/rad]
K = 0.01;    % Motor constant [N⋅m/A = V⋅s/rad]
R = 1.0;     % Armature resistance [Ω]
L = 0.5;     % Armature inductance [H]

% ── State Space (UofM velocity model for reference) ───────────────
% x = [velocity; current]
A_uom = [-b/J,  K/J;
         -K/L, -R/L];
B_uom = [0; 1/L];
C_uom = [1, 0];
D_uom = 0;

sys_uom = ss(A_uom, B_uom, C_uom, D_uom);
fprintf('UofM open loop poles:\n');
eig(A_uom)

% ── Extended to Position Control ──────────────────────────────────
% Add position as first state: position_dot = velocity
% x = [position; velocity; current]
A = [0,    1,     0;
     0,  -b/J,  K/J;
     0,  -K/L, -R/L];

B = [0;
     0;
     1/L];

C = [1, 0, 0];   % measure position
D = 0;

sys_ol = ss(A, B, C, D);

fprintf('Extended open loop poles:\n');
ol_poles = eig(A)

% ── Pole Placement ────────────────────────────────────────────────
% Place complex conjugate poles for fast response
% This gives a non-trivial damping ratio that isn't obvious from gains
desired_poles = [-5+4j, -5-4j, -6];
K_gain = place(A, B, desired_poles);

fprintf('Control gains: K = [%.4f, %.4f, %.4f]\n', ...
        K_gain(1), K_gain(2), K_gain(3));

% ── Closed Loop System ────────────────────────────────────────────
A_cl = A - B * K_gain;
sys_cl = ss(A_cl, B, C, D);

fprintf('Closed loop poles:\n');
cl_poles = eig(A_cl)

% ── Damping Ratio Analysis ────────────────────────────────────────
% For each complex conjugate pair, compute damping ratio
% zeta = -sigma / sqrt(sigma^2 + omega^2)
% where pole = sigma + j*omega
fprintf('\n--- Damping Ratio Analysis ---\n');
min_zeta = inf;
for i = 1:length(cl_poles)
    sigma = real(cl_poles(i));
    omega = imag(cl_poles(i));
    if abs(omega) > 1e-6
        zeta = -sigma / sqrt(sigma^2 + omega^2);
        wn   = sqrt(sigma^2 + omega^2);
        fprintf('  Pole %d: %.4f + %.4fj  ->  zeta = %.4f, wn = %.4f rad/s\n', ...
                i, sigma, omega, zeta, wn);
        min_zeta = min(min_zeta, zeta);
    else
        fprintf('  Pole %d: %.4f (real)\n', i, sigma);
    end
end

% ── Assertions ────────────────────────────────────────────────────
fprintf('\n--- Assertions ---\n');

% Stability: all poles must have negative real part
assert(all(real(cl_poles) < 0), ...
       'UNSTABLE: one or more closed loop poles have positive real part!');
fprintf('OK: all poles stable\n');

% Performance: slowest pole must be fast enough for control loop
min_convergence = -2.0;   % rad/s - tune to your loop deadline
assert(all(real(cl_poles) < min_convergence), ...
       sprintf('TOO SLOW: a pole is slower than %.1f rad/s', min_convergence));
fprintf('OK: all poles faster than %.1f rad/s\n', min_convergence);

% Performance: fastest pole not too aggressive
max_convergence = -50.0;  % rad/s - beyond this demands excessive current
assert(all(real(cl_poles) > max_convergence), ...
       sprintf('TOO AGGRESSIVE: a pole is faster than %.1f rad/s', max_convergence));
fprintf('OK: no poles more aggressive than %.1f rad/s\n', max_convergence);

% Damping: complex poles must not be too underdamped
% Below 0.5 risks mechanical oscillation stressing the gearbox
min_damping = 0.5;
if min_zeta < inf
    assert(min_zeta >= min_damping, ...
           sprintf('UNDERDAMPED: zeta = %.4f < %.4f, oscillation will stress gearbox!', ...
                   min_zeta, min_damping));
    fprintf('OK: damping ratio %.4f >= %.4f\n', min_zeta, min_damping);
end

% ── Export Closed Loop Matrix for constexpr Verification ──────────
fprintf('\n--- Closed Loop A matrix (paste into C++) ---\n');
fprintf('constexpr Matrix<double, 3, 3> A_cl = {{\n');
for i = 1:3
    fprintf('    {{ %12.8f, %12.8f, %12.8f }}', A_cl(i,1), A_cl(i,2), A_cl(i,3));
    if i < 3
        fprintf(',');
    end
    fprintf('\n');
end
fprintf('}};\n\n');

fprintf('Expected eigenvalues (from Octave):\n');
for i = 1:3
    fprintf('    %12.8f + %12.8fj\n', real(cl_poles(i)), imag(cl_poles(i)));
end

% ── Bode Plot ─────────────────────────────────────────────────────
figure(1);
w = logspace(-2, 3, 500);

[mag_ol, phase_ol] = bode(sys_ol, w);
[mag_cl, phase_cl] = bode(sys_cl, w);
mag_ol   = squeeze(mag_ol);
phase_ol = squeeze(phase_ol);
mag_cl   = squeeze(mag_cl);
phase_cl = squeeze(phase_cl);

subplot(2,1,1);
semilogx(w, 20*log10(mag_ol),        'b--', 'LineWidth', 1.5); hold on;
semilogx(w, 20*log10(mag_cl),        'r-',  'LineWidth', 1.5);
semilogx([w(1), w(end)], [0, 0],     'k:',  'LineWidth', 1.0);
ylabel('Magnitude [dB]');
title('Bode Plot: DC Motor Position Control');
legend('Open Loop', 'Closed Loop', '0 dB');
grid on;

subplot(2,1,2);
semilogx(w, phase_ol,                    'b--', 'LineWidth', 1.5); hold on;
semilogx(w, phase_cl,                    'r-',  'LineWidth', 1.5);
semilogx([w(1), w(end)], [-180, -180],   'k:',  'LineWidth', 1.0);
ylabel('Phase [deg]');
xlabel('Frequency [rad/s]');
legend('Open Loop', 'Closed Loop', '-180°');
grid on;

% ── Step Response ─────────────────────────────────────────────────
figure(2);
t = linspace(0, 3, 1000);

[y_ol, t_ol] = step(sys_ol, t);
[y_cl, t_cl] = step(sys_cl, t);

steady_state = y_cl(end);
settled = find(abs(y_cl - steady_state) > 0.02 * abs(steady_state));

plot(t_ol, y_ol, 'b--', 'LineWidth', 1.5); hold on;
plot(t_cl, y_cl, 'r-',  'LineWidth', 1.5);

if ~isempty(settled)
    t_settle = t_cl(settled(end));
    fprintf('Settling time (2%%): %.3f s\n', t_settle);
    plot([t_settle, t_settle], [0, steady_state], 'k--', 'LineWidth', 1.0);
    text(t_settle + 0.05, steady_state * 0.5, ...
         sprintf('t_{settle} = %.2fs', t_settle));
end

xlabel('Time [s]');
ylabel('Position [rad]');
title('Step Response: Open Loop vs Closed Loop');
legend('Open Loop', 'Closed Loop');
grid on;

% ── Underdamped Example ───────────────────────────────────────────
fprintf('\n--- Attempting underdamped pole placement ---\n');
bad_poles = [-5+8j, -5-8j, -6];   % high imaginary part -> low damping ratio
K_bad    = place(A, B, bad_poles);
A_bad    = A - B * K_bad;
bad_eigs = eig(A_bad);

fprintf('Underdamped closed loop poles:\n');
for i = 1:length(bad_eigs)
    sigma = real(bad_eigs(i));
    omega = imag(bad_eigs(i));
    if abs(omega) > 1e-6
        zeta = -sigma / sqrt(sigma^2 + omega^2);
        fprintf('  Pole %d: %.4f + %.4fj  ->  zeta = %.4f\n', ...
                i, sigma, omega, zeta);
        if zeta < min_damping
            fprintf('  WARNING: underdamped! would be a compile error in C++\n');
        end
    end
end
