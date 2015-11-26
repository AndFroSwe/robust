close all;clear; clc;
format compact

f_samp = 10e3; %Hz
f_nyq = f_samp/2; %Hz
cutoff = 200; %Hz
numtaps = 2 %Order+1 of the filter

%Nyquistfrekvensen är den högsta frekvensen som kan mätas innan vikning
%inträffar.
%Tumregel för skärfrekvens är samplingsfrekvensen delat på 3.
%%
%------------------------------------------------
% Create a signal for demonstration.
%------------------------------------------------
% 320 samples of (1000Hz + 15000 Hz + 40000Hz) at 48 kHz
sample_rate = 48000;
nsamples = 320;
 
F_1KHz = 1000;
A_1KHz = 1.0;
 
F_15KHz = 15000;
A_15KHz = 0.5;

F_40KHz = 40000;
A_40KHz = 0.5;
 
t = [0:(nsamples-1)]/sample_rate;
signal = A_1KHz * sin(2*pi*F_1KHz*t) + A_15KHz*sin(2*pi*F_15KHz*t)+ A_40KHz*sin(2*pi*F_40KHz*t);
%%
%Creating filter
lpFilt = designfilt('lowpassfir', 'FilterOrder', numtaps-1, 'CutoffFrequency', cutoff, 'SampleRate', f_samp)
dataOut = filter(lpFilt,signal);

%%
%------------------------------------------------
% Plot the original and filtered signals.
%------------------------------------------------
 
% The first N-1 samples are "corrupted" by the initial conditions
warmup = numtaps - 1;
 
% The phase delay of the filtered signal
delay = (warmup/2)/sample_rate;
 
% Plot the original signal
plot(t, signal)
hold on; grid on;
% Plot the filtered signal, shifted to compensate for the phase delay
plot(t-delay, dataOut, 'r-')
 
% Plot just the "good" part of the filtered signal.  The first N-1
% samples are "corrupted" by the initial conditions.
p = plot(t(warmup:end)-delay, dataOut(warmup:end), 'g');
p(1).LineWidth = 2;

legend('Input Data','Filtered Data','Good Filtered Data','Location','NorthWest')
title('Plot of Input and Filtered Data')
xlabel('Time [t]');ylabel('Amplitude');
%%
%Export to file
coeff = lpFilt.Coefficients
start = '['; stop = ']';
fid=fopen('Coeffs.txt','w');
fprintf(fid,start);
fprintf(fid,'%f, ', coeff(1:(end-1)));
fprintf(fid,'%f', coeff(end));
fprintf(fid,stop)
fclose(fid);