#pragma once

#include "PTREEpredefines.hpp"

#if __cplusplus >= PTREE_DEFAULT_CPP_STD

#include <string>

namespace ptree
{
	namespace color
	{
#if !PTREE_OS_WIN32
		void rgbSet(unsigned int r, unsigned int g, unsigned int b);
		void rgbBgSet(unsigned int r, unsigned int g, unsigned int b);
		void set(int _color);
		void reset();
		void bold();

		std::string rgbGet(unsigned int r, unsigned int g, unsigned int b);
		std::string getReset();
		std::string getBold();

#else
# 		define __PTREE_WINDOWS
# 		warning "Windows OS is not supported"
#endif // !PTREE_OS_WIN32
	}
}

#else
# 	error "Minimum C++ standard required"
# 	pragma message("Standard required is C++" PTREE_TOSTRING(PTREE_DEFAULT_CPP_STD) ", current standard is " PTREE_TOSTRING(__cplusplus))
#endif // __cplusplus

