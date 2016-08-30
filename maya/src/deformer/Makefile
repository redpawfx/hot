#
# Include platform specific build settings
#
include ./buildconfig_luma


.SUFFIXES: .cpp .cc .o .so .c .bundle


OUTPUT = ./

PLUGIN = hotOceanDeformer.$(EXT)

COMPILE  = $(CC) -c $(CFLAGS) $(INCLUDES)

.cpp.o:
	$(COMPILE) $< -o $@

plugins: $(PLUGIN)

depend:
	makedepend $(INCLUDES) *.cc *.cpp

clean:
	-rm -f ./*~ ./*.o $(OUTPUT)*.so


##################
# Specific Rules #
##################

SOURCES =	./hotOcean.cpp\
		

OBJS = $(SOURCES:.cpp=.o)

all:  $(SOURCES) $(PLUGIN)

$(PLUGIN): $(OBJS)
	-rm -f $(OUTPUT)$@
	$(LD) -o $(OUTPUT)$@ $(OBJS) $(LIBS) $(LIBS_GL_EXTRA)


