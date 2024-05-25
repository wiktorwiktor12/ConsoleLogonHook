# welcome to the uhh consolelogonhook repo
to build, u clone the shit, build with vs 2022 and enjoy,

to use the compiled dll, put it into system32, then go to these keys in registry (as trusted installer) and edit
DllPath in them to the consolelogonhook dll

the list is as follows
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX

then rename windows.ui.logon.dll so it uses the console logon and then itll just work and do its thing

unfinished atm ofc, atm u got the security options (ctrl alt delete) and the status view (updates, please wait, logon etc)
![pork](https://github.com/wiktorwiktor12/ConsoleLogonHook/assets/85626296/2c6474d0-7dd3-4eef-a6da-f58623a85227)
Aye pork a freak bruh
𝓕𝓻𝓮𝓪𝓴𝔂Cheddar

