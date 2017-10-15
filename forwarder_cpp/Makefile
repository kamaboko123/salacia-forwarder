# Makefile

SRC = src/raw_sock.cpp src/NetIf.cpp
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/raw_sock

l2sw : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

