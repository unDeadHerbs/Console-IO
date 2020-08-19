#include <unistd.h>

#include "../ioconsole.hpp"
using std::endl;
using std::flush;
using udh::cio;

int main() {
  sleep(1);
  cio << "a\n b\n  c\n";
  sleep(1);
  cio << "a" << endl << " b" << endl << "  c" << flush;
  sleep(1);
  cio << "\n\n(" << cio.size().first << "," << cio.size().second << ")"
      << flush;
  sleep(2);
}
