/* inc/argx.hpp
 * Owned and created by: pcannon09
 */

#pragma once

#include "macros.hpp"

#if __cplusplus >= 201402L || defined(ARGX_AS_PYTHON_PACKAGE)
#include <string>
#include <vector>

#include "../inc/types.hpp"

namespace argx
{
	class Argx
	{
	private:
		std::string id;

		static std::vector<ARGXOptions> options;
		static std::vector<std::string> *mainArgs;

		static unsigned int mainArgc;

	public:

#if defined(ARGX_AS_PYTHON_PACKAGE)
		/**
		 * @brief Create Argx with the specific `id` and `args` as a python package
		 * @param id Set the ID of the Argx
		 * @param args Set the `argv` of the `main()` function with the specific list contents in python
		 */
		Argx(const std::string &id, const std::vector<std::string> &args);

		/**
		 * @brief Call the deconstructor of the Argx option (SPECIFIC FOR PYTHON)
		 */
		void destroy();
#else
		/**
		 * @brief Create Argx with the specific `id`, `argc` and `argv`
		 * @param id Set the ID of the Argx
		 * @param argc Set the `argc` of the `main()` function
		 * @param argv Set the `argv` of the `main()` function
		 */
		Argx(const std::string &id, int argc, char *argv[]);
#endif

		Argx();

		/**
		 * @brief Deconstruct allocated objects
		 */
		~Argx();

		/**
		 * @brief Get argument position with specified `arg`
		 * @param arg Find argument
		 * @return int Return position of found `arg` from the options
		 */
		int getArgPos(const std::string &arg);

		/**
		 * @brief Add param options
		 * @param option Add the option to the main params
		 */
		void add(ARGXOptions option) const;

		/**
		 * @brief Get the param from `id`
		 * @param id The ID to get
		 * @return ARGXParam Returnted parameter to get
		 */
		ARGXParam getParam(const std::string& id);

		/**
		 * @brief Create documentation for the parameters with the specific style, title and main information
		 * @param style Set the style using ARGXStyle
		 * @param title Title for docs
		 * @param Main information
		 * @return std::string Documentation as a string
		 */
		std::string createDocs(ARGXStyle style, const std::string &title, const std::string &mainInfo);

		/**
		 * @brief Normal parameter or sub-paramter to its corresponding ID
		 * @param param Parameter value or name
		 * @return std::string ID of the param or sub-param
		 */
		std::string paramToID(const std::string &param);

		/**
		 * @brief Find parameter and sub-parameter index
		 * @param id ID to find
		 * @return int Index
		 */
		int findParam(const std::string& id);

		/**
		 * @brief Get the main options from the `main()` function as argc
		 * @return int Number of params including the executable param
		 */
		int getArgc() const;

		/**
		 * @brief Get argument using ID
		 * @param arg Argument to find
		 * @return int Argument position
		 */
		int getArgIDPos(const std::string &arg);

		/**
		 * @brief Get the incorrect arguments and sub-arguments that were not registered
		 * @param argv Main arguments from argv
		 * @return int Argument position
		 */
		int getWrongArgs(const std::vector<std::string> &argv);

		/**
		 * @brief Get if param exists in the param options
		 * @param id ID to get
		 * @return bool
		 */
		bool paramExists(const std::string &id);

		/**
		 * @brief Check if the `tag` exists in option with ID of `id`
		 * @param id ID from option
		 * @param tag Tag to find
		 * @return bool Return false if there is no match, else, return true
		 */
		bool hasTag(const std::string &id, const std::string &tag);

		/**
		 * @brief Get if sub-param exists in the param options
		 * @param id ID to get
		 * @return bool
		 */
		bool subParamExists(const std::string &id);

		/**
		 * @brief Compare if `options` contains the required `id`, if the ID does not exist, return false
		 * @param options Return ARGXOptions vector
		 * @param id ID to find
		 * @return bool
		 */
		bool compareArgs(std::vector<ARGXOptions> options, std::vector<std::string> argv);

		/**
		 * @brief Get Options from specified ID
		 * @param id ID to find
		 * @param ARGXOptions Option information
		 */
		ARGXOptions getOption(const std::string &id);

		/**
		 * @brief Get the sub-param from `id`
		 * @param param Original param
		 * @param id The ID to get
		 * @return bool
		 */
		bool getSubParam(const argx::ARGXParam &param, const std::string &id);

		/**
		 * @brief Get sub-parameter values, starting from the first value found until the first found value that corresponds to a registered parameter
		 * @param id ID to find to get sub-value
		 * @return std::vector<std::string> Values found from first to last
		 */
		std::vector<std::string> getSubValue(const std::string &id);

		/**
		 * @brief Get main arguments from `main()` function `argv`
		 * @return std::vector<std::string> Vector of strings for main arguments from `argv`
		 */
		std::vector<std::string> getMainArgs() const;

		/**
		 * @brief Get main set options as ARGXOptions
		 * @return std::vector<ARGXOptions> Options to return
		 */
		std::vector<ARGXOptions> getOptions() const;

		/**
		 * @brief Get Argx ID
		 * @return std::string Argx ID
		 */
		std::string getID() const;
	};
}

#else
# 	error "Must compile with C++14 support or newer"
# 	pragma message("Current standard is " ARGX_TOSTRING(__cplusplus))
#endif
