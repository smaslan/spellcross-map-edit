function [y] = randgmanrm(shape, scale, limits=[0 10], N=1)

    % maximum of PDF
    imode = 1.0/((shape - 1.0)*scale);
    
    % get random number lower than limit
    y = [];
    while numel(y) < N
        num = gamrnd(shape,scale,N,1)*imode;    
        y = [y;num(num <= limits(2) & num >= limits(1))];
    endwhile
    y = y(1:N);

endfunction