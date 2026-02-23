function a = eigen(a)
    % Shifted QR (Symmetric/Real Case) matching eig_shifted_qr in eigen.hpp
    n_size = size(a, 1);
    if n_size <= 1
        return;
    end
    
    a = balance(a);
    a = hessen(a);
    
    % Dual-mode epsilon calculation
    matrix_norm = norm(a, 1) + norm(a, inf);
    eps_val = eps * matrix_norm;
    if eps_val == 0
        eps_val = eps;
    end
    
    n = n_size;
    iter = 0;
    max_iter = 100 * n_size; % CONSTEIG_MAX_ITER is 100
    
    while n > 1 && iter < max_iter
        subdiag = abs(a(n, n-1));
        diag_sum = abs(a(n, n)) + abs(a(n-1, n-1));
        
        if subdiag <= eps_val * diag_sum || subdiag <= eps_val
            a(n, n-1) = 0;
            n = n - 1;
            continue;
        end
        
        mu = wilkinsonshift(a(n-1, n-1), a(n, n-1), a(n, n));
        eyeS = eye(n_size);
        shifted = a - (mu * eyeS);
        [Q, R] = qr_hessenberg(shifted);
        a = (R * Q) + (mu * eyeS);
        iter = iter + 1;
    end
endfunction
