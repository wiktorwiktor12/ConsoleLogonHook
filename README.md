# Welcome to the ConsoleLogonHook respository

This project hooks into and provide a graphical user interface to the console logon screen for Windows 10


> [!WARNING] 
**This project is currently in various stages of development so use at your own risk.**

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork the repository if you dont have write access
2. Download the source code as a zip by pressing the green "Code" button and hitting download as zip
3. Unzip the downloaded source and open the solution (ConsoleLogonHook.sln) with Visual Studio 2022 and build it in release configuration
4. enjoy
 
## How to use
1. Copy the 2 compiled DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from ConsoleLogonHook-dui-ui\x64\Release into %SYSTEMROOT%\System32
> [!NOTE]
> **It will most likely ask you to provide adminsistrator permission to copy to System32, just accept the warning**
> 
3. Import the following registry key as TrustedInstaller by copying it into a text file and save as a .reg file then running with psexec64


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

5. Take ownership of and rename Windows.UI.Logon.dll to anything else so it will invoke console login, it'll just work and do its thing
