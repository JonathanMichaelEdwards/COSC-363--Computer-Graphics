T = [ 1 0 0 0;    % transformation 
      0 1 0 -1; 
      0 0 1 0; 
      0 0 0 1];

p = [-3 5 2 0];  % vector

% Transformation matrix has no effect on a "VECTOR"
Mp = T*p'        % Transformed vector
