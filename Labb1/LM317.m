clear all; close all; clc;
% Calculate parameters of LM317

% Given
R1 = 240;
Vref = 1.25;

% Wanted
Vo = 5;

%% Calculation
R2 = R1*((Vo/Vref - 1))