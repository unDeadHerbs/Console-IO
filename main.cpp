#include <unistd.h>
#include "ncursespp.hpp"

int main() {
	sleep(1);
	auto screen = ncurses::Get();
	screen->refreshScreen();
	sleep(1);
	auto& f = screen->getFrame();
	f[1][1] = 'a';
	screen->refreshScreen();
	sleep(1);
}
