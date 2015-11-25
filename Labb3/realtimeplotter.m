close all; clc; clear all;
delete(instrfindall);
delete(timerfindall);

global data;

% Check device manage which COM port to use
data.serialObject = serial('COM14', 'BaudRate', 921600, 'DataBits', 8, 'StopBits', 1, 'InputBufferSize', 4096);
data.L = 4096;                                  % Number of data points                                 
data.f = 80000;                                 % Sampling frequency (microcontroller)
data.T = 1/data.f;                              % Sampling period (microcontroller)
data.fVec = data.f/2*linspace(0,1,data.L/2);    % Frequency vector for FFT
data.fScreen = 50;                              % Update frequency for screen
data.yVec = zeros(1,data.L);                    % Data vector
data.tVec = (1:data.L)*1/data.f;                % Time vector
data.fRead = 500;                               % Update data vector frequency

fopen(data.serialObject);

% For some reason the microcontroller is not sending sometimes
if data.serialObject.BytesAvailable == 0
    disp('Error: Plug out and plug in microcontroller');
    fclose(data.serialObject);
    return 
end

% Start plotting, just empty data first time
data.figureHandle = figure(1);                  
data.plotHandle = plot(data.tVec,data.yVec);
xlabel('t'); ylabel('Amplitude [V]');
grid on;

% Start timers for processing and gethering data
processDataTimer = timer('TimerFcn', @(x,y)processData(), 'Period', 1/data.fScreen, 'ExecutionMode', 'fixedRate');
readDataTimer = timer('TimerFcn', @(x,y)readData(), 'Period', 1/data.fRead, 'ExecutionMode', 'fixedRate');

disp('Starting timers.');

start(processDataTimer);
start(readDataTimer);

input('Press enter to exit.')

stop(processDataTimer);
stop(readDataTimer);

fclose(data.serialObject);