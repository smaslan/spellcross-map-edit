%====================================================================================================
% Spellcross gamma correction generator
%====================================================================================================
close all;
clear all;
clc;

% this folder
mfld = fileparts(mfilename('fullpath'));
cd(mfld);

file = fullfile(mfld, 'GAMMA.TBL');


lin = [0:63].'

gma = linspace(0.8,1.8,11)


tbl = uint8(round(63*((lin/63).^(1./gma))));

plot(tbl)

fw = fopen(file,'w');
fwrite(fw,tbl(:));
fclose(fw);





