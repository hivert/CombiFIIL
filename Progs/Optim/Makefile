CXXFLAGS    = -fcilkplus -std=c++11 -g -Wall -O3 -funroll-loops -mavx -mtune=native -flax-vector-conversions
PROGS = sort cycle democilk demovect demovect2

all: ${PROGS}

sort: sort.cpp
cycle: cycle.cpp
democilk: democilk.cpp
demovect: demovect.cpp
demovect2: demovect2.cpp

clean:
	rm -f ${PROGS}
