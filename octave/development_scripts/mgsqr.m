function [Q,R] = mgsqr(A)

[m,n] = size(A);
Q = zeros(m,n); 
R = zeros(n,n);

V = A;

for j = 1:n
  R(j,j) = norm(V(1:n,j));
  Q(1:n,j) = (1/R(j,j))*V(1:n,j);
  for k = (j+1):n
    R(j,k) = Q(1:n,j)'*V(1:n,k);
    V(1:n,k) = V(1:n,k) - (R(j,k)*Q(1:n,j));
  endfor
endfor

end
