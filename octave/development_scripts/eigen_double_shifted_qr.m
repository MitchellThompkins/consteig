function a = eigen_double_shifted_qr(a)
    % Double-Shift QR matching eig_double_shifted_qr in eigen.hpp
    S = size(a, 1);
    if S <= 1, return; end
    
    a = balance(a);
    a = hessen(a);

    matrix_norm = norm(a, 1) + norm(a, inf);
    eps_val = eps * matrix_norm;
    if eps_val == 0, eps_val = eps; end

    n = S;
    total_iter = 0;
    max_total_iter = 100 * S;
    its = 0;

    while n > 1 && total_iter < max_total_iter
        l = n;
        while l > 1
            diagonal_sum = abs(a(l, l)) + abs(a(l - 1, l - 1));
            if abs(a(l, l - 1)) <= eps_val * diagonal_sum || abs(a(l, l - 1)) <= eps_val
                a(l, l - 1) = 0;
                break;
            end
            l = l - 1;
        end

        if l == n
            n = n - 1;
            its = 0;
            continue;
        end

        if l + 1 == n
            % 2x2 block detected - in the C++ version, we skip 2 steps
            n = n - 2;
            its = 0;
            continue;
        end

        % Compute shifts
        if its > 0 && mod(its, 10) == 0
            if mod(its, 20) == 0
                sshift = abs(a(n, n - 1)) + abs(a(n - 1, n - 2));
            else
                sshift = abs(a(l + 1, l));
                if l + 2 <= n, sshift = sshift + abs(a(l + 2, l + 1)); end
            end
            h11 = 0.75 * sshift + a(n, n);
            h12 = -0.4375 * sshift;
            h21 = sshift;
            s = h11 + h11;
            t = h11 * h11 - h12 * h21;
        else
            s = a(n - 1, n - 1) + a(n, n);
            t = a(n - 1, n - 1) * a(n, n) - a(n - 1, n) * a(n, n - 1);
        end

        a = francis_qr_step(a, l, n, s, t);
        total_iter = total_iter + 1;
        its = its + 1;
    end
endfunction
