@echo off
setlocal

REM First, try to find cl.exe if it's already in PATH
where cl >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo Using cl.exe from PATH...
    goto :build
)

REM Try to find Visual Studio installation paths
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2022 Community...
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2022 Professional...
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2022 Enterprise...
    call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2019 Build Tools...
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2019 Community...
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2019 Professional...
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Visual Studio 2019 Enterprise...
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

REM Try Windows SDK Build Tools
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    echo Using Windows SDK Build Tools...
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    goto :build
)

REM Check for MinGW as fallback
where gcc >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo Using MinGW GCC compiler...
    goto :build_mingw
)

echo ERROR: Could not find Visual Studio installation or compiler!
echo.
echo Please install one of the following:
echo   1. Visual Studio 2022 Community (free): https://visualstudio.microsoft.com/vs/community/
echo   2. Visual Studio 2019 Community (free): https://visualstudio.microsoft.com/vs/older-downloads/
echo   3. Visual Studio Build Tools: https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio
echo   4. MinGW-w64: https://www.mingw-w64.org/downloads/
echo.
echo After installation, make sure to include "MSVC v143 - VS 2022 C++ x64/x86 build tools"
echo or "C++ Build Tools" in your installation.
exit /b 1

:build
echo Building bt122_monitor.exe with MSVC...
if "%1"=="debug" (
    cl /nologo /Zi /Od /DDEBUG /Fe:bt122_monitor.exe main.c bgapi_parser.c bgapi_commands.c
) else (
    cl /nologo /O2 /Fe:bt122_monitor.exe main.c bgapi_parser.c bgapi_commands.c
)

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

echo Build successful!
goto :end

:build_mingw
echo Building bt122_monitor.exe with MinGW GCC...
if "%1"=="debug" (
    gcc -Wall -Wextra -g -O0 -std=c99 -DDEBUG -o bt122_monitor.exe main.c bgapi_parser.c bgapi_commands.c
) else (
    gcc -Wall -Wextra -O2 -std=c99 -o bt122_monitor.exe main.c bgapi_parser.c bgapi_commands.c
)

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

echo Build successful!

:end