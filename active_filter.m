% Design for active filter, non inverting
clc; close all; clear all;
%% Define parameters
Ri = 300; % Input resistance [ohm]
Rf = 300; % Feedback resistance [ohm]
C = 100e-12;  % Capacitance [F]

%% Test bode for active filter equation
% Amplification of op amp
% A = @(w, RI, RF, CF) -(RF/(RI*(1+1i*w*CF*RF))); 
% Passive filter 
A = @(w, RF, CF) 1/(1 + w*RF*CF); 
% Decibel conversion
dB = @(in) 20*log10(in);

freq = 1:1e3;
for i = 1:length(freq)
%     signal_filtered = A(freq(i), Ri, Rf, C); 
    signal_filtered = A(freq(i), 300, 1e-6);  
    amp(i) = dB(abs(signal_filtered));
    arg(i) = phase(signal_filtered);
%     amp(i) = abs(A(freq(i), Ri, Rf, C));
end

% Plot parameters
subplot(2,1,1)
loglog(freq, amp)
title('Bode plot of filter')
xlabel('Rad/s')
ylabel('Amplification')
subplot(2,1,2)
loglog(freq, arg,'r')
xlabel('Rad/s')
ylabel('Phase')
grid on