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

	ptree::PTREE tree("main", treeFlags);

	tree.setDir(".");

	std::cout << tree.tree() << "\n";

	return 0;
}

