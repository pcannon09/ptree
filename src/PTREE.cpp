#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include <iostream>

#include "../inc/ptree/PTREE.hpp"
#include "../inc/ptree/PTREEColor.hpp"

namespace fs = std::filesystem;

namespace ptree
{
	// PROTECTED //
	std::string PTREE::__parseColor(const std::string &_tree)
	{
		std::string total;
		std::string str;
		std::stringstream ss{_tree};

		std::string dirIndicator = "[DIR]";
		std::string fileIndicator = "[FILE]";

		while (std::getline(ss, str))
		{
			size_t dirIndicatorPos = str.find(dirIndicator);
			size_t fileIndicatorPos = str.find(fileIndicator);

			bool dirOK = false;
			bool fileOK = false;

			// Find for `[DIR]` (directory) indicator
			if (dirIndicatorPos != std::string::npos)
				dirOK = true;

			else if (fileIndicatorPos != std::string::npos)
				fileOK = true;

			if (dirOK)
			{
				// rgb(50, 100, 180)
				std::string rgbColor = ptree::color::rgbGet(50, 100, 180);
				std::string before = str.substr(0, dirIndicatorPos);
        		std::string after = str.substr(dirIndicatorPos + dirIndicator.size());

				if (!this->flags.showFileType)
					total += before + rgbColor + after + ptree::color::getReset() + "\n";

				else total += before + rgbColor + "[DIR] " + after + ptree::color::getReset() + "\n";
			}

			else if (fileOK)
			{
 				// rgb(70, 185, 0)
				std::string rgbColor = ptree::color::rgbGet(70, 185, 0);
				std::string before = str.substr(0, fileIndicatorPos);
        		std::string after = str.substr(fileIndicatorPos + fileIndicator.size());

				if (!this->flags.showFileType)
					total += ptree::color::getBold() + before + rgbColor + after + ptree::color::getReset() + "\n";

				else total += ptree::color::getBold() + before + rgbColor + "[FILE] " + after + ptree::color::getReset() + "\n";
			}

			ptree::color::reset();
		}

		return total;
	}

	// PUBLIC //
	PTREE::PTREE(const std::string &_id, const PTREE_Flags &_flags)
		: id(_id), flags(_flags)
	{ }

	PTREE::~PTREE()
	{ }

	void PTREE::setDir(const std::string &_dir)
	{ this->defaultDir = _dir; }

	// 								name 		   deepness  	  total-str
	std::pair<std::vector<std::pair<std::string, unsigned int>>, std::string> PTREE::scan(const std::string &_path)
	{
    	static fs::path root = fs::canonical(_path);
    	static std::vector<bool> lastFlags; // shared with directTree

		static std::string totalFiles;

		std::string finalEntry;

    	std::vector<std::pair<std::string, unsigned int>> result;
    	std::vector<fs::directory_entry> entries;

		try
		{
    		for (const auto &entry : fs::directory_iterator(_path))
        		entries.push_back(entry);
        }

        catch (fs::filesystem_error &e)
        {
			std::cout << e.what() << "\n";
        }

    	for (size_t i = 0 ; i < entries.size() ; i++)
    	{
        	const fs::directory_entry &entry = entries[i];

        	const bool isLast = (i == entries.size() - 1);

			unsigned int depth = 0;

        	try
        	{
            	fs::path relative = fs::relative(entry.path(), root);

            	depth = std::distance(relative.begin(), relative.end());

            	if (lastFlags.size() < depth)
                	lastFlags.resize(depth, false);

            	lastFlags[depth - 1] = isLast;

				// Check if hidden
				// If the `if` statement returns `true`; continue as hidden path
				if (entry.path().filename().string().front() == '.' && !this->flags.showHidden)
					continue;

				finalEntry = (this->flags.showFullPath ? entry.path().string() : entry.path().filename().string());

				std::string resultInput = (entry.is_directory() ? "[DIR]" : "[FILE]") + finalEntry;

            	result.push_back({ resultInput, depth });

				totalFiles += this->__parseColor(this->directTree(resultInput, depth));

				// If directOutput is set
				// Output the result from `i` index
            	if (this->flags.directOutput) std::cout << this->__parseColor(this->directTree(resultInput, depth));;

            	if (fs::is_directory(entry.path()))
            	{
                	std::vector<std::pair<std::string, unsigned int>> subResult = scan(entry.path().string()).first;

                	result.insert(result.end(), subResult.begin(), subResult.end());
            	}
        	}

        	catch (const fs::filesystem_error &e)
        	{
            	result.push_back({ e.what(), depth });
        	}
        }

    	return { result, totalFiles };
	}

	std::string PTREE::directTree(std::string _path, unsigned int _depth)
	{
    	static std::vector<bool> lastFlags; // track last entries

    	std::string prefix;

    	for (unsigned int i = 0 ; i < _depth ; i++)
    	{
        	if (i == _depth - 1)
        	{
        		// Show path separators
            	// EXAMPLE: └── OR ├──
            	if (lastFlags.size() > i && lastFlags[i])
                	prefix += this->flags.style.bottomRight + this->flags.style.line + this->flags.style.line + " ";

            	else prefix += this->flags.style.center + this->flags.style.line + this->flags.style.line + " ";
        	}
        	
        	else
        	{
            	if (lastFlags.size() > i && lastFlags[i]) prefix += "    ";
            	else prefix += this->flags.style.center + "   ";
        	}
    	}

    	return prefix + _path + "\n";
	}

	std::string PTREE::tree()
	{
		// Show which directory is being showed
		if (this->flags.directOutput)
			std::cout << this->defaultDir << "\n";

		std::string scanned = this->scan(this->defaultDir).second;

		// If NOT directOutput (`false`), output everything at the end of the scan
		if (!this->flags.directOutput)
			std::cout << this->defaultDir << "\n" << scanned << "\n";

		return "";
	}

	std::string PTREE::parseColor(const std::string &_tree)
	{
		if (_tree.empty()) return "";

		return this->__parseColor(_tree);
	}

	std::string PTREE::getDefaultDir() const
	{ return this->defaultDir; }

	std::string PTREE::getID() const
	{ return this->id; }
}

