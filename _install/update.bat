@echo off
REM Firmware update script - sends hex file over serial to bootloader
REM Requires FTDI USB serial adapter

powershell -ExecutionPolicy Bypass -File "%~dp0update.ps1" %*
pause
