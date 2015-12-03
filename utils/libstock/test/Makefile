CC = g++

CSRCS = $(shell ls *.c ../*.c 2> /dev/null)
CXXSRCS = $(shell ls *.cpp ../*.cpp 2> /dev/null)
OBJS = $(CSRCS:.c=.o) $(CXXSRCS:.cpp=.o)
HEADS = $(shell ls *.h ../*.h 2> /dev/null) $(shell ls *.hpp ../*.h 2> /dev/null)

CFLAGS = -g -O3 -Wall -std=c++0x -fopenmp -DTEST -I..
LFLAGS = -lm -lcppunit

PROD = ./a.out

all: $(PROD)

$(PROD): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .c .cpp .o
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
.cpp.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS): $(HEADS)

.PHONY: clean install
clean:
	rm -f $(PROD) $(OBJS)
