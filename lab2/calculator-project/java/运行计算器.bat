@echo off
cd /d "%~dp0"
title Scientific Calculator

echo Starting Scientific Calculator...
if exist "%~dp0calculator.jar" (
    start "" javaw -jar "%~dp0calculator.jar"
    if errorlevel 1 java -jar "%~dp0calculator.jar"
) else (
    start "" javaw -cp "%~dp0" Main
    if errorlevel 1 java -cp "%~dp0" Main
)

