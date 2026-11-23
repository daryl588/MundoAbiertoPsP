TARGET = EBOOT
OBJS = main.o

CFLAGS = -O2 -G0 -Wall

PSPSDK = $(shell psp-config --pspsdk-path)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Mundo Abierto PSP - Demo 1.1

include $(PSPSDK)/lib/build.mak
