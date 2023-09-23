CXX = g++
# CFLAGS = -I./include -L./lib -ljpeg -lfont -lm -O0 -Wall -g 

client:client.cpp
	$(CXX) $^ -o $@ 

server:server.cpp
	$(CXX) $^ -o $@ 

# .PHONY:all
