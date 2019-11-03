# Makefile

SRC = vlan_switch.cpp ../../src/comlib.cpp ../../src/Ethernet.cpp ../../src/MacAddress.cpp ../../src/NetIf.cpp ../../src/dlib.cpp ../../src/Exception.cpp ../../src/ARP.cpp ../../src/IPAddress.cpp ../../src/IPv4.cpp
C_OPT = -std=c++11 -Wall
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/switch

switch : $(SRC)
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
