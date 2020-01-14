CC = g++ -std=c++11
CLIBS = -pthread 
CFLAGS = -g -Wall -O2 -lm -lrt
all: Metadata.o Socket.o Coordinator.o LRCCN LRCDN

Metadata.o: Metadata.cc
	$(CC) $(CFLAGS) -c $<

Socket.o: Socket.cc
	$(CC) $(CFLAGS) -c $<

Coordinator.o: Coordinator.cc Metadata.o Socket.o
	$(CC) $(CFLAGS) -c $<

LRCCN: LRCCN.cc Metadata.o Socket.o Coordinator.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

Datanode.o: Datanode.cc Socket.o
	$(CC) $(CFLAGS) -c $<

LRCDN: LRCDN.cc Socket.o Datanode.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean:
	rm LRCCN LRCDN *.o 
