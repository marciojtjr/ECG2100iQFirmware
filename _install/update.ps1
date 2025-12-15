# Firmware update script - sends hex file over serial to bootloader
# Detects FTDI port, sends 'up' command, waits for bootloader, sends hex file

param(
    [string]$HexFile = "$PSScriptRoot\iq_bluetooth.hex",
    [int]$BaudRate = 115200,
    [int]$Timeout = 10
)

# Find FTDI COM port
function Find-FTDIPort {
    $ports = Get-WmiObject Win32_PnPEntity | Where-Object { $_.Name -match "COM\d+" -and $_.Name -match "FTDI|USB Serial|USB-SERIAL" }
    if ($ports) {
        $portName = [regex]::Match($ports.Name, "COM\d+").Value
        return $portName
    }

    # Fallback: list all COM ports
    $comPorts = [System.IO.Ports.SerialPort]::GetPortNames()
    if ($comPorts.Count -eq 1) {
        return $comPorts[0]
    }
    elseif ($comPorts.Count -gt 1) {
        Write-Host "Available COM ports:"
        $comPorts | ForEach-Object { Write-Host "  $_" }
        $selection = Read-Host "Enter COM port (e.g., COM3)"
        return $selection
    }
    return $null
}

# Main script
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "IQ Firmware Updater" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan

# Check hex file exists
if (-not (Test-Path $HexFile)) {
    Write-Host "ERROR: Hex file not found: $HexFile" -ForegroundColor Red
    exit 1
}

Write-Host "Hex file: $HexFile"

# Find FTDI port
Write-Host "`nSearching for FTDI serial port..."
$comPort = Find-FTDIPort

if (-not $comPort) {
    Write-Host "ERROR: No COM port found" -ForegroundColor Red
    exit 1
}

Write-Host "Found port: $comPort" -ForegroundColor Green

# Open serial port
try {
    $port = New-Object System.IO.Ports.SerialPort $comPort, $BaudRate, None, 8, One
    $port.Handshake = [System.IO.Ports.Handshake]::XOnXOff
    $port.ReadTimeout = $Timeout * 1000
    $port.WriteTimeout = 5000
    $port.DtrEnable = $true
    $port.RtsEnable = $true
    $port.Open()

    Write-Host "Port opened: $comPort @ $BaudRate baud (XON/XOFF)" -ForegroundColor Green
}
catch {
    Write-Host "ERROR: Failed to open port: $_" -ForegroundColor Red
    exit 1
}

try {
    # Clear buffers
    $port.DiscardInBuffer()
    $port.DiscardOutBuffer()

    # Send 'up' command
    Write-Host "`nSending 'up' command..."
    $port.WriteLine("up")

    # Wait for bootloader response
    Write-Host "Waiting for bootloader..."
    $startTime = Get-Date
    $response = ""
    $bootloaderReady = $false

    while (((Get-Date) - $startTime).TotalSeconds -lt $Timeout) {
        if ($port.BytesToRead -gt 0) {
            $data = $port.ReadExisting()
            $response += $data
            Write-Host $data -NoNewline

            if ($response -match "Update mode|Bootloader|update") {
                $bootloaderReady = $true
                Write-Host "`n"
                break
            }
        }
        Start-Sleep -Milliseconds 100
    }

    if (-not $bootloaderReady) {
        Write-Host "`nERROR: Bootloader not responding (timeout)" -ForegroundColor Red
        exit 1
    }

    Write-Host "Bootloader ready!" -ForegroundColor Green
    Start-Sleep -Milliseconds 500
    $port.DiscardInBuffer()

    # Send hex file
    Write-Host "`nSending hex file (XON/XOFF flow control)..."
    $lines = Get-Content $HexFile
    $totalLines = $lines.Count
    $lineNum = 0
    $response = ""
    $successCount = 0
    $errorCount = 0

    Write-Host "Lines to send: $totalLines"

    foreach ($line in $lines) {
        $lineNum++

        # Send line
        $port.WriteLine($line)

        # Read any available response (non-blocking)
        while ($port.BytesToRead -gt 0) {
            $data = $port.ReadExisting()
            $response += $data
            foreach ($char in $data.ToCharArray()) {
                if ($char -eq '.') {
                    Write-Host $char -NoNewline -ForegroundColor Green
                    $successCount++
                }
                elseif ($char -eq 'E') {
                    Write-Host $char -NoNewline -ForegroundColor Red
                    $errorCount++
                }
                elseif ($char -match "[A-Za-z]") {
                    Write-Host $char -NoNewline -ForegroundColor Gray
                }
            }
        }

        # Progress every 50 lines
        if ($lineNum % 50 -eq 0) {
            $percent = [math]::Round(($lineNum / $totalLines) * 100)
            Write-Host " [$percent%]" -NoNewline -ForegroundColor Cyan
        }
    }

    Write-Host "`nFile sent, waiting for completion..." -ForegroundColor Yellow

    # Wait for final responses
    $timeout = 10
    $startTime = Get-Date
    while (((Get-Date) - $startTime).TotalSeconds -lt $timeout) {
        if ($port.BytesToRead -gt 0) {
            $data = $port.ReadExisting()
            $response += $data
            Write-Host $data -NoNewline -ForegroundColor Gray
            $startTime = Get-Date  # Reset timeout on data
        }
        if ($response -match "App start") {
            break
        }
        Start-Sleep -Milliseconds 50
    }

    Write-Host "`n"
    Write-Host "Records: $successCount OK, $errorCount errors"

    if ($response -match "App start") {
        Write-Host "`n============================================" -ForegroundColor Cyan
        Write-Host "Firmware update complete!" -ForegroundColor Green
        Write-Host "============================================" -ForegroundColor Cyan
    }
    elseif ($errorCount -gt 0) {
        Write-Host "`nUpdate had errors" -ForegroundColor Red
    }
    else {
        Write-Host "`nTransfer complete" -ForegroundColor Yellow
    }
}
catch {
    Write-Host "ERROR: $($_.Exception.Message)" -ForegroundColor Red
}
finally {
    if ($port.IsOpen) {
        $port.Close()
    }
    $port.Dispose()
}
