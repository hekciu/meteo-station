all: server

build_dir=build
commons_dir=../common

source_files = auth.c _utils.c handle_client.c _database.c _parse.c
build_files = $(source_files:%.c=$(build_dir)/%.o)

server: main.c $(build_files) $(commons_dir)/*.h
	gcc -Wall --std=c99 -o server $(build_files) -lpthread -lpq main.c 

debug: main.c $(build_files) $(commons_dir)/*.h
	gcc -g -Wextra -Wall -o debug $(build_files) -lpthread -lpq main.c 

.PHONY: make_build_dir
make_build_dir: 
	mkdir -p build

$(build_dir)/%.o: %.c make_build_dir
	gcc -Wall -o $@ -c $<

