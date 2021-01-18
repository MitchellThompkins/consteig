function eigvals=eigen_ds_lsvep( A )
    p = size(A,1);
    H = hessen( A, size(A,1) );

    while(p>2)
      q = p-1;
      s = H(q,q)+H(p,p);
      t = (H(q,q)*H(p,p)) - (H(q,p)*H(p,q));
      
      x = (H(1,1)*H(1,1)) + H(1,2)*H(2,1) - s*H(1,1) + t;
      y = H(2,1)*(H(1,1) + H(2,2) - s);
      z = H(2,1)*H(3,2);
      
      for k=0:p-3  
        r = max(1,k);
      endfor
      p = p-1;
      eigvals = 1;
      
    endwhile
    
end