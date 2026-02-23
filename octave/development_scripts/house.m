function P = house(a)
    % Householder Reflection matching householder.hpp
    R = size(a, 1);
    alphaSum = 0;
    for i = 2:R
        alphaSum = alphaSum + a(i, 1)^2;
    end
    
    if abs(alphaSum) < eps
        P = eye(R);
        return;
    end
    
    if a(1, 1) < 0
        sign_val = -1;
    else
        sign_val = 1;
    end
    
    alpha = -1 * sign_val * sqrt(alphaSum + a(1, 1)^2);
    r_sq = 0.5 * (alpha^2 - a(1, 1) * alpha);
    
    if abs(r_sq) < eps
        P = eye(R);
        return;
    end
    
    r = sqrt(r_sq);
    oneOverTwoR = 1 / (2 * r);
    
    v = zeros(R, 1);
    v(1, 1) = (a(1, 1) - alpha) * oneOverTwoR;
    for i = 2:R
        v(i, 1) = a(i, 1) * oneOverTwoR;
    end
    
    P = eye(R) - 2 * (v * v');
endfunction
