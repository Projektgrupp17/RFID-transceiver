# RFID-transceiver Makefile v0.91 17/4 (v0.9 16/4-2020).
# Author: Fredrik Gölman and Patrik Sjöström.
.POSIX:
.SUFFIXES:
GCC=g++
CPPFLAGS=-g -Wall -I./lib
LDFLAGS=-g
LDLIBS=-L/lib -lCR95HF -lusb-1.0

all: rfid
rfid: main.o reader/reader.o util/util.o simulate/simulate.o
	$(GCC) $(LDFLAGS) -o rfid main.o reader/reader.o util/util.o simulate/simulate.o $(LDLIBS)
reader/reader.o: reader/reader.cpp reader/reader.hpp util/util.hpp data/TagData.hpp
	$(GCC) $(CPPFLAGS) -o reader/reader.o -c reader/reader.cpp
simulate/simulate.o: simulate/simulate.cpp simulate/simulate.hpp data/TagData.hpp
	$(GCC) $(CPPFLAGS) -o simulate/simulate.o -c simulate/simulate.cpp
util/util.o: util/util.cpp util/util.hpp
	$(GCC) $(CPPFLAGS) -o util/util.o -c util/util.cpp
main.o: main.cpp reader/reader.hpp simulate/simulate.hpp data/TagData.hpp config/Config.hpp 
	$(GCC) $(CPPFLAGS) -o main.o -c main.cpp
clean:
	rm -f core rfid main.o reader/reader.o simulate/simulate.o util/util.o
