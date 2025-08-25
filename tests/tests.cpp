#include <iostream>
#include "../inc/ptree/PTREEColor.hpp"

// Error codes:
// Success: 			0
// Unknown error:  		1
// Unsupported OS: 		2

namespace ptree::tests
{
	int colorTests()
	{
#if defined(__PTREE_WINDOWS)
		return 2;
#endif

		ptree::color::rgbSet(210, 90, 30); std::cout << "This is an orange-like text\n";
		ptree::color::reset(); std::cout << "This is some normal text\n";
		ptree::color::rgbBgSet(6, 56, 222); std::cout << "This is a blue background\n";
		ptree::color::rgbSet(2, 200, 30); std::cout << "This is an green-like text with some background\n";
		ptree::color::reset();

		return 0;
	}
}

