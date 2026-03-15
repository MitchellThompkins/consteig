function P = house_hessenberg(a)
    % Householder Reflection matching the partial reflection logic 
    % in householder.hpp (typically used for Hessenberg reduction)
    %
    % Logic:
    %   k=1;
    %   n=length(A);
    %   alpha=-1*sign(A(k+1,k))*sqrt(sum(A((k+1:n),k).^2));
    %   r=sqrt(0.5*(alpha^2-A(k+1,k)*alpha));
    %   v(1:k)=0;
    %   v(k+1)=(A(k+1,k)-alpha)/2/r;
    %   for i=k+2:n
    %     v(i)=A(i,k)/2/r;
    %   endfor
    %   u=v';

    R = size(a, 1);
    alphaSum = 0;
    
    % Sum squares starting from index 2 (1-based) to match i=1 in C++
    for i = 2:R
        alphaSum = alphaSum + a(i, 1)^2;
    end
    
    if abs(alphaSum) < eps
        P = eye(R);
        return;
    end
    
    % Sign based on a(2,1) to match a(1,0) in C++
    if a(2, 1) < 0
        sign_val = -1;
    else
        sign_val = 1;
    end
    
    alpha = -1 * sign_val * sqrt(alphaSum);
    r_sq = 0.5 * (alpha^2 - a(2, 1) * alpha);
    
    if abs(r_sq) < eps
        P = eye(R);
        return;
    end
    
    r = sqrt(r_sq);
    oneOverTwoR = 1 / (2 * r);
    
    v = zeros(R, 1);
    % v(1) remains 0 to preserve the first row/column
    v(2, 1) = (a(2, 1) - alpha) * oneOverTwoR;
    for i = 3:R
        v(i, 1) = a(i, 1) * oneOverTwoR;
    end
    
    P = eye(R) - 2 * (v * v');
endfunction
