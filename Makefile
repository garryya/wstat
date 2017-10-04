EXE	:= wstat

GCCVER	    = /grid/common/pkgs/gcc/v4.8.3
#GCCVER	    = /grid/common/pkgs/gcc/v4.4.5
# GCC	    = $(GCCVER)/bin/g++
GCC	    = g++
CFLAGS	    = -Wall -Wno-unused-variable -std=c++0x
CXX_DEBUG   := -g


$(EXE) : wstat_main.cpp wstat.cpp
	$(GCC) $(CFLAGS) $(CXX_DEBUG) $(LDFLAGS) wstat_main.cpp wstat.cpp -o $(EXE)
	# $(BUILD_CMD)

default: build

build: $(EXE)

clean:
	rm -f *.o $(EXE) *~

all: clean build

