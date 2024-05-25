# welcome to the uhh consolelogonhook repo
to build, u clone the shit, build with vs 2022 and enjoy,

to use the compiled dll, put it into system32, then go to these keys in registry (as trusted installer) and edit
DllPath in them to the consolelogonhook dll

the list is as follows
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX

then itll just work and do its thing

unfinished atm ofc, atm u got the security options (ctrl alt delete) and the status view (updates, please wait, logon etc)