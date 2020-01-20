CC = g++ -std=c++11
CLIBS = -pthread 
CFLAGS = -g -Wall -O2 -lm -lrt
all: tinyxml2.o Config.o Metadata.o Socket.o Coordinator.o LRCCN LRCDN

tinyxml2.o: Util/tinyxml2.cpp Util/tinyxml2.h
	$(CC) $(CFLAGS) -c $<

Config.o: Config.cc tinyxml2.o
	$(CC) $(CFLAGS) -c $<

Metadata.o: Metadata.cc Config.o tinyxml2.o
	$(CC) $(CFLAGS) -c $<

Socket.o: Socket.cc
	$(CC) $(CFLAGS) -c $<

Coordinator.o: Coordinator.cc Metadata.o Config.o tinyxml2.o Socket.o
	$(CC) $(CFLAGS) -c $<

LRCCN: LRCCN.cc Metadata.o Config.o tinyxml2.o Socket.o Coordinator.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

Datanode.o: Datanode.cc Socket.o Config.o tinyxml2.o
	$(CC) $(CFLAGS) -c $<

LRCDN: LRCDN.cc Socket.o Datanode.o Config.o tinyxml2.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean:
	rm LRCCN LRCDN *.o 
