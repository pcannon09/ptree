#!/bin/bash

# PCANNON OPEN.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards
# NOTES:
# Modified to open `tests/` dir

defaultEdit="$1" # Set the default editor in parameter $1 (such as: code, vim, nvim, ...)

if [ "$1" == "" ]; then
	defaultEdit="nvim"
fi

if [ -f "src/main.cpp" ]; then
	$defaultEdit src/main.cpp $(find src/ tests/ inc/ -type f) CMakeLists.txt

	exit

else
	if [ -d "./test/" ]; then
		if [ ! -d "./test/src" ]; then
			$defaultEdit test/main.cpp $(find test/ src/ inc/ -type f) CMakeLists.txt

		else
			$defaultEdit test/src/main.cpp $(find test/src test/inc test/ src/ inc/ -type f) CMakeLists.txt
		fi

		exit

	elif [ -d "./tests/" ]; then
		if [ ! -d "./tests/src" ]; then
			$defaultEdit tests/main.cpp $(find tests/ src/ inc/ -type f) CMakeLists.txt

		else
			$defaultEdit tests/src/main.cpp $(find tests/src tests/inc tests/ src/ inc/ -type f) CMakeLists.txt
		fi

		exit
	fi
fi

# DEFAULT
$defaultEdit src/main.cpp $(find src/ inc/ -type f) CMakeLists.txt

