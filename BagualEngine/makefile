#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = clang++

SEQ_CC = clang++
PAR_CC = nvcc

EMPTY =

# define any compile-time flags
CFLAGS = -std=c++11 -O3
DEBUGCFLAGS = -std=c++11 -g

# define any directories containing header files other than /usr/include
#
#INCLUDES = -I $(HOME)/cxsc/include
INCLUDES = -I ./third/linux/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L $(HOME)/cxsc/lib
LFLAGS = -L ./third/linux/lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lSDL2 -lm

# define the C source files
CSRCS = main.cpp BagualEngine.cpp BagualCamera.cpp BagualCameraManager.cpp BagualCameraSettings.cpp BagualDraw.cpp BagualMath.cpp BagualRenderer.cpp BagualSettings.cpp BagualTypes.cpp
#CUDASRCS = int-haar-cuda.cu cuda-tests.cu

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro CSRCS
# with the .o suffix
#
COBJS = $(CSRCS:.cpp=.o)
#CUDAOBJS = $(CUDASRCS:.cu=.o)

# define the executable file 
MAIN = BagualEngine.exe

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

#
# Selects the compiler to be used...
#

ifdef PAR
CC = $(PAR_CC)
else
CC = $(SEQ_CC)
endif

ifdef DEBUG
FLAGS = $(DEBUGCFLAGS)
else
FLAGS = $(CFLAGS)
endif

.PHONY: depend clean

all: compile message

compile: $(MAIN)

message:
	@echo  
	@echo	Bagual Engine has been compiled
	@echo  

$(MAIN): $(COBJS)
	$(CC) $(FLAGS) $(INCLUDES) -o $(MAIN) $(COBJS) $(LFLAGS) $(LIBS)

################################

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

################################

CUDAOBJS: $(CUDAOBJS)

cuda-tests.o: cuda-tests.cu
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

int-haar-cuda.o: int-haar-cuda.cu
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

################################

clean:
	$(RM) *.o *~ $(MAIN)

rebuild: clean all

depend: $(CSRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
