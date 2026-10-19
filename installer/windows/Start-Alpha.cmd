@echo off
setlocal
powershell.exe -NoProfile -File "%~dp0Start-Alpha.ps1" -Config "%~dp0alpha-runtimes.json" %*
set "GEODE_ALPHA_EXIT=%ERRORLEVEL%"
if not "%GEODE_ALPHA_EXIT%"=="0" pause
exit /b %GEODE_ALPHA_EXIT%
