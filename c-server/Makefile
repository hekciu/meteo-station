all: server

build_dir=build
commons_dir=../common

source_files = auth.c _utils.c handle_client.c
build_files = $(source_files:%.c=$(build_dir)/%.o)

server: main.c $(build_files) $(commons_dir)/*.h
	mkdir -p build
	gcc -Wall -o server -lpthread $^

$(build_dir)/%.o: %.c 
	gcc -Wall -o $@ -c $<

