all: simple.run snek.run

%.bin: %*.cpp
	clang++ -std=c++1z -Wall -Wextra -Wparentheses -Wno-dangling-else -lncurses -ltinfo -g  -lncurses -ltinfo -o $@ $^ ../ioconsole.cpp

.PHONY: %.run
%.run: %.bin
	./$<
