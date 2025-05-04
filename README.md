# ConsoleLogonHook
Welcome!

This project hooks onto and provides a graphical user interface for the console logon screen in Windows 10.

> [!WARNING]
> This project is now considered obsolete by me due to the existance of AuthUX
> Would recommend for you to use AuthUX instead please
> https://github.com/explorer7-team/AuthUX

> [!WARNING]
> CREDITS GO TO @mishaproductions FOR THE DUI LIBRARY
> 
> https://github.com/MishaProductions/dui70
> 
> check out the original repo for the dui library here
> 
> i was too lazy to properly embed into the clh repo


> [!WARNING]
> **THIS PROJECT IS IN EARLY DEVELOPMENT AND MIGHT BE UNSTABLE**
>
> You may encounter issues using a Microsoft account, however, this will work perfectly fine on local accounts.
>
> Disabling the lockscreen and the <kbd>CTRL</kbd> <kbd>Alt</kbd> <kbd>Del</kbd> logon keybinds is recommended as they're not fully implemented yet.
>
> **Knowing this, You might use this at your own risk.**
>

## How to contribute to the project
The following steps explain how you can contribute to the project
1. Fork this repository.
2. Pull using git commandline, or any Git UI manager (such as Github Desktop, etc.)
3. Enjoy.
 
## Installation
> [!WARNING]
> **This will require administrator privileges to be installed.**
>

1. Copy the 2 DLL files (ConsoleLogonHook.dll and ConsoleLogonUI.dll) from [Releases](https://github.com/wiktorwiktor12/ConsoleLogonHook/releases) into %SYSTEMROOT%\System32

~~2. Take `authui.dll` from Windows 7 SP1, rename it to `au7hui.dll` and place it into %SYSTEMROOT%\System32, Or alternatively, just copy au7hui.dll from releases. This is required, as ConsoleLogonHook will push resources from it to use in the logon screen.~~ No longer necessary as of v1.0.6

~~3. Take ownership of `ConsoleLogon.dll` and replace it with the version from [Releases](https://github.com/wiktorwiktor12/ConsoleLogonHook/releases), this a temporary solution until a pdb offset finder system is added in.~~ **No longer necessary as of v1.0.3**

2. Open a CMD window as TrustedInstaller via PsExec64 and copy and paste the following commands:

```cmd
reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX /v DllPath /t REG_SZ /d %systemroot%\System32\ConsoleLogonHook.dll /f
```
or merge the regkey in the release zip as trusted installer.


3. Take ownership of the file `Windows.UI.Logon.dll` and rename it to something else. Example: `Windows.UI.Logon.dll.bak`, this is required as it will force the use of the console logon screen.
