GCC_DEFAULT=            g++ -fPIC -W -Wall -pedantic
GCC_DEBUG=              $(GCC_DEFAULT) -O0 -ggdb3 -pg# -DDEBUG# -DCHECK 
GCC_RELEASE=            $(GCC_DEFAULT) -O3 -ggdb3
#GCC=                    $(GCC_DEBUG)
GCC=                    $(GCC_RELEASE)


V_AaGL=          AaGL-0.0.1
H_AaGL=          include
S_AaGL=          src
D_AaGL=          doc
I_AaGL=          -I$(H_AaGL)

P_GLEW=         $(HOME)/3rdPartyLibs/glew-1.7.0
I_GLEW=         -I$(P_GLEW)/include

P_AaToolkit=		$(HOME)/wrk/toolkit
I_AaToolkit=		-I$(P_AaToolkit)/include

# Règles #

HDR=            $(wildcard $(H_AaGL)/*.h $(H_AaGL)/*.hh)
SRC=            $(wildcard $(S_AaGL)/*.cc)
OBJ=            $(SRC:%.cc=%.o)
ALL=            make.depend lib/libAaGL.a lib/libAaGL.so

all:            $(ALL)

lib/libAaGL.a:   $(OBJ)
								ar rcs lib/libAaGL.a $(OBJ)

lib/libAaGL.so:  make.depend $(OBJ)
								$(GCC) -shared $(OBJ) -o lib/libAaGL.so

%.o:            %.cc
								$(GCC) -c $(I_AaGL) $(I_AaToolkit) $(I_GLEW) $< -o $@

# Ménage #

doxygen:                
								rm -f $(D_AaGL)/*
								doxygen doxygen.cfg

clean:
								rm -f src/*.o
								rm -f doc/*
								rm -f $(ALL)
								find . -name "*~" -exec rm {} \;

archive:        clean
								tar zcvf ~/tgz/$(V_AaGL).tgz *

# Dépendances #

dep:
								$(GCC) -MM $(I_AaGL) $(I_AaToolkit) $(I_GLEW) $(SRC) >make.depend

make.depend:    $(HDR) $(SRC)
								$(GCC) -MM $(I_AaGL) $(I_AaToolkit) $(I_GLEW) $(SRC) >make.depend

include make.depend

