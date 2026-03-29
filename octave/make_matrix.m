% Construct a test matrix of the given category and size S.
%
% Categories:
%   random_sym         - symmetric matrix with normally distributed entries
%   random_nonsym      - non-symmetric matrix with normally distributed entries
%   defective          - Jordan block (single repeated eigenvalue), randomly rotated
%   nearly_defective   - near-Jordan block (slightly perturbed diagonal), randomly rotated
%   non_normal         - upper-triangular + evenly spaced diagonal, randomly rotated
%   clustered          - eigenvalues packed into [1, 1+1e-5], randomly rotated
%   repeated           - all eigenvalues equal to 1, randomly rotated
%   companion          - companion matrix of a random polynomial
%   graded             - entries scaled by random powers of 10 (dynamic range 10^6)
%   large_jordan       - pure Jordan block at eigenvalue 2 (no rotation)
%   toeplitz           - Toeplitz matrix with random first row
%   nearly_reducible   - nearly block upper-triangular (lower-left block scaled 1e-5)
%   random_non_normal  - L+U with independent random L and U, randomly rotated
%   hamiltonian        - block [M G; H -M'] with symmetric G, H
%   sparse_interior    - ~40% fill random sparse matrix

function A = make_matrix(category, S)
    if strcmp(category, 'random_sym')
        % Plain random matrix with normally distributed entries (mean 0, unit
        % variance), giving both positive and negative values. Symmetric variant
        % ensures all-real eigenvalues (A + A' is symmetric by construction).
        A = randn(S);
        A = A + A';
    elseif strcmp(category, 'random_nonsym')
        % Plain random matrix with normally distributed entries (mean 0, unit
        % variance), giving both positive and negative values.
        A = randn(S);
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
        [Q, ~] = qr(randn(S));
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
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
    elseif strcmp(category, 'non_normal')
        % Non-normal matrix: strictly upper triangular random part plus evenly
        % spaced diagonal (eigenvalues 1..S). Non-normal matrices have
        % non-orthogonal eigenvectors, making them harder to solve than symmetric
        % ones. Rotated by a random orthogonal Q.
        A = triu(randn(S), 1) + diag(linspace(1, S, S));
        [Q, ~] = qr(randn(S));
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
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
    elseif strcmp(category, 'repeated')
        % Exactly repeated eigenvalues: all S eigenvalues equal to 1.
        % The worst-case limit of clustering. Built eigenvalue-first like
        % 'clustered': diag(e) has the desired eigenvalues; Q*diag(e)*Q'
        % scrambles it into a dense matrix via a random orthogonal Q.
        e = ones(1, S);
        A = diag(e);
        [Q, ~] = qr(randn(S));
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
        % Non-normal matrix: built as A = L + U where L is strictly lower
        % triangular and U is upper triangular with a well-spread diagonal
        % (linspace(1,S,S)). Independent random L and U make AA' != A'A
        % (non-normality) overwhelmingly likely in practice. Eigenvalues are
        % computed by eig() since L+U is not triangular.
        % Rotated by a random orthogonal Q to hide the triangular structure.
        L = tril(randn(S), -1);
        U = triu(randn(S), 1) + diag(linspace(1, S, S));
        A = L + U;
        [Q, ~] = qr(randn(S));
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
        % Matrix with ~40% fill: most entries are zero, scattered randomly
        % across all positions including the diagonal. Tests that the solver
        % handles matrices with many structural zeros without special treatment.
        % 40% is sparse enough to be meaningfully different from a dense random
        % matrix, while avoiding the near-singular matrices that lower fill rates
        % produce for an 8x8 matrix with normally distributed entries.
        A = full(sprandn(S, S, 0.4));
    else
        error('Unknown category: %s', category);
    end
end
