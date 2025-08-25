#include <filesystem>
#include <string>
#include <vector>
#include <utility>

#include <iostream>

#include "../inc/ptree/PTREE.hpp"

namespace fs = std::filesystem;

namespace ptree
{
	PTREE::PTREE(const std::string &_id, const PTREE_Flags &_flags)
		: id(_id), flags(_flags)
	{ }

	PTREE::~PTREE()
	{ }

	void PTREE::setDir(const std::string &_dir)
	{ this->defaultDir = _dir; }

	// 						name 		   deepness
	std::vector<std::pair<std::string, unsigned int>> PTREE::scan(const std::string &_path)
	{
    	static fs::path root = fs::canonical(_path);
    	static std::vector<bool> lastFlags; // shared with directTree

    	std::vector<std::pair<std::string, unsigned int>> result;
    	std::vector<fs::directory_entry> entries;

    	for (const auto &entry : fs::directory_iterator(_path))
        	entries.push_back(entry);

    	for (size_t i = 0; i < entries.size(); i++)
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

            	result.push_back({ entry.path().string(), depth });

            	if (this->flags.directOutput)
                	std::cout << this->directTree(entry.path().filename().string(), depth);

            	if (fs::is_directory(entry.path()))
            	{
                	std::vector<std::pair<std::string, unsigned int>> subResult = scan(entry.path().string());

                	result.insert(result.end(), subResult.begin(), subResult.end());
            	}
        	}

        	catch (const fs::filesystem_error &e)
        	{
            	result.push_back({ e.what(), depth });
        	}
    	}

    	return result;
	}

	std::string PTREE::directTree(std::string _path, unsigned int _depth)
	{
    	static std::vector<bool> lastFlags; // track last entries

    	std::string prefix;

    	for (unsigned int i = 0; i < _depth; i++)
    	{
        	if (i == _depth - 1)
        	{
            	// EXAMPLE: └── OR ├──
            	if (lastFlags.size() > i && lastFlags[i])
                	prefix += this->flags.style.bottomRight + this->flags.style.line + this->flags.style.line + " ";

            	else
                	prefix += this->flags.style.center + this->flags.style.line + this->flags.style.line + " ";
        	}
        	
        	else
        	{
            	if (lastFlags.size() > i && lastFlags[i])
                	prefix += "    ";

            	else
                	prefix += this->flags.style.center + "   ";
        	}
    	}

    	return prefix + _path + "\n";
	}

	std::string PTREE::tree()
	{
		// Show which directory is being showed
		if (this->flags.directOutput)
			std::cout << this->defaultDir << "\n";

		this->scan(this->defaultDir);

		return "";
	}

	std::string PTREE::getDefaultDir() const
	{ return this->defaultDir; }

	std::string PTREE::getID() const
	{ return this->id; }
}

