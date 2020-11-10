Write-Host Check XServer instance
$is_xserver_running=C:\"Program Files"\VcXsrv\xwininfo.exe -root -display localhost:0.0
if ([string]::IsNullOrEmpty($is_xserver_running))
{
    Write-Host Launching X Server instance for display 0.0
    C:\"Program Files"\VcXsrv\xlaunch.exe -run config.xlaunch
}
else
{
    Write-Host X Server instance running
}