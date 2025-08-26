#include <iostream>
#include <string>

#include "../inc/ptree/PTREEColor.hpp"

namespace ptree
{
	namespace color
	{
		void rgbSet(unsigned int r, unsigned int g, unsigned int b)
		{
			std::cout << "\033[38;2;" + std::to_string(r) + ";"
				+ std::to_string(g) + ";"
				+ std::to_string(b) + "m";
		}

		std::string rgbGet(unsigned int r, unsigned int g, unsigned int b)
		{
			return "\033[38;2;" + std::to_string(r) + ";"
				+ std::to_string(g) + ";"
				+ std::to_string(b) + "m";
		}

		void rgbBgSet(unsigned int r, unsigned int g, unsigned int b)
		{
			std::cout << "\033[48;2;" + std::to_string(r) + ";"
				+ std::to_string(g) + ";"
				+ std::to_string(b) + "m";
		}

		void set(int _color)
		{ std::cout << "\033[" << _color << "m"; }

		void reset()
		{ std::cout << "\033[0m"; }

		void bold()
		{ std::cout << "\033[1m"; }
	}
}
