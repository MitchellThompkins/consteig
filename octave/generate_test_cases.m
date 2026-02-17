% Script to generate C++ test cases for consteig
% Run this in Octave/Matlab to generate 'test_dependencies/generated_cases.hpp'

filename = 'test_dependencies/generated_cases.hpp';
fid = fopen(filename, 'w');

fprintf(fid, '#ifndef GENERATED_CASES_HPP\n');
fprintf(fid, '#define GENERATED_CASES_HPP\n\n');
fprintf(fid, '#include "../consteig.hpp"\n\n');
fprintf(fid, 'namespace consteig {\n\n');

% 1. Symmetric Matrix (Real Eigenvalues)
NUM_CASES = 50;
S = 10;

fprintf(fid, '// Symmetric Matrix Test Cases\n');
fprintf(fid, 'static constexpr Matrix<double, %d, %d> mat_sym[%d] = {\n', S, S, NUM_CASES);
for n = 1:NUM_CASES
    A_sym = rand(S);
    A_sym = A_sym + A_sym'; % Make symmetric
    e_sym = eig(A_sym);
    
    % Store eigen values for later
    e_sym_all(:, n) = e_sym;

    fprintf(fid, '    {{{\n');
    for i = 1:S
        fprintf(fid, '        {');
        for j = 1:S
            if j > 1
                fprintf(fid, ', ');
            endif
            fprintf(fid, '%.16e', A_sym(i,j));
        end
        if i < S
            fprintf(fid, '},\n');
        else
            fprintf(fid, '}\n');
        endif
    end
    if n < NUM_CASES
        fprintf(fid, '    }}},\n');
    else
        fprintf(fid, '    }}}\n');
    endif
end
fprintf(fid, '};\n\n');

fprintf(fid, 'static constexpr Matrix<Complex<double>, %d, 1> eigs_sym[%d] = {\n', S, NUM_CASES);
for n = 1:NUM_CASES
    e_sym = e_sym_all(:, n);
    fprintf(fid, '    {{{\n');
    for i = 1:S
        fprintf(fid, '        { { {%.16e, %.16e} } }', real(e_sym(i)), imag(e_sym(i)));
        if i < S
            fprintf(fid, ',\n');
        else
            fprintf(fid, '\n');
        endif
    end
    if n < NUM_CASES
        fprintf(fid, '    }}},\n');
    else
        fprintf(fid, '    }}}\n');
    endif
end
fprintf(fid, '};\n\n');

% 2. Non-Symmetric Matrix (Complex Eigenvalues)
S = 10;
fprintf(fid, '// Non-Symmetric Matrix Test Cases\n');
fprintf(fid, 'static constexpr Matrix<double, %d, %d> mat_nonsym[%d] = {\n', S, S, NUM_CASES);
for n = 1:NUM_CASES
    A_nonsym = rand(S);
    e_nonsym = eig(A_nonsym);
    
    % Store for later
    e_nonsym_all(:, n) = e_nonsym;

    fprintf(fid, '    {{{\n');
    for i = 1:S
        fprintf(fid, '        {');
        for j = 1:S
            if j > 1
                fprintf(fid, ', ');
            endif
            fprintf(fid, '%.16e', A_nonsym(i,j));
        end
        if i < S
            fprintf(fid, '},\n');
        else
            fprintf(fid, '}\n');
        endif
    end
    if n < NUM_CASES
        fprintf(fid, '    }}},\n');
    else
        fprintf(fid, '    }}}\n');
    endif
end
fprintf(fid, '};\n\n');

fprintf(fid, 'static constexpr Matrix<Complex<double>, %d, 1> eigs_nonsym[%d] = {\n', S, NUM_CASES);
for n = 1:NUM_CASES
    e_nonsym = e_nonsym_all(:, n);
    fprintf(fid, '    {{{\n');
    for i = 1:S
        fprintf(fid, '        { { {%.16e, %.16e} } }', real(e_nonsym(i)), imag(e_nonsym(i)));
        if i < S
            fprintf(fid, ',\n');
        else
            fprintf(fid, '\n');
        endif
    end
    if n < NUM_CASES
        fprintf(fid, '    }}},\n');
    else
        fprintf(fid, '    }}}\n');
    endif
end
fprintf(fid, '};\n\n');

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
        endif
        fprintf(fid, '%.16e', A_qr(i,j));
    end
    if i < rows
        fprintf(fid, '},\n');
    else
        fprintf(fid, '}\n');
    endif
end
fprintf(fid, '}}};\n\n');

fprintf(fid, '} // namespace consteig\n');
fprintf(fid, '#endif\n');

fclose(fid);
