#ifndef COLOUR_CHARS_HPP
#define COLOUR_CHARS_HPP

#include <deque>
#include <optional>
#include <string>

namespace udh {
// TODO: Specialize on number of colours supported with an "UNKNOWN"
// option.
struct colour_char {
  enum COLOUR {
    BLACK = 0b000,
    RED = 0b001,
    GREEN = 0b010,
    YELLOW = 0b011,
    BLUE = 0b100,
    MAGENTA = 0b101,
    CYAN = 0b110,
    WHITE = 0b111
  };
  // GRAY
  // ,BRIGHT_RED,BRIGHT_GREEN,BRIGHT_YELLOW,BRIGHT_BLUE,BRIGHT_MAGENTA,BRIGHT_CYAN,BRIGHT_WHITE};
  char c;
  int bg;
  int fg;
  colour_char() = default;
  colour_char(char ch, COLOUR fg_ = WHITE)
      : c(ch), bg(COLOUR::BLACK), fg(fg_) {}
  colour_char &operator=(char ch) {
    c = ch;
    return *this;
  }
  operator char() const { return c; }
};

struct slice_range {
  int start;
  int end;
  int step = 1;
  slice_range &scope(int const size) {
    if (start < 0)
      start += size;
    if (end < 0)
      end += size;
    return *this;
  }
};

class colour_string {
  /// The underlying container.
  std::basic_string<colour_char> s;
  /// A normal string for speed.
  std::string _str;
  colour_string(std::basic_string<colour_char> cs, std::string str)
      : s(cs), _str(str) {}

public:
  colour_string() = default;
  colour_string(std::string str, std::optional<colour_char::COLOUR> fg)
      : _str(str) {
    s.reserve(str.size());
    if (fg)
      for (auto const &c : str) {
        s.push_back(c);
        s.rbegin()->fg = *fg;
      }
    else
      for (auto const &c : str)
        s.push_back(c);
  }
  colour_string(std::string str) : _str(str) {
    s.reserve(str.size());
    for (auto const &c : str)
      s.push_back(c);
  }
  auto size() const { return _str.size(); }
  auto c_str() const { return _str.c_str(); }
  colour_string operator+=(colour_string const &rhs) {
    s += rhs.s;
    _str += rhs._str;
    return *this;
  }
  colour_string operator+(colour_string const &rhs) const {
    return {s + rhs.s, _str + rhs._str};
  }
  colour_string &operator+=(colour_char const &rhs) {
    s += rhs;
    _str += rhs;
    return *this;
  }

  // This is a bug in clang, making this cost prevents accidental
  // assigning to it.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-qualifiers"
  [[nodiscard]] auto const operator[](int pos) {
#pragma clang diagnostic pop
    // Enable python style reverse indexing.
    if (pos < 0)
      pos += s.size();
    // TODO: Const until can write to both containers.
    return s[pos];
  }
  [[nodiscard]] colour_string const operator[](slice_range range) {
    // TODO: return type const so that it can later hand back a
    // reference to a mutable sub-string.
    range.scope(_str.size());
    // TODO: range.step
    return {s.substr(range.start, range.end),
            _str.substr(range.start, range.end)};
  }
  colour_string &set(int pos, colour_char val) {
    s[pos] = val;
    _str[pos] = val;
    return *this;
  }
  auto pop_back() {
    _str.pop_back();
    return s.pop_back();
  }
  inline constexpr void resize(size_t count, colour_char ch = ' ') {
    s.resize(count, ch);
    _str.resize(count, ch);
  }
  void colour(slice_range range, colour_char::COLOUR fg,
              std::optional<colour_char::COLOUR> bg = {}) {
    // TODO: range.step
    range.scope(_str.size());
    for (int p = range.start; p <= range.end; p++) {
      s[p].fg = fg;
      if (bg)
        s[p].bg = *bg;
    }
  }
};
} // namespace udh
#endif
