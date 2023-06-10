.PHONY: all clean
objects=wrapper.o misc.o NetworkAction.o Rio.o HttpResponse.o HttpRequest.o LogOutput.o HttpServer.o shoot.o

all:$(objects)
	g++ -std=c++17 -o shoot.out $(objects) -lpthread

shoot.o:HttpServer.o shoot.cpp
	g++ -std=c++17 -c shoot.cpp

wrapper.o:wrapper.h wrapper.cpp
	g++ -std=c++17 -c wrapper.cpp

NetworkAction.o:wrapper.o NetworkAction.hpp NetworkAction.cpp
	g++ -std=c++17 -c NetworkAction.cpp

Rio.o:Rio.hpp Rio.cpp
	g++ -std=c++17 -c Rio.cpp

HttpResponse.o:HttpResponse.hpp HttpResponse.cpp Rio.o
	g++ -std=c++17 -c HttpResponse.cpp

HttpRequest.o:HttpRequest.hpp HttpRequest.cpp Rio.o
	g++ -std=c++17 -c HttpRequest.cpp

misc.o:misc.hpp misc.cpp
	g++ -std=c++17 -c misc.cpp

LogOutput.o:LogOutput.hpp LogOutput.cpp misc.o
	g++ -std=c++17 -c LogOutput.cpp

HttpServer.o:HttpServer.hpp HttpServer.cpp wrapper.o LogOutput.o HttpRequest.o HttpResponse.o NetworkAction.o
	g++ -std=c++17 -c HttpServer.cpp

clean:
	-rm -f $(objects)
