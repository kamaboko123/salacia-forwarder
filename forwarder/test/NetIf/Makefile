# Makefile

SRC = netif_test.cpp ../../src/NetIf.cpp ../../src/MacAddress.cpp ../../src/comlib.cpp ../../src/IPAddress.cpp ../../src/Ethernet.cpp ../../src/ARP.cpp ../../src/Exception.cpp
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/netif

netif : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(INCLUDE) -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

