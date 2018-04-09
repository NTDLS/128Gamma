[Setup]
;-- Main Setup Information
 AppName=128Gamma Encryption
 AppVerName=128Gamma 1.0.3.6
 AppCopyright=Copyright © 1995-2002 NetWorkDLS, Inc.
 DefaultDirName={pf}\128Gamma Encryption
 DefaultGroupName=128Gamma
 UninstallDisplayIcon={app}\128Gamma.Exe
 UninstallStyle = modern
 WizardSmallImageFile=Resources\128Logo.Bmp
 WizardImageFile=Resources\NetSetup.Bmp
 AppMutex=128Gamma Encryption
 Uninstallable=Yes
 LicenseFile=Resources\EULA.txt
 
;-- Windows 2000 & XP (Support Dialog)
 AppPublisher=NetWorkDLS
 AppPublisherURL=http://www.NetWorkDLS.com/
 AppUpdatesURL=http://www.NetWorkDLS.com/
 AppVersion=1.0.3.6

[Types]
 Name: "Full"; Description: "Full installation"
 Name: "Compact"; Description: "Compact installation"
 Name: "Custom"; Description: "Custom installation"; Flags: iscustom

[Components]
 Name: "Main"; Description: "Main Files";     Types: Full Compact Custom; Flags: fixed;
 Name: "DLL";  Description: "128Gamma DLL's"; Types: Full Compact Custom; Flags: fixed;
 Name: "Help"; Description: "Help File";      Types: Full Custom;
 Name: "Command"; Description: "Command Prompt Support";  Types: Full;
 Name: "Thread";  Description: "Advanced Threading Support"; Types: Full;
 Name: "Tracking"; Description: "Use Tracking Support";   Types: Full;

[UninstallRun]
 Filename: "{app}\RegFix.Exe"; Parameters: "Uninstall"

[Files]
 Source: "..\128Gamma.Exe";               DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Main
 Source: "Resources\EULA.txt";            DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Main
 Source: "RegFix\RegFix.Exe";             DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Main
  Source: "Libraries\Registry.Dll";       DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: DLL
 Source: "Libraries\Windows.Dll";         DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: DLL
 Source: "Libraries\BlowFish.Dll";        DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: DLL
 Source: "Help\128Gamma.Chm";             DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Help
 Source: "128Cmd\128Cmd.Exe";             DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Command
 Source: "128Query\128Query.Exe";         DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Tracking
 Source: "128Priority\128Priority.Exe";   DestDir: "{app}"; CopyMode: "alwaysoverwrite"; Components: Thread

[Icons]
 Name: "{group}\Register 128Gamma";  Filename: "{app}\Register.Exe";    Components: Main
 Name: "{group}\128Gamma Help";      Filename: "{app}\128Gamma.Chm";    Components: Help
 Name: "{group}\128Gamma Query";     Filename: "{app}\128Query.Exe";    Components: Tracking
 Name: "{group}\Set Priority";       Filename: "{app}\128Priority.Exe"; Components: Thread

[Registry]
;-- Create 128Gamma "ALL FILES" Encryption Shell Extention
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption"; ValueType: string;  ValueData: "Open With 128&Gamma"
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\Command"; ValueType: string;  ValueData: """{app}\128Gamma.Exe"" %1"

;-- Create 128Gamma "Folder" Encryption Shell Extention
 Root: HKCR; Subkey: "Directory\shell\Encrypt Folder"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "Directory\shell\Encrypt Folder"; ValueType: string;  ValueData: "Open With 128&Gamma"
 Root: HKCR; Subkey: "Directory\shell\Encrypt Folder\command"; ValueType: string;  ValueData: """{app}\128Gamma.Exe"" %1"

;-- Create 128Gamma "Single File" Association
 Root: HKCR; Subkey: ".128"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: ".128"; ValueType: string;  ValueData: "128File"
 Root: HKCR; Subkey: "128File"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "128File"; ValueType: string;  ValueName: "NeverShowExt"; ValueData:""
 Root: HKCR; Subkey: "128File"; ValueType: string;  ValueData:"128Gamma Encrypted File"
 Root: HKCR; Subkey: "128File\Shell\Open\Command"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "128File\Shell\Open\Command"; ValueType: string;  ValueData:"""{app}\128Gamma.Exe"" %1"
 Root: HKCR; Subkey: "128File\DefaultIcon"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "128File\DefaultIcon"; ValueType: string;  ValueData:"{app}\128Gamma.Exe,2"

