.phony: all clean

run: crt.cpp mod_ops.cpp mod_ops.h
	g++ --std=c++20 crt.cpp mod_ops.cpp -o $@

all: run

clean:
	rm run