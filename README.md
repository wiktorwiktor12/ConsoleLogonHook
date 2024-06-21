# ConsoleLogonHook
Welcome!

This project hooks onto and provides a graphical user interface for the console logon screen in Windows 10.

> [!WARNING]
> **THIS PROJECT IS IN EARLY DEVELOPMENT AND MIGHT BE UNSTABLE**
>
> You may encounter issues using a microsoft account, this will work perfectly on local accounts
>
> Disabling the lockscreen and the <kbd>CTRL</kbd> <kbd>Alt</kbd> <kbd>Del</kbd> logon keybinds is recommended as they're not fully implemented yet.
>
> **Knowing this, You might use this at your own risk.**
>

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork this repository.
2. Pull using git commandline or any git ui manager such as GitHub Desktop.
3. Enjoy.
 
## Installation
> [!NOTE]
> **This will require administrator privileges to be installed.**
>

1. Copy the 2 DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from [Releases](https://github.com/wiktorwiktor12/ConsoleLogonHook/releases) into %SYSTEMROOT%\System32

> [!NOTE]
Step 2 might not be necessary in a future release.
> 
2. Take `authui.dll` from Windows 7 SP1, rename it to `au7hui.dll` and place it into %SYSTEMROOT%\System32, Or alternatively, just copy au7hui.dll from releases. This is required, as ConsoleLogonHook will push resources from it to use in the logon screen.

3. Take ownership of `ConsoleLogon.dll` and replace it with the version from [Releases](https://github.com/wiktorwiktor12/ConsoleLogonHook/releases), this a temporary solution until a pdb offset finder system is added in.

4. Open a CMD window as TrustedInstaller via PsExec64 and copy and paste the following commands:

```cmd
reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f
```

5. Take ownership of the file `Windows.UI.Logon.dll` and rename it to something else. Example: `Windows.UI.Logon.dll.bak`, this is required as it will force the use of the console logon screen.
