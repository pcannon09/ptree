#include <iostream>
#include <cstdio>

#include <unistd.h>
#include <sys/ioctl.h>

#include "../inc/ptree/PTREEColor.hpp"

#ifdef __DEBUG
# 	undef __DEBUG
#endif

#define __DEBUG(_x) 	std::cout << "[ TESTS-DEBUG ] " << (_x) << "\n";

// Error codes:
// Success: 					0
// Unknown error:  				1
// Unsupported OS: 				2
// Unsupported colors 			3

namespace ptree::tests
{
	int colorTestsSupportsColor()
	{
		if (!isatty(STDOUT_FILENO)) return false;
		if (!std::getenv("TERM")) return false;
		
		__DEBUG("colorTestsSupportsColor() succeeded in isatty and TERM test, making the last test");

		// Check if ANSI color support
		return std::string(std::getenv("TERM")) != "dumb";
	}

	////

	int colorTests()
	{
#if defined(__PTREE_WINDOWS)
		__DEBUG("Windows OS is not supported")
		return 2;
#endif

		if (!ptree::tests::colorTestsSupportsColor())
		{ __DEBUG("No color support for the current terminal"); return 3; }


		__DEBUG("Succeeded in tests");

		ptree::color::rgbSet(210, 90, 30); std::cout << "This is an orange-like text\n";
		ptree::color::reset(); std::cout << "This is some normal text\n";
		ptree::color::rgbBgSet(6, 56, 222); std::cout << "This is a blue background\n";
		ptree::color::bold();
		ptree::color::rgbSet(2, 200, 30); std::cout << "This is an green-like text with some background\n";
		ptree::color::reset();

		return 0;
	}
}

#undef __DEBUG

