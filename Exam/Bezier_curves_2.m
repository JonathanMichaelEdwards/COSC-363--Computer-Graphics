clc, clear, close all
% pg 16 lec 10 - Quadratic
syms t;

p0 = [2 0 2];
p1 = [1 -4 2];
p2 = [0 -2 1];

for i = 1:3
    Q = simplifyFraction((1-t)^2*p0(i) + 2*(1-t)*t*p1(i) + t^2*p2(i))  %  x(t), y(t), z(t)
end
