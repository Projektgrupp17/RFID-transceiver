# RFID-transceiver Makefile 1.0 4/5 (v0.92 24/4, v0.91 17/4, v0.9 16/4-2020).
# Author: Fredrik Gölman and Patrik Sjöfors.
.POSIX:
.SUFFIXES:
GCC=g++
CLIBS=/lib
TLIBS=./tests
POCOH=./include
CUSTOM_DEP=/usr/local/lib
CPPFLAGS=-g -Wall -I$(CLIBS) -I$(POCOH) -I$(POCOH)/Poco/ -I$(POCOH)/Poco/Net -I$(POCOH)/Poco/JSON -I$(POCOH)/Poco/Dynamic -I$(TLIBS) -I$(TLIBS)/gtest -I$(TLIBS)/gtest/internal
LDFLAGS=-g
LDLIBS=-L/lib -L/usr/local/lib/poco/lib -L/home/drax/Projects/dev/projects/RFID-transceiver/lib -lCR95HF -lusb-1.0 -lPocoNet -lPocoJSON -lPocoFoundation -lgtest -lgtest_main -lpthread
OBJFILES=main.o reader/reader.o simulate/simulate.o transmit/transmit.o util/util.o util/interpreter.o config/config.o tests/test.o data/tag_data.o data/tag.o data/tag_collection.o data/tag_deactivator.o

all: rfid test
rfid: $(OBJFILES)
	$(GCC) $(LDFLAGS) -o rfid main.o reader/reader.o util/util.o util/interpreter.o simulate/simulate.o config/config.o data/tag_data.o data/tag.o data/tag_collection.o data/tag_deactivator.o transmit/transmit.o $(LDLIBS)
main.o: main.cpp reader/reader.hpp simulate/simulate.hpp config/config.hpp util/util.hpp
	$(GCC) $(CPPFLAGS) -o main.o -c main.cpp
reader/reader.o: reader/reader.cpp reader/reader.hpp util/util.hpp data/tag_data.hpp config/config.hpp transmit/transmit.hpp include/libcr95hf.h
	$(GCC) $(CPPFLAGS) -o reader/reader.o -c reader/reader.cpp
simulate/simulate.o: simulate/simulate.cpp simulate/simulate.hpp data/tag_data.hpp config/config.hpp util/util.hpp transmit/transmit.hpp
	$(GCC) $(CPPFLAGS) -o simulate/simulate.o -c simulate/simulate.cpp
transmit/transmit.o: transmit/transmit.cpp transmit/transmit.hpp data/tag_data.hpp config/config.hpp include/Poco/Exception.h include/Poco/StreamCopier.h include/Poco/Net/NetException.h include/Poco/Net/HTTPRequest.h include/Poco/Net/HTTPResponse.h include/Poco/Net/HTTPClientSession.h include/Poco/JSON/Object.h
	$(GCC) $(CPPFLAGS) -o transmit/transmit.o -c transmit/transmit.cpp
util/util.o: util/util.cpp util/util.hpp
	$(GCC) $(CPPFLAGS) -o util/util.o -c util/util.cpp
util/interpreter.o: util/interpreter.cpp util/interpreter.hpp
	$(GCC) $(CPPFLAGS) -o util/interpreter.o -c util/interpreter.cpp
config/config.o: config/config.cpp config/config.hpp
	$(GCC) $(CPPFLAGS) -o config/config.o -c config/config.cpp
data/tag_data.o: data/tag_data.cpp data/tag_data.hpp
	$(GCC) $(CPPFLAGS) -o data/tag_data.o -c data/tag_data.cpp
data/tag.o: data/tag.cpp data/tag.hpp data/tag_data.hpp
	$(GCC) $(CPPFLAGS) -o data/tag.o -c data/tag.cpp
data/tag_collection.o: data/tag_collection.cpp data/tag_collection.hpp data/tag.hpp data/tag_data.hpp transmit/transmit.hpp config/config.hpp
	$(GCC) $(CPPFLAGS) -o data/tag_collection.o -c data/tag_collection.cpp
data/tag_deactivator.o: data/tag_deactivator.cpp data/tag_deactivator.hpp data/tag_data.hpp data/tag_collection.hpp transmit/transmit.hpp config/config.hpp
	$(GCC) $(CPPFLAGS) -o data/tag_deactivator.o -c data/tag_deactivator.cpp
tests/test.o: tests/test.cpp config/config.hpp util/util.hpp util/interpreter.hpp data/tag_data.hpp data/tag_collection.hpp transmit/transmit.hpp
	$(GCC) $(CPPFLAGS) -o tests/test.o -c tests/test.cpp
clean:
	rm -f core rfid test $(OBJFILES)
test: $(OBJFILES)
	$(GCC) $(LDFLAGS) -o test tests/test.o config/config.o util/util.o util/interpreter.o data/tag.o data/tag_data.o data/tag_collection.o data/tag_deactivator.o transmit/transmit.o $(LDLIBS)
check:
	./test
#dependencies:
#custom_dependencies:
#	sudo rm -f -r $(CUSTOM_DEP)/poco
#	sudo rm -f -r $(CUSTOM_DEP)/cr95hf
#	sudo mkdir $(CUSTOM_DEP)/poco
#	sudo mkdir $(CUSTOM_DEP)/poco/lib
#	sudo mkdir $(CUSTOM_DEP)/cr95hf
#	sudo cp lib/libPoco* $(CUSTOM_DEP)/poco/lib
#	sudo cp lib/libCR95HF.so $(CUSTOM_DEP)/cr95hf
#	echo "${CUSTOM_DEP}/poco/lib" >> poco.conf
#	echo "${CUSTOM_DEP}/cr95hf" >> cr95hf.conf
#	sudo cp poco.conf cr95hf.conf /etc/ld.so.conf.d
#	rm -f poco.conf cr95hf.conf
#	sudo ldconfig
#dependencies:
#	sudo cp lib/libPoco* /usr/lib
#	sudo cp lib/libCR95HF.so /usr/lib64
#	sudo ldconfig
#clean_dependencies:
#	sudo rm -f -r $(CUSTOM_DEP)/poco
#	sudo rm -f /etc/ld.so.conf.d/poco.conf
#	sudo rm -f /usr/lib/libPoco*
#	sudo rm -f /usr/lib64/libCR95HF.so
#	sudo ldconfig