#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include <iostream>

#include "../inc/ptree/PTREE.hpp"
#include "../inc/ptree/PTREEColor.hpp"

namespace fs = std::filesystem;

#ifdef __PTREE_GET_OUTPUT_INFO
# 	undef __PTREE_GET_OUTPUT_INFO
#endif

// NOTE:
// Remember to have a prefix (this, PTREE::, ...) with a sufix `()` or `({PARAM1}, {PARAM2}, ...)`
#if PTREE_DEV
# 	define __PTREE_GET_OUTPUT_INFO __outputInfo
#else
# 	define __PTREE_GET_OUTPUT_INFO outputInfo
#endif

namespace ptree
{
	// PROTECTED //
	std::vector<std::string> PTREE::__outputInfo()
	{
		std::vector<std::string> strs;

		this->info.totalAll = this->info.totalDirs + this->info.totalFiles;

		strs.push_back("Dirs : " + std::to_string(this->info.totalDirs));
		strs.push_back("Files: " + std::to_string(this->info.totalFiles));
		strs.push_back("Total: " + std::to_string(this->info.totalAll));

		return strs;
	}

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
 				// BOLD: rgb(70, 185, 0)
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

	std::vector<std::string> PTREE::outputInfo()
	{
		std::vector<std::string> outInfo = this->__PTREE_GET_OUTPUT_INFO();

		if (outInfo.empty())
			return { "ERR: Could not get information" };

		return outInfo;
	}

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
            		this->info.totalDirs++;

                	std::vector<std::pair<std::string, unsigned int>> subResult = scan(entry.path().string()).first;

                	result.insert(result.end(), subResult.begin(), subResult.end());
            	}

            	else this->info.totalFiles++;
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
                	prefix += this->style.bottomRight + this->style.bottomRight + this->style.line + " ";

            	else prefix += this->style.bottomRight + this->style.line + " ";
        	}
        	
        	else
        	{
            	if (lastFlags.size() > i && lastFlags[i]) prefix += "   ";
            	else prefix += this->style.center + "  ";
        	}
    	}

    	return prefix + _path + "\n";
	}

	int PTREE::tree()
	{
		// ONLY UST SHOW THE DIRECTORY THAT IS GOING TO BE PROCESSED
		if (this->flags.directOutput)
		{
			std::cout << this->defaultDir << "\n";
			this->info.totalDirs++; // Include the dir that is going to be processed
		}

		// Total output:
		// * The `PTREE::scan()` function will do the output (if NOT !this->flags.directOutput)
		// 	 of the tree, BUT NOT THE INFORMATION OUTPUT
		std::string scanned = this->scan(this->defaultDir).second;

		// If NOT `this->flags.directOutput` (`false`), output everything at the end of the scan
		if (!this->flags.directOutput)
		{
			std::cout << this->defaultDir << "\n" << scanned << "\n";
		}
        
        // Separator with info and the tree
		std::cout << "\n";

        // INFO - BOX
        {
			std::vector<std::string> outputInfoStrs = this->outputInfo();

			if (outputInfoStrs.empty())
    			return 0;

			// Longest info line length
			size_t maxLen = 0;
			for (auto &s : outputInfoStrs)
    			if (s.size() > maxLen)
        			maxLen = s.size();

			// Box-drawing characters
			const std::string h  = this->style.line;
			const std::string v  = this->style.center;
			const std::string tl = this->style.topRight;
			const std::string br = this->style.bottomLeft;
			const std::string tr = this->style.topLeft;
			const std::string bl = this->style.bottomRight;

			// `repeat()` lambda function helper
			auto repeat = [](const std::string &_s, size_t _n)
			{
    			std::string out;

    			out.reserve(_s.size() * _n);
    			
    			for (size_t i = 0; i < _n; i++) out += _s;

    			return out;
			};

			// full box width = content + 2 spaces (left / right)
			// Top border with INFO text centered
			std::string infoTitle = " INFO ";
			std::string topBorder;

			size_t boxWidth = maxLen + 2;
			size_t sideLen = (boxWidth > infoTitle.size())
               	? (boxWidth - infoTitle.size()) / 2
               	: 0;

			try
			{
				topBorder = tl + repeat(h, sideLen) + infoTitle
                	+ repeat(h, boxWidth - sideLen - infoTitle.size()) + tr + "\n";
            }

            catch (const std::length_error &lenErr)
            {
				topBorder = tl + h + h + tr + "\n";
            }

			std::cout << topBorder;

			// Body
			for (auto &x : outputInfoStrs)
			{
    			std::string out = v + " " + x;

    			out += std::string(maxLen - x.size(), ' ');
    			out += " " + v + "\n";

    			std::cout << out;
			}

			// Bottom border
			std::string bottomBorder = bl + repeat(h, boxWidth) + br + "\n";

			std::cout << bottomBorder;
		}

		return 0;
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

