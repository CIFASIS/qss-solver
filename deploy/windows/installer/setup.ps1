param (
    [string]$InstallDir,
    [string]$TmpDir
     )


Write-Host QSS Solver initial setup...
Write-Host $TmpDir
cd $TmpDir

# Check initial configuration for required dependencies.
$ubuntu_installed = Get-AppxPackage -Name  CanonicalGroupLimited.Ubuntu20.04onWindows
if ($ubuntu_installed.Status -eq "Ok") {
    Write-Host Ubuntu 20.04 for Windows already installed, continue QSS Solver setup.
} else {
    Write-Host Preparing Ubuntu 20.04 package...
    Add-AppxPackage .\Ubuntu_2004.appx
    
    Write-Host Installing Ubuntu 20.04...
    ubuntu2004 install --root
}

cp .\qss-solver.deb $InstallDir\scripts\

cd $InstallDir

# Remove Windows EOF extra characters, needed by Jenkins installer.

$original_file = Convert-Path '.\scripts\qss-solver-setup.sh'
$text = [IO.File]::ReadAllText($original_file) -replace "`r`n", "`n"
[IO.File]::WriteAllText($original_file, $text)

$original_file = Convert-Path '.\scripts\qss-solver-config.sh'
$text = [IO.File]::ReadAllText($original_file) -replace "`r`n", "`n"
[IO.File]::WriteAllText($original_file, $text)

$original_file = Convert-Path '.\scripts\export-vcxsrv-vars.sh'
$text = [IO.File]::ReadAllText($original_file) -replace "`r`n", "`n"
[IO.File]::WriteAllText($original_file, $text)

Write-Host Install QSS Solver
bash.exe ./scripts/qss-solver-setup.sh

ubuntu2004.exe config --default-user qsssolver

Write-Host Configure QSS Solver
bash.exe ./scripts/qss-solver-config.sh
