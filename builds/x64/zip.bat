@echo off

set NAME=Spellcross-Map-Editor-V0.90-beta
set ZPATH=%cd%

CALL :ZIPIT %ZPATH% %NAME%
EXIT

:ZIPIT
set FNAME=%~nx2
7z.exe a "%1\%FNAME%.7z" "%2"
EXIT /B