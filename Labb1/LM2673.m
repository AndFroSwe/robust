clear all; close all; clc;
% Calculate parameters of LM2673

% Given 
Vfb = 1.21;
R1 = 1e3;
Vinmax = 24;
Vsat = 0.3;
Vd=0.5;
freqswitch = 1000/260; % [Vµs]
const_Radj = 37.125;
tss = 50e-3;    %[s]



% Wanted
Vout = 12;
Imax = 2;

%% Step 2 
R2 = R1*(Vout/Vfb-1)

%% Step 3
ET = ((Vinmax - Vout - Vsat)*(Vout + Vd)/(Vinmax - Vsat + Vd))*freqswitch
fprintf('Yields 47µH inductance\n')

%% Step 4
fprintf('Table 13 yields C5 inductance for both 5V and 12 V (we choose 100µF)\n')

%% Step 8
Radj = const_Radj/(Imax*1.5)

%% Softstart
Css = (3.7e-6*tss)/((0.63 + 2.6*((Vout + 0.5)/Vinmax)))
disp('No softstart limits: 33-1000nF')