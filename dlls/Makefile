#
# Sandbot Linux Makefile
#

CPP = g++

# gcc flags - thanks RoboCop
# TODO: linux should be defined along with __linux__ in gcc but it isn't?
OPTFLAGS = \
		-m32 -Wall -Wextra \
		-O2 -std=c++14 -march=core2 -mfpmath=sse \
		-finline-functions -falign-loops=2 -falign-jumps=2 -falign-functions=2 \
		-Dlinux

CPPFLAGS = ${OPTFLAGS} -lstdc++ -lm -I../dlls -I../engine -I../common -I../pm_shared -I../metamod

OBJ = \
	bot.o \
	bot_client.o \
	bot_combat.o \
	bot_navigate.o \
	bot_halflife.o \
	bot_opposingforce.o \
	bot_cstrike.o \
	bot_dod.o \
	bot_tfc.o \
	bot_gunman.o \
	bot_ns.o \
	bot_ship.o \
	bot_hunger.o \
	dll.o \
	engine.o \
	game.o \
	h_export.o \
	linkfunc.o \
	meta_api.o \
	util.o \
	waypoint.o

# by default build sandbot.so in release mode
.DEFAULT_GOAL := sandbot.so

# if debug is requested add debug flags and build sandbot.so
# could have debug: OPTFLAGS += -DDEBUG as well but that would
# mean the symbols are from a build that is different to release
debug: OPTFLAGS += -g
debug: sandbot.so

symbols: debug
symbols: 
	-objcopy --only-keep-debug sandbot.so sandbot.so.debug
	-cp sandbot.so.debug ../symbols/

# build sandbot.so
sandbot.so: ${OBJ}
	${CPP} -m32 -fPIC -shared -o $@ ${OBJ}
	-cp $@ ../release/symbols/
	-cp $@ ../release/sandbot/half-life/valve/dlls/
	-cp $@ ../release/sandbot/half-life/gearbox/dlls/
	-cp $@ ../release/sandbot/half-life/dod/dlls/
	# -cp $@ ../release/sandbot/half-life/ns/dlls/
	# cp -f $@ /home/$$USER/.local/share/steam/steamapps/half-life/valve/dlls

# clean up build files
clean:
	-rm -f *.o
	-rm -f *.so
	-rm -f *.debug

%.o: %.cpp
	${CPP} ${CPPFLAGS} -c $< -o $@

%.o: %.c
	${CPP} ${CPPFLAGS} -c $< -o $@
