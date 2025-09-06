#include <iostream>
#include <algorithm>
#include <filesystem>
#include <vector>

#include "../inc/ptree/PTREE.hpp"
#include "../vendor/argx/inc/Argx.hpp"

#undef __PTREE_MAKE_TESTS // TESTING

#ifdef ARGX_AS_PYTHON_PACKAGE
# 	error "Must have support for C++ instead of Python for ARGX"
#endif

#ifdef __PTREE_MAKE_TESTS
# 	include "../tests/tests.hpp"
#endif

namespace fs = std::filesystem;

enum Bool
{
	False = 0,
	True  = 1,
	None  = 2,
};

typedef struct PTREE_OptionInfo
{
	std::string id;
	std::string value;

	bool worked;

	argx::ARGXOptions option;
} PTREE_OptionInfo;

namespace ptree::priv
{
	const std::string &licenseString = R"(
PTree Copyright (C) 2025  pcannon09
This program comes with ABSOLUTELY NO WARRANTY; for details type `ptree --help'.
This is free software, and you are welcome to redistribute it
under certain conditions; type `ptree --help' for details.)";

	std::vector<PTREE_OptionInfo> optionsWorked;

	Bool toBool(const std::string &_str)
	{
		if (_str[0] == 't' || _str[0] == 'T' || _str[0] == '1') return True;
		else if (_str[0] == 'f' || _str[0] == 'F' || _str[0] == '0') return False;

		return None;
	}

	std::pair<int, bool> toNumComplex(const std::string &_str)
	{
		if (std::all_of(_str.begin(), _str.end(), ::isdigit))
			return { std::stoi(_str), true };

		// FAIL
		return { 0, false };
	}

	int toNum(const std::string &_str)
	{ return ptree::priv::toNumComplex(_str).first; }

	namespace param
	{
		void setOptionValue(argx::Argx *_mainArgx, const std::string &_id, int *_value)
		{
			size_t pos = _mainArgx->getArgIDPos(_id);

			std::pair<int, bool> cInt = { 0, false };

			if (_mainArgx->getMainArgs().size() > pos + 1)
				cInt = ptree::priv::toNumComplex(_mainArgx->getMainArgs()[ pos + 1 ]);

			else cInt = ptree::priv::toNumComplex(_mainArgx->getOption(_id).defaultValue);

			ptree::priv::optionsWorked.push_back({_id, std::to_string(cInt.first), cInt.second, _mainArgx->getOption(_id)});

			*_value = cInt.first;
		}

		void setOptionValue(argx::Argx *_mainArgx, const std::string &_id, bool *_value)
		{
			size_t pos = _mainArgx->getArgIDPos(_id);

			Bool cBool = None;

			if (_mainArgx->getMainArgs().size() > pos + 1)
				cBool = ptree::priv::toBool(_mainArgx->getMainArgs()[ pos + 1 ]);

			else cBool = ptree::priv::toBool(_mainArgx->getOption(_id).defaultValue);

			if (cBool != None)
			{
				ptree::priv::optionsWorked.push_back({_id, std::to_string(cBool), true, _mainArgx->getOption(_id)});

				*_value = cBool;

				return;
			}

			ptree::priv::optionsWorked.push_back({_id, std::to_string(false), false, _mainArgx->getOption(_id)});

			*_value = false;
		}
	}
}

