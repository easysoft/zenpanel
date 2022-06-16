@echo off
set "HKLMU=HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
set "HKLCID=HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Installer\Dependencies"
::::: 判断系统是否已经安装了 Visual C++ 2015 
reg query %HKLMU%\{e9d78d68-c26c-4da7-9158-99355d8ef3ad}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 Redistributable (x64) - 14.10.25017
reg query %HKLMU%\{8D4F7A6D-6B81-3DC8-9C21-6008E4866727}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x64 Minimum Runtime - 14.10.25017
reg query %HKLMU%\{E512788E-C50B-3858-A4B9-73AD5F3F9E93}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x64 Additional Runtime - 14.10.25017
reg query %HKLMU%\{d992c12e-cab2-426f-bde3-fb8c53950b0d}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.24215
reg query %HKLMU%\{50A2BC33-C9CD-3BF1-A8FF-53C10A0B183C}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.24215
reg query %HKLMU%\{EF1EC6A9-17DE-3DA9-B040-686A1E8A8B04}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.24215
reg query %HKLMU%\{323dad84-0974-4d90-a1c1-e006c7fdbb7d}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.24212
reg query %HKLMU%\{FAAD7243-0141-3987-AA2F-E56B20F80E41}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.24212
reg query %HKLMU%\{F20396E5-D84E-3505-A7A8-7358F0155F6C}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.24212
reg query %HKLMU%\{2cbcedbb-f38c-48a3-a3e1-6c6fd821a7f4}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.24123
reg query %HKLMU%\{FDBE9DB4-7A91-3A28-B27E-705EF7CFAE57}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.24123
reg query %HKLMU%\{21134089-9B59-34C8-BE11-929D26AD5207}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.24123
reg query %HKLMU%\{e46eca4f-393b-40df-9f49-076faf788d83}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.23026
reg query %HKLMU%\{0D3E9E15-DE7A-300B-96F1-B4AF12B96488}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.23026
reg query %HKLMU%\{BC958BD2-5DAC-3862-BB1A-C1BE0790438D}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.23026

reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{e9d78d68-c26c-4da7-9158-99355d8ef3ad}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 Redistributable (x64) - 14.10.25017
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{8D4F7A6D-6B81-3DC8-9C21-6008E4866727}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x64 Minimum Runtime - 14.10.25017
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{E512788E-C50B-3858-A4B9-73AD5F3F9E93}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x64 Additional Runtime - 14.10.25017
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{d992c12e-cab2-426f-bde3-fb8c53950b0d}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.24215
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{50A2BC33-C9CD-3BF1-A8FF-53C10A0B183C}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.24215
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{EF1EC6A9-17DE-3DA9-B040-686A1E8A8B04}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.24215
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{323dad84-0974-4d90-a1c1-e006c7fdbb7d}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.24212
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{FAAD7243-0141-3987-AA2F-E56B20F80E41}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.24212
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{F20396E5-D84E-3505-A7A8-7358F0155F6C}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.24212
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{2cbcedbb-f38c-48a3-a3e1-6c6fd821a7f4}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.24123
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{FDBE9DB4-7A91-3A28-B27E-705EF7CFAE57}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.24123
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{21134089-9B59-34C8-BE11-929D26AD5207}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.24123
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{e46eca4f-393b-40df-9f49-076faf788d83}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x64) - 14.0.23026
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{0D3E9E15-DE7A-300B-96F1-B4AF12B96488}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Minimum Runtime - 14.0.23026
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{BC958BD2-5DAC-3862-BB1A-C1BE0790438D}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x64 Additional Runtime - 14.0.23026

reg query "%HKLCID%\,,amd64,14.0,bundle">nul 2>nul&&set VC=Microsoft Visual C++ 2017 Redistributable x64

REG QUERY %HKLMU%>NUL 2>NUL && (
  FOR /F %%1 IN ('REG QUERY %HKLMU%') DO (
    REG QUERY %%1 /v DisplayName | FINDSTR "Microsoft Visual C++ 201" > NUL 2>NUL && (
      set VC=Microsoft Visual C++
      GOTO END
    )
  )
)
 
:END

if defined VC (echo INSTALLED: %VC%)
