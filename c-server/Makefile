all: server

build_dir=build
commons_dir=../common

source_files = auth.c _utils.c handle_client.c
build_files = $(source_files:%.c=$(build_dir)/%.o)

server: main.c $(build_files) $(commons_dir)/*.h
	gcc -Wall -o server -lpthread $^

.PHONY: make_build_dir
make_build_dir: 
	mkdir -p build

$(build_dir)/%.o: %.c make_build_dir
	gcc -Wall -o $@ -c $<

