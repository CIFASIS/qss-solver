#define companyname     "CIFASIS Conicet"
#define appname         "QSS Solver"
#define installername   "QSSSolver"
#define appdesc         "Modeling and simulation environment for continuous and hybrid systems."
#define mainexe         "qss-solver.bat"
#define config          "config.xlaunch"

; Scripts files needed for the installer and troubleshooting
#define enable_wsl         "enable-wsl.ps1"
#define check_wsl          "check-wsl.ps1"
#define test_wsl           "test-wsl.ps1"
#define check_vcxsrv       "check-vcxsrv.ps1"
#define test_vcxsrv        "test-vcxsrv.ps1"
#define migrate_settings   "migrate-settings.ps1"
#define setup              "setup.ps1"
#define ubuntu             "Ubuntu_2004.appx"
#define qss_solver_setup   "qss-solver-setup.sh"
#define qss_solver_config  "qss-solver-config.sh"
#define check_qss_solver   "check-qss-solver.ps1"
#define qss_solver_icon    "qss-solver-icon.ico"
#define export_vcxsrv_vars "export-vcxsrv-vars.sh"
#define export_vcxsrv_ps   "export-vcxsrv-vars.ps1"
#define enable_opengl      "enable-opengl.ps1"
#define disable_opengl     "disable-opengl.ps1"
#define config_no_opengl   "config-no-opengl.xlaunch"
#define config_opengl      "config-opengl.xlaunch"

#define BUILD_DIR   "." 

