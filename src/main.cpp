#include <iostream>
#include "../inc/ptree/PTREE.hpp"

#undef __PTREE_MAKE_TESTS // TESTING

#if defined(__PTREE_MAKE_TESTS)
# 	include "../tests/tests.hpp"
#endif

int main()
{
#if defined(__PTREE_MAKE_TESTS)
	int ret = ptree::tests::colorTests();

	if (ret != 0)
	{
		std::cout << "[ FATAL ] FAILED TO TEST COLOR\n";

		return ret;
	}

	return 0;
#endif

	ptree::PTREE_Flags treeFlags;
	treeFlags.directOutput = true;
	treeFlags.showFileType = false;
	treeFlags.directOutput = true;
	treeFlags.showFullPath = false;
	treeFlags.showFileInfo = true;
	treeFlags.colorOutput = false;

	ptree::PTREE tree("main", treeFlags);

	tree.setDir("./");
	
	return tree.tree();
}

