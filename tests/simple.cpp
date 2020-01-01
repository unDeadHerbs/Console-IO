#include <unistd.h>
#include "../ioconsole.hpp"
using udh::cio;

int main() {
	sleep(1);
	cio << "a\n b\n  c";
	sleep(3);
	cio << "\n\n(" << cio.size().first << "," << cio.size().second << ")";
	sleep(3);
}
