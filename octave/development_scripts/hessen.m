function H = hessen(A)
    % Hessenberg Reduction matching hessenberg.hpp
    n = size(A, 1);
    H = A;
    for k = 1:n-2
        P_sub = house(H(k+1:n, k));
        P = eye(n);
        P(k+1:n, k+1:n) = P_sub;
        H = P * H * P;
    end
endfunction
