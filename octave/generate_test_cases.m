% Script to generate C++ test cases for consteig
% Run this in Octave/Matlab to generate 'test_dependencies/generated_cases.hpp'
% and individual test files in 'eigen/tests/'

% Fix seed for reproducibility
rand('seed', 42);
randn('seed', 42);

output_cases_file = 'test_dependencies/generated_cases.hpp';
fid = fopen(output_cases_file, 'w');

fprintf(fid, '#ifndef GENERATED_CASES_HPP\n');
fprintf(fid, '#define GENERATED_CASES_HPP\n\n');
fprintf(fid, '#include "../consteig.hpp"\n\n');
fprintf(fid, 'namespace consteig {\n\n');

NUM_RANDOM_CASES = 50;
NUM_ROBUST_CASES = 10;

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
    
    for n = 1:num_cases
        if strcmp(category, 'random')
            if strcmp(type_str, 'sym')
                A = rand(S);
                A = A + A';
            else
                A = rand(S);
            end
        elseif strcmp(category, 'defective')
            A = zeros(S);
            val = randn();
            for i = 1:S
                A(i,i) = val;
                if i < S, A(i, i+1) = 1.0; end
            end
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'nearly_defective')
            A = zeros(S);
            val = randn();
            eps_val = 1e-6;
            for i = 1:S
                A(i,i) = val + i*eps_val;
                if i < S, A(i, i+1) = 1.0; end
            end
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'non_normal')
            A = triu(randn(S), 1) + diag(linspace(1, S, S));
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'clustered')
            e = [ones(1, S-1), 1 + 1e-5];
            A = diag(e);
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'repeated')
            e = ones(1, S);
            A = diag(e);
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'companion')
            p = randn(1, S+1);
            A = compan(p);
        elseif strcmp(category, 'graded')
            A = randn(S) .* 10.^(rand(S)*2 - 1);
        elseif strcmp(category, 'large_jordan')
            A = zeros(S);
            val = 2.0;
            for i = 1:S
                A(i,i) = val;
                if i < S, A(i, i+1) = 1.0; end
            end
        elseif strcmp(category, 'toeplitz')
            A = toeplitz(randn(1, S));
        elseif strcmp(category, 'nearly_reducible')
            A = randn(S);
            A(floor(S/2)+1:end, 1:floor(S/2)) *= 1e-5;
        elseif strcmp(category, 'random_non_normal')
            A = randn(S);
            A = A + triu(randn(S), 1);
        elseif strcmp(category, 'hamiltonian')
            n_half = floor(S/2);
            if n_half * 2 == S
                M = randn(n_half);
                G = randn(n_half); G = G + G';
                H = randn(n_half); H = H + H';
                A = [M G; H -M'];
            else
                A = randn(S);
            end
        elseif strcmp(category, 'sparse_interior')
            A = full(sprand(S, S, 0.5));
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

% Random Cases
generate_cases(fid, 'sym', 4, NUM_RANDOM_CASES, 'fast', 'random');
generate_cases(fid, 'nonsym', 4, NUM_RANDOM_CASES, 'fast', 'random');
generate_cases(fid, 'sym', 8, NUM_RANDOM_CASES, 'slow', 'random');
generate_cases(fid, 'nonsym', 8, NUM_RANDOM_CASES, 'slow', 'random');

ROBUST_CATEGORIES = {'defective', 'nearly_defective', 'non_normal', 'clustered', 'repeated', ...
                     'companion', 'graded', 'large_jordan', 'toeplitz', 'nearly_reducible', ...
                     'random_non_normal', 'hamiltonian', 'sparse_interior'};

for c = 1:length(ROBUST_CATEGORIES)
    cat = ROBUST_CATEGORIES{c};
    generate_cases(fid, 'nonsym', 4, NUM_ROBUST_CASES, 'fast', cat);
    generate_cases(fid, 'nonsym', 8, NUM_ROBUST_CASES, 'slow', cat);
end

% 3. QR Decomposition Test Case
S = 4;
A_qr = rand(S);
[Q, R] = qr(A_qr);
fprintf(fid, '// QR Decomposition Test Case\n');
fprintf(fid, 'static constexpr Matrix<double, 4, 4> mat_qr\n{{{\n');
for i = 1:4
    fprintf(fid, '    {');
    for j = 1:4
        if j > 1, fprintf(fid, ', '); end
        fprintf(fid, '%.16e', A_qr(i,j));
    end
    if i < 4, fprintf(fid, '},\n'); else fprintf(fid, '}\n'); end
end
fprintf(fid, '}}};\n\n');

fprintf(fid, '} // namespace consteig\n');
fprintf(fid, '#endif\n');
fclose(fid);

% Now generate the C++ test files
[status, output] = system('rm eigen/tests/generated_*.test.cpp');

% Write a file containing both fast and slow variants of the same test case,
% selected at compile time by the CONSTEIG_SLOW_TESTS preprocessor flag.
function write_test_file(filename, category, type, index)
    fid = fopen(filename, 'w');
    fprintf(fid, '#include "generated_test_helpers.hpp"\n');

    if ~strcmp(category, 'random')
        fprintf(fid, '#ifdef ENABLE_ROBUSTNESS\n');
    end

    fast_name  = [category '_fast_' num2str(index)];
    slow_name  = [category '_slow_' num2str(index)];
    fast_check = ['check_single_' category '_' type '_fast<' num2str(index) '>'];
    slow_check = ['check_single_' category '_' type '_slow<' num2str(index) '>'];

    fprintf(fid, '#ifndef CONSTEIG_SLOW_TESTS\n');
    fprintf(fid, 'TEST(generated_tests, %s) { static_assert(%s(), "Test %s failed"); SUCCEED(); }\n', ...
            fast_name, fast_check, fast_name);
    fprintf(fid, '#else\n');
    fprintf(fid, 'TEST(generated_tests, %s) { static_assert(%s(), "Test %s failed"); SUCCEED(); }\n', ...
            slow_name, slow_check, slow_name);
    fprintf(fid, '#endif\n');

    if ~strcmp(category, 'random')
        fprintf(fid, '#endif\n');
    end
    fclose(fid);
end

% Random sym (one file per case, both fast 4x4 and slow 8x8 inside)
for i = 0:NUM_RANDOM_CASES-1
    write_test_file(sprintf('eigen/tests/generated_sym_%d.test.cpp', i), 'random', 'sym', i);
end

% Random nonsym (one file per case, both fast 4x4 and slow 8x8 inside)
for i = 0:NUM_RANDOM_CASES-1
    write_test_file(sprintf('eigen/tests/generated_nonsym_%d.test.cpp', i), 'random', 'nonsym', i);
end

% Robust cases (one file per category+index, both fast 4x4 and slow 8x8 inside)
for c = 1:length(ROBUST_CATEGORIES)
    cat = ROBUST_CATEGORIES{c};
    for i = 0:NUM_ROBUST_CASES-1
        write_test_file(sprintf('eigen/tests/generated_robust_%s_%d.test.cpp', cat, i), cat, 'nonsym', i);
    end
end
