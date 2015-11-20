clear all; close all; clc;format compact;
% Calculate parameters of A4973

% Given 

Itrip = 1.2; %A



% Wanted
Vref = 1; %0-1V
Rt = 30e3; %Ohm
Ct = 680e-9; %F +/- 5%


%% Current limiter
Rs = Vref/(2*Itrip) %Ohm

%% Driver disable time
Toff = Rt*Ct %s
