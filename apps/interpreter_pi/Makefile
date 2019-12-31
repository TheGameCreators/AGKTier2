# armv7 doesn't seem to increase performance, so use armv6 for everything
#ARMV7 := $(shell cat /proc/cpuinfo | grep ARMv7)
ARMV7 =

ifeq ($(ARMV7),)
  CFLAGS = -O2 -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -fsigned-char
  SUFFIX =
else
  CFLAGS = -O2 -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -fsigned-char
  SUFFIX = V7
endif

CC = g++-4.8
ODIR = build/obj$(SUFFIX)
INC = -I../../common/include -I../../common -I../interpreter -I../../bullet -I../../bullet/BulletCollision/CollisionShapes -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
LDFLAGS = -L../../platform/pi/Lib/Release$(SUFFIX) -L/opt/vc/lib/

_OBJS = Core.o \
        interpreter.o \
        ProgramData.o 
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: setup $(OBJS) AGKPlayer

setup:
	mkdir -p build/obj$(SUFFIX)

$(ODIR)/%.o: %.cpp
	$(CC) -DIDE_PI -c $(INC) -o $@ $< $(CFLAGS)
	
$(ODIR)/%.o: ../interpreter/%.cpp
	$(CC) -DIDE_PI -c $(INC) -o $@ $< $(CFLAGS)
	
AGKPlayer: | $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o build/PiPlayer$(SUFFIX) $(LDFLAGS) -lAGKPi -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lXrandr -lpthread -lopenal -ludev -lrt -lXxf86vm -lXext -lX11

clean:
	rm -rf build/obj$(SUFFIX)/*
	rm -f build/PiPlayer$(SUFFIX)
