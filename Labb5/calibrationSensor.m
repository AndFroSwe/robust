clear all; close all; clc;

ampere = [-0.268 0 0.270];
voltage = [1 1.87 2.59];
linearSens = polyfit(voltage, ampere, 1)

plot(ampere, voltage)

grid on;