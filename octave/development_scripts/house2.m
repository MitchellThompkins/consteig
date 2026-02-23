function [u,nu] = house2(x)
    % [u,nu] = housegen(x)
    % Generate Householder reflection.
    % G. W. Stewart, "Matrix Algorithms, Volume 1", SIAM, 1998.
    % [u,nu] = housegen(x).
    % H = I - uu' with Hx = -+ nu e_1
    %    returns nu = norm(x).
    u = x;
    nu = norm(x);
    if nu == 0
        u(1) = sqrt(2);
        return
    endif
    u = x/nu;
    if u(1) >= 0
        u(1) = u(1) + 1;
        nu = -nu;
    else
        u(1) = u(1) - 1;
    endif
    u = u/sqrt(abs(u(1)));
endfunction
