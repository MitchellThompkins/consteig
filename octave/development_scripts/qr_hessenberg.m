function [Q, R] = qr_hessenberg(A)
    % Optimized QR for Upper Hessenberg Matrix matching qr.hpp
    n = size(A, 1);
    Q = eye(n);
    R = A;
    
    for i = 1:n-1
        x = R(i, i);
        y = R(i+1, i);
        
        if abs(y) > 1e-15
            mag = sqrt(x^2 + y^2);
            c = x / mag;
            s = y / mag;
            
            % R = G * R
            for j = i:n
                r_i = R(i, j);
                r_ip1 = R(i+1, j);
                R(i, j) = c * r_i + s * r_ip1;
                R(i+1, j) = -s * r_i + c * r_ip1;
            end
            
            % Q = Q * G'
            for j = 1:n
                q_i = Q(j, i);
                q_ip1 = Q(j, i+1);
                Q(j, i) = c * q_i + s * q_ip1;
                Q(j, i+1) = -s * q_i + c * q_ip1;
            end
            R(i+1, i) = 0;
        end
    end
endfunction
