# Makefile

CC = g++
SRC = route_table_test.cpp ../../src/RouteTable.cpp ../../src/IPAddress.cpp ../../src/comlib.cpp ../../src/Exception.cpp
INCLUDE = -I ../../include
C_OPT = -std=c++11 -Wall -O0
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/rtbl

rtbl : $(SRC)
	mkdir -p $(TARGET_DIR)
	$(CC) -o $(TARGET) $(INCLUDE) $(C_OPT) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	$(CC) -o $(TARGET)_debug $(INCLUDE) $(C_OPT) -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
