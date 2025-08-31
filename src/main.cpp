#include <iostream>

#include "../inc/ptree/PTREE.hpp"

#undef __PTREE_MAKE_TESTS // TESTING

#if defined(__PTREE_MAKE_TESTS)
# 	include "../tests/tests.hpp"
#endif

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
#ifdef ARGX_AS_PYTHON_PACKAGE
# 	error "Cannot test ARGX library, ARGX is set as a python library; Undefine or do not compile with `ARGX_AS_PYTHON_PACKAGE` macro to make it as a C++ package"
#endif
	ret = ptree::tests::argTests();

	if (ret != 0)
	{
		std::cerr << "[ FATAL ] FAILED TO TEST ARGS (ARGX)\n";

		return ret;
	}

	return 0;
#endif

	ptree::PTREE_Flags treeFlags;
	treeFlags.directOutput = true;
	treeFlags.showFileType = true;
	treeFlags.directOutput = true;
	treeFlags.showFullPath = false;
	treeFlags.showFileInfo = true;
	treeFlags.colorOutput = true;
	treeFlags.showHidden = true;
	treeFlags.showFileSize = true;
	treeFlags.showDirSize = true;

	ptree::PTREE tree("main", treeFlags);

	tree.setDir("./");
	
	return tree.tree();
}

