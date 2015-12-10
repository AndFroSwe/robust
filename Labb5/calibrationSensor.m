clear all; close all; clc;

ampere = [-0.136 0 0.152];
voltage = [1.52 1.9 2.25];
linearSens = polyfit(voltage, ampere, 1)

plot(ampere, voltage)

grid on;