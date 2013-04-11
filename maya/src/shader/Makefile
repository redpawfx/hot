#Makefile for MRAY Shaders

CC = /usr/local/gcc-4.1.2/bin/g++412
LINK = /usr/local/gcc-4.1.2/bin/g++412


##########################
## CUSTOMIZATION  here
DEVKIT = /usr/autodesk/maya2012-x64/devkit/mentalray/include
BUILDDIR = ./

########################

BUILDNAME = hotOcean.so

INCLUDE = 	-I$(DEVKIT)			\
			-I../include 		\
			-I/usr/X11R6/include\
			-I./include         \
			-I/redpawFX/dev/gitHub/redpawFX/hot/3rdparty/linux/include\
			-I/usr/include/OpenEXR\
			-I/usr/local/gcc-4.1.2/lib/gcc/x86_64-unknown-linux-gnu/4.1.2/include \
			-I.


CFLAGS += \
	-DBits64_  -DUNIX -D_BOOL  -DFUNCPROTO -D_GNU_SOURCE -DLINUX_64  -DBUILD_SD \
	-fno-strict-aliasing -DREQUIRE_IOSTREAM -Wno-deprecated  \
	-Wno-multichar -Wno-comment -Wno-sign-compare -funsigned-char \
	-Wno-reorder -fno-gnu-keywords -ftemplate-depth-25 -pthread -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES\
	-ansi -fPIC -g -c -O2 -m64 -fexpensive-optimizations \
	-finline-functions -funroll-loops -fomit-frame-pointer \
	-frerun-cse-after-loop -fstrength-reduce  -fforce-addr   \
	-DLINUX -DLINUX_X86 -DX86 -DBIT64 -DEVIL_ENDIAN -D_GNU_SOURCE -D_THREAD_SAVE -D_REENTRANT \
	-DSYSV -DSVR4 -Dinline=__inline__ -DSSE_INTRINSICS $(INCLUDE)

LIBS = -lImath /redpawFX/dev/gitHub/redpawFX/hot/3rdparty/linux/lib/libfftw3f.a /redpawFX/dev/gitHub/redpawFX/hot/3rdparty/linux/lib/libblitz.a

LIBDIR = 	-L/usr/local/lib
						

LDFLAGS = -shared -export-dynamic -static-libgcc -Wl --whole-archive -Bsymbolic $(LIBDIR)

all:
	$(CC) $(CFLAGS) ./hotOcean.cpp

	$(LINK) $(LDFLAGS) $(LIBS) -o $(BUILDDIR)/$(BUILDNAME) *.o

clean:
	-rm -f ./*.o ./*.so $(BUILDDIR)/$(BUILDNAME)

