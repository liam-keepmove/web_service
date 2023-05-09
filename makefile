objects=wrapper.o miscFunc.o NetworkAction.o Rio.o HttpResponse.o HttpRequest.o LogOutput.o HttpServer.o shoot.o

all:$(objects)
	g++ -std=gnu++11 -o shoot.out $(objects) -lpthread

shoot.o:HttpServer.o shoot.cpp
	g++ -std=gnu++11 -c shoot.cpp

wrapper.o:wrapper.h wrapper.cpp
	g++ -std=gnu++11 -c wrapper.cpp

NetworkAction.o:wrapper.o NetworkAction.hpp NetworkAction.cpp
	g++ -std=gnu++11 -c NetworkAction.cpp

Rio.o:Rio.hpp Rio.cpp
	g++ -std=gnu++11 -c Rio.cpp

HttpResponse.o:HttpResponse.hpp HttpResponse.cpp Rio.o
	g++ -std=gnu++11 -c HttpResponse.cpp

HttpRequest.o:HttpRequest.hpp HttpRequest.cpp Rio.o
	g++ -std=gnu++11 -c HttpRequest.cpp

miscFunc.o:miscFunc.hpp miscFunc.cpp
	g++ -std=gnu++11 -c miscFunc.cpp

LogOutput.o:LogOutput.hpp LogOutput.cpp miscFunc.o
	g++ -std=gnu++11 -c LogOutput.cpp

HttpServer.o:HttpServer.hpp HttpServer.cpp wrapper.o LogOutput.o HttpRequest.o HttpResponse.o Pool.o NetworkAction.o
	g++ -std=gnu++11 -c HttpServer.cpp

clean:
	-rm -f $(objects)
