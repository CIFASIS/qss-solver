param (
    [switch]$Install 
)

Write-Host Checking QSS Solver installation

$qss_solver_installed = bash.exe -c "command -v qss-solver"

if ([string]::IsNullOrEmpty($qss_solver_installed)) {
    Write-Host QSS Solver installation failed
    if ($Install) {  
        Write-Host Adding QSS Solver install key.  
        New-Item -Path HKCU:\Software\QSSSolver -Name QSSSolverInstallFailed –Force
    }
} else {
    Write-Host QSS Solver successfully installed!
}
