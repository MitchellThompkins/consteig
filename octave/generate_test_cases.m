% Script to generate C++ test cases for consteig
% Run this in Octave/Matlab to generate 'eigen/tests/generated_cases.hpp'
% and individual test files in 'eigen/tests/'

addpath(fileparts(mfilename('fullpath')));

% Fix seed for reproducibility, removing this will produce truly (well _mostly_)
% random matrices
rand('seed', 42);
randn('seed', 42);

output_cases_file = 'eigen/tests/generated_cases.hpp';
fid = fopen(output_cases_file, 'w');

fprintf(fid, '#ifndef GENERATED_CASES_HPP\n');
fprintf(fid, '#define GENERATED_CASES_HPP\n\n');
fprintf(fid, '#include "../../consteig.hpp"\n\n');
fprintf(fid, 'namespace consteig\n{\n\n');

NUM_RANDOM_CASES = 50;
NUM_ROBUST_CASES = 50;

MATRIX_SIZE = 8;

function generate_cases(fid, type_str, S, num_cases, suffix, category)
    if nargin < 6
        category = 'random';
    end

    if strcmp(type_str, 'sym')
        prefix = 'sym';
    else
        prefix = 'nonsym';
    end
    
    var_name = ['mat_' category '_' prefix '_' suffix];
    eig_var_name = ['eigs_' category '_' prefix '_' suffix];

    fprintf(fid, '// %s %s Matrix Test Cases (%dx%d) - %s\n', category, type_str, S, S, suffix);
    fprintf(fid, 'static constexpr Matrix<double, %d, %d> %s[%d] = {\n', S, S, var_name, num_cases);
    
    eigs_all = zeros(S, num_cases);
    vecs_all = zeros(S, S, num_cases);
    
    for n = 1:num_cases
        if strcmp(category, 'random')
            if strcmp(type_str, 'sym')
                A = make_matrix('random_sym', S);
            else
                A = make_matrix('random_nonsym', S);
            end
        else
            A = make_matrix(category, S);
        end

        [V, D] = eig(A);
        e = diag(D);
        eigs_all(:, n) = e;
        vecs_all(:, :, n) = V;

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
            fprintf(fid, '    }}},\n\n');
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
            fprintf(fid, '    }}},\n\n');
        else
            fprintf(fid, '    }}}\n');
        end
    end
    fprintf(fid, '};\n\n');

    eigvec_var_name = ['vecs_' category '_' prefix '_' suffix];
    fprintf(fid, 'static constexpr Matrix<Complex<double>, %d, %d> %s[%d] = {\n', S, S, eigvec_var_name, num_cases);
    for n = 1:num_cases
        V = vecs_all(:, :, n);
        fprintf(fid, '    {{{\n');
        for i = 1:S
            fprintf(fid, '        {{');
            for j = 1:S
                if j > 1
                    fprintf(fid, ', ');
                end
                fprintf(fid, '{%.16e, %.16e}', real(V(i,j)), imag(V(i,j)));
            end
            if i < S
                fprintf(fid, '}},\n');
            else
                fprintf(fid, '}}\n');
            end
        end
        if n < num_cases
            fprintf(fid, '    }}},\n\n');
        else
            fprintf(fid, '    }}}\n');
        end
    end
    fprintf(fid, '};\n\n');
end

% Random Cases
size_str = sprintf('%dx%d', MATRIX_SIZE, MATRIX_SIZE);
generate_cases(fid, 'sym', MATRIX_SIZE, NUM_RANDOM_CASES, size_str, 'random');
generate_cases(fid, 'nonsym', MATRIX_SIZE, NUM_RANDOM_CASES, size_str, 'random');

ROBUST_CATEGORIES = {'defective', 'nearly_defective', 'non_normal', 'clustered', 'repeated', ...
                     'companion', 'graded', 'large_jordan', 'toeplitz', 'nearly_reducible', ...
                     'random_non_normal', 'hamiltonian', 'sparse_interior'};

for c = 1:length(ROBUST_CATEGORIES)
    cat = ROBUST_CATEGORIES{c};
    generate_cases(fid, 'nonsym', MATRIX_SIZE, NUM_ROBUST_CASES, size_str, cat);
end

% 3. QR Decomposition Test Case
A_qr = randn(MATRIX_SIZE);
[Q, R] = qr(A_qr);
fprintf(fid, '// QR Decomposition Test Case\n');
fprintf(fid, 'static constexpr Matrix<double, %d, %d> mat_qr\n{{{\n', MATRIX_SIZE, MATRIX_SIZE);
for i = 1:MATRIX_SIZE
    fprintf(fid, '    {');
    for j = 1:MATRIX_SIZE
        if j > 1, fprintf(fid, ', '); end
        fprintf(fid, '%.16e', A_qr(i,j));
    end
    if i < MATRIX_SIZE, fprintf(fid, '},\n'); else fprintf(fid, '}\n'); end
end
fprintf(fid, '}}};\n\n');

fprintf(fid, '} // namespace consteig\n');
fprintf(fid, '#endif\n');
fclose(fid);

% Now generate the C++ test files
[status, output] = system('rm eigen/tests/generated_*.test.cpp');

% Write a file containing the variant of the test case
function write_test_file(filename, category, type, index, S)
    fid = fopen(filename, 'w');
    fprintf(fid, '#include "generated_test_helpers.hpp"\n');

    size_str = sprintf('%dx%d', S, S);
    test_name  = [category '_' size_str '_' num2str(index)];
    test_check = ['check_single_' category '_' type '_' size_str '<' num2str(index) '>'];

    fprintf(fid, 'TEST(generated_tests, %s)\n{\n    static_assert(%s(), "Test %s failed");\n    SUCCEED();\n}\n', ...
            test_name, test_check, test_name);

    fclose(fid);
end

% Random sym (one file per case, matrix size inside)
for i = 0:NUM_RANDOM_CASES-1
    write_test_file(sprintf('eigen/tests/generated_sym_%d.test.cpp', i), 'random', 'sym', i, MATRIX_SIZE);
end

% Random nonsym (one file per case, matrix size inside)
for i = 0:NUM_RANDOM_CASES-1
    write_test_file(sprintf('eigen/tests/generated_nonsym_%d.test.cpp', i), 'random', 'nonsym', i, MATRIX_SIZE);
end

% Robust cases (one file per category+index, matrix size inside)
for c = 1:length(ROBUST_CATEGORIES)
    cat = ROBUST_CATEGORIES{c};
    for i = 0:NUM_ROBUST_CASES-1
        write_test_file(sprintf('eigen/tests/generated_robust_%s_%d.test.cpp', cat, i), cat, 'nonsym', i, MATRIX_SIZE);
    end
end
