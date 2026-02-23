function a = balance(a)
    % Balancing algorithm matching balance<T, S> in eigen.hpp
    n = size(a, 1);
    converged = false;
    factor = 2.0;
    
    for iter = 1:10
        if converged, break; end
        converged = true;
        for i = 1:n
            row_norm = 0;
            col_norm = 0;
            for j = 1:n
                if i ~= j
                    row_norm = row_norm + abs(a(i, j));
                    col_norm = col_norm + abs(a(j, i));
                end
            end
            
            if row_norm > 0 && col_norm > 0
                f = 1;
                s = row_norm + col_norm;
                while row_norm < col_norm / factor
                    f = f * factor;
                    row_norm = row_norm * factor;
                    col_norm = col_norm / factor;
                end
                while row_norm > col_norm * factor
                    f = f / factor;
                    row_norm = row_norm / factor;
                    col_norm = col_norm * factor;
                end
                
                if (row_norm + col_norm) < 0.95 * s
                    converged = false;
                    for j = 1:n
                        a(i, j) = a(i, j) * f;
                    end
                    for j = 1:n
                        a(j, i) = a(j, i) / f;
                    end
                end
            end
        end
    end
endfunction
