TARGET = main
OBJS = bin/main.o bin/callbacks.o bin/vram.o bin/model.o

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

psp: create_bin all

create_bin:
	mkdir -p bin/
	cp src/* bin/
