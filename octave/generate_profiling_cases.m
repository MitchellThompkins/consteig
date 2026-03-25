% Generate profiling matrices and C++ source files for compile-time measurement.
% Produces matrices at multiple sizes across all categories, plus individual
% .cpp files that force constexpr eigenvalues() evaluation for timing.
%
% Usage: run from the repository root directory
%   octave octave/generate_profiling_cases.m

addpath(fileparts(mfilename('fullpath')));

rand('seed', 99);
randn('seed', 99);

SIZES = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20];
NUM_SAMPLES = 10;

CATEGORIES = {'random_sym', 'random_nonsym', ...
              'defective', 'nearly_defective', 'non_normal', 'clustered', ...
              'repeated', 'companion', 'graded', 'large_jordan', 'toeplitz', ...
              'nearly_reducible', 'random_non_normal', 'hamiltonian', ...
              'sparse_interior'};

% ---------- write C++ matrix literal ----------

function write_matrix(fid, A, S)
    fprintf(fid, '{{\n');
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
    fprintf(fid, '}}');
end

% ---------- main generation ----------

% Header file with all profiling matrices
hdr = fopen('profiling/generated_profiling_cases.hpp', 'w');
fprintf(hdr, '#ifndef GENERATED_PROFILING_CASES_HPP\n');
fprintf(hdr, '#define GENERATED_PROFILING_CASES_HPP\n\n');
fprintf(hdr, '#include "../consteig.hpp"\n\n');
fprintf(hdr, 'namespace consteig\n{\n\n');

% Ensure output directory exists and clean old generated .cpp files
if ~exist(fullfile('profiling', 'compile_time'), 'dir')
    mkdir(fullfile('profiling', 'compile_time'));
end
old_files = dir(fullfile('profiling', 'compile_time', 'profile_*.cpp'));
for k = 1:length(old_files)
    delete(fullfile(old_files(k).folder, old_files(k).name));
end

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
            fprintf(cpp, 'static constexpr auto result = eigenvalues(mat);\n\n');
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
