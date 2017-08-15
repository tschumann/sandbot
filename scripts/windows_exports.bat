
:: make sure the file is empty
break > windows_exports.txt

:: hooray for hard-coded paths...
SET dumpbin="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\bin\HostX64\x64\dumpbin"
SET python="C:\Users\schum\AppData\Local\Programs\Python\Python36\python"

:: dump the exports from each game .dll file
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\valve\dlls\hl.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\bshift\dlls\hl.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\gearbox\dlls\opfor.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\cstrike\dlls\mp.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\czeror\dlls\czeror.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\dod\dlls\dod.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\tfc\dlls\tfc.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\rewolf\dlls\gunman.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\ns\dlls\ns.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\hunger\dlls\einar.dll" >> windows_exports.txt
%dumpbin% /EXPORTS "C:\Program Files (x86)\Steam\steamapps\common\Half-Life\ship\dlls\ship.dll" >> windows_exports.txt

:: process the exports so they can be copy+pasted into exports.cpp
%python% format_windows_exports.py > sandbot_windows_exports.txt
