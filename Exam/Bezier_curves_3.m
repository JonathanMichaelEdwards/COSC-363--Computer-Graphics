clc, clear, close all
% pg 17 lec 10 - Cubic
syms t;

p0 = [0 0];
p1 = [0 1];
p2 = [1 1];
p3 = [1 0];

for i = 1:2
    Q = simplifyFraction((1-t)^3*p0(i) + 3*(1-t)^2*t*p1(i) + 3*(1-t)*t^2*p2(i) + t^3*p3(i))   %  x(t), y(t)
end
