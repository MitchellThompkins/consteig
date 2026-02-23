function P=house(A,phi)
##  k=1;
##  n=length(A);
##  alpha=-1*sign(A(k+1,k))*sqrt(sum(A((k+1:n),k).^2));
##  r=sqrt(0.5*(alpha^2-A(k+1,k)*alpha));
##  v(1:k)=0;
##  v(k+1)=(A(k+1,k)-alpha)/2/r;
##  for i=k+2:n
##    v(i)=A(i,k)/2/r;
##  endfor
##  u=v';
  k = 1;
  n = length(A);
  
  x(1:k) = 0;
  x(1) = abs(A1)*exp(i*phi) - (-exp(i*phi) * norm(A));
  for i=k+2:n
    x(i) = A(i,k)
  endfor
  
  P=eye(n,n)-2*u*u';
endfunction
