#include <iostream>
#include <algorithm>
#include <filesystem>

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

namespace ptree::priv
{
	const std::string &licenseString = R"(
PTree Copyright (C) 2025  pcannon09
This program comes with ABSOLUTELY NO WARRANTY; for details type `ptree --help'.
This is free software, and you are welcome to redistribute it
under certain conditions; type `ptree --help' for details.)";

	Bool toBool(const std::string &_str)
	{
		if (_str[0] == 't' || _str[0] == 'T' || _str[0] == '1') return True;
		else if (_str[0] == 'f' || _str[0] == 'F' || _str[0] == '0') return False;

		return None;
	}

	int toNum(const std::string &_str)
	{
		if (std::all_of(_str.begin(), _str.end(), ::isdigit))
			return std::stoi(_str);

		// FAIL
		return 0;
	}

	namespace param
	{
		void setOptionValue(argx::Argx *_mainArgx, const std::string &_id, int *_value)
		{
			size_t pos = _mainArgx->getArgIDPos(_id);

			int cInt = ptree::priv::toNum(_mainArgx->getOption(_id).defaultValue);

			if (_mainArgx->getMainArgs().size() > pos + 1)
				cInt = ptree::priv::toNum(_mainArgx->getMainArgs()[pos + 1]);

			*_value = cInt;
		}

		void setOptionValue(argx::Argx *_mainArgx, const std::string &_id, bool *_value)
		{
			size_t pos = _mainArgx->getArgIDPos(_id);

			Bool cBool = ptree::priv::toBool(_mainArgx->getOption(_id).defaultValue);

			if (_mainArgx->getMainArgs().size() > pos + 1)
				cBool = ptree::priv::toBool(_mainArgx->getMainArgs()[ pos + 1 ]);

			*_value = cBool;
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
		showHiddenOption.defaultValue = "false";
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
		sizeModeOption.hasAnySubParams = true;
		sizeModeOption.hasSubParams = false;

		mainArgx.add(helpOption);

		mainArgx.add(directOutputOption);
		mainArgx.add(showFileTypeOption);
		mainArgx.add(showFullPathOption);
		mainArgx.add(colorOutputOption);
		mainArgx.add(showHiddenOption);
		mainArgx.add(showFileSizeOption);
		mainArgx.add(forceSizeOption);
		mainArgx.add(sizeModeOption);
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
	tree.setDir(".");

	bool setDefaultDir = false;

	// If argument not registered, check if the dir exists and set it if so
	if (!mainArgx.compareArgs(mainArgx.getOptions(), mainArgx.getMainArgs()))
	{
		size_t toFind = mainArgx.getMainArgs().size() - 1;

		if (fs::exists(mainArgx.getMainArgs()[toFind]))
			tree.setDir(mainArgx.getMainArgs()[toFind]);

		else setDefaultDir = true;
	}

	// Argument not registered, failed to set the dir
	if (setDefaultDir)
	{
		int wrongArgPos = mainArgx.getWrongArgs(mainArgx.getMainArgs());
		
		std::cerr << "[ PTREE ] Unknown option " << mainArgx.getMainArgs()[argx::Argx::formatWrongArgs(wrongArgPos)] << "\n";

		return 1;
	}

	return tree.tree();
}

