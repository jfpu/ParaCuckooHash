CC=g++

# CFLAGS=-std=c++11 -Werror -Wall -Wextra -lpthread -lboost_system -lboost_thread
CFLAGS=-Wall -Wextra -lpthread -lboost_system -lboost_thread -I../util/sparsehash-c11

TARGET=benchmark/main

EXECUTABLE=run_benchmark

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(TARGET).cpp

clean:
	$(RM) $(EXECUTABLE)

