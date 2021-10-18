objects=client_error.c parse_uri.c print_request.c service_dynamic.c service_static.c doit.c start.c
create:$(objects)
	gcc -o start.out $(objects) -lcsapp
