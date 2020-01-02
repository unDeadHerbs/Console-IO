#include "ioconsole.hpp"
#include <ncurses.h>

namespace udh {

NCursesConsole::NCursesConsole() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	getmaxyx(stdscr, screenRows, screenCols);
	win = newwin(screenRows, screenCols, 0, 0);
	screenResizedTriger(0);
	cursor = std::make_pair(0, 0);
}

NCursesConsole::~NCursesConsole() { endwin(); }

void NCursesConsole::refreshScreen() {
	for (uint row = 0; row < size().first; row++) {
		wmove((WINDOW*)win, row, 0);
		waddstr((WINDOW*)win, f[row].c_str());
	}
	wmove((WINDOW*)win, cursor.first, cursor.second);
	wrefresh((WINDOW*)win);
}

void NCursesConsole::screenResizedTriger(int code) {
	getmaxyx((WINDOW*)win, screenRows, screenCols);
	if (f.size() != screenRows) {
		for (uint i = f.size(); i < screenRows; i++) f.push_back("");
		for (uint i = f.size(); i > screenRows; i--) f.pop_back();
	}
	for (uint j = 0; j < f.size(); j++) {
		if (f[j].size() != screenCols) {
			for (uint i = f[j].size(); i < screenCols; i++) f[j].push_back(' ');
			for (uint i = f[j].size(); i > screenCols; i--) f[j].pop_back();
		}
	}
	refreshScreen();
}

int NCursesConsole::getKey() const {
	nodelay(stdscr, FALSE);
	return getch();
}

std::pair<uint, uint> NCursesConsole::size() const {
	return std::make_pair(screenRows, screenCols);
}

int NCursesConsole::ostream_buffer::sync() {
	auto s = this->str();
	auto& cio = *Get();
	for (auto& c : s) {
		if (c == '\n') {
			if (cio.cursor.first == cio.screenRows - 1) break;
			cio.cursor.first++;
			cio.cursor.second = 0;
		} else {
			if (cio.cursor.second == cio.screenCols - 1) continue;
			cio.f[cio.cursor.first][cio.cursor.second++] = c;
		}
	}
	cio.refreshScreen();
	this->str("");
	return 0;
}

std::ostream& NCursesConsole::ostream() {
	static std::unique_ptr<ostream_buffer> buffer_instance;
	static std::unique_ptr<std::ostream> Instance;
	if (!Instance) {
		buffer_instance = std::unique_ptr<ostream_buffer>(new ostream_buffer);
		Instance =
		    std::unique_ptr<std::ostream>(new std::ostream(&*buffer_instance));
	}
	return *Instance;
}

std::string& NCursesConsole::operator[](uint row) {
	return f[row];
	// This will need to make a sub class that forwards the [] as a <<
	// so that the buffering isn't interfeared with
}

}  // namespace udh
