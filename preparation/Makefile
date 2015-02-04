CC = c++
INCLUDE = -I$(shell root-config --incdir)
OPFLAGS = -O2
CFLAGS = -Wall -Wextra -std=c++11 $(INCLUDE) $(OPFLAGS) $(MACROS)
LDFLAGS = $(shell root-config --libs) -lTreePlayer -lHistPainter -lTMVA

all: splitTuples

splitTuples: splitTuples.o
	@ $(CC) $(CFLAGS) $(LDFLAGS) $+ -o $@

%.o: %.cpp
	@ $(CC) $(CFLAGS) -c $< -o $@
