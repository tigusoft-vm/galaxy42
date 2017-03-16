// Copyrighted (C) 2015-2016 Antinet.org team, see file LICENCE-by-Antinet.txt

#pragma once
#ifndef C_TNETDBG_HPP
#define C_TNETDBG_HPP


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "mo_reader.hpp"

#include <memory> // for UsePtr

extern unsigned char g_dbg_level;


/// This macros will be moved later to glorious-cpp library or other

const char * dbg__FILE__(const char * name);

void g_dbg_level_set(unsigned char level, std::string why, bool quiet=false);

#define _my__FILE__ (dbg__FILE__(__FILE__))

#define SHOW_DEBUG
#ifdef SHOW_DEBUG


extern const bool g_is_windows_console;

void write_to_console(const std::string& obj);

#define DBGLVL(N) if (!(N>=g_dbg_level)) break

#define _main_dbg(X) std::ostringstream oss; oss << X; write_to_console(oss.str())

#define _dbg4(X) do { DBGLVL(  5); _main_dbg("\033[90mdbg4: " << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl);} while(0)
#define _dbg3(X) do { DBGLVL( 10); _main_dbg("\033[37mdbg3: " << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl);} while(0)
#define _dbg2(X) do { DBGLVL( 20); _main_dbg("\033[37mdbg2: " << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl);} while(0)
#define _dbg1(X) do { DBGLVL( 30); _main_dbg("\033[97mdbg1: " << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl);} while(0)
#define _info(X) do { DBGLVL( 40); _main_dbg("\033[94minfo: " << _my__FILE__ << ':' << __LINE__ << " " << X  << "\033[0m" << ::std::endl);} while(0)
#define _note(X) do { DBGLVL( 50); _main_dbg("\033[36mnote: " << _my__FILE__ << ':' << __LINE__ << " " << X  << "\033[0m" << ::std::endl);} while(0)
#define _clue(X) do { DBGLVL( 50); _main_dbg("\n\033[96mclue: " << _my__FILE__ << ':' << __LINE__ << " " << X  << "\033[0m" << ::std::endl);} while(0)

#define _fact_level(LVL_MAIN, LVL_EXTRA, X) do { DBGLVL(LVL_MAIN); \
	std::ostringstream oss; \
	oss<<"\033[92m"; \
	oss<< X; \
	do { DBGLVL(LVL_EXTRA); oss << " (msg from " << _my__FILE__ << ':' << __LINE__ << ")"; } while(0); \
	oss << "\033[0m" << ::std::endl; \
	write_to_console(oss.str()); } while(0)
#define _stat(X) _fact_level( 90, 30, X)
#define _fact(X) _fact_level(100, 30, X)
#define _goal(X) _fact_level(150, 30, X)
/// yellow code
//        ::std::cerr<<"Warn! " << _my__FILE__ << ':' << __LINE__ << " " << X << "\033[0m" << ::std::endl; 

#define _warn(X) do { DBGLVL(100); \
	std::ostringstream oss; \
	oss<<"\033[93m"; for (int i=0; i<70; ++i) oss<<'!'; oss<<::std::endl; \
	oss<< ( "WARN:" ) << _my__FILE__ << ':' << __LINE__ << " " << X << "\033[0m" << ::std::endl; \
	write_to_console(oss.str());\
} while(0)
/// red code
//        ::std::cerr<<"ERROR! " << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl; 

#define _erro(X) do { DBGLVL(200); \
	std::ostringstream oss; \
	oss<<"\033[91m\n"; for (int i=0; i<70; ++i) oss<<'!'; oss<<::std::endl; \
	oss<< ("ERROR:") << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl; \
	oss<<"\n\n"; for (int i=0; i<70; ++i) oss<<'!'; oss<<"\033[0m"<<::std::endl; \
	write_to_console(oss.str());\
} while(0)
#define _mark(X) do { DBGLVL(150); \
	std::ostringstream oss; \
	oss<<"\033[95m\n"; for (int i=0; i<70; ++i) oss<<'='; oss<<::std::endl; \
	oss<<"MARK* " << _my__FILE__ << ':' << __LINE__ << " " << X << ::std::endl; \
	for (int i=0; i<70; ++i) oss<<'='; oss<<"\033[0m"<<::std::endl; \
	write_to_console(oss.str());\
	} while(0)

#else

#define _dbg4(X) do {} while(0)
#define _dbg3(X) do {} while(0)
#define _dbg2(X) do {} while(0)
#define _dbg1(X) do {} while(0)
#define _info(X) do {} while(0)
#define _note(X) do {} while(0)
#define _clue(X) do {} while(0)
#define _fact(X) do {} while(0)
#define _goal(X) do {} while(0)
#define _warn(X) do {} while(0)
#define _erro(X) do {} while(0)
#define _mark(X) do {} while(0)

#endif

template<class T> T& _UsePtr(const std::shared_ptr<T> & ptr, int line, const char* file) {
	if (!ptr) { _erro("Failed pointer, for " << file << ":" << line);
		std::abort();
	}
	return *ptr;
}

template<class T> T& _UsePtr(const std::unique_ptr<T> & ptr, int line, const char* file) {
	if (!ptr) { _erro("Failed pointer, for " << file << ":" << line);
		std::abort();
	}
	return *ptr;
}

#define UsePtr(PTR) _UsePtr(PTR,__LINE__,__FILE__)

// TODO this is not really "debug", move to other file
#define _UNUSED(x) (void)(x)

#define _NOTREADY() do { _erro("This code is not implemented yet! in "<<__FUNCTION__);\
	throw std::runtime_error("Code not implemented yet! In: " + std::string(__FUNCTION__)); } while(0)

#define _NOTREADY_warn() do { _warn("This code is not implemented yet! in "<<__FUNCTION__);\
	} while(0)


// this assert could be helpful, maybe use in release
#define _check_abort(X) do { if (!(X)) { \
	_erro("Assertation failed, will abort: (" << #X << ")" << _my__FILE__ << ':' << __LINE__); \
	::std::abort(); } \
} while(0)

// this assert could be helpful, maybe use in release
#define _assert(X) do { if (!(X)) { _erro("Assertation failed (_assert) at " << _my__FILE__ << ':' << __LINE__); ::std::abort(); }  } while(0)

//        _warn("Going to throw exception. What: " << except_var.what()
// this one is unused; leaving for translators if used again later.
#define _unused_throw_error_msg \
	_warn( mo_file_reader::gettext("L_what_exception_program_throw") << ": " << except_var.what() \

// TODO-r-deprecate: ?
#define _throw_error_detail( EXCEPT , MSG ) do { auto except_var = EXCEPT;  \
	_warn( "Except: " << except_var.what() \
		<< "; Details:" << MSG); \
		throw except_var; } while(0)

#define _throw_error( EXCEPT ) do { auto except_var = EXCEPT;  \
	_warn( "Except: " << except_var.what() \
		<< "."); \
		throw except_var; } while(0)

#define _throw_error_runtime( MSG ) _throw_error( std::runtime_error( MSG ) )

namespace ui { class exception_error_exit; }

void must_be_exception_type_error_exit(const ui::exception_error_exit &x);

#define _throw_exit( EXCEPT ) do { auto except_var = EXCEPT;  \
	must_be_exception_type_error_exit(except_var); /* assert (in compile time) that EXCEPT is of proper exception type */ \
	_warn("Going to throw exception (for EXIT) What: " << except_var.what() \
		<< "."); \
		throw except_var; } while(0)

#define _throw_error_rethrow( ) do { \
	_warn("re-throw"); \
		throw ; } while(0)


// TODO-r-deprecate:
#define _assert_throw(COND) do { \
	if (!(COND)) { \
		std::ostringstream oss; \
		oss << "Assert-throw failed: " << "" # COND ; \
		_throw_error( std::runtime_error(oss.str()) ); \
		}\
	} while(0)



#define UI_EXECUTE_OR_EXIT( FUNC ) \
				try { \
					try { \
			FUNC(); \
					} catch UI_CATCH_RETHROW( "" # FUNC ); \
				} catch(...) { \
					std::cout << "Error occured (see above) - we will exit now" << std::endl; \
					_throw_exit( ui::exception_error_exit("Error in: " # FUNC ) ); \
				}




/** TODO document
throw std::logic_error("foo");
throw _except2( std::logic_error("foo") , "x=" << x << "y=" << y ) ;
throw _except( std::logic_error("foo")) ;
*/

std::string debug_this();

#define _dbg4n(X) _dbg4(debug_this() << X)
#define _dbg3n(X) _dbg3(debug_this() << X)
#define _dbg2n(X) _dbg2(debug_this() << X)
#define _dbg1n(X) _dbg1(debug_this() << X)
#define _infon(X) _info(debug_this() << X)
#define _noten(X) _note(debug_this() << X)
#define _cluen(X) _clue(debug_this() << X)
#define _factn(X) _fact(debug_this() << X)
#define _warnn(X) _warn(debug_this() << X)
#define _erron(X) _erro(debug_this() << X)
#define _markn(X) _mark(debug_this() << X)

std::string to_string(const std::wstring &input); // TODO

#endif // include guard
