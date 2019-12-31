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

# list of dependencies so that changes to these files cause a rebuild of source files
DEPS = template.h

# list of object files to compile, they will be placed in the $(ODIR) folder
# matching .cpp files will be searched for below
_OBJS = Core.o \
        template.o  
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# entry point, create folders, compile source files, and link the executable
all: setup $(OBJS) Executable

# make sure the build folders exist
setup:
	mkdir -p build/obj$(SUFFIX)

# compile a source file, first search path is the template folder
$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -DIDE_PI -c $(INC) -o $@ $< $(CFLAGS)

# second search path is the interpreter folder, used as an example, not used
$(ODIR)/%.o: ../interpreter/%.cpp $(DEPS)
	$(CC) -DIDE_PI -c $(INC) -o $@ $< $(CFLAGS)

# link the .o files
Executable: | $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o build/PiPlayer$(SUFFIX) $(LDFLAGS) -lAGKPi -lavcodec -lavformat -lavutil -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lXrandr -lpthread -lopenal -ludev -lrt -lXxf86vm -lXext -lX11


# delete compiled files
clean:
	rm -rf build/obj$(SUFFIX)/*
	rm -f build/PiApp$(SUFFIX)
