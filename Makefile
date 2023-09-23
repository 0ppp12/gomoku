CXX = g++
# CFLAGS = -I./include -L./lib -ljpeg -lfont -lm -O0 -Wall -g 

client:client.cpp
	$(CC) $^ -o $@ 

server:server.cpp
	$(CC) $^ -o $@ 

# .PHONY:all
