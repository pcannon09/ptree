#pragma once

#include "PTREEpredefines.hpp"

#if __cplusplus >= PTREE_DEFAULT_CPP_STD

#include <string>
#include <vector>
#include <utility>

namespace ptree
{
	typedef struct PTREE_Style 
	{
		std::string bottomLeft = "╯";
		std::string bottomRight = "╰";
		std::string topLeft = "╮";
		std::string topRight = "╭";

		std::string center = "│";
		std::string line = "─";

		/*
		 * Style examples
		 * Can be used with any string
		   ┏ ┐
		   └ ┛
		   ╔ ╗
		   ╚ ╝

		   ┌ ────── ┐ 
		   └ ────── ┘ 
		   ╭ ╮
		   ╰ ╯

		   │
		 */
	} PTREE_Style;

	typedef struct PTREE_Flags
	{
		bool directOutput = true;
		bool colorOutput = true;
		bool showHidden = false;
		bool showFileType = false;
		bool showFullPath = false;

		PTREE_Style style;
	} PTREE_Flags;

	typedef struct PTREE_Info
	{
		int totalDirs;
		int totalFiles;
		int totalAll;
	} PTREE_Info;

	class PTREE
	{
	private:
		std::string id;
		std::string defaultDir;

		std::string treeOutput;

		PTREE_Flags flags;
		PTREE_Info info;

	protected:
		std::string __parseColor(const std::string &_tree);

	public:
		PTREE(const std::string &_id, const PTREE_Flags &_flags);
		~PTREE();

		void setDir(const std::string &_dir);

		std::pair<std::vector<std::pair<std::string, unsigned int>>, std::string> scan(const std::string &_path);

		std::string tree();
		std::string directTree(std::string _path, unsigned int _depth);

		std::string parseColor(const std::string &_tree);

		std::string getDefaultDir() const;
		std::string getID() const;
	};
}

#else
# 	error "Minimum C++ standard required"
# 	pragma message("Standard required is C++" PTREE_TOSTRING(PTREE_DEFAULT_CPP_STD) ", current standard is " PTREE_TOSTRING(__cplusplus))
#endif // __cplusplus

