function myeigs=eigen( A )
    n = size(A,1);
    myeigs = zeros(n);
    if ( n == 1 )
        myeigs(1) = A(1,1);
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
        myeigs = [A(n,n) ; eigen( A(1:n-1, 1:n-1) )];
    endif
end
