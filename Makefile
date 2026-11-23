TARGET = EBOOT
OBJS = main.o

CFLAGS = -O2 -G0 -Wall
LIBS = -lpspgu -lpspge

PSPSDK = $(shell psp-config --pspsdk-path)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Mundo Abierto PSP - Demo 1.2

include $(PSPSDK)/lib/build.mak