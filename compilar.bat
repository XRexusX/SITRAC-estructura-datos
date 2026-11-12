@echo off
REM Compila SITRAC con g++ (MinGW / MSYS2 / w64devkit). Genera sitrac.exe
g++ -std=c++11 -Wall -Wextra -static -o sitrac.exe src\*.cpp
if errorlevel 1 (
    echo.
    echo *** ERROR de compilacion ***
    exit /b 1
)
echo Compilado: sitrac.exe
