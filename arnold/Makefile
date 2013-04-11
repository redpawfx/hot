#Makefile for Arnold Shaders

CC = /usr/local/gcc-4.1.2/bin/g++

C++ = /usr/local/gcc-4.1.2/bin/g++

EXT	= so

SHADER = bs_Ocean.$(EXT)

INCLUDE = 	\
			-I$(ARNOLD_HOME)/include 		\
			-I/Volumes/sv-dev01/devRepo/johnc/3rdParty/github/redpawFX/hot/3rdparty/linux/include \
			-I/usr/include/OpenEXR \
			-I.

CFLAGS += -c -Wall -Wsign-compare -g -fvisibility=hidden -fPIC -D_LINUX $(INCLUDE)

.SUFFIXES: .cpp .cc .o .so .c .bundle

EXT	= so

OUTPUT = ./

RELEASE = /Volumes/sv-dev01/devRepo/johnc/arnold/shaders/published/4.0.12/Linux-x86_64

LD			= $(C++)

COMPILE  = $(CC) -c $(CFLAGS) $(INCLUDES)

.cpp.o:
	$(COMPILE) $< -o $@

LIBS = -lai -lblitz -lfftw3f -lfftw3f_threads -lImath

LIBDIR = 	-L$(ARNOLD_HOME)/bin -L/Volumes/sv-dev01/devRepo/johnc/3rdParty/github/redpawFX/hot/3rdparty/linux/lib	\

LDFLAGS = -shared -export-dynamic --whole-archive -Bsymbolic   $(LIBDIR)

SOURCES =  ./src/Loader.cpp ./src/bs_Ocean.cpp ./src/bs_oceanFoam.cpp

OBJS = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(SHADER)

$(SHADER): $(OBJS)
	-rm -f $(OUTPUT)$@
	$(LD) -o $(OUTPUT)$@ $(OBJS) $(LDFLAGS) $(LIBS)
	-mkdir -p $(RELEASE)
	 cp $(SHADER) $(RELEASE)
clean:
	-rm -f ./src/*.o *.so

install:
	-mkdir -p $(RELEASE)
	 cp $(SHADER) $(RELEASE)

