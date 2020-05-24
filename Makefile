# RFID-transceiver Makefile 1.0 4/5 (v0.92 24/4, v0.91 17/4, v0.9 16/4-2020).
# Author: Fredrik Gölman and Patrik Sjöfors.
.POSIX:
.SUFFIXES:
GCC=g++
SRC=./src
BUILDPATH=./build
CLIBS=/lib
TLIBS=./tests
POCOH=./include
CUSTOM_DEP=/usr/local/lib
CPPFLAGS=-g -Wall -I$(CLIBS) -I$(POCOH) -I$(POCOH)/Poco/ -I$(POCOH)/Poco/Net -I$(POCOH)/Poco/JSON -I$(POCOH)/Poco/Dynamic -I$(TLIBS) -I$(TLIBS)/gtest -I$(TLIBS)/gtest/internal
LDFLAGS=-g
LDLIBS=-L/lib -L/usr/local/lib/poco/lib -L/home/drax/Projects/dev/projects/RFID-transceiver/lib -lCR95HF -lusb-1.0 -lPocoNet -lPocoJSON -lPocoFoundation -lgtest -lgtest_main -lpthread
OBJFILES=$(BUILDPATH)/main.o $(BUILDPATH)/reader.o $(BUILDPATH)/simulator.o $(BUILDPATH)/transmitter.o $(BUILDPATH)/util.o $(BUILDPATH)/interpreter.o $(BUILDPATH)/config.o $(BUILDPATH)/test.o $(BUILDPATH)/tag_data.o $(BUILDPATH)/work_manager.o $(BUILDPATH)/tag.o $(BUILDPATH)/tag_collection.o $(BUILDPATH)/tag_deactivator.o

all: rfid test
rfid: $(OBJFILES)
	$(GCC) $(LDFLAGS) -o rfid $(BUILDPATH)/main.o $(BUILDPATH)/reader.o $(BUILDPATH)/util.o $(BUILDPATH)/interpreter.o $(BUILDPATH)/simulator.o $(BUILDPATH)/config.o $(BUILDPATH)/work_manager.o $(BUILDPATH)/tag_data.o $(BUILDPATH)/tag.o $(BUILDPATH)/tag_collection.o $(BUILDPATH)/tag_deactivator.o $(BUILDPATH)/transmitter.o $(LDLIBS)
$(BUILDPATH)/main.o: main.cpp $(SRC)/reader.hpp $(SRC)/simulator.hpp $(SRC)/config.hpp $(SRC)/util.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/main.o -c main.cpp
$(BUILDPATH)/reader.o: $(SRC)/reader.cpp $(SRC)/reader.hpp $(SRC)/util.hpp $(SRC)/tag_data.hpp $(SRC)/config.hpp $(SRC)/transmitter.hpp include/libcr95hf.h
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/reader.o -c $(SRC)/reader.cpp
$(BUILDPATH)/simulator.o: $(SRC)/simulator.cpp $(SRC)/simulator.hpp $(SRC)/tag_data.hpp $(SRC)/config.hpp $(SRC)/util.hpp $(SRC)/transmitter.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/simulator.o -c $(SRC)/simulator.cpp
$(BUILDPATH)/transmitter.o: $(SRC)/transmitter.cpp $(SRC)/transmitter.hpp $(SRC)/tag_data.hpp $(SRC)/config.hpp include/Poco/Exception.h include/Poco/StreamCopier.h include/Poco/Net/NetException.h include/Poco/Net/HTTPRequest.h include/Poco/Net/HTTPResponse.h include/Poco/Net/HTTPClientSession.h include/Poco/JSON/Object.h
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/transmitter.o -c $(SRC)/transmitter.cpp
$(BUILDPATH)/util.o: $(SRC)/util.cpp $(SRC)/util.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/util.o -c $(SRC)/util.cpp
$(BUILDPATH)/interpreter.o: $(SRC)/interpreter.cpp $(SRC)/interpreter.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/interpreter.o -c $(SRC)/interpreter.cpp
$(BUILDPATH)/config.o: $(SRC)/config.cpp $(SRC)/config.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/config.o -c $(SRC)/config.cpp
$(BUILDPATH)/work_manager.o: $(SRC)/work_manager.cpp $(SRC)/work_manager.hpp $(SRC)/tag_collection.hpp $(SRC)/tag.hpp $(SRC)/tag_deactivator.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/work_manager.o -c $(SRC)/work_manager.cpp
$(BUILDPATH)/tag_data.o: $(SRC)/tag_data.cpp $(SRC)/tag_data.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/tag_data.o -c $(SRC)/tag_data.cpp
$(BUILDPATH)/tag.o: $(SRC)/tag.cpp $(SRC)/tag.hpp $(SRC)/tag_data.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/tag.o -c $(SRC)/tag.cpp
$(BUILDPATH)/tag_collection.o: $(SRC)/tag_collection.cpp $(SRC)/tag_collection.hpp $(SRC)/tag.hpp $(SRC)/tag_data.hpp $(SRC)/transmitter.hpp $(SRC)/config.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/tag_collection.o -c $(SRC)/tag_collection.cpp
$(BUILDPATH)/tag_deactivator.o: $(SRC)/tag_deactivator.cpp $(SRC)/tag_deactivator.hpp $(SRC)/tag_data.hpp $(SRC)/tag_collection.hpp $(SRC)/transmitter.hpp $(SRC)/config.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/tag_deactivator.o -c $(SRC)/tag_deactivator.cpp
$(BUILDPATH)/test.o: tests/test.cpp $(SRC)/config.hpp $(SRC)/util.hpp $(SRC)/interpreter.hpp $(SRC)/tag_data.hpp $(SRC)/tag_collection.hpp $(SRC)/transmitter.hpp
	$(GCC) $(CPPFLAGS) -o $(BUILDPATH)/test.o -c tests/test.cpp
clean:
	rm -f core rfid test $(OBJFILES)
test: $(OBJFILES)
	$(GCC) $(LDFLAGS) -o test $(BUILDPATH)/test.o $(BUILDPATH)/config.o $(BUILDPATH)/util.o $(BUILDPATH)/interpreter.o $(BUILDPATH)/work_manager.o $(BUILDPATH)/tag.o $(BUILDPATH)/tag_data.o $(BUILDPATH)/tag_collection.o $(BUILDPATH)/tag_deactivator.o $(BUILDPATH)/transmitter.o $(LDLIBS)
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