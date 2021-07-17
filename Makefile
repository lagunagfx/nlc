# -*- MakeFile -*-

TARGET = output
SRC = main.cpp midi.cpp
OBJ = main.o midi.o
DEPS = midi.h

CXX = g++
CFLAGS= -Wall -g -O3 -I/usr/include/rtmidi
LDFLAGS = -lrtmidi

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: install clean

clean:
	rm -v $(OBJ) $(TARGET)
