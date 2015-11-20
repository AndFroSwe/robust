% Matlab script for calculating component sizes in lab 3
% 
clc; close all; clear all;
%% Define parameters
% Define unknown component as 0

R = 0;       % [ohm]
C = 33e-6;       % [F]
fc= 1e3;   % Cutoff frequency [Hz]

%% Calculate parameters
% Calculate components
if R == 0 && C ~= 0
    R = (2*pi*fc*C)^(-1);
    fprintf('For fc=%d Hz and capacitance %0.2f microF \n R=%0.2f ohm\r', fc, C*1e6, R)
elseif C == 0 && R ~= 0
    C = (2*pi*fc*R)^(-1);
    fprintf('For fc=%d Hz and resistance %0.2f R \n C=%0.2f nF\r', fc, R, C*1e9)
else
    error('One R or C must be 0')
end

% Time constant
tau = R*C;
fprintf('Time constant is %0.2f microS\r', tau*1e6) 