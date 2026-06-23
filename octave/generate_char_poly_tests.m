% Generates char_poly test reference values for tests/matrix/char_poly_reference.hpp
% Run from the repo root: octave octave/generate_char_poly_tests.m

rand('seed', 42);
randn('seed', 42);

output_file = fullfile('tests', 'matrix', 'char_poly_reference.hpp');
fid = fopen(output_file, 'w');

fprintf(fid, '#ifndef CHAR_POLY_REFERENCE_HPP\n');
fprintf(fid, '#define CHAR_POLY_REFERENCE_HPP\n\n');
fprintf(fid, '#include <consteig/consteig.hpp>\n\n');
fprintf(fid, 'namespace char_poly_ref\n{\n\n');

function print_matrix_init(fid, A, varname, type_str)
    [R, C] = size(A);
    fprintf(fid, 'static constexpr consteig::Matrix<%s, %du, %du> %s{{\n', type_str, R, C, varname);
    for i = 1:R
        fprintf(fid, '    {');
        for j = 1:C
            if j > 1
                fprintf(fid, ', ');
            end
            if strcmp(type_str, 'float')
                fprintf(fid, '%.8eF', A(i,j));
            else
                fprintf(fid, '%.16e', A(i,j));
            end
        end
        if i < R
            fprintf(fid, '},\n');
        else
            fprintf(fid, '}\n');
        end
    end
    fprintf(fid, '}};\n\n');
end

function print_poly_init(fid, coeffs, varname, type_str)
    N = length(coeffs);
    fprintf(fid, 'static constexpr consteig::Matrix<%s, %du, 1u> %s{{\n', type_str, N, varname);
    for i = 1:N
        if strcmp(type_str, 'float')
            val_str = sprintf('%.8eF', coeffs(i));
        else
            val_str = sprintf('%.16e', coeffs(i));
        end
        if i < N
            fprintf(fid, '    {%s},\n', val_str);
        else
            fprintf(fid, '    {%s}\n', val_str);
        end
    end
    fprintf(fid, '}};\n\n');
end

% 1x1
A = [5.0];
print_matrix_init(fid, A, 'mat_1x1', 'double');
print_poly_init(fid, poly(A), 'poly_1x1', 'double');

% 2x2 diagonal
A = diag([2.0, 3.0]);
print_matrix_init(fid, A, 'mat_2x2_diag', 'double');
print_poly_init(fid, poly(A), 'poly_2x2_diag', 'double');

% 2x2 diagonal (float)
A = diag([2.0, 3.0]);
print_matrix_init(fid, A, 'mat_2x2_diag_f', 'float');
print_poly_init(fid, poly(A), 'poly_2x2_diag_f', 'float');

% 2x2 general
A = [1.0, 2.0; 3.0, 4.0];
print_matrix_init(fid, A, 'mat_2x2_general', 'double');
print_poly_init(fid, poly(A), 'poly_2x2_general', 'double');

% 3x3 upper triangular
A = [1.0, 2.0, 0.0; 0.0, 3.0, 4.0; 0.0, 0.0, 5.0];
print_matrix_init(fid, A, 'mat_3x3_upper_tri', 'double');
print_poly_init(fid, poly(A), 'poly_3x3_upper_tri', 'double');

% 3x3 general
A = [4.0, 2.0, 1.0; 1.0, 3.0, 2.0; 0.0, 1.0, 5.0];
print_matrix_init(fid, A, 'mat_3x3_general', 'double');
print_poly_init(fid, poly(A), 'poly_3x3_general', 'double');

% 4x4 diagonal
A = diag([1.0, 2.0, 3.0, 4.0]);
print_matrix_init(fid, A, 'mat_4x4_diag', 'double');
print_poly_init(fid, poly(A), 'poly_4x4_diag', 'double');

% 5x5 random nonsymmetric
A = randn(5, 5);
print_matrix_init(fid, A, 'mat_5x5_random', 'double');
print_poly_init(fid, poly(A), 'poly_5x5_random', 'double');

% 8x8 random nonsymmetric
A = randn(8, 8);
print_matrix_init(fid, A, 'mat_8x8_random', 'double');
print_poly_init(fid, poly(A), 'poly_8x8_random', 'double');

% 10x10 random symmetric
B = randn(10, 10);
A = B + B';
print_matrix_init(fid, A, 'mat_10x10_sym', 'double');
print_poly_init(fid, poly(A), 'poly_10x10_sym', 'double');

fprintf(fid, '} // namespace char_poly_ref\n\n');
fprintf(fid, '#endif // CHAR_POLY_REFERENCE_HPP\n');
fclose(fid);
printf('Wrote %s\n', output_file);
