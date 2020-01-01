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

NCursesConsole& NCursesConsole::flush() {
	refreshScreen();
	return *this;
}

NCursesConsole& NCursesConsole::put(char c) {
	// TODO: Have more of the others call this.
	if (c == '\n') {
		if (cursor.first == screenRows - 1) return *this;
		cursor.first++;
		cursor.second = 0;
	} else {
		if (cursor.second == screenCols - 1) return *this;
		f[cursor.first][cursor.second++] = c;
	}
	return *this;
	// TODO: have some buffer management here for if it should flush.
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
	return *this;
}

NCursesConsole& NCursesConsole::operator<<(char const* rhs) {
	*this << std::string(rhs);  // extra lazy
	return *this;
}
NCursesConsole& NCursesConsole::operator<<(int rhs) {
	*this << std::to_string(rhs);  // extra lazy
	return *this;
}
NCursesConsole& NCursesConsole::operator<<(uint rhs) {
	*this << std::to_string(rhs);  // extra lazy
	return *this;
}

NCursesConsole& NCursesConsole::operator<<(char rhs) {
	*this << std::to_string(rhs);  // extra lazy
	return *this;
}

std::string& NCursesConsole::operator[](uint row) {
	return f[row];
	// This will need to make a sub class that forwards the [] as a <<
	// so that the buffering isn't interfeared with
}
// virtual basic_console& operator>>(basic_streambuf<char_type, traits_type>*
// __sb)=0;
// virtual basic_console& operator>>(bool& __n)=0;
// virtual basic_console& operator>>(short& __n)=0;
// virtual basic_console&  operator>>(unsigned short& __n)=0;
// virtual basic_console& operator>>(int& __n)=0;
// virtual basic_console& operator>>(unsigned int& __n)=0;
// virtual basic_console& operator>>(long& __n)=0;
// virtual basic_console& operator>>(unsigned long& __n)=0;
// virtual basic_console& operator>>(long long& __n)=0;
// virtual basic_console& operator>>(unsigned long long& __n)=0;
// virtual basic_console& operator>>(float& __f)=0;
// virtual basic_console& operator>>(double& __f)=0;
// virtual basic_console& operator>>(long double& __f)=0;
// virtual basic_console& operator>>(void*& __p)=0;

}  // namespace udh
