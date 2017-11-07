# Makefile

SRC = mac_table_test.cpp ../../src/comlib.cpp ../../src/MacAddress.cpp ../../src/NetIf.cpp ../../src/Ethernet.cpp ../../src/dlib.cpp ../../src/MacTable.cpp
C_OPT = -std=c++11 -Wall
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/mac_table

switch : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(C_OPT) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(C_OPT) -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
