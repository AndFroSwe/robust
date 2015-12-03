clear all, close all, clc
V = [0.160 0.500 1.11 1.44 1.75 2.05, 2.34 2.98 3.3];
A = [-0.5 -0.4 -0.2 -0.1 0 0.1 0.2 0.4 0.5];

plot(A, V);
xlabel('A')
ylabel('V')

grid on;