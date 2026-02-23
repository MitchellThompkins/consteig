function out = wilkinsonshift(a, b, c)
    % Wilkinson shift matching wilkinsonShift in eigen.hpp
    delta = (a - c) / 2;
    if delta == 0
        delta = eps;
    end
    disc = delta^2 + b^2;
    if delta < 0
        s = -sqrt(disc);
    else
        s = sqrt(disc);
    end
    out = c - (b^2) / (delta + s);
endfunction
