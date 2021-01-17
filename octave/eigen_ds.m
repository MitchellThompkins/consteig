% Implicit QR step using a Francis double shift
% (there should really be some re-scalings for floating point)
% Compute double-shift poly and initial column of H^2 + b*H + c*I\
function H=eigen_ds( H )
  [b,c] = poly_ds(H);
  C1 = H(1:3,1:2)*H(1:2,1);
  C1(1:2) = C1(1:2) + b*H(1:2,1);
  C1(1) = C1(1) + c;
  % Apply a similarity associated with the first step of QR on C
  v = house(C1);
  H(1:3,:) = H(1:3,:)-2*v*(v'*H(1:3,:));
  H(:,1:3) = H(:,1:3)-(H(:,1:3)*(2*v))*v';
  % Do "bulge chasing" to return to Hessenberg form
  % (compare to lec27hess).
  %
  n = length(H);
  for j = 1:n-2
    k = min(j+3,n);
    % -- Find W = I-2vv’ to put zeros below H(j+1,j), H := WHW’
    v = house(H(j+1:k,j));
    H(j+1:k,:) = H(j+1:k,:)-2*v*(v'*H(j+1:k,:));
    H(:,j+1:k) = H(:,j+1:k)-(H(:,j+1:k)*(2*v))*v';
    H(k,j) = 0;
  endfor
endfunction

##function eigvals=eigen_ds( A )
##    p = size(A,1);
##    H = hessen( A, size(A,1) );
##
##    while(p>2)
##      q = p-1;
##      s = H(q,q)+H(p,p);
##      t = (H(q,q)*H(p,p)) - (H(q,p)*H(p,q));
##      
##      x = (H(1,1)*H(1,1)) + H(1,2)*H(2,1) - s*H(1,1) + t;
##      y = H(2,1)*(H(1,1) + H(2,2) - s);
##      z = H(2,1)*H(3,2);
##      
##      for k=0:p-3  
##        r = max(1,k);
##      endfor
##      p = p-1;
##      eigvals = 1;
##      
##    endwhile
##    
##end

