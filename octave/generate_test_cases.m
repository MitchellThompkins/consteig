% Script to generate C++ test cases for consteig
% Run this in Octave/Matlab to generate 'eigen/tests/generated_cases.hpp'
% and individual test files in 'eigen/tests/'

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
            % Plain random matrix with normally distributed entries (mean 0, unit
            % variance), giving both positive and negative values. Symmetric variant
            % ensures all-real eigenvalues (A + A' is symmetric by construction).
            if strcmp(type_str, 'sym')
                A = randn(S);
                A = A + A';
            else
                A = randn(S);
            end
        elseif strcmp(category, 'defective')
            % Defective matrix: a Jordan block with a single repeated eigenvalue and
            % ones on the superdiagonal. A defective matrix cannot be diagonalized
            % (it has fewer linearly independent eigenvectors than eigenvalues).
            % Rotated by a random orthogonal Q so it doesn't look trivially like a
            % Jordan block.
            A = zeros(S);
            val = randn();
            for i = 1:S
                A(i,i) = val;
                if i < S, A(i, i+1) = 1.0; end
            end
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'nearly_defective')
            % Nearly defective: a Jordan-like block where the diagonal entries are
            % slightly perturbed (spacing 1e-6) so the matrix is technically
            % diagonalizable but is extremely ill-conditioned. Eigenvalues are
            % almost-repeated, stressing deflation and shift selection.
            % Rotated by a random orthogonal Q.
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
            % Non-normal matrix: strictly upper triangular random part plus evenly
            % spaced diagonal (eigenvalues 1..S). Non-normal matrices have
            % non-orthogonal eigenvectors, making them harder to solve than symmetric
            % ones. Rotated by a random orthogonal Q.
            A = triu(randn(S), 1) + diag(linspace(1, S, S));
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'clustered')
            % Clustered eigenvalues: all S eigenvalues distinct but packed into a
            % tiny interval [1, 1+1e-5] via linspace. No eigenvalue is clearly
            % separated from its neighbours, which stresses shift selection and
            % deflation throughout the entire iteration (unlike 'repeated' where
            % the solver can at least detect exact equality).
            % Built eigenvalue-first: diag(e) is trivial; Q*diag(e)*Q' scrambles
            % it into a dense matrix via a random orthogonal Q.
            e = linspace(1, 1 + 1e-5, S);
            A = diag(e);
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'repeated')
            % Exactly repeated eigenvalues: all S eigenvalues equal to 1.
            % The worst-case limit of clustering. Built eigenvalue-first like
            % 'clustered': diag(e) has the desired eigenvalues; Q*diag(e)*Q'
            % scrambles it into a dense matrix via a random orthogonal Q.
            e = ones(1, S);
            A = diag(e);
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'companion')
            % Companion matrix of a random polynomial. Companion matrices arise in
            % polynomial root-finding and tend to be ill-conditioned with eigenvalues
            % spread across the complex plane.
            p = randn(1, S+1);
            A = compan(p);
        elseif strcmp(category, 'graded')
            % Graded matrix: entries scaled by random powers of 10 spanning [-3, 3],
            % giving a 10^6 dynamic range. The wide magnitude spread is the whole
            % point of this case — it stresses the balancing step which must rescale
            % the matrix before iteration to avoid floating-point over/underflow.
            A = randn(S) .* 10.^(rand(S)*6 - 3);
        elseif strcmp(category, 'large_jordan')
            % Large Jordan block at eigenvalue 2: a pure Jordan block with no
            % rotation applied. The largest possible defective structure for size S,
            % used to measure the hard accuracy floor for defective matrices.
            A = zeros(S);
            val = 2.0;
            for i = 1:S
                A(i,i) = val;
                if i < S, A(i, i+1) = 1.0; end
            end
        elseif strcmp(category, 'toeplitz')
            % Toeplitz matrix: constant diagonals (each diagonal has the same value).
            % Arises in signal processing and time-series problems. Random first row
            % gives a non-symmetric Toeplitz matrix.
            A = toeplitz(randn(1, S));
        elseif strcmp(category, 'nearly_reducible')
            % Nearly block-triangular matrix: the lower-left block is scaled by 1e-5,
            % making the matrix almost block upper-triangular. Stresses the coupling
            % detection between subproblems during deflation.
            A = randn(S);
            A(floor(S/2)+1:end, 1:floor(S/2)) *= 1e-5;
        elseif strcmp(category, 'random_non_normal')
            % Explicitly non-normal matrix: built as A = L + U where L is strictly
            % lower triangular and U is upper triangular with a well-spread diagonal
            % (linspace(1,S,S)). This guarantees AA' != A'A (non-normality) while
            % keeping the eigenvalues known and spread (they are the diagonal of U).
            % Rotated by a random orthogonal Q to hide the triangular structure.
            L = tril(randn(S), -1);
            U = triu(randn(S), 1) + diag(linspace(1, S, S));
            A = L + U;
            [Q, R] = qr(randn(S));
            A = Q * A * Q';
        elseif strcmp(category, 'hamiltonian')
            % Hamiltonian matrix: block structure [M G; H -M'] where G and H are
            % symmetric. Eigenvalues come in +/- pairs by construction, arising in
            % optimal control and Riccati equation problems.
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
            % Matrix with ~20% fill: most entries are zero, scattered randomly
            % across all positions including the diagonal. Tests that the solver
            % handles matrices with many structural zeros without special treatment.
            A = full(sprand(S, S, 0.2));
        else
            error('Unknown category: %s', category);
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
