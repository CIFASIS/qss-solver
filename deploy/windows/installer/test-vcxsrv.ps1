param (
    [string]$InstallPath='C:\Program Files\VcXsrv' 
)

Write-Host Check VcXsrv installation folder. 

if (Test-Path -Path $InstallPath) {
    Write-Host $InstallPath
    Write-Host Found VcXsrv installation folder!
} else {
    Write-Host VcXsrv installation folder no found on this machine!
}