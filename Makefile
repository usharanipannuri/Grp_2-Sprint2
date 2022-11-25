server: server.o Login.o
	g++ server.o Login.o -o server -l pthread

server.o:server.cpp
	g++ -c server.cpp

Login.o:Login.cpp
	g++ -c Login.cpp

clean:
	rm *.o server
