# Makefile

SRC = mac_address_test.cpp ../../src/MacAddress.cpp
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/mac_address

mac_address : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(INCLUDE) -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

