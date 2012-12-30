CXX = clang++
PROG = galaga
OBJS = main.o asset_manager.o ship.o bullet.o galaga.o gamemodule.o enemy.o \
	utilities.o particle.o particle_manager.o background.o background_manager.o \
	powerup.o
LIBS = allegro-5.0 allegro_main-5.0 allegro_dialog-5.0 allegro_ttf-5.0 \
	allegro_image-5.0 allegro_font-5.0 allegro_primitives-5.0 allegro_audio-5.0 \
	allegro_acodec-5.0
CFLAGS = -ggdb -stdlib=libc++ -std=c++0x $(shell pkg-config --cflags ${LIBS})
LDFLAGS = $(shell pkg-config --libs ${LIBS})

all: ${PROG}

${PROG}: ${OBJS}
	${CXX} ${CFLAGS} -o $@ $+ ${LDFLAGS}

%.o: %.cpp
	${CXX} ${CFLAGS} -c $< -o $@

clean:
	rm -f ${OBJS} ${PROG}
