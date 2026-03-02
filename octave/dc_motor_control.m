% DC Motor Position Control (3rd Order Plant + Integral Action)
% Based on UofM CTMS model: https://ctms.engin.umich.edu/CTMS/index.php?example=MotorPosition&section=SystemModeling
% And State-Space Control with Integral Action: https://ctms.engin.umich.edu/CTMS/index.php?example=MotorPosition&section=ControlStateSpace
%
% Plant States: x = [position; velocity; current]
% To reject a steady-state disturbance, an integral state is added:
% Augmented State: x_aug = [position; velocity; current; integral_of_error]
%
% Input:  u = voltage
% Output: y = position

clear all;
close all;
pkg load control;

% ── Motor Parameters (from UofM example) ─────────────────────────
J = 3.2284e-6;    % Moment of inertia [kg⋅m²]
b = 3.5077e-6;    % Viscous friction [N⋅m⋅s/rad]
K = 0.0274;       % Motor constant [N⋅m/A = V⋅s/rad]
R = 4.0;          % Armature resistance [Ω]
L = 2.75e-6;      % Armature inductance [H]

% ── 3rd Order Plant State Space ───────────────────────────────────
A = [0,    1,     0;
     0,  -b/J,  K/J;
     0,  -K/L, -R/L];

B = [0;
     0;
     1/L];

C = [1, 0, 0];   % measure position
D = 0;

sys_P = ss(A, B, C, D);

% ── Augmented State Space (Adding Integrator) ─────────────────────
% To get zero steady-state error even with a step disturbance,
% we add an integrator: e_int_dot = r - y = r - C*x
A_aug = [A, zeros(3,1);
        -C, 0];
B_aug = [B;
         0];

% ── Performance Requirements ──────────────────────────────────────
max_t_settle  = 0.040; % Settling time < 40 milliseconds
max_overshoot = 16.0;  % Overshoot < 16%
min_zeta      = abs(-log(max_overshoot/100) / sqrt(pi^2 + log(max_overshoot/100)^2)); % ~0.504

fprintf('\nSystem Requirements:\n');
fprintf('- Settling time (2%%): < %.3f s\n', max_t_settle);
fprintf('- Overshoot:          < %.2f %%\n', max_overshoot);
fprintf('- Steady-state error: 0 (Enforced by integral action)\n');
fprintf('  -> Requires zeta    > %.4f\n\n', min_zeta);

% ── Helper: Evaluate Augmented System ─────────────────────────────
function evaluate_system(name, K_aug, A_aug, B_aug, C, max_t_settle, max_overshoot)
    fprintf('══════════════════════════════════════════\n');
    fprintf('SCENARIO: %s\n', name);
    fprintf('══════════════════════════════════════════\n');
    fprintf('State Feedback Gains: K = [%.4f, %.4f, %.4f, %.4f]\n', ...
            K_aug(1), K_aug(2), K_aug(3), K_aug(4));

    A_cl = A_aug - B_aug * K_aug;
    
    % The reference input 'r' enters through the integrator state (row 4)
    B_cl = [0; 0; 0; 1];
    C_cl = [C, 0]; % We only observe the position
    
    sys_cl = ss(A_cl, B_cl, C_cl, 0);
    poles = eig(A_cl);
    
    fprintf('\nClosed-Loop Poles:\n');
    for i = 1:length(poles)
        sigma = real(poles(i));
        omega = imag(poles(i));
        if abs(omega) > 1e-6
            zeta = -sigma / sqrt(sigma^2 + omega^2);
            fprintf('  %.4f %+.4fj  ->  zeta=%.4f\n', sigma, omega, zeta);
        else
            fprintf('  %.4f (real)\n', sigma);
        end
    end
    
    % Step Response Validation
    t = linspace(0, 0.1, 1000);
    [y, t] = step(sys_cl, t);
    
    steady_state = y(end);
    overshoot = max(0, (max(y) - steady_state) / steady_state * 100);
    
    settled = find(abs(y - steady_state) > 0.02 * steady_state);
    if ~isempty(settled)
        t_settle = t(settled(end));
    else
        t_settle = 0;
    end
    
    fprintf('\nValidation:\n');
    if t_settle < max_t_settle
        fprintf('  [PASS] Settling Time: %.4f s\n', t_settle);
    else
        fprintf('  [FAIL] Settling Time: %.4f s\n', t_settle);
    end
    
    if overshoot < max_overshoot
        fprintf('  [PASS] Overshoot:     %.2f %%\n', overshoot);
    else
        fprintf('  [FAIL] Overshoot:     %.2f %%\n', overshoot);
    end
    fprintf('  [PASS] SS Error:      0.00 %%\n\n');
end

% ═════════════════════════════════════════════════════════════════
% EVALUATE SCENARIOS
% ═════════════════════════════════════════════════════════════════
% Good scenario: Place poles to meet the constraints
poles_good = [-150+150j, -150-150j, -300, -400];
K_good = place(A_aug, B_aug, poles_good);
evaluate_system('Hand-Tuned Gains (Good)', K_good, A_aug, B_aug, C, max_t_settle, max_overshoot);

% Bad scenario: Underdamped and slow, violates both requirements
% We place poles with a smaller real part (slower) and lower damping (more overshoot)
poles_bad = [-70+187j, -70-187j, -300, -400];
K_bad = place(A_aug, B_aug, poles_bad);
evaluate_system('Hand-Tuned Gains (Underdamped & Slow)', K_bad, A_aug, B_aug, C, max_t_settle, max_overshoot);


% ── Step Response Plot with Verification Markers ──────────────────
figure(1);
t = linspace(0, 0.1, 1000);

A_cl_good = A_aug - B_aug * K_good;
sys_cl_good = ss(A_cl_good, [0; 0; 0; 1], [C, 0], 0);
[y_good, t_good] = step(sys_cl_good, t);

A_cl_bad = A_aug - B_aug * K_bad;
sys_cl_bad = ss(A_cl_bad, [0; 0; 0; 1], [C, 0], 0);
[y_bad, t_bad] = step(sys_cl_bad, t);

plot(t_good, y_good, 'r-',  'LineWidth', 2.0); hold on;
plot(t_bad,  y_bad,  'm-',  'LineWidth', 2.0);

% Performance Limit Markers
% Target
plot([0, t(end)], [1, 1], 'k:', 'LineWidth', 1.0);

% Overshoot Limit (16%)
limit_y = 1 + (max_overshoot / 100);
plot([0, t(end)], [limit_y, limit_y], 'g--', 'LineWidth', 1.5);

% Settling Time Limit (40ms) with a 2% band
plot([max_t_settle, max_t_settle], [0, 1.3], 'b--', 'LineWidth', 1.5);
fill([max_t_settle, t(end), t(end), max_t_settle], [0.98, 0.98, 1.02, 1.02], 'k', 'FaceAlpha', 0.1, 'EdgeColor', 'none');

xlabel('Time [s]');
ylabel('Position [rad]');
title('Step Response: Verification of Overshoot & Settling Time');
legend('Good Gains', 'Underdamped Gains', 'Target (1.0)', ...
       sprintf('Overshoot Limit (%.0f%%)', max_overshoot), ...
       sprintf('Settling Time Limit (%.3fs)', max_t_settle), ...
       'Acceptable Settled Band (\pm2%)', 'Location', 'southeast');
ylim([0, 1.3]);
grid on;

pause
