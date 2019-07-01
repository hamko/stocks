CC = g++

CSRCS = $(shell ls *.c 2> /dev/null)
CXXSRCS = $(shell ls *.cpp 2> /dev/null)
OBJS = $(CSRCS:.c=.o) $(CXXSRCS:.cpp=.o)
HEADS = $(shell ls *.h 2> /dev/null) $(shell ls *.hpp 2> /dev/null)

CFLAGS = -g -Wall --std=c++0x -O3 -I/usr/local/include -DUSE_TRANSACTION -std=c++0x
LFLAGS = -lm -lcurl -ltidy -lsqlite3 -lhamkoutil

PROD = ./a.out

all: $(PROD)

$(PROD): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .c .cpp .o
.c.o:
	$(CC) $(CFLAGS) -c $< 
.cpp.o:
	$(CC) $(CFLAGS) -c $< 

$(OBJS): $(HEADS)

.PHONY: clean install
clean:
	rm -f $(PROD) $(OBJS)
