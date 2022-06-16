@echo off
set "HKLMU=HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
::::: 判断系统是否已经安装了 Visual C++ 2015 
:: { Visual C++ 2015 }
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{58b3beca-b999-4f6f-a48c-81681136a620}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 Redistributable (x86) - 14.10.25017
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{582EA838-9199-3518-A05C-DB09462F68EC}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x86 Minimum Runtime - 14.10.25017
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{68306422-7C57-373F-8860-D26CE4BA2A15}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x86 Additional Runtime - 14.10.25017
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{e2803110-78b3-4664-a479-3611a381656a}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24215
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{BBF2AC74-720C-3CB3-8291-5E34039232FA}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.24215
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{69BCE4AC-9572-3271-A2FB-9423BDA36A43}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24215
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{462f63a8-6347-4894-a1b3-dbfe3a4c981d}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24212
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{37B55901-995A-3650-80B1-BBFD047E2911}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.24212
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{844ECB74-9B63-3D5C-958C-30BD23F19EE4}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24212
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{206898cc-4b41-4d98-ac28-9f9ae57f91fe}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24123
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{06AE3BCC-7612-39D3-9F3B-B6601D877D02}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.24123
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{03AC7A79-F8AF-38FC-9DA0-98DAB4F4B1CD}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24123
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{74d0e5db-b326-4dae-a6b2-445b9de1836e}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.23026
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{A2563E55-3BEC-3828-8D67-E5E8B9E8B675}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.23026
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{BE960C1C-7BAD-3DE6-8B1A-2616FE532845}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.23026
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{8FD71E98-EE44-3844-9DAD-9CB0BBBC603C}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Redistributable - 14.0.24210
reg query HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{65E5BD06-6392-3027-8C26-853107D3CF1A}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24210

reg query %HKLMU%\{58b3beca-b999-4f6f-a48c-81681136a620}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 Redistributable (x86) - 14.10.25017
reg query %HKLMU%\{582EA838-9199-3518-A05C-DB09462F68EC}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x86 Minimum Runtime - 14.10.25017
reg query %HKLMU%\{68306422-7C57-373F-8860-D26CE4BA2A15}>nul 2>nul&&set VC=Microsoft Visual C++ 2017 x86 Additional Runtime - 14.10.25017
reg query %HKLMU%\{e2803110-78b3-4664-a479-3611a381656a}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24215
reg query %HKLMU%\{BBF2AC74-720C-3CB3-8291-5E34039232FA}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.24215
reg query %HKLMU%\{69BCE4AC-9572-3271-A2FB-9423BDA36A43}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24215
reg query %HKLMU%\{462f63a8-6347-4894-a1b3-dbfe3a4c981d}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24212
reg query %HKLMU%\{37B55901-995A-3650-80B1-BBFD047E2911}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.24212
reg query %HKLMU%\{844ECB74-9B63-3D5C-958C-30BD23F19EE4}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24212
reg query %HKLMU%\{206898cc-4b41-4d98-ac28-9f9ae57f91fe}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24123
reg query %HKLMU%\{06AE3BCC-7612-39D3-9F3B-B6601D877D02}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.24123
reg query %HKLMU%\{03AC7A79-F8AF-38FC-9DA0-98DAB4F4B1CD}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24123
reg query %HKLMU%\{74d0e5db-b326-4dae-a6b2-445b9de1836e}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.23026
reg query %HKLMU%\{A2563E55-3BEC-3828-8D67-E5E8B9E8B675}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Minimum Runtime - 14.0.23026
reg query %HKLMU%\{BE960C1C-7BAD-3DE6-8B1A-2616FE532845}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.23026
reg query %HKLMU%\{8FD71E98-EE44-3844-9DAD-9CB0BBBC603C}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24210
reg query %HKLMU%\{65E5BD06-6392-3027-8C26-853107D3CF1A}>nul 2>nul&&set VC=Microsoft Visual C++ 2015 x86 Additional Runtime - 14.0.24210


:: { Old Visual C++ }
reg query %HKLMU%\{b64ca997-b626-4abb-a046-5ca2d92ed659}>nul 2>nul&&set VC=Microsoft Visual C++ 2012 Redistributable X86
reg query %HKLMU%\{f65db027-aff3-4070-886a-0d87064aabb1}>nul 2>nul&&set VC=Microsoft Visual C++ 2013 Redistributable X86
reg query %HKLMU%\{FF66E9F6-83E7-3A3E-AF14-8DE9A809A6A4}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 Redistributable X86
reg query %HKLMU%\{9A25302D-30C0-39D9-BD6F-21E6EC160475}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 SP1 Redistributable X86
reg query %HKLMU%\{1F1C2DFC-2D24-3E06-BCB8-725134ADF989}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 SP1 Redistributable ATL Security Update X86
reg query %HKLMU%\{350AA351-21FA-3270-8B7A-835434E766AD}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 Redistributable X86
reg query %HKLMU%\{8220EEFE-38CD-377E-8595-13398D740ACE}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 SP1 Redistributable X86
reg query %HKLMU%\{4B6C7001-C7D6-3710-913E-5BC23FCE91E6}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 SP1 Redistributable ATL Security Update X86
reg query %HKLMU%\{2B547B43-DB50-3139-9EBE-37D419E0F5FA}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 Redistributable IA64
reg query %HKLMU%\{5827ECE1-AEB0-328E-B813-6FC68622C1F9}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 SP1 Redistributable IA64
reg query %HKLMU%\{977AD349-C2A8-39DD-9273-285C08987C7B}>nul 2>nul&&set VC=Microsoft Visual C++ 2008 SP1 Redistributable ATL Security Update IA64
if defined VC echo INSTALLED: %VC%


