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
	// cursor = std::make_pair(screenRows - 1, screenCols - 1);
	cursor = std::make_pair(0, 0);
}

NCursesConsole::~NCursesConsole() { endwin(); }

void NCursesConsole::refreshScreen() {
	// debug(2, "drawToScreen");
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

NCursesConsole& NCursesConsole::operator<<(std::string rhs) {
	// TODO: add wrapping
	for (auto& c : rhs) {
		if (c == '\n') {
			if (cursor.first == screenRows - 1) break;
			cursor.first++;
			cursor.second = 0;
		} else {
			if (cursor.second == screenCols - 1) continue;
			f[cursor.first][cursor.second++] = c;
		}
	}
	refreshScreen();  // this should be behind a buffer.
	return *this;
}

NCursesConsole& NCursesConsole::operator<<(int rhs) {
	*this << std::to_string(rhs);  // extra lazy
	return *this;
}
}  // namespace udh
