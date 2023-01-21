# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

# create dlls/run_tests so that test.h enables test code
New-Item -Path "dlls" -Name "run_tests.hxx" -ItemType "file" -Force
# build the solution
& "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" /t:Build /p:Configuration=Release "dlls\sandbot-2019.sln"
# remove dlls/run_tests.hxx so that subsequent work doesn't get messed up by its presence
Remove-Item dlls\run_tests.hxx

# run the tests
& "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" dlls\Release\sandbot.dll --logger:"console;verbosity=Normal"

Pop-Location