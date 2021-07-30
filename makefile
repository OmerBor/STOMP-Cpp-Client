CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread -lboost_thread

all: EchoClient
	g++ -o bin/StompBookClubClient bin/ConnectionHandler.o bin/echoClient.o bin/readInput.o bin/frameSTOMP.o bin/Frame.o $(LDFLAGS) 

EchoClient: bin/ConnectionHandler.o bin/echoClient.o bin/readInput.o bin/frameSTOMP.o bin/Frame.o
	
bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/readInput.o: src/readInput.cpp
	g++ $(CFLAGS) -o bin/readInput.o src/readInput.cpp	

bin/frameSTOMP.o: src/frameSTOMP.cpp
	g++ $(CFLAGS) -o bin/frameSTOMP.o src/frameSTOMP.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
