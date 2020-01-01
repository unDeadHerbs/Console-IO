#include <unistd.h>
#include "ioconsole.hpp"
using udh::cio;

int main() {
	sleep(1);
	cio << "an bn  c";
	sleep(1);
	cio << "nn(" << cio.size().first << "," << cio.size().second << ")";
	sleep(1);
}
