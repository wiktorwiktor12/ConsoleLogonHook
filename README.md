# Welcome to the ConsoleLogonHook respository

This project hooks into and provide a graphical user interface to the console logon screen for Windows 10

> [!WARNING]
> **THIS PROJECT IS IN EARLY DEVELOPMENT AND MIGHT BE UNSTABLE**
>
> You may encounter issues using a microsoft account, this will work perfectly on local accounts
> 
> **Knowing this, You might use this at your own risk.**
>

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork this repository
2. Pull using git commandline or any git ui manager such as github desktop
3. Enjoy
 
## Installation
> [!WARNING]
> **This LogonUI modification will require administrator privileges to be installed.**
>

1. Copy the 2 DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from [Releases](https://github.com/wiktorwiktor12/ConsoleLogonHook/releases) into %SYSTEMROOT%\System32

> [!NOTE]
Step 2 might not be necessary in a future release.
> 
2. Take `authui.dll` from Windows 7 SP1, rename it to `au7hui.dll` and place it into %SYSTEMROOT%\System32 , this is so the ConsoleLogonHook can pull the resources from it to have the Windows 7 logonUI, Or alternatively, just copy au7hui.dll from releases  

3. Take ownership of ConsoleLogon.dll and replace it with version, 10.0.19041.3636 (should be in releases), this only temporary until a pdb offset finder system is written as they are hardcoded atm

4. Open a CMD window as TrustedInstaller via psexec64 and run the following commands:

```
reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f
```

5. Take ownership of the file `Windows.UI.Logon.dll` and rename it to something elsem as an example `Windows.UI.Logon.bak.dll`, so it will fallback to the console login instead.