;-- Create 128Gamma "Package File" Association
 Root: HKCR; Subkey: ".128Pkg"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: ".128Pkg"; ValueType: string;  ValueData: "128PkgFile"
 Root: HKCR; Subkey: "128PkgFile"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "128PkgFile"; ValueType: string;  ValueName: "NeverShowExt"; ValueData:""
 Root: HKCR; Subkey: "128PkgFile"; ValueType: string;  ValueData:"128Gamma Encrypted Package"
 Root: HKCR; Subkey: "128PkgFile\Shell\Open\Command"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "128PkgFile\Shell\Open\Command"; ValueType: string;  ValueData:"""{app}\128Gamma.Exe"" %1"
 Root: HKCR; Subkey: "128PkgFile\DefaultIcon"; Flags: uninsdeletekey noerror;
 Root: HKCR; Subkey: "128PkgFile\DefaultIcon"; ValueType: string;  ValueData:"{app}\128Gamma.Exe,1"

;-- Create Neptune "ABI-Coder" Association
 Root: HKCR; Subkey: ".abi"; Flags: createvalueifdoesntexist noerror;
 Root: HKCR; Subkey: ".abi"; ValueType: string;         Flags: createvalueifdoesntexist; ValueData: "Neptune.FileType"
 Root: HKCR; Subkey: "Neptune.FileType\";               Flags: createvalueifdoesntexist uninsdeletekey noerror;
 Root: HKCR; Subkey: "Neptune.FileType\DefaultIcon";    Flags: createvalueifdoesntexist uninsdeletekey noerror;
 Root: HKCR; Subkey: "Neptune.FileType\DefaultIcon";    Flags: createvalueifdoesntexist uninsdeletekey noerror; ValueType: string;  ValueData:"{app}\128Gamma.Exe,3"

;-- Create 128Gamma Encryption "New File" Shell Extention
; Root: HKCR; Subkey: ".128\ShellNew"; Flags: uninsdeletekey noerror;
; Root: HKCR; Subkey: ".128\ShellNew"; ValueType: string;  ValueName: "NullFile"; ValueData:""

;-- Delete Old "Open With" Key
 Root: HKCR; Subkey: "Unknown\shell\openas\command"; Flags: deletekey;
 Root: HKCR; Subkey: "Unknown\shell\openas"; Flags: deletekey;

;-- Create New "Open With" Key
 Root: HKCR; Subkey: "*\Shell\openas\command";
 Root: HKCR; Subkey: "*\Shell\openas\command"; ValueType: string;  ValueData: "rundll32.exe shell32.dll,OpenAs_RunDLL %1"

;-- Set "Open With" Key To Default
 Root: HKCR; Subkey: "*\Shell"; ValueType: string;  ValueData: "openas"
 
;-- Create Path Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Path";
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Path"; ValueType: string;  ValueName: "Path";       ValueData:"{app}"
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Path"; ValueType: string;  ValueName: "Executable"; ValueData:"{app}\128Gamma.Exe"
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Path"; ValueType: string;  ValueName: "HelpPath";   ValueData:"{app}\128Gamma.Chm"

;-- Create Dialog Information Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\PasswordDialog"; ValueType: dword;  ValueName: "TopMost"; ValueData:1
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\StatusDialog";   ValueType: dword;  ValueName: "TopMost"; ValueData:1

;-- Create Last Use Information Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\LastRun"; ValueType: string;  ValueName: "User"; ValueData:"N/A"
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\LastRun"; ValueType: string;  ValueName: "Time"; ValueData:"N/A"
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\LastRun"; ValueType: string;  ValueName: "Date"; ValueData:"N/A"

;-- Create Misc Settings Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Settings"; ValueType: dword;  ValueName: "UseCompression"; ValueData:0
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Settings"; ValueType: dword;  ValueName: "DeleteSource"; ValueData:0

;-- Create Version Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Version"; ValueType: string;  ValueName: "ShortVersion"; ValueData:3.5
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Version"; ValueType: string;  ValueName: "LongVersion"; ValueData:3.5 - Not Revised

;-- Create Registration Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry";
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; Flags: createvalueifdoesntexist; ValueType: dword;  ValueName: "128Code";    ValueData:0
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; Flags: createvalueifdoesntexist; ValueType: dword;  ValueName: "128Reg1";    ValueData:0
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; Flags: createvalueifdoesntexist; ValueType: dword;  ValueName: "128Reg2";    ValueData:0
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; Flags: createvalueifdoesntexist; ValueType: dword;  ValueName: "128Reg3";    ValueData:0
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; Flags: createvalueifdoesntexist; ValueType: dword;  ValueName: "Registered"; ValueData:0
 
;-- Create Priority Keys For Use Within 128Gamma
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Priority"; ValueType: dword;  ValueName: "ProcessBoost"; ValueData:1
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Priority"; ValueType: dword;  ValueName: "ThreadBoost";  ValueData:1
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Priority"; ValueType: dword;  ValueName: "Process";      ValueData:2
 Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Priority"; ValueType: dword;  ValueName: "Thread";       ValueData:4

;-- Create Registration Keys For Use Within 128Gamma (If Registered)
;Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry";
;Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; ValueType: dword;  ValueName: "128Code";    ValueData:29856748
;Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; ValueType: dword;  ValueName: "128Reg1";    ValueData:4782
;Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; ValueType: dword;  ValueName: "128Reg2";    ValueData:1974
;Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; ValueType: dword;  ValueName: "128Reg3";    ValueData:9184
;Root: HKCR; Subkey: "*\Shell\128Gamma_Encryption\AppInfo\Registry"; ValueType: dword;  ValueName: "Registered"; ValueData:1

