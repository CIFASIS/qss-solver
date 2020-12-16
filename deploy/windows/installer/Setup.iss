#define companyname     "CIFASIS Conicet"
#define appname         "QSS Solver Setup"
#define installername   "QSSSolverSetup"
#define appdesc         "QSS Solver initial setup, enable WSL for Windows 10 machines and install VcXsrv"

; Scripts files needed for the installer and troubleshooting
#define enable_wsl         "enable-wsl.ps1"
#define check_wsl          "check-wsl.ps1"
#define vcxsrv             "vcxsrv-64.exe"

#define BUILD_DIR   "." 

[Setup]
AppName={#appname}
AppVerName={#appname}
AppID=QSS_Solver
DefaultDirName={userpf}\{#companyname}\{#appname}
DefaultGroupName={#companyname}\{#appname} 
ShowLanguageDialog=auto
DisableWelcomePage=true
OutputBaseFilename={#installername}
OutputDir={#BUILD_DIR}
UninstallLogMode=append
UninstallDisplayName={#appname}
AppPublisher={#companyname}
ArchitecturesInstallIn64BitMode=x64
DisableDirPage=yes

[Files]
; Installation scripts.
Source: {#BUILD_DIR}\{#check_wsl}; DestDir:{tmp}; Flags: dontcopy
Source: {#BUILD_DIR}\{#enable_wsl}; DestDir:{tmp}; BeforeInstall: checkWSL; AfterInstall: enableWSL;
Source: {#BUILD_DIR}\{#vcxsrv}; DestDir: {tmp}; AfterInstall: InstallVcXsrv; 

[Code]
const                                                             
  QuitMessageError = 'Error. QSS Solver required dependency failed to install.';
  WarningMessage = 'WSL installation requires to restart the computer, please close all running applications before continuing.'; 

var 
  CancelWithoutPrompt: boolean;

procedure CleanInstallationRegistryKeys;
begin
  RegDeleteKeyIncludingSubKeys(HKCU, 'Software\QSSSolver\WSLEnabled');
  RegDeleteKeyIncludingSubKeys(HKCU, 'Software\QSSSolver\WSLRestartNeeded');
end;

procedure CancelButtonClick(CurPageID: Integer; var Cancel, Confirm: Boolean);
begin
  if CurPageID=wpInstalling then
    Confirm := not CancelWithoutPrompt;
end;

procedure checkWSL; 
var
  ResultCode: Integer;
  FileName: String;
  Params: String;
  ScriptParams: String;
  WSLEnabled: Boolean;
    
begin
  ExtractTemporaryFile('{#check_wsl}');
  FileName := '"' + ExpandConstant('{tmp}\{#check_wsl}') + '"';
  Params := '-ExecutionPolicy Bypass -File ';
  ScriptParams := ' -Install';
  if not Exec('powershell.exe', Params+FileName+ScriptParams, '', SW_HIDE,
      ewWaitUntilTerminated, ResultCode) then
  begin
    CleanInstallationRegistryKeys;
    Exit;
  end;
  WSLEnabled := RegKeyExists(HKCU, 'Software\QSSSolver\WSLEnabled');
  if (not WSLEnabled) then
  begin
    if MsgBox(WarningMessage, mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDNO then
    begin
      CleanInstallationRegistryKeys;
      CancelWithoutPrompt := true;
      WizardForm.Close;
      Exit;
    end;
  end;
end;

procedure enableWSL; 
var
  ResultCode: Integer;
  FileName: String;
  Params: String;
  ScriptParams: String;
  WSLEnabled: Boolean;
    
begin
  ExtractTemporaryFile('{#enable_wsl}');
  WSLEnabled := RegKeyExists(HKCU, 'Software\QSSSolver\WSLEnabled');
  if (not WSLEnabled) then
  begin
    FileName := '"' + ExpandConstant('{tmp}\{#enable_wsl}') + '"';
    Params := '-ExecutionPolicy Bypass -File ';
    ScriptParams := ' -Install';
    if not Exec('powershell.exe', Params+FileName+ScriptParams, '', SW_HIDE,
        ewWaitUntilTerminated, ResultCode) then
    begin
      MsgBox(QuitMessageError, mbConfirmation, MB_YESNO or MB_DEFBUTTON2);
      CleanInstallationRegistryKeys;
      Exit;
    end;
    WSLEnabled := RegKeyExists(HKCU, 'Software\QSSSolver\WSLRestartNeeded');
    if (not WSLEnabled) then
    begin
      MsgBox(QuitMessageError, mbConfirmation, MB_YESNO or MB_DEFBUTTON2);
      CleanInstallationRegistryKeys;
      Exit;
    end;
  end;
end;

procedure InstallVcXsrv();
var
  ResultCode: Integer;
  VcXsrvInstalled: Boolean;
  
begin
  VcXsrvInstalled := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\VcXsrv');
  if not VcXsrvInstalled then
  begin
    if not Exec(ExpandConstant('{tmp}\{#vcxsrv}'), '', '', SW_SHOW,
       ewWaitUntilTerminated, ResultCode) then
    begin
      CleanInstallationRegistryKeys;
      Exit;
    end;
   
    VcXsrvInstalled := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\VcXsrv');
    if not VcXsrvInstalled then
    begin
      CleanInstallationRegistryKeys;
      Exit;
    end;
  end;
end;

function NeedRestart(): Boolean;
var
WSLRestartNeeded: Boolean;
  
begin
  WSLRestartNeeded := RegKeyExists(HKCU, 'Software\QSSSolver\WSLRestartNeeded');
  CleanInstallationRegistryKeys;
  Result := WSLRestartNeeded;
end;
