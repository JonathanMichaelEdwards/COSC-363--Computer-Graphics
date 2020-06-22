clc, clear, close all

p0 = [2 1 -20];
C  = [6 3 -23];
d  = [8/10 0 -6/10]; 
r  = 2;
s = p0 - C;

t1 = -(s*d') + sqrt((s*d')^2 - (s*s') + r^2)
t2 = -(s*d') - sqrt((s*d')^2 - (s*s') + r^2)