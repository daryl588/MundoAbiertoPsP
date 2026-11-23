TARGET = EBOOT
OBJS = main.o
CFLAGS = -O2 -G0 -Wall
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Mundo Abierto PSP - Demo 1.1
include $(PSPSDK)/lib/build.mak
