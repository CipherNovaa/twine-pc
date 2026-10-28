@echo off
rem TWINE's test kit for a Windows PC (made by make -C port usbkit). Run it from the stick: a system report, a few
rem short test runs, then the game to play. Everything it writes goes to results\ next to this file; nothing to
rem %APPDATA% or the registry (the file "portable" keeps the game's settings, saves and cards here too).
rem   TEST.bat           all of it
rem   TEST.bat noplay    without starting the game at the end
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0kit\test.ps1" %*
if not "%1"=="noplay" pause
