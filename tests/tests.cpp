#include <iostream>
#include <cstdio>
#include <string>

#include <unistd.h>
#include <sys/ioctl.h>

#include "../inc/ptree/PTREEColor.hpp"
#include "../vendor/argx/inc/Argx.hpp"

#ifdef __PTREE_DEBUG
# 	undef __PTREE_DEBUG
#endif

#define __PTREE_DEBUG(_x) 	std::cout << "[ TESTS-DEBUG ] " << (_x) << "\n"

// Error codes:
// Success: 					0
// Unknown error:  				1
// Unsupported OS: 				2
// Unsupported colors: 			3
// Not a valid lib version:  	4
// Other ARGX error:        	5

namespace ptree::tests
{
	/// COLORS
	int colorTestsSupportsColor()
	{
		if (!isatty(STDOUT_FILENO)) return false;
		if (!std::getenv("TERM")) return false;

		__PTREE_DEBUG("colorTestsSupportsColor() succeeded in isatty and TERM test, making the last test");

		// Check if ANSI color support
		return std::string(std::getenv("TERM")) != "dumb";
	}

	int colorTests()
	{
#if defined(__PTREE_WINDOWS)
		__PTREE_DEBUG("Windows OS is not supported")
			return 2;
#endif

		if (!ptree::tests::colorTestsSupportsColor())
		{  __PTREE_DEBUG("No color support for the current terminal"); return 3; }

		__PTREE_DEBUG("Succeeded in tests");

		ptree::color::rgbSet(210, 90, 30); std::cout << "This is an orange-like text\n";
		ptree::color::reset(); std::cout << "This is some normal text\n";
		ptree::color::rgbBgSet(6, 56, 222); std::cout << "This is a blue background\n";
		ptree::color::bold();
		ptree::color::rgbSet(2, 200, 30); std::cout << "This is an green-like text with some background and **boldness**\n";
		ptree::color::reset();

		return 0;
	}

	/// CLI ARGUMENTS
	int argTests()
	{
		int testNum = 0;
		int testCount = 4;

		__PTREE_DEBUG("Testing ARGX");

		testNum++;

		if (ARGX_VERSION_MAJOR != 1 && ARGX_VERSION_MINOR < 2)
		{
			__PTREE_DEBUG("Unsupported ARGX version, required is: 1.2.y");
			__PTREE_DEBUG("[ TEST:  " + std::to_string(testNum) + "/" + std::to_string(testCount) + " ]");

			return 4; // Not a valid lib version
		}

		__PTREE_DEBUG("[ PASSED:  " + std::to_string(testNum) + "/" + std::to_string(testCount) + " ]");

		// CLI fake-params
		std::vector<std::string> fakeArgv = {
			"--test",
			"-t",
			"-t", "sub",
		};

		std::vector<char*> argv;

		for (const auto &x : fakeArgv)
		{ argv.emplace_back((char*)(x.c_str())); }

		int argc = argv.size();

		argx::Argx mainArgx("main-argx", argc, argv.data());

		{
			argx::ARGXOptions testOption;
			argx::ARGXOptions testSubOption;

			testSubOption.id = "sub1";
			testSubOption.info = "Sub param 1 - testing";
			testSubOption.param = "sub";
			testSubOption.sparam = "s";
			testSubOption.hasAnySubParams = false;
			testSubOption.hasSubParams = true;

			testOption.id = "test";
			testOption.param = "--test";
			testOption.sparam = "-t";
			testOption.info = "Testing param";
			testOption.hasAnySubParams = false;
			testOption.hasSubParams = true;
			testOption.subParams.emplace_back(testSubOption);

			mainArgx.add(testOption);
		}

		std::string docs = mainArgx.createDocs(argx::ARGXStyle::Simple, "PTREE ARGX Testing", "Testing for PTREE - ARGX");

		if (mainArgx.getArgc() <= 1) std::cout << docs << "\n";

		if (mainArgx.getParam("test").exists)
		{
			testNum++;

			__PTREE_DEBUG("[ PASSED:  " + std::to_string(testNum) + "/" + std::to_string(testCount) + "]");

			if (mainArgx.getSubParam(mainArgx.getParam("test"), "sub1"))
			{
				testNum++;

				__PTREE_DEBUG("[ PASSED:  " + std::to_string(testNum) + "/" + std::to_string(testCount) + " ]");
			}

			else
			{
				__PTREE_DEBUG("[ FAILED ] Could not get sub-param (ID: sub1)");
				__PTREE_DEBUG("[ TEST:  " + std::to_string(testNum) + "/" + std::to_string(testCount) + " ]");

				return 5; // Other ARGX error
			}
		}

		testNum++;
		std::cout << docs << "\n";

		__PTREE_DEBUG("[ FINISHED TESTS ]");
		__PTREE_DEBUG("[ TEST:  " + std::to_string(testNum) + "/" + std::to_string(testCount) + " ]");
		__PTREE_DEBUG("[ INFO ] Argx version");
		__PTREE_DEBUG(std::to_string(ARGX_VERSION_MAJOR) + "." + std::to_string(ARGX_VERSION_MINOR) + "." + std::to_string(ARGX_VERSION_PATCH));
		__PTREE_DEBUG(ARGX_VERSION_STD);

		return 0;
	}
}

#undef  __PTREE_DEBUG

