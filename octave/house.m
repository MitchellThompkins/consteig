function P=house(A)
  k=1;
  n=length(A);
  aerfa=-1*sign(A(k+1,k))*sqrt(sum(A((k+1:n),k).^2));
  r=sqrt(0.5*(aerfa^2-A(k+1,k)*aerfa));
  v(1:k)=0;
  v(k+1)=(A(k+1,k)-aerfa)/2/r;
  for i=k+2:n
    v(i)=A(i,k)/2/r;
  endfor
  v=v';
  P=eye(n,n)-2*v*v';
endfunction
