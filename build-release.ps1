# get the location of this file
$scriptpath = $MyInvocation.MyCommand.Path
# get the directory path to this file
$wd = Split-Path $scriptpath
# set the working directory as this file's directory
Push-Location $wd

# build Sandbot
MSBuild.exe /t:Build /p:Platform=x86 /p:Configuration=Release "sandbot.sln"

# TODO: copy sandbot.dll across to dist\sandbot\

Copy-Item -Path dlls\Release\vc143.pdb -Destination dist\symbols\
Copy-Item -Path Release\sandbot.pdb -Destination dist\symbols\

# NOTE: ship metamod-p 1.21.0.37 for now
# MSBuild.exe /t:Build /p:Configuration=Release "metamod-p\metamod\metamod.sln"

# Copy-Item -Path metamod-p\metamod\Release\metamod.dll -Destination dist\sandbot\Half-Life\dod\addons\metamod\dlls\
# Copy-Item -Path metamod-p\metamod\Release\metamod.dll -Destination dist\sandbot\Half-Life\gearbox\addons\metamod\dlls\
# Copy-Item -Path metamod-p\metamod\Release\metamod.dll -Destination dist\sandbot\Half-Life\Hunger\addons\metamod\dlls\
# Copy-Item -Path metamod-p\metamod\Release\metamod.dll -Destination dist\sandbot\Half-Life\ns\addons\metamod\dlls\
# Copy-Item -Path metamod-p\metamod\Release\metamod.dll -Destination dist\sandbot\Half-Life\rewolf\addons\metamod\dlls\
# Copy-Item -Path metamod-p\metamod\Release\metamod.dll -Destination dist\sandbot\Half-Life\valve\addons\metamod\dlls\

Remove-Item release.zip -Force

$compress = @{
	Path = "dist\readme.txt", "dist\sandbot\Half-Life"
	CompressionLevel = "Fastest"
	DestinationPath = "release.zip"
}

Compress-Archive @compress
