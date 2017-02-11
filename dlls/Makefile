#
# HPB_bot makefile for Linux
#
# created: 12/16/2000 botman (botman@mailandnews.com)
#

# the following specifies the path to your MOD...
MOD_DIR = /usr/hlds_l/tfc

CPP = gcc

BASEFLAGS = -Dstricmp=strcasecmp -Dstrcmpi=strcasecmp

CPPFLAGS = ${BASEFLAGS} -m32 -O2 -w -lm -I../engine -I../common -I../pm_shared  -I../metamod

OBJ = bot.o bot_client.o bot_combat.o bot_navigate.o bot_start.o \
	dll.o engine.o h_export.o linkfunc.o meta_api.o util.o waypoint.o \
	bot_game.o bot_halflife.o bot_opposingforce.o bot_cstrike.o bot_dod.o \
	bot_tfc.o bot_gunman.o bot_ns.o bot_ship.o bot_hunger.o

sandbot.so: ${OBJ}
	${CPP} -m32 -fPIC -shared -o $@ ${OBJ} -ldl
	# cp -f HPB_bot_i486.so ${MOD_DIR}/dlls

clean:
	-rm -f *.o
	-rm -f *.so	

%.o:	%.cpp
	${CPP} ${CPPFLAGS} -c $< -o $@

%.o:	%.c
	${CPP} ${CPPFLAGS} -c $< -o $@

