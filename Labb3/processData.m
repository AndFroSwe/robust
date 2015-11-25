function processData()

    global data;
    
    % Perform FFT of data vector, used to find frequency
    Y = fft(data.yVec-mean(data.yVec),data.L)/data.L;
    
    % Frequency of signal where |FFT| is maximum
    fSig = data.fVec(find(max(abs(Y(1:data.L/2)))==abs(Y(1:data.L/2))));
    
    % First time the data vector is zeros(L,1), do this to avoid crash
    if length(fSig) > 1
       fSig = 300; 
    end
    
    % Plot four waves
    timeWindow = 4/fSig;

    % Do the magic
    figure(data.figureHandle);
    set(data.plotHandle,'YData',data.yVec*(3.3/256),'XData',data.tVec);
    xlim([0 timeWindow]);
    
end