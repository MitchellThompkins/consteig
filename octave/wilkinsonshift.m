function out=wilkinsonshift( a,b,c )
    # Calculate Wilkinson's shift for symmetric matrices: 
    delta = (a-c)/2;
    out = c - sign(delta)*b^2/(abs(delta) + sqrt(delta^2+b^2));
end