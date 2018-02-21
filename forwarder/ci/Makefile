# Makefile

#コンパイラ
CC = g++
CFLAGS = -std=c++11 -Wall
INCLUDE = -I../include
LIB = `cppunit-config --libs`

#テストしたいライブラリ
LIBSRCDIR = ../src
LIBSRC = $(wildcard $(LIBSRCDIR)/*.cpp)

#テストコード
TESTSRCDIR = ./src
TESTSRC = $(wildcard $(TESTSRCDIR)/*.cpp)

#cppunitのmain関数(共通)
CPPUNITMAIN = $(TESTSRCDIR)/main/test_main.cpp

#出力先
OBJDIR = ./obj
LIBOBJDIR = $(OBJDIR)/lib
TESTOBJDIR = $(OBJDIR)/test

LIBOBJECTS = $(addprefix $(LIBOBJDIR)/, $(notdir $(LIBSRC:.cpp=.o)))
TESTOBJECTS = $(addprefix $(LIBOBJDIR)/, $(notdir $(LIBSRC:.cpp=.o)))

TARGETDIR = ./bin
TARGETS =  $(addprefix $(TARGETDIR)/, $(notdir $(TESTSRC:.cpp=)))

all : $(TARGETS)

#実行ファイル生成
$(TARGETDIR)/% : $(TESTSRCDIR)/%.cpp $(LIBOBJECTS) $(TESTOBJECTS) $(CPPUNITMAIN)
	mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(LIBOBJECTS) $(CPPUNITMAIN) $(LIB)

#テスト対象のライブラリのobjファイル
$(LIBOBJDIR)/%.o: $(LIBSRCDIR)/%.cpp
	mkdir -p $(LIBOBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

#テストコードのobjファイル
$(TESTOBJDIR)/%.o: $(TESTSRCDIR)/%.cpp
	mkdir -p $(TESTOBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean: 
	rm -rf $(OBJDIR) $(TARGETDIR)
