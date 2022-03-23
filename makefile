objects=csapp_changed.o client_error.o parse_uri.o fill_http_request.o service_dynamic.o service_static.o doit.o sbuf.o get_time.o start.o

create:$(objects)
	g++ -std=gnuc++11 -o tinywebd.out $(objects) -lpthread

client_error.o:tinyweb.h client_error.cpp
	g++ -std=gnuc++11 -c client_error.cpp

parse_uri.o:parse_uri.cpp
	g++ -std=gnuc++11 -c parse_uri.cpp

fill_http_request.o:fill_http_request.cpp
	g++ -std=gnuc++11 -c fill_http_request.cpp

service_static.o:tinyweb.h service_static.cpp
	g++ -std=gnuc++11 -c service_static.cpp

service_dynamic.o:tinyweb.h service_dynamic.cpp
	g++ -std=gnuc++11 -c service_dynamic.cpp

doit.o:tinyweb.h doit.cpp
	g++ -std=gnuc++11 -c doit.cpp

start.o:sbuf.h tinyweb.h start.cpp
	g++ -std=gnuc++11 -c start.cpp

sbuf.o:sbuf.h sbuf.cpp
	g++ -std=gnuc++11 -c sbuf.cpp
	
csapp_changed.o:csapp_changed.h csapp_changed.cpp
	g++ -std=gnuc++11 -c csapp_alter.cpp

get_time.o:get_time.cpp
	g++ -std=gnuc++11 -c get_time.cpp

clean:
	rm -f $(objects)
