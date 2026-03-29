function H = francis_qr_step(H, l, n, s, t)
    % Francis Implicit Double-Shift Step matching francis_qr_step in eigen.hpp
    % Indices are 1-based in Octave: l and n must be 1-based (l=1, n=S)
    
    S = size(H, 1);
    p1 = H(l, l) * H(l, l) + H(l, l + 1) * H(l + 1, l) - s * H(l, l) + t;
    p2 = H(l + 1, l) * (H(l, l) + H(l + 1, l + 1) - s);
    if l + 2 <= n
        p3 = H(l + 1, l) * H(l + 2, l + 1);
    else
        p3 = 0;
    end

    for k = l:n-1
        if k + 2 <= n
            m = 3;
            norm_val = sqrt(p1 * p1 + p2 * p2 + p3 * p3);
            if p1 < 0, sign_p1 = -1; else sign_p1 = 1; end
            v1 = p1 + sign_p1 * norm_val;
            v2 = p2;
            v3 = p3;
        else
            m = 2;
            norm_val = sqrt(p1 * p1 + p2 * p2);
            if p1 < 0, sign_p1 = -1; else sign_p1 = 1; end
            v1 = p1 + sign_p1 * norm_val;
            v2 = p2;
            v3 = 0;
        end

        if norm_val > 0
            v_sum_sq = v1 * v1 + v2 * v2 + v3 * v3;
            beta = 2 / v_sum_sq;

            % Left application
            if k > l, j_start = k - 1; else j_start = k; end
            for j = j_start:S
                sum_val = beta * (v1 * H(k, j) + v2 * H(k + 1, j) + (m == 3) * v3 * H(k + 2, j));
                H(k, j) = H(k, j) - sum_val * v1;
                H(k + 1, j) = H(k + 1, j) - sum_val * v2;
                if m == 3, H(k + 2, j) = H(k + 2, j) - sum_val * v3; end
            end

            % Right application
            if k + 3 < n + 1, upper_row = k + 3; else upper_row = n; end
            for i = 1:min(upper_row, S)
                sum_val = beta * (v1 * H(i, k) + v2 * H(i, k + 1) + (m == 3) * v3 * H(i, k + 2));
                H(i, k) = H(i, k) - sum_val * v1;
                H(i, k + 1) = H(i, k + 1) - sum_val * v2;
                if m == 3, H(i, k + 2) = H(i, k + 2) - sum_val * v3; end
            end

            % Zero bulge
            if k > l
                H(k + 1, k - 1) = 0;
                if m == 3, H(k + 2, k - 1) = 0; end
            end
        end

        if k < n - 1
            p1 = H(k + 1, k);
            p2 = H(k + 2, k);
            if k < n - 2, p3 = H(k + 3, k); end
        end
    end
endfunction
