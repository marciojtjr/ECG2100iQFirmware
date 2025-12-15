@echo off
REM Copy hex files to _install directory

set SCRIPT_DIR=%~dp0
set INSTALL_DIR=%SCRIPT_DIR%_install

REM Create _install directory if it doesn't exist
if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
    echo Created _install directory
)

REM Copy hex files
echo Copying hex files to _install...

copy "%SCRIPT_DIR%IQ\Bootloader\iq_bootloader.hex" "%INSTALL_DIR%\" /Y
if errorlevel 1 (
    echo ERROR: Failed to copy iq_bootloader.hex
) else (
    echo Copied iq_bootloader.hex
)

copy "%SCRIPT_DIR%IQ\Bluetooth\iq_bluetooth.hex" "%INSTALL_DIR%\" /Y
if errorlevel 1 (
    echo ERROR: Failed to copy iq_bluetooth.hex
) else (
    echo Copied iq_bluetooth.hex
)

echo.
echo Done!
pause
