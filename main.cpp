#include <unistd.h>
#include "ioconsole.hpp"
using udh::NCursesConsole;

int main() {
	sleep(1);
	auto screen = NCursesConsole::Get();
	screen->refreshScreen();
	sleep(1);
	auto& f = screen->getFrame();
	f[1][1] = 'a';
	screen->refreshScreen();
	sleep(1);
}
