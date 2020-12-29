powershell.exe -ExecutionPolicy ByPass -File ../scripts/check-vcxsrv.ps1
cd C:\Users\%USERNAME%\QSSSolver
wsl -u qsssolver -d Ubuntu-20.04 bash -ic "/opt/qss-solver/bin/run_win.sh"
