#include "snek_class.hpp"

int main() {
	Snek s;
	while (s.Alive()) s.move(Snek::Direction::left);
	return 0;
}
