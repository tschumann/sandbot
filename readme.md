Sandbot
=======

Sandbot is a GoldSource engine bot based off HPB_Bot by botman (see `docs/ReadMe.txt`), specifically HPB bot template version 4 (`hpb_bot_template_4.zip`) with navigation code from HPB bot version 4 (`hpb_bot_plugin.zip`).


Building
--------
```
git clone https://github.com/tschumann/sandbot
git submodule init
git submodule update
```

### Windows

Open dlls/sandbot-2019.sln

### Linux

```
sudo apt-get install gcc gcc-multilib g++-multilib
cd dlls
make
```


Running tests
-------------
```
powershell ./test.ps1
```


Debugging
---------
gdb -s sandbot.so.debug -e sandbot.so to load Sandbot into gdb with debug symbols (if you "info functions" you'll get a per-file breakdown whereas if you do the same with gdb -e sandbot.so you won't).

### Core dumps on Ubuntu

Enable core dumps:
```
ulimit -c unlimited
```

See what generates core dumps:
```
sysctl kernel.core_pattern
```

Start Steam in debug mode:
```
STEAM_DEBUGGER=gdb ~/.steam/steam/steam.sh
```

Note that Steam games don't seem to generate core dumps, but rather generates breakpad minidumps.

Steam generates minidumps at `/tmp/dumps/`

### Debugging minidumps
Clone and build Google Breakpad:
```
git clone https://github.com/google/breakpad
cd breakpad
git clone https://chromium.googlesource.com/linux-syscall-support src/third_party/lss
./configure CXXFLAGS=-m32 CFLAGS=-m32 CPPFLAGS=-m32 && make
```

If you want to recompile for 64-bit you'll need to make distclean first.

If you can an error about N_UNDF being undefined see https://stackoverflow.com/a/36455846
(in `src/common/stabs_reader.h` change `#include <a.out.h>` to `#include <linux/a.out.h>` and run `env ac_cv_header_a_out_h=yes CXXFLAGS=-m32 CFLAGS=-m32 CPPFLAGS=-m32 ./configure`)

Convert the minidump to a core dump:
```
~/Documents/breakpad/src/tools/linux/md2core/minidump-2-core /tmp/dumps/assert_*_1.dmp > out.core
Load it all up in gdb:
gdb -s ~/Documents/sandbot/symbols/sandbot.so.debug -e ~/.steam/steam/steamapps/common/Half-Life/hl_linux -c out.core
```

TODO: figure out loading sandbot symbols into gdb


MetaMod
-------
Change gamedll to addons/metamod/dlls/metamod.dll to test MetaMod. sandbot.dll should go in the same directory (but change the path in plugins.ini so it points to the top level dlls/ directory).


Releasing
---------
```
git tag -a v0.x -m "Sandbot v0.x"
```

On Windows copy dlls/Release/vc141.pdb and dlls/Release/sandbot.pdb to release/symbols/
On Linux run `make`, git add the .so files, `make symbols` then git add the .so.debug file.


License
-------

GPL v2 because MetaMod is GPL v2.