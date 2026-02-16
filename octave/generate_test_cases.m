% Script to generate C++ test cases for consteig
% Run this in Octave/Matlab to generate 'generated_test_data.hpp'

% 1. Symmetric Matrix (Real Eigenvalues)
S = 5;
A_sym = rand(S);
A_sym = A_sym + A_sym'; % Make symmetric
e_sym = eig(A_sym);

printf("// Symmetric Matrix Test Case\n");
% Print A_sym
[rows, cols] = size(A_sym);
printf("    static constexpr Matrix<double, %d, %d> mat_sym\n", rows, cols);
printf("    {{{\n");
for i = 1:rows
    printf("        {");
    for j = 1:cols
        if j > 1
            printf(", ");
        endif
        printf("%.16e", A_sym(i,j));
    end
    if i < rows
        printf("},\n");
    else
        printf("}\n");
    endif
end
printf("    }}};\n\n");

printf("// Expected Eigenvalues (sorted):\n");
e_sym = sort(e_sym);
for i = 1:length(e_sym)
    printf("// %.16e\n", e_sym(i));
end
printf("\n");

% 2. Non-Symmetric Matrix (Complex Eigenvalues)
S = 5;
A_nonsym = rand(S);
e_nonsym = eig(A_nonsym);

printf("// Non-Symmetric Matrix Test Case\n");
% Print A_nonsym
[rows, cols] = size(A_nonsym);
printf("    static constexpr Matrix<double, %d, %d> mat_nonsym\n", rows, cols);
printf("    {{{\n");
for i = 1:rows
    printf("        {");
    for j = 1:cols
        if j > 1
            printf(", ");
        endif
        printf("%.16e", A_nonsym(i,j));
    end
    if i < rows
        printf("},\n");
    else
        printf("}\n");
    endif
end
printf("    }}};\n\n");

printf("// Expected Eigenvalues (unsorted complex):\n");
for i = 1:length(e_nonsym)
    printf("// %.16e + %.16ei\n", real(e_nonsym(i)), imag(e_nonsym(i)));
end
printf("\n");

% 3. QR Decomposition Test Case
S = 4;
A_qr = rand(S);
[Q, R] = qr(A_qr);

printf("// QR Decomposition Test Case\n");
% Print A_qr
[rows, cols] = size(A_qr);
printf("    static constexpr Matrix<double, %d, %d> mat_qr\n", rows, cols);
printf("    {{{\n");
for i = 1:rows
    printf("        {");
    for j = 1:cols
        if j > 1
            printf(", ");
        endif
        printf("%.16e", A_qr(i,j));
    end
    if i < rows
        printf("},\n");
    else
        printf("}\n");
    endif
end
printf("    }}};\n\n");

printf("// Reference Q (Householder from Octave):\n");
% Print Q
[rows, cols] = size(Q);
printf("    static constexpr Matrix<double, %d, %d> ref_q\n", rows, cols);
printf("    {{{\n");
for i = 1:rows
    printf("        {");
    for j = 1:cols
        if j > 1
            printf(", ");
        endif
        printf("%.16e", Q(i,j));
    end
    if i < rows
        printf("},\n");
    else
        printf("}\n");
    endif
end
printf("    }}};\n\n");

printf("// Reference R:\n");
% Print R
[rows, cols] = size(R);
printf("    static constexpr Matrix<double, %d, %d> ref_r\n", rows, cols);
printf("    {{{\n");
for i = 1:rows
    printf("        {");
    for j = 1:cols
        if j > 1
            printf(", ");
        endif
        printf("%.16e", R(i,j));
    end
    if i < rows
        printf("},\n");
    else
        printf("}\n");
    endif
end
printf("    }}};\n\n");
