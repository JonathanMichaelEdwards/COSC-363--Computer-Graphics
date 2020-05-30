T = [ 1 0 0 0;    % transformation 
      0 1 0 -1; 
      0 0 1 0; 
      0 0 0 1];

p = [-3 5 2 1];  % point

Mp = T*p'        % Transformed point
