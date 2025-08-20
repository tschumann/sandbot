# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

# build Sandbot
MSBuild.exe /t:Build /p:Platform=x86 /p:Configuration=Debug "sandbot.sln"

Copy-Item -Path dlls\Release\vc143.pdb -Destination dist\symbols\
Copy-Item -Path Release\sandbot.pdb -Destination dist\symbols\

# build metamod-p - Release crashes for some reason
MSBuild.exe /t:Build /p:Configuration=Debug "metamod-p\metamod\metamod.sln"

Copy-Item -Path metamod-p\metamod\Debug\metamod.dll -Destination dist\sandbot\Half-Life\dod\addons\metamod\dlls\
Copy-Item -Path metamod-p\metamod\Debug\metamod.dll -Destination dist\sandbot\Half-Life\gearbox\addons\metamod\dlls\
Copy-Item -Path metamod-p\metamod\Debug\metamod.dll -Destination dist\sandbot\Half-Life\Hunger\addons\metamod\dlls\
Copy-Item -Path metamod-p\metamod\Debug\metamod.dll -Destination dist\sandbot\Half-Life\ns\addons\metamod\dlls\
Copy-Item -Path metamod-p\metamod\Debug\metamod.dll -Destination dist\sandbot\Half-Life\rewolf\addons\metamod\dlls\
Copy-Item -Path metamod-p\metamod\Debug\metamod.dll -Destination dist\sandbot\Half-Life\valve\addons\metamod\dlls\

Remove-Item release.zip -Force

$compress = @{
	Path = "dist\readme.txt", "dist\sandbot\Half-Life"
	CompressionLevel = "Fastest"
	DestinationPath = "release.zip"
}

Compress-Archive @compress
