TARGET = main
SRCS = $(shell find src/ -type f -name '*.c')
OBJS = $(patsubst src/%.c, bin/%.o, $(SRCS))

INCDIR = ./include
CFLAGS = -G0 -Wall -Ofast
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgum -lpspgu -lm

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = psp3d
PSP_FW_VERSION=600

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

bin/%.o: src/%.c
	@mkdir -p bin/
	@mkdir -p bin/components
	@mkdir -p bin/systems
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
