TARGET = main
OBJS = bin/main.o bin/callbacks.o bin/vram.o bin/model.o bin/upng.o bin/ecmanager.o bin/components/motion.o bin/components/render.o bin/components/transform.o bin/systems/drawing.o

INCDIR = ./include
CFLAGS = -G0 -Wall -Ofast
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgum -lpspgu -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Lights Sample
PSP_FW_VERSION=600

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

bin/%.o: src/%.c
	@mkdir -p bin/
	@mkdir -p bin/components
	@mkdir -p bin/systems
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
