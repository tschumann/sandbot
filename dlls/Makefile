#
# HPB_bot makefile for Linux
#
# created: 12/16/2000 botman (botman@mailandnews.com)
#

CPP = gcc

ARCHFLAG = -march=i686

BASEFLAGS = -Dstricmp=strcasecmp -Dstrcmpi=strcasecmp

# from https://github.com/APGRoboCop/hpb_bot_plugin/blob/master/Makefile
OPTFLAGS = -mtune=pentium4 -mmmx -msse -msse2 -O2 -m32 -mfpmath=sse \
		-funroll-loops -ffunction-sections -fomit-frame-pointer \
		-fexpensive-optimizations -finline-functions -ffast-math \
		-falign-loops=2 -falign-jumps=2 -falign-functions=2 \

CPPFLAGS = ${BASEFLAGS} ${OPTFLAGS} ${ARCHFLAG} -w -lm -I../dlls -I../engine -I../common -I../pm_shared -I../metamod

OBJ = bot.o bot_client.o bot_combat.o bot_navigate.o bot_start.o \
	dll.o engine.o h_export.o linkfunc.o meta_api.o util.o waypoint.o \
	bot_game.o bot_halflife.o bot_opposingforce.o bot_cstrike.o bot_dod.o \
	bot_tfc.o bot_gunman.o bot_ns.o bot_ship.o bot_hunger.o

sandbot.so: ${OBJ}
	# build for release
	${CPP} -m32 -fPIC -shared -o $@ ${OBJ} -ldl
	# cp -f sandbot.so /home/$USER/.local/share/steam/steamapps/half-life/valve/dlls

sandbot.so.debug: ${OBJ}
	# build debug symbols
	${CPP} -m32 -fPIC -shared -g -o $@ ${OBJ} -ldl
	objcopy --only-keep-debug $@ $@

clean:
	-rm -f *.o
	-rm -f *.so	

%.o:	%.cpp
	${CPP} ${CPPFLAGS} -c $< -o $@

%.o:	%.c
	${CPP} ${CPPFLAGS} -c $< -o $@
