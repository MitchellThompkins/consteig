function eigvals=eigen( A )
    n = size(A,1);
    eigvals = zeros(n);
    if ( n == 1 )
        eigvals(1) = A(1,1);
    else
        I = eye( n );
        # Reduction to Hessenberg form:
        A_og = A;
        A = hessen( A, size(A,1) );
        # Let's start the shifted QR algorithm with 
        while( norm(A(n,n-1)) > 1e-10 )

            mu = wilkinsonshift( A(n-1,n-1), A(n,n), A(n-1,n) );
            # This line should use faster Hessenberg reduction:
            [Q,R] = mgsqr(A - mu*I);
            # This line needs speeding up, currently O(n^3) operations!: 
            A = R*Q + mu*I;
        endwhile
        # Deflation and recurse:
        eigvals = [A(n,n) ; eigen( A(1:n-1, 1:n-1) )];
    endif
end

# T = [ -4.4529e-01  4.9063e+00 -8.7871e-01  6.3036e+00; -6.3941e+00  1.3354e+01  1.6668e+00  1.1945e+01; 3.6842e+00 -6.6617e+00 -6.0021e-02 -7.0043e+00; 3.1209e+00 -5.2052e+00 -1.4130e+00 -2.8484e+00; ];
