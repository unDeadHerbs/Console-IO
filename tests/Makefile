all: simple_run snek_run

simple.bin:
	clang++ -std=c++1z -Wall -Wextra -Wparentheses -Wno-dangling-else -lncurses -ltinfo -g  -lncurses -ltinfo -o simple.bin simple.cpp ../ioconsole.cpp

.PHONY: simple_run
simple_run: simple.bin
	./simple.bin

snek.bin:
	clang++ -std=c++1z -Wall -Wextra -Wparentheses -Wno-dangling-else -lncurses -ltinfo -g  -lncurses -ltinfo -o snek.bin snek*.cpp ../ioconsole.cpp

.PHONY: snek_run
snek_run: snek.bin
	./snek.bin
