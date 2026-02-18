% Script to generate C++ test cases for consteig
% Run this in Octave/Matlab to generate 'test_dependencies/generated_cases.hpp'

filename = 'test_dependencies/generated_cases.hpp';
fid = fopen(filename, 'w');

fprintf(fid, '#ifndef GENERATED_CASES_HPP\n');
fprintf(fid, '#define GENERATED_CASES_HPP\n\n');
fprintf(fid, '#include "../consteig.hpp"\n\n');
fprintf(fid, 'namespace consteig {\n\n');

NUM_CASES = 50;

function generate_cases(fid, type_str, S, num_cases, suffix)
    if strcmp(type_str, 'sym')
        fprintf(fid, '// Symmetric Matrix Test Cases (%dx%d) - %s\n', S, S, suffix);
        var_name = ['mat_sym_' suffix];
        eig_var_name = ['eigs_sym_' suffix];
    else
        fprintf(fid, '// Non-Symmetric Matrix Test Cases (%dx%d) - %s\n', S, S, suffix);
        var_name = ['mat_nonsym_' suffix];
        eig_var_name = ['eigs_nonsym_' suffix];
    end

    fprintf(fid, 'static constexpr Matrix<double, %d, %d> %s[%d] = {\n', S, S, var_name, num_cases);
    
    eigs_all = zeros(S, num_cases);
    
    for n = 1:num_cases
        if strcmp(type_str, 'sym')
            A = rand(S);
            A = A + A';
        else
            A = rand(S);
        end
        e = eig(A);
        eigs_all(:, n) = e;

        fprintf(fid, '    {{{\n');
        for i = 1:S
            fprintf(fid, '        {');
            for j = 1:S
                if j > 1
                    fprintf(fid, ', ');
                end
                fprintf(fid, '%.16e', A(i,j));
            end
            if i < S
                fprintf(fid, '},\n');
            else
                fprintf(fid, '}\n');
            end
        end
        if n < num_cases
            fprintf(fid, '    }}},\n');
        else
            fprintf(fid, '    }}}\n');
        end
    end
    fprintf(fid, '};\n\n');

    fprintf(fid, 'static constexpr Matrix<Complex<double>, %d, 1> %s[%d] = {\n', S, eig_var_name, num_cases);
    for n = 1:num_cases
        e = eigs_all(:, n);
        fprintf(fid, '    {{{\n');
        for i = 1:S
            fprintf(fid, '        { { {%.16e, %.16e} } }', real(e(i)), imag(e(i)));
            if i < S
                fprintf(fid, ',\n');
            else
                fprintf(fid, '\n');
            end
        end
        if n < num_cases
            fprintf(fid, '    }}},\n');
        else
            fprintf(fid, '    }}}\n');
        end
    end
    fprintf(fid, '};\n\n');
end

% Fast cases (5x5)
generate_cases(fid, 'sym', 5, NUM_CASES, 'fast');
generate_cases(fid, 'nonsym', 5, NUM_CASES, 'fast');

% Slow cases (8x8)
generate_cases(fid, 'sym', 8, NUM_CASES, 'slow');
generate_cases(fid, 'nonsym', 8, NUM_CASES, 'slow');

% 3. QR Decomposition Test Case
S = 4;
A_qr = rand(S);
[Q, R] = qr(A_qr);

fprintf(fid, '// QR Decomposition Test Case\n');
[rows, cols] = size(A_qr);
fprintf(fid, 'static constexpr Matrix<double, %d, %d> mat_qr\n', rows, cols);
fprintf(fid, '{{{\n');
for i = 1:rows
    fprintf(fid, '    {');
    for j = 1:cols
        if j > 1
            fprintf(fid, ', ');
        end
        fprintf(fid, '%.16e', A_qr(i,j));
    end
    if i < rows
        fprintf(fid, '},\n');
    else
        fprintf(fid, '}\n');
    end
end
fprintf(fid, '}}};\n\n');

fprintf(fid, '} // namespace consteig\n');
fprintf(fid, '#endif\n');

fclose(fid);
