% Design for active filter
clc; close all; clear all;
%% Define paramters
Ri = 1e3; % Input resistance [ohm]
Rf = 1e6; % Feedback resistance [ohm]
C = 1000e-6;  % Capacitance [F]
%% Test bode for active filter equation


A = @(w, R1, R2, C1) abs(1 + R2/(R1*(1 + 1i*w*C*R2))); 

freq = 1:1e3;
for i = freq
    amp(i) = A(freq(i), Ri, Rf, C);
end

% Plot parameters
loglog(freq, amp)
title('Bode plot of filter')
xlabel('Rad/s')
ylabel('Amplification')
grid on