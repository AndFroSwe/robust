function readData()

    global data;
    persistent i;   % Used for contineous refill data vector

    if isempty(i)
        i = 1; 
    end

    % Number of data in serial buffer
    n = data.serialObject.BytesAvailable;

    if n < 1
        return 
    end

    % Read the buffer and put it in a vector
    % Here some optimization can be made
    tmp = fread(data.serialObject, n, 'char');

    % Fill data vector with new data
    for k = 1:n
        data.yVec(i) = tmp(k);
        i = i + 1;
        if i > data.L
            i = 1; 
        end
    end
end