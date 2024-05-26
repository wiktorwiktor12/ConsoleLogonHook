# welcome to the consolelogonhook repo
how to build:
1. download source code
2. build with visual studio 2022
3. enjoy

how to use:
1. drag and drop the compiled DLL into system32
2. edit these keys by setting the DllPath value to be the path of the consolelogonhook dll
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleBlockedShutdownResolver`
   
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLockScreen`
   
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.UI.Logon.Controller.ConsoleLogonUX`
   
   `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\WindowsRuntime\ActivatableClassId\Windows.Internal.Shell.PlatformExtensions.ConsoleCredUX`

3. rename Windows.UI.Logon.dll to literally anything so it uses the console login, it'll just work and do its thing

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
