# ConsoleLogonHook
Welcome!

This project hooks onto and provides a graphical user interface for the console logon screen in Windows 10.

> [!WARNING] 
> **This project is currently in development, use it at your own risk.**
> 
> You may encounter issues using a Microsoft account, however, this will work perfectly on local accounts.
> 
> I would reccommend disabling the lock screen and <kbd>CTRL</kbd> <kbd>Alt</kbd> <kbd>Del</kbd> logon keybinds as they're not fully implemented yet
>

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork the repository
2. Pull using git commandline, or any Git UI manager (such as Github Desktop, etc.)
3. Enjoy!
 
## Installation
> [!NOTE]
> **This will require administrator privileges to be installed.**
>

1. Copy the 2 DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from [Releases](https://github.com/wiktorwiktor12/ConsoleLogonHook/releases) into %SYSTEMROOT%\System32

> [!NOTE]
Step 2 might not be necessary in a future release.
> 
2. Take `authui.dll` from Windows 7 SP1, rename it to `au7hui.dll` and place it into %SYSTEMROOT%\System32, Or alternatively, just copy au7hui.dll from releases. This is required, as ConsoleLogonHook will push resources from it to use in the logon screen.

3. Take ownership of ConsoleLogon.dll and replace it with version 10.0.19041.3636 (should be in releases). This is only temporary, until a PDB offset finder system is written (as they are hardcoded at the moment).

4. Open a CMD window as TrustedInstaller via PsExec64 and copy and paste the following commands:

```cmd
reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f
```

5. Take ownership of the file `Windows.UI.Logon.dll` and rename it to something else. Example: `Windows.UI.Logon.dll.bak`, this is required as it will force the use of the console logon screen.
