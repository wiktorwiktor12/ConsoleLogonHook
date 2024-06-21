# Welcome to the ConsoleLogonHook respository

This project hooks into and provide a graphical user interface to the console logon screen for Windows 10


> [!WARNING] 
**This project is currently in various stages of development so use at your own risk.**

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork the repository if you dont have write access
2. Pull using git commandline or any git ui manager such as github desktop
3. enjoy
 
## How to use
1. Copy the 2 compiled DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from ConsoleLogonHook\x64\Release into %SYSTEMROOT%\System32
> [!NOTE]
> **It will most likely ask you to provide adminsistrator permission to copy to System32, just accept the warning**
>
2. Take and rename authui.dll to au7hui.dll from windows 7 sp1 and copy into %SYSTEMROOT%\System32 , this is so the windows 7 consolelogonui can pull the resources from it (note: this step may not be necessary in the future)
 Or alternatively, just copy au7hui.dll from a release 
3. Take ownership of ConsoleLogon.dll and replace it with version, 10.0.19041.3636 (should be in releases)
4. Import the following registry key as TrustedInstaller by copying it into a text file and save as a .reg file then running with psexec64, or using any other method to run something as trusted installer


```
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver]
"DllPath"="C:\\Windows\\System32\\ConsoleLogonHook.dll"

[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen]
"DllPath"="C:\\Windows\\System32\\ConsoleLogonHook.dll"

[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX]
"DllPath"="C:\\Windows\\System32\\ConsoleLogonHook.dll"

[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX]
"DllPath"="C:\\Windows\\System32\\ConsoleLogonHook.dll"
```

4. Take ownership of and rename Windows.UI.Logon.dll to anything else so it will invoke console login, it'll just work and do its thing
