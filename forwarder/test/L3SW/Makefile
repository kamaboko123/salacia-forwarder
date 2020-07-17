# Makefile

SRC = l3sw.cpp ../../src/comlib.cpp ../../src/Ethernet.cpp ../../src/MacAddress.cpp ../../src/dlib.cpp ../../src/Exception.cpp ../../src/ARP.cpp ../../src/IPAddress.cpp ../../src/NetIf.cpp ../../src/IPv4.cpp ../../src/RouteTable.cpp
C_OPT = -std=c++11 -Wall
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/l3sw

l3sw : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) $(C_OPT) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(INCLUDE) $(C_OPT) -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
