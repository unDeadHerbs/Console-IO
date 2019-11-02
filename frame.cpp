#include "frame.hpp"

frame::frame(char ch) {
	f.push_back("");
	f[0].push_back(ch);
}

frame::frame(std::string s) { f.push_back(s); }

frame::frame(uint r) { resize(r, r); }

std::string& frame::operator[](uint rhs) { return f[rhs]; }

char& frame::operator[](std::pair<uint, uint> rhs) {
	return f[rhs.first][rhs.second];
}

void frame::add_row(std::string row) { f.push_back(row); }

std::pair<uint, uint> frame::size() const {
	return std::make_pair(f.size(), f.size() ? f[0].size() : 0);
}

void frame::resize(uint rows, uint cols) {
	/// debug(3, "resize f");
	// debug(4,
	//"setting size to " + std::to_string(rows) + ":" + std::to_string(cols));
	// debugv(4, "size was " + std::to_string(size().first) + ":" +
	// std::to_string(size().second));
	if (f.size() != rows) {
		for (uint i = f.size(); i < rows; i++) f.push_back("");
		for (uint i = f.size(); i > rows; i--) f.pop_back();
	}
	for (uint j = 0; j < f.size(); j++) {
		if (f[j].size() != cols) {
			//  debugv(5, "row size was " + std::to_string(f[j].size()));
			for (uint i = f[j].size(); i < cols; i++) f[j].push_back(' ');
			for (uint i = f[j].size(); i > cols; i--) f[j].pop_back();
			// debugv(5, "row size is now " + std::to_string(f[j].size()));
		}
	}
	/*if (f.size() != 0)
	  debugv(4, "size is now " + std::to_string(f.size()) + ":" +
	                std::to_string(f[0].size()));
	else
	debugv(4, "size is now 0:0");*/
}

void frame::drop_row() { f.pop_back(); }

void frame::place(frame sub, int row, int col) {
	//  debug(2, "Placing sub-F");
	for (uint r = std::max(0, row);
	     r < size().first && r - row < sub.size().first; r++) {
		for (uint c = std::max(0, col);
		     c < f[r].size() && c - col < sub[r - row].size(); c++) {
			//  debug(5, "update display location " + std::to_string(r) + "," +
			//           std::to_string(c) + " to " + sub[r - row][c - col]);
			f[r][c] = sub[r - row][c - col];
		}
	}
}
void frame::place(frame sub, std::pair<int, int> pos) {
	place(sub, pos.first, pos.second);
}

void frame::clear() {
	auto s = size();
	resize(0, 0);
	resize(s.first, s.second);
}
