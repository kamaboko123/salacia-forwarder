# Makefile

SRC = cache.cpp ../../src/MacAddress.cpp ../../src/comlib.cpp ../../src/Exception.cpp ../../src/IPAddress.cpp ../../src/ARP.cpp
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/cache

hashmap : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) -std=c++11 -O0 -g $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(ICNLUDE) -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