int main(int _argc, char **_argv)
{
#if defined(__PTREE_MAKE_TESTS)
	int ret = 0;

	ret = ptree::tests::colorTests();

	if (ret != 0)
	{
		std::cerr << "[ FATAL ] FAILED TO TEST COLOR\n";

		return ret;
	}

	// Check at compile time if the ARGX library is set as a python package
	ret = ptree::tests::argTests();

	if (ret != 0)
	{
		std::cerr << "[ FATAL ] FAILED TO TEST ARGS (ARGX)\n";

		return ret;
	}

	return 0;
#endif

	ptree::PTREE_Flags treeFlags;

	argx::Argx mainArgx("main-argx", _argc, _argv);

	// ARGX: ADD PARAMS
	{
		// General options
		argx::ARGXOptions helpOption;

		helpOption.id = "help";
		helpOption.param = "--help";
		helpOption.sparam = "-h";
		helpOption.info = "Get this help";
		helpOption.hasAnySubParams = false;
		helpOption.hasSubParams = false;

		// Set `PTREE_Flags` option
		argx::ARGXOptions directOutputOption;
		argx::ARGXOptions showFileTypeOption;
		argx::ARGXOptions showFullPathOption;
		argx::ARGXOptions colorOutputOption;
		argx::ARGXOptions showHiddenOption;
		argx::ARGXOptions showFileSizeOption;
		argx::ARGXOptions forceSizeOption;
		argx::ARGXOptions sizeModeOption;
		argx::ARGXOptions showInfoOption;

		directOutputOption.id = "direct-output";
		directOutputOption.param = "--direct-output";
		directOutputOption.sparam = "-do";
		directOutputOption.info = "Set direct output to the console once file or directory is detected";
		directOutputOption.defaultValue = std::to_string(treeFlags.directOutput);
		directOutputOption.hasAnySubParams = true;
		directOutputOption.hasSubParams = false;

		showFileTypeOption.id = "show-file-type";
		showFileTypeOption.param = "--show-file-type";
		showFileTypeOption.sparam = "-sft";
		showFileTypeOption.info = "Show if the processed file or dir is a file or directory";
		showFileTypeOption.defaultValue = std::to_string(treeFlags.showFileType);
		showFileTypeOption.hasAnySubParams = true;
		showFileTypeOption.hasSubParams = false;

		showFullPathOption.id = "show-full-path";
		showFullPathOption.param = "--show-full-path";
		showFullPathOption.sparam = "-sfp";
		showFullPathOption.info = "Show full file path from the root directory (/)";
		showFullPathOption.defaultValue = std::to_string(treeFlags.showFullPath);
		showFullPathOption.hasAnySubParams = true;
		showFullPathOption.hasSubParams = false;

		colorOutputOption.id = "color-output";
		colorOutputOption.param = "--color";
		colorOutputOption.sparam = "-c";
		colorOutputOption.info = "Show color when showing the files and directories";
		colorOutputOption.defaultValue = std::to_string(treeFlags.colorOutput);
		colorOutputOption.hasAnySubParams = true;
		colorOutputOption.hasSubParams = false;

		showHiddenOption.id = "show-hidden";
		showHiddenOption.param = "--show-hidden";
		showHiddenOption.sparam = "-a";
		showHiddenOption.info = "Show hidden files";
		showHiddenOption.defaultValue = std::to_string(treeFlags.showHidden);
		showHiddenOption.hasAnySubParams = true;
		showHiddenOption.hasSubParams = false;

		showFileSizeOption.id = "show-size";
		showFileSizeOption.param = "--show-file-size";
		showFileSizeOption.sparam = "-s";
		showFileSizeOption.info = "Show the file size (DEFAULT in MiB)\nOPTIONS: KiB, MiB, GiB and TiB";
		showFileSizeOption.defaultValue = std::to_string(treeFlags.showFileSize);
		showFileSizeOption.hasAnySubParams = true;
		showFileSizeOption.hasSubParams = false;

		forceSizeOption.id = "force-file-size";
		forceSizeOption.param = "--force-size";
		forceSizeOption.sparam = "-s";
		forceSizeOption.info = "Force the default or previously set size";
		forceSizeOption.defaultValue = std::to_string(treeFlags.forceSizeMode);
		forceSizeOption.hasAnySubParams = true;
		forceSizeOption.hasSubParams = false;

		sizeModeOption.id = "size-mode";
		sizeModeOption.param = "--size-mode";
		sizeModeOption.sparam = "-sm";
		sizeModeOption.info = "Set size mode: 0 = KiB, 1 = MiB, 2 = GiB, 3 = TiB";
		sizeModeOption.defaultValue = std::to_string(treeFlags.sizeMode);
		sizeModeOption.tag = "r03"; // range from 0 to 3
		sizeModeOption.hasAnySubParams = true;
		sizeModeOption.hasSubParams = false;

		showInfoOption.id = "show-info";
		showInfoOption.param = "--info";
		showInfoOption.sparam = "-i";
		showInfoOption.info = "Show information from the received files and some other relevant statistics";
		showInfoOption.defaultValue = std::to_string(treeFlags.showInfo);
		showInfoOption.hasAnySubParams = true;
		showInfoOption.hasSubParams = false;

		mainArgx.add(helpOption);

		mainArgx.add(directOutputOption);
		mainArgx.add(showFileTypeOption);
		mainArgx.add(showFullPathOption);
		mainArgx.add(colorOutputOption);
		mainArgx.add(showHiddenOption);
		mainArgx.add(showFileSizeOption);
		mainArgx.add(forceSizeOption);
		mainArgx.add(sizeModeOption);
		mainArgx.add(showInfoOption);
	}

	std::string mainArgxDocs = mainArgx.createDocs(argx::ARGXStyle::Professional, "PTREE Help", "Command Line Usage:\n");

	// HELP:
	if (mainArgx.getParam("help").exists)
	{
		std::cout << mainArgxDocs << "\n" << ptree::priv::licenseString << "\n";

		return 0;
	}

	// BOOL
	if (const std::string &ID = "direct-output" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.showFileType);

	if (const std::string &ID = "show-file-type" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.showFileType);

	if (const std::string &ID = "show-full-path" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.showFullPath);

	if (const std::string &ID = "color-output" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.colorOutput);

	if (const std::string &ID = "show-hidden" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.showHidden);

	if (const std::string &ID = "show-size" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.showFileSize);

	if (const std::string &ID = "force-file-size" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.forceSizeMode);

	if (const std::string &ID = "show-info" ;
			mainArgx.getParam(ID).exists)
		ptree::priv::param::setOptionValue(&mainArgx, ID, &treeFlags.showInfo);

	// INT
	if (const std::string &ID = "size-mode" ;
			mainArgx.getParam(ID).exists)
	{
		int tmpSizeMode = treeFlags.sizeMode;

		ptree::priv::param::setOptionValue(&mainArgx, ID, &tmpSizeMode);

		treeFlags.sizeMode = tmpSizeMode;
	}
	
	// Construct `tree` variable after flags
	// The flags use `treeFlags` variable and must be constructed after it to be used
	ptree::PTREE tree("main", treeFlags);

	bool isDefaultDirFound = false;

	std::string foundDir = ".";

	// If argument not registered, check if the dir exists and set it if so
	if (!mainArgx.compareArgs(mainArgx.getOptions(), mainArgx.getMainArgs()))
	{
		for (size_t i = 1 ; i < mainArgx.getMainArgs().size() ; ++i)
		{
			const std::string p = mainArgx.getMainArgs()[i];

			if (fs::exists(p))
			{
				if (isDefaultDirFound) continue;

				std::cout << p << "\n";

				isDefaultDirFound = true;
				foundDir = p;

				continue;
			}

			// Arg not found
			else
			{
				if (!fs::exists(p)) continue;

				bool checkErrors = true;

				// Check for wrong **SUB-params**
				for (const auto &x : ptree::priv::optionsWorked)
				{
					if (x.option.tag == "r03")
					{
						int intVal = ptree::priv::toNum(x.value);

						if (!(intVal >= 0 && intVal <= 3))
						{
							std::cerr << "[ PTREE ] Not a valid range for " << x.id << ". Make sure to have a value between 0 and 3\n";

							return 2;
						}

						checkErrors = false;
					}

					if (!x.worked)
					{
						int wrongArgPos = mainArgx.getWrongArgs(mainArgx.getMainArgs());

						std::cerr << "[ PTREE ] Unknown sub-option " << mainArgx.getMainArgs()[argx::Argx::formatWrongArgs(wrongArgPos)] << "\n";

						return 1;
					}
				}

				// As sub-options were checked, now check for the **NON-sub** params

				if (!mainArgx.compareArgs(mainArgx.getOptions(), mainArgx.getMainArgs()) && checkErrors)
				{
					int wrongArgPos = mainArgx.getWrongArgs(mainArgx.getMainArgs());

					std::cerr << "[ PTREE ] Unknown option " << mainArgx.getMainArgs()[argx::Argx::formatWrongArgs(wrongArgPos)] << "\n";
				}

				return 1;
			}
		}
	}

	tree.setDir(foundDir);

	return tree.tree();
}

