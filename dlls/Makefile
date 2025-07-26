#
# Sandbot Linux Makefile
#

CPP = g++

# gcc flags - thanks RoboCop
OPTFLAGS = \
		-m32 -Wall -Wextra \
		-O2 -std=c++14 -march=core2 -mfpmath=sse \
		-finline-functions -falign-loops=2 -falign-jumps=2 -falign-functions=2

CPPFLAGS = ${OPTFLAGS} -lstdc++ -lm -I../dlls -I../engine -I../common -I../pm_shared -I../metamod-p/metamod

OBJ = \
	bot.o \
	bot_client.o \
	bot_combat.o \
	bot_navigate.o \
	bot_halflife.o \
	bot_opposingforce.o \
	bot_dod.o \
	bot_tfc.o \
	bot_dmc.o \
	bot_gunman.o \
	bot_svencoop.o \
	bot_ns.o \
	bot_ship.o \
	bot_hunger.o \
	dll.o \
	engine.o \
	game.o \
	game_halflife.o \
	game_opposingforce.o \
	game_dayofdefeat.o \
	game_teamfortressclassic.o \
	game_deathmatchclassic.o \
	game_ricochet.o \
	game_gunmanchronicles.o \
	game_svencoop.o \
	game_naturalselection.o \
	game_theship.o \
	game_theyhunger.o \
	h_export.o \
	linkfunc.o \
	meta_api.o \
	util.o \
	waypoint.o \
	../common/interface.o

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
	-cp $@ ../output/symbols/
	-cp $@ ../output/sandbot/half-life/valve/dlls/
	-cp $@ ../output/sandbot/half-life/gearbox/dlls/
	# NOTE: uncomment to test locally
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
