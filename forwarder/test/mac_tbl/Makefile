# Makefile

SRC = mac_tbl_test.cpp ../../src/MacAddress.cpp ../../src/NetIf.cpp ../../src/comlib.cpp ../../src/Ethernet.cpp ../../src/Exception.cpp ../../src/IPAddress.cpp ../../src/ARP.cpp
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/mac_tbl

mac_tbl : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(INCLUDE) -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
