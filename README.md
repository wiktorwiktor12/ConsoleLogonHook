# Welcome to the ConsoleLogonHook respository

This project hooks into and provide a graphical user interface to the console logon screen for Windows 10


> [!WARNING] 
**This project is currently in various stages of development so use at your own risk.**

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork the repository if you dont have write access
2. Download the source code by 
3. Unzip the downloaded source and open the solution (ConsoleLogonHook.sln) with Visual Studio 2022 and build it in release configuration
4. enjoy

## How to use
1. Copy the 2 compiled DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from ConsoleLogonHook-dui-ui\x64\Release into %SYSTEMROOT%\System32
> [!IMPORTANT]
> It will most likely ask you to provide adminsistrator permission to copy to System32, just hit accept
3. edit these keys by setting the DllPath value to be the path of the consolelogonhook dll
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver`
   
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen`
   
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX`
   
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX`

4. Take ownership of and rename Windows.UI.Logon.dll to anything else so it will invoke console login, it'll just work and do its thing

this is currently unfinished, as of right now you have
1. security options (ctrl alt delete)
2. status view (updates, please wait, logon, etc)

(if you're lazy to do the regedit stuff, copy and paste this into a .reg file)
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
