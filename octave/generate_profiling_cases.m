% Generate profiling matrices and C++ source files for compile-time measurement.
% Produces matrices at multiple sizes across all categories, plus individual
% .cpp files that force constexpr eigvals() evaluation for timing.
%
% Usage: run from the repository root directory
%   octave octave/generate_profiling_cases.m

rand('seed', 99);
randn('seed', 99);

SIZES = [2, 4, 6, 8, 10, 12, 14, 16];
NUM_SAMPLES = 10;

CATEGORIES = {'random_sym', 'random_nonsym', ...
              'defective', 'nearly_defective', 'non_normal', 'clustered', ...
              'repeated', 'companion', 'graded', 'large_jordan', 'toeplitz', ...
              'nearly_reducible', 'random_non_normal', 'hamiltonian', ...
              'sparse_interior'};

% ---------- matrix constructors ----------

function A = make_matrix(category, S)
    if strcmp(category, 'random_sym')
        A = randn(S);
        A = A + A';
    elseif strcmp(category, 'random_nonsym')
        A = randn(S);
    elseif strcmp(category, 'defective')
        A = zeros(S);
        val = randn();
        for i = 1:S
            A(i,i) = val;
            if i < S, A(i, i+1) = 1.0; end
        end
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
    elseif strcmp(category, 'nearly_defective')
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
        A = triu(randn(S), 1) + diag(linspace(1, S, S));
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
    elseif strcmp(category, 'clustered')
        e = linspace(1, 1 + 1e-5, S);
        A = diag(e);
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
    elseif strcmp(category, 'repeated')
        e = ones(1, S);
        A = diag(e);
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
    elseif strcmp(category, 'companion')
        p = randn(1, S+1);
        A = compan(p);
    elseif strcmp(category, 'graded')
        A = randn(S) .* 10.^(rand(S)*6 - 3);
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
        L = tril(randn(S), -1);
        U = triu(randn(S), 1) + diag(linspace(1, S, S));
        A = L + U;
        [Q, ~] = qr(randn(S));
        A = Q * A * Q';
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
        A = full(sprandn(S, S, 0.4));
    else
        error('Unknown category: %s', category);
    end
end

% ---------- write C++ matrix literal ----------

function write_matrix(fid, A, S)
    fprintf(fid, '{{{\n');
    for i = 1:S
        fprintf(fid, '    {');
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
    fprintf(fid, '}}}');
end

% ---------- main generation ----------

% Header file with all profiling matrices
hdr = fopen('profiling/generated_profiling_cases.hpp', 'w');
fprintf(hdr, '#ifndef GENERATED_PROFILING_CASES_HPP\n');
fprintf(hdr, '#define GENERATED_PROFILING_CASES_HPP\n\n');
fprintf(hdr, '#include "../consteig.hpp"\n\n');
fprintf(hdr, 'namespace consteig\n{\n\n');

% Clean old generated .cpp files
[~, ~] = system('rm -f profiling/compile_time/profile_*.cpp');

for si = 1:length(SIZES)
    S = SIZES(si);
    size_str = sprintf('%dx%d', S, S);

    for ci = 1:length(CATEGORIES)
        cat = CATEGORIES{ci};
        var_name = sprintf('profiling_mat_%s_%s', cat, size_str);

        fprintf(hdr, '// %s %s\n', cat, size_str);
        fprintf(hdr, 'static constexpr Matrix<double, %d, %d> %s[%d] = {\n', ...
                S, S, var_name, NUM_SAMPLES);

        for n = 1:NUM_SAMPLES
            A = make_matrix(cat, S);

            fprintf(hdr, '    ');
            write_matrix(hdr, A, S);
            if n < NUM_SAMPLES
                fprintf(hdr, ',\n\n');
            else
                fprintf(hdr, '\n');
            end

            % Write individual .cpp file for this (category, size, sample)
            cpp_name = sprintf('profiling/compile_time/profile_%s_%d_%d.cpp', ...
                               cat, S, n - 1);
            cpp = fopen(cpp_name, 'w');
            fprintf(cpp, '#include "../../consteig.hpp"\n\n');
            fprintf(cpp, 'using namespace consteig;\n\n');
            fprintf(cpp, 'static constexpr Matrix<double, %d, %d> mat\n', S, S);
            write_matrix(cpp, A, S);
            fprintf(cpp, ';\n\n');
            fprintf(cpp, 'static constexpr auto result = eigvals(mat);\n\n');
            fprintf(cpp, 'int main() { return 0; }\n');
            fclose(cpp);
        end

        fprintf(hdr, '};\n\n');
    end
end

fprintf(hdr, '} // namespace consteig\n');
fprintf(hdr, '#endif\n');
fclose(hdr);

printf('Generated profiling matrices and %d .cpp files\n', ...
       length(SIZES) * length(CATEGORIES) * NUM_SAMPLES);
