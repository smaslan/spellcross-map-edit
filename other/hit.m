%====================================================================================================
% Unit-unit hit algorithm
%====================================================================================================
close all;
clear all;
clc;

% this folder
mfld = fileparts(mfilename('fullpath'));
cd(mfld);

morale_list = linspace(0.2,1.0,7);

figure;
for m = 1:numel(morale_list)

    morale = morale_list(m); 

    x = randgmanrm(3.5,2.0,[0.75*morale 3/morale],1000000)*(morale);
    
    
    
    [nn,xx] = hist(x,50,1);
    plot(xx,nn)
    hold on;

endfor
hold off;


