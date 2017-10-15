# Makefile

SRC = mac_table_test.cpp ../../src/MacTable.cpp ../../src/MacEntry.cpp ../../src/NetIf.cpp
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/mac_entry

mac_entry : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

