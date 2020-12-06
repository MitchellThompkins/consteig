function H = hessen(A,L)
  
  m = length(A);
  M = house( A(m-L+1:end,m-L+1:end) );
  if( L<=2 )
    PP = M;
    P = PP;
    H = A;
  else
    PP=eye(m);
    PP(m-L+2:end,m-L+2:end) = M(2:end,2:end);
    
    A = PP*A*PP;
    H = hessen(A,L-1);
  endif

endfunction
