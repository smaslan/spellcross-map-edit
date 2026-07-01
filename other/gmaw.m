%====================================================================================================
% Spellcross gamma correction viewer
%====================================================================================================
close all;
clear all;
clc;

% this folder
mfld = fileparts(mfilename('fullpath'));
cd(mfld);

%file = fullfile(mfld, 'ENG_ORIG.TBL');
file = fullfile(mfld, 'GAMMA.TBL');

fr = fopen(file,'r');
dta = fread(fr);
tbl = reshape(dta,[64,11]);
fclose(fr);

plot(tbl)



