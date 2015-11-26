clear all, close all, clc;

% Inputs
fc = 200;
fs = 80e3;  % Sampling freq
filter_order = 2;
filter_type = 'low';

% In rads
Ts = 1/fs;
wc = fc*(2*pi);
ws = (2*pi)/Ts;
wn = wc/(ws/2);

% Calc tf
[num, den ] = butter(filter_order, wn, filter_type);
Gz = tf(num, den, Ts)