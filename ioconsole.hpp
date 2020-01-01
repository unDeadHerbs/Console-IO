#ifndef __IOCONSOLE_HPP__
#define __IOCONSOLE_HPP__

// Following along with c++/ostream.

// I'm leaving majory portions commented out untill i both know what
// they are for and find them useful.

#include <deque>
#include <ios>
#include <memory>
#include <string>
#include <utility>

namespace udh {
template <class _CharT, class _Traits = std::char_traits<_CharT>>
class basic_console {  //: virtual public basic_ios<_CharT,_Traits>{
 public:
	/*
	// Types
	typedef _CharT char_type;
	typedef _Traits traits_type;
	typedef typename traits_type::int_type int_type;
	typedef typename traits_type::pos_type pos_type;
	typedef typename traits_type::off_type off_type;
	*/

	/*
	  explicit basic_console(basic_streambuf<char_type,traits_type>* __sb)
	  { this->init(_sb); }
	*/
	virtual ~basic_console() {}

 protected:
	/*
	  I think both of thses are for c++03

	  Do I need a move constructor?
	  basic_console(basic_console&& __rhs);

	  They have a `27.7.2.3 Assign/swap`?
	*/

	/*
	  void swap(basic_console& __rhs);
	*/

	// Both of these are to be deleted in cio?
	basic_console(const basic_console& __rhs);
	basic_console& operator=(const basic_console& __rhs);

 public:
	/*
	  what this do?
	  class sentry;
	*/

	// some sort of ability to have a printer function passed, and then just call
	// that?
	basic_console& operator<<(basic_console* (*__pf)(basic_console&)) {
		return __pf(*this);
	}
	/*
	  The same thing for the base classes?

	  basic_ostream& operator<<(basic_ios<char_type, traits_type>&(*__pf)(
	  basic_ios<char_type,traits_type>&)){
	  __pf(*this);
	  return *this;
	  }

	  basic_ostream& operator<<(ios_base& (*__pf)(ios_base&)){ __pf(*this); return
	  *this; }
	*/

	// Force implimentation of most basic types.
	// Commente out tempraraly because I want to be lazy.
	/*
	  basic_ostream& operator<<(bool);
	  basic_ostream& operator<<(short);
	  basic_ostream& operator<<(unsigned short);
	  basic_ostream& operator<<(int);
	  basic_ostream& operator<<(unsigned int);
	  basic_ostream& operator<<(long);
	  basic_ostream& operator<<(unsigned long);
	  basic_ostream& operator<<(long long);
	  basic_ostream& operator<<(unsigned long long);
	  basic_ostream& operator<<(float);
	  basic_ostream& operator<<(double);
	  basic_ostream& operator<<(long double);
	  basic_ostream& operator<<(const void*);
	  basic_ostream& operator<<(basic_streambuf<char_type, traits_type>*);
	*/

	/* todo later
	   basic_ostream& put(char_type);
	   basic_ostream& write(const char_type*, streamsize);
	   basic_ostream& flush();
	*/
 protected:
	basic_console() {}  // do nothing as we are for extension
};
typedef basic_console<char> console;

/**
 * A small wrapper for ncurses.
 *
 * This class defines a singelton object for manageing the ncurses
 * library.
 */
class NCursesConsole : console {
 protected:
	/**
	 * This is a pointer to a WINDOW object in ncurses, but the ncurses
	 * libaray makes a bunch of globabl variables so i'm encapsulating
	 * it in udh_frame.cpp where it won't bother anything else.
	 */
	void* win;
	typedef unsigned int uint;
	uint screenRows, screenCols;
	std::deque<std::string> f;

	/**
	 * Anti-Copy Constructor
	 *
	 * Making a private copy constructor to prevent usage.
	 */
	NCursesConsole(NCursesConsole const&) = delete;

	/**
	 * Anti-Asignment Operator
	 *
	 * Making a prevent asignment operator to prevent usage.
	 */
	NCursesConsole operator=(NCursesConsole const&) = delete;
	/**
	 * Anti-Constructor
	 *
	 * Prevent users from constructing this class.
	 */
	NCursesConsole();
	/**
	 * Keep a pointer to the Singleton.
	 */
	std::pair<uint, uint> cursor;

 public:
	~NCursesConsole();
	static NCursesConsole* Get() {
		static std::unique_ptr<NCursesConsole> Instance;
		if (!Instance)
			Instance = std::unique_ptr<NCursesConsole>(new NCursesConsole);
		return Instance.get();
	}

	// TODO: move cursor functions

	/**
	 * Actuly render the chars to the screen.
	 */
	void refreshScreen();

	/**
	 * re-check the size of the screen.
	 *
	 * TODO: set up the handler for this.
	 * TODO: have functions be adddable to this.
	 */
	void screenResizedTriger(int code);

	/**
	 * Read in one char from the user.
	 */
	int getKey() const;

	std::pair<uint, uint> size() const;
	void flush();

	NCursesConsole& operator<<(std::string rhs);
	NCursesConsole& operator<<(int rhs);
	std::string& operator[](uint);
};
static auto& cio = *NCursesConsole::Get();
};  // namespace udh
#endif
