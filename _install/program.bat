@echo off
REM Script to program IQ device with avrdude
REM Programs both bootloader and application firmware

setlocal

REM Configuration
set PROGRAMMER=atmelice_pdi
set DEVICE=atxmega64a3u

REM Paths
set SCRIPT_DIR=%~dp0
set AVRDUDE="%SCRIPT_DIR%avrdude-v8.1-windows-x64\avrdude.exe"

REM Hex files
set BOOTLOADER_HEX=%SCRIPT_DIR%iq_bootloader.hex
set APPLICATION_HEX=%SCRIPT_DIR%iq_bluetooth.hex

echo ============================================
echo IQ Device Programmer (avrdude)
echo ============================================
echo Programmer:  %PROGRAMMER%
echo Device:      %DEVICE%
echo Bootloader:  %BOOTLOADER_HEX%
echo Application: %APPLICATION_HEX%
echo ============================================

REM Check avrdude exists
if not exist %AVRDUDE% (
    echo ERROR: avrdude.exe not found at %AVRDUDE%
    exit /b 1
)

REM Check hex files
if not exist "%BOOTLOADER_HEX%" (
    echo ERROR: Bootloader hex not found: %BOOTLOADER_HEX%
    exit /b 1
)

if not exist "%APPLICATION_HEX%" (
    echo ERROR: Application hex not found: %APPLICATION_HEX%
    exit /b 1
)

REM Erase if requested
if "%1"=="--erase" (
    echo.
    echo Erasing chip...
    %AVRDUDE% -c %PROGRAMMER% -p %DEVICE% -e
    if errorlevel 1 (
        echo ERROR: Chip erase failed
        exit /b 1
    )
    echo Chip erased successfully
)

REM Program bootloader
echo.
echo Programming bootloader...
%AVRDUDE% -c %PROGRAMMER% -p %DEVICE% -U flash:w:"%BOOTLOADER_HEX%":i
if errorlevel 1 (
    echo ERROR: Bootloader programming failed
    exit /b 1
)
echo Bootloader programmed successfully

REM Program application
echo.
echo Programming application...
%AVRDUDE% -c %PROGRAMMER% -p %DEVICE% -U flash:w:"%APPLICATION_HEX%":i
if errorlevel 1 (
    echo ERROR: Application programming failed
    exit /b 1
)
echo Application programmed successfully

echo.
echo ============================================
echo Programming complete!
echo ============================================

pause
