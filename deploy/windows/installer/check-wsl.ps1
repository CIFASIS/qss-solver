param (
    [switch]$Install 
)

Write-Host Checking if WSL is enabled. 

# Check initial configuration for required dependencies.
$is_wsl_enabled =  Get-WindowsOptionalFeature -Online -FeatureName Microsoft-Windows-Subsystem-Linux | ? state -eq Enabled | Select FeatureName

if ($is_wsl_enabled) {
    Write-Host WSL enabled.
    if ($Install) {
        Write-Host Adding QSS Solver install key.
        New-Item -Path HKCU:\Software\QSSSolver -Name WSLEnabled –Force
    }
} else {
    Write-Host WSL is not enabled on this computer.
}
