clear all, close all, clc;

% Inputs
fc = 600;
fs = 10e3;  % Sampling freq
filter_order = 2;
filter_type = 'high';

% In rads
Ts = 1/fs;
wc = fc*(2*pi);
ws = (2*pi)/Ts;
wn = wc/(ws/2);

% Calc tf
[num, den ] = butter(filter_order, wn, filter_type);
Gz = tf(num, den, Ts)