#include <unistd.h>
#include "../ioconsole.hpp"
using udh::cio;
using udh::endl;

int main() {
	sleep(1);
	cio << "a\n b\n  c\n"
	    << "a" << endl
	    << " b" << endl
	    << "  c";
	sleep(1);
	cio << "\n\n(" << cio.size().first << "," << cio.size().second << ")";
	sleep(2);
}
