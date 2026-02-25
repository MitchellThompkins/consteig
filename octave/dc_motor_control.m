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

fprintf('\nExtended open loop poles:\n');
ol_poles = eig(A)

% ── Performance Requirements ──────────────────────────────────────
min_convergence = -2.0;   % slowest pole allowed (rad/s)
max_convergence = -50.0;  % fastest pole allowed (rad/s)
min_damping     = 0.55;   % minimum damping ratio

% ── Helper: Check Performance ─────────────────────────────────────
function result = check_performance(poles, min_conv, max_conv, min_damp)
    result = true;
    for i = 1:length(poles)
        sigma = real(poles(i));
        omega = imag(poles(i));
        % Stability
        if sigma >= 0
            result = false;
            return;
        end
        % Convergence rate bounds
        if sigma >= min_conv || sigma <= max_conv
            result = false;
            return;
        end
        % Damping ratio for complex poles
        if abs(omega) > 1e-6
            zeta = -sigma / sqrt(sigma^2 + omega^2);
            if zeta < min_damp
                result = false;
                return;
            end
        end
    end
end

% ── Helper: Print Pole Analysis ───────────────────────────────────
function print_poles(poles, min_damp)
    for i = 1:length(poles)
        sigma = real(poles(i));
        omega = imag(poles(i));
        if abs(omega) > 1e-6
            zeta = -sigma / sqrt(sigma^2 + omega^2);
            wn   = sqrt(sigma^2 + omega^2);
            fprintf('  Pole %d: %+.4f %+.4fj  ->  zeta=%.4f, wn=%.4f rad/s', ...
                    i, sigma, omega, zeta, wn);
            if zeta < min_damp
                fprintf('  *** UNDERDAMPED ***');
            end
        else
            fprintf('  Pole %d: %+.4f (real)', i, sigma);
        end
        fprintf('\n');
    end
end

% ═════════════════════════════════════════════════════════════════
% APPROACH 1: Hand-Tuned PID Gains (Good)
% Designer picks gains directly, we find out where the poles ended up
% ═════════════════════════════════════════════════════════════════
fprintf('\n══════════════════════════════════════════\n');
fprintf('SCENARIO 1: Hand-Tuned Gains (Good)\n');
fprintf('══════════════════════════════════════════\n');

Kp_good     = 123.0;
Kd_good     = 20.49;
Ki_eff_good = 2.0;
K_good = [Kp_good, Kd_good, Ki_eff_good];

fprintf('Hand-tuned gains: Kp=%.4f, Kd=%.4f, Ki_eff=%.4f\n', Kp_good, Kd_good, Ki_eff_good);

A_cl_good = A - B * K_good;
sys_cl_good = ss(A_cl_good, B, C, D);
poles_good = eig(A_cl_good);

fprintf('Resulting poles:\n');
print_poles(poles_good, min_damping);

if check_performance(poles_good, min_convergence, max_convergence, min_damping)
    fprintf('PASS: all performance requirements met\n');
else
    fprintf('FAIL: performance requirements violated\n');
end

% ═════════════════════════════════════════════════════════════════
% APPROACH 2: Hand-Tuned PID Gains (Underdamped)
% Engineer increased Kp chasing faster response - poles look fine
% on paper but damping ratio is too low
% ═════════════════════════════════════════════════════════════════
fprintf('\n══════════════════════════════════════════\n');
fprintf('SCENARIO 2: Hand-Tuned Gains (Underdamped)\n');
fprintf('══════════════════════════════════════════\n');

Kp_bad     = 375.0;
Kd_bad     = 62.49;
Ki_eff_bad = 2.0;
K_bad = [Kp_bad, Kd_bad, Ki_eff_bad];

fprintf('Hand-tuned gains: Kp=%.4f, Kd=%.4f, Ki_eff=%.4f\n', ...
        Kp_bad, Kd_bad, Ki_eff_bad);

A_cl_bad = A - B * K_bad;
sys_cl_bad = ss(A_cl_bad, B, C, D);
poles_bad = eig(A_cl_bad);

fprintf('Resulting poles:\n');
print_poles(poles_bad, min_damping);

if check_performance(poles_bad, min_convergence, max_convergence, min_damping)
    fprintf('PASS: all performance requirements met\n');
else
    fprintf('FAIL: performance requirements violated - would be compile error in C++\n');
end

% ── Bode Plot ─────────────────────────────────────────────────────
figure(1);
w = logspace(-2, 3, 500);

[mag_ol,   phase_ol]   = bode(sys_ol,        w);
[mag_good, phase_good] = bode(sys_cl_good, w);
[mag_bad,  phase_bad]  = bode(sys_cl_bad,    w);

mag_ol    = squeeze(mag_ol);    phase_ol    = squeeze(phase_ol);
mag_good  = squeeze(mag_good);  phase_good  = squeeze(phase_good);
mag_bad   = squeeze(mag_bad);   phase_bad   = squeeze(phase_bad);

subplot(2,1,1);
semilogx(w, 20*log10(mag_ol),            'b--', 'LineWidth', 1.5); hold on;
semilogx(w, 20*log10(mag_good),          'r-',  'LineWidth', 1.5);
semilogx(w, 20*log10(mag_bad),           'm-',  'LineWidth', 1.5);
semilogx([w(1), w(end)], [0, 0],         'k:',  'LineWidth', 1.0);
ylabel('Magnitude [dB]');
title('Bode Plot: DC Motor Position Control');
legend('Open Loop', 'Good Gains', 'Underdamped Gains', '0 dB');
grid on;

subplot(2,1,2);
semilogx(w, phase_ol,                        'b--', 'LineWidth', 1.5); hold on;
semilogx(w, phase_good,                      'r-',  'LineWidth', 1.5);
semilogx(w, phase_bad,                       'm-',  'LineWidth', 1.5);
semilogx([w(1), w(end)], [-180, -180],       'k:',  'LineWidth', 1.0);
ylabel('Phase [deg]');
xlabel('Frequency [rad/s]');
legend('Open Loop', 'Good Gains', 'Underdamped Gains', '-180°');
grid on;

% ── Step Response ─────────────────────────────────────────────────
figure(2);
t = linspace(0, 3, 1000);

[y_ol,   t_ol]   = step(sys_ol,        t);
[y_good, t_good] = step(sys_cl_good, t);
[y_bad,  t_bad]  = step(sys_cl_bad,    t);

plot(t_ol,   y_ol,   'b--', 'LineWidth', 1.5); hold on;
plot(t_good, y_good, 'r-',  'LineWidth', 1.5);
plot(t_bad,  y_bad,  'm-',  'LineWidth', 1.5);

% Settling time for good gains
steady_state = y_good(end);
settled = find(abs(y_good - steady_state) > 0.02 * abs(steady_state));
if ~isempty(settled)
    t_settle = t_good(settled(end));
    fprintf('\nGood gains settling time (2%%): %.3f s\n', t_settle);
    plot([t_settle, t_settle], [0, steady_state], 'k--', 'LineWidth', 1.0);
    text(t_settle + 0.05, steady_state * 0.4, ...
         sprintf('t_{settle}=%.2fs', t_settle));
end

xlabel('Time [s]');
ylabel('Position [rad]');
title('Step Response: Open Loop vs Good Gains vs Underdamped Gains');
legend('Open Loop', 'Good Gains', 'Underdamped Gains');
grid on;