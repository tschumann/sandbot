# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

# create dlls/run_tests.hxx so that test.h enables test code
New-Item -Path "dlls" -Name "run_tests.hxx" -ItemType "file" -Force
# build the solution
MSBuild.exe /t:Build /p:Platform=x86 /p:Configuration=Release "sandbot.sln"
# remove dlls/run_tests.hxx so that subsequent work doesn't get messed up by its presence
Remove-Item dlls\run_tests.hxx

# run the tests
vstest.console.exe Release\sandbot.dll --logger:"console;verbosity=Normal"

# create dlls/run_tests.hxx so that test.h enables test code
New-Item -Path "dlls" -Name "run_tests.hxx" -ItemType "file" -Force
# build the solution
MSBuild.exe /t:Build /p:Platform=x64 /p:Configuration=Release "sandbot.sln"
# remove dlls/run_tests.hxx so that subsequent work doesn't get messed up by its presence
Remove-Item dlls\run_tests.hxx

# run the tests
vstest.console.exe x64\Release\sandbot.dll --logger:"console;verbosity=Normal"

Pop-Location