[Setup]
AppName={#appname}
AppVerName={#appname}
AppID=QSS_Solver
; install into the current user's Program Files directory
DefaultDirName={userpf}\{#companyname}\{#appname}
DefaultGroupName={#companyname}\{#appname} 
ShowLanguageDialog=auto
DisableWelcomePage=true
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
OutputBaseFilename={#installername}
OutputDir={#BUILD_DIR}
UninstallLogMode=append
UninstallDisplayName={#appname}
AppPublisher={#companyname}
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=lowest

[Files]
; Installation dependencies.
Source: {#BUILD_DIR}\{#ubuntu}; DestDir: {tmp} 

; Installation helpers.
Source: {#BUILD_DIR}\{#test_wsl}; DestDir: {tmp}

; Installation scripts.
Source: {#BUILD_DIR}\{#enable_wsl}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#check_wsl}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#check_vcxsrv}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#test_vcxsrv}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#test_wsl}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#enable_opengl}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#disable_opengl}; DestDir: {app}\scripts
Source: {#BUILD_DIR}\{#migrate_settings}; DestDir: {app}\scripts  
Source: {#BUILD_DIR}\{#qss_solver_setup}; DestDir: {app}\scripts; 
Source: {#BUILD_DIR}\{#qss_solver_config}; DestDir: {app}\scripts; 
Source: {#BUILD_DIR}\{#check_qss_solver}; DestDir: {app}\scripts;
Source: {#BUILD_DIR}\{#export_vcxsrv_vars}; DestDir: {app}\scripts;
Source: {#BUILD_DIR}\{#export_vcxsrv_ps}; DestDir: {app}\scripts;
Source: {#BUILD_DIR}\{#config_opengl}; DestDir: {app}\scripts;
Source: {#BUILD_DIR}\{#config_no_opengl}; DestDir: {app}\scripts;
Source: {#BUILD_DIR}\{#setup}; DestDir: {app}\scripts; AfterInstall: InstallQSSSolver; 

; Main application binaries -- the path may change depending on your options
source: {#BUILD_DIR}\{#mainexe}; DestDir: {app}\bin
Source: {#BUILD_DIR}\{#config}; DestDir: {app}\bin
Source: {#BUILD_DIR}\{#qss_solver_icon}; DestDir: {app}\bin

[Icons]
Name: {group}\{#appname}; Filename: {app}\bin\{#mainexe}; WorkingDir: {app}\bin; IconFilename: "{app}\bin\qss-solver-icon.ico";
Name: {group}\Uninstall; Filename: {uninstallexe};
Name: {userdesktop}\{#appname}; Filename: {app}\bin\{#mainexe}; WorkingDir: {app}\bin; Tasks: "desktopicon\user"; IconFilename: "{app}\bin\qss-solver-icon.ico";
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#appname}"; Filename: {app}\bin\{#mainexe}; Tasks: quicklaunchicon; IconFilename: "{app}\bin\qss-solver-icon.ico";

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"
Name: desktopicon\user; Description: "For the current user only"; GroupDescription: "Additional icons:";
Name: quicklaunchicon; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; 

[Run]
Filename: {app}\bin\{#mainexe}; WorkingDir: {app}\bin; Description: "Run {#appname}";  Flags: PostInstall RunAsCurrentUser NoWait;

[UninstallRun]
Filename: "powershell.exe"; \
  Parameters: "-ExecutionPolicy Bypass -Command ""wsl apt-get -y remove qss-solver"""; \
  WorkingDir: {app}; Flags: waituntilterminated

[UninstallDelete]
Type: filesandordirs; Name: {app}\bin
Type: filesandordirs; Name: {app}\scripts

[Code]
const
  QuitMessageError = '***ERROR***'#13#10#13#10 +
                     'Required dependencies are not installed.'#13#10 +
                     'Please download and run the initial setup installer from: ADD LINK'#13#10 +
                     'first and then run this installer again.'#13#10#13#10;
  
var
  CancelWithoutPrompt: boolean;

procedure CleanInstallationRegistryKeys;
begin
  RegDeleteKeyIncludingSubKeys(HKCU, 'Software\QSSSolver\WSLEnabled');
  RegDeleteKeyIncludingSubKeys(HKCU, 'Software\QSSSolver\NetworkConnFailed');
  RegDeleteKeyIncludingSubKeys(HKCU, 'Software\QSSSolver\VPNConnFailed');
  RegDeleteKeyIncludingSubKeys(HKCU, 'Software\QSSSolver\QSSSolverInstallFailed');
end;

procedure testWSL; 
var
  ResultCode: Integer;
  FileName: String;
  Params: String;
  ScriptParams: String;
    
begin
  ExtractTemporaryFile('{#test_wsl}');
  FileName := '"' + ExpandConstant('{tmp}\{#test_wsl}') + '"';
  Params := '-ExecutionPolicy Bypass -File ';
  ScriptParams := ' -Install';
  Exec('powershell.exe', Params+FileName+ScriptParams, '', SW_HIDE, ewWaitUntilTerminated, ResultCode); 
end;

function QSSSolverDependencies(): Boolean;
var
Dependency: Boolean;

begin
  // Check for required dependencies before starting the installer.
  Dependency := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\VcXsrv');
  if not Dependency then
  begin
    Result := False;
    Exit;
  end;
  testWSL;
  Dependency := RegKeyExists(HKCU, 'Software\QSSSolver\WSLEnabled');
  if not Dependency then
  begin
    Result := False;
    Exit;
  end;
  Result := True;
end;

function InitializeSetup(): Boolean;
var

begin
  if not QSSSolverDependencies() then
  begin
    MsgBox(QuitMessageError, mbError, MB_OK);
    CleanInstallationRegistryKeys;
    Result := False;
    Abort;
  end;
  Result := True;
end;

procedure InstallQSSSolver();
var
FileName: String;
Params: String;
ScriptParams: String;
NetworkConnFailed: Boolean;
QSSSolverInstallFailed: Boolean;
ResultCode: Integer;

begin
  FileName := '"' + ExpandConstant('{app}\scripts\{#setup}') + '"';
  Params := '-ExecutionPolicy Bypass -File ';
  ScriptParams := ' -InstallDir "' + ExpandConstant('{app}') + '" -TmpDir "' + ExpandConstant('{tmp}') + '"';
  if not Exec('powershell.exe', Params+FileName+ScriptParams, '', SW_SHOW,
      ewWaitUntilTerminated, ResultCode) then
  begin
    CleanInstallationRegistryKeys;
    MsgBox('QSS Solver installation failed.',mbError,MB_OK)
    CancelWithoutPrompt := true;
    WizardForm.Close;
  end;
  NetworkConnFailed := RegKeyExists(HKCU, 'Software\QSSSolver\NetworkConnFailed');
  QSSSolverInstallFailed := RegKeyExists(HKCU, 'Software\QSSSolver\QSSSolverInstallFailed');
  CleanInstallationRegistryKeys;
  if NetworkConnFailed then begin
    MsgBox('Network connection failed. Please check your connection and run the installer again',mbError,MB_OK)
    CancelWithoutPrompt := true;
    WizardForm.Close;
  end;
  if QSSSolverInstallFailed then begin
    MsgBox('QSS Solver installation failed. Please check your connection and run the installer again',mbError,MB_OK)
    CancelWithoutPrompt := true;
    WizardForm.Close;
  end;  
end;

procedure CancelButtonClick(CurPageID: Integer; var Cancel, Confirm: Boolean);
begin
  if CurPageID=wpInstalling then
    Confirm := not CancelWithoutPrompt;
end;
