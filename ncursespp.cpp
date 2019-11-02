#include "ncursespp.hpp"
#include <ncurses.h>

ncurses::ncurses() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	getmaxyx(stdscr, screenRows, screenCols);
	win = newwin(screenRows, screenCols, 0, 0);
	display.resize(screenRows, screenCols);
	cursor = std::make_pair(screenRows - 1, screenCols - 1);
	refreshScreen();
}

ncurses::~ncurses() { endwin(); }

frame& ncurses::getFrame() { return display; }

void ncurses::refreshScreen() {
	// debug(2, "drawToScreen");
	for (uint row = 0; row < display.size().first; row++) {
		wmove((WINDOW*)win, row, 0);
		waddstr((WINDOW*)win, display[row].c_str());
	}
	wmove((WINDOW*)win, cursor.first, cursor.second);
	wrefresh((WINDOW*)win);
}

void ncurses::screenResizedTriger(int code) {
	// debug(1, "screenResizedTriger: " + std::to_string(code));
	getmaxyx((WINDOW*)win, screenRows, screenCols);
	refreshScreen();
}

int ncurses::getKey() const {
	nodelay(stdscr, FALSE);
	return getch();
}

std::pair<uint, uint> ncurses::size() const {
	return std::make_pair(screenRows, screenCols);
}
