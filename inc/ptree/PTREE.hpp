#pragma once

#include "PTREEpredefines.hpp"

#if __cplusplus >= PTREE_DEFAULT_CPP_STD

#include <string>
#include <vector>
#include <utility>
#include <cinttypes>

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
		bool forceSizeMode = false;

		bool showHidden = false;
		bool showFileType = false;
		bool showFullPath = false;
		bool showFiles = false;
		bool showFileSize = true;
		bool showDirSize = showFileSize;

		// 0 = KiB
		// 1 = MiB
		// 2 = GiB
		// 3 = TiB
		unsigned int sizeMode = 1;
	} PTREE_Flags;

	typedef struct PTREE_Info
	{
		unsigned int totalDirs;
		unsigned int totalFiles;
		unsigned int totalAll;
		unsigned int totalSize;
	} PTREE_Info;

	class PTREE
	{
	private:
		std::string id;
		std::string defaultDir;

		// .first = Files, .second = Dirs
		std::pair<std::vector<std::string>, std::vector<std::string>> processedFilesDirs;

		std::string treeOutput;

		PTREE_Flags flags;
		PTREE_Style style;
		PTREE_Info info;

	protected:
		// Wrapper for the following functions:
		std::string __parseColor(const std::string &_tree);
		std::vector<std::string> __outputInfo();

		float __getFileSize(const std::string &_file);
		float __getDirSize(const std::string &_dir);

		// No Wrapper for the following functions:
		std::pair<float, std::string> __convertSize(const float _size);

	public:
		PTREE(const std::string &_id, const PTREE_Flags &_flags);
		~PTREE();

		void setDir(const std::string &_dir);

		std::pair<std::vector<std::pair<std::string, unsigned int>>, std::string> scan(const std::string &_path);

		int tree();

		float getFileSize(const std::string &_file);
		float getDirSize(const std::string &_dir);

		std::vector<std::string> outputInfo();

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

