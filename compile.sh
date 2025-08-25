#!/bin/bash

# PCANNON COMPILE.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards

set -e

source ./utils/inc/sh/colors.sh

if ! command -v cmake > /dev/null 2>&1; then
	echo -e "$BRIGHT_RED Please have 'cmake' installed $RESET"
	exit
fi

if ! command -v ninja > /dev/null 2>&1; then
	echo -e "$BRIGHT_RED Please have 'ninja' installed $RESET"
	exit
fi

if ! command -v jq > /dev/null 2>&1; then
	echo -e "$BRIGHT_RED Please have 'jq' installed $RESET"
	exit
fi

COMPILATION_FILE_PATH=".private/dev/compilation.json"
PROJECT_INFO_PATH=".private/project.json"

cores=$(jq '.cores' "$COMPILATION_FILE_PATH")
enableBackup=$(jq -r '.enableBackup' "$COMPILATION_FILE_PATH")
projectName=$(jq -r '.exeName' "$PROJECT_INFO_PATH")
readarray -t compileMacros < <(jq -r '.macros // [] | .[]' "$COMPILATION_FILE_PATH")

if [ ! -d "./build" ]; then
	mkdir build
fi

function ninjaComp() {
	cmake --build build -j"$cores" -v
}

if [ "$enableBackup" == "YES" ]; then
	printf "${BOLD}${GREEN}[ INFO ] Backing up executable\n${RESET}"

	exeBakPath="./build/bin/exeBackup"

	if [ ! -d "$exeBakPath" ]; then
		mkdir -p "$exeBakPath"
	fi

	if [ -f "build/bin/$projectName" ]; then
		cp "./build/bin/$projectName" "$exeBakPath/$projectName-$(date +%s)"
	fi

	echo -e " [ Done ]"
fi

if [ "$1" == "setup" ]; then
	for macro in "${compileMacros[@]}"; do
		if [ -n "$macro" ]; then
			if [[ "$macro" != *=* ]]; then
				compilerFlags+=" -D$macro"
			else
				compilerFlags+=" -D$macro"
			fi
		else
			echo -e "${BRIGHT_YELLOW}${BOLD}[ WARN ] Skipping empty macro \`${macro}\`${RESET}"
		fi
	done

	cmakeCommand="cmake -S . -B build -G Ninja -DCMAKE_POLICY_VERSION_MINIMUM=3.5 $compilerFlags"

	echo -e "${BRIGHT_BLUE}${BOLD}[ * ] Running CMake:\n$cmakeCommand${RESET}"
	eval "$cmakeCommand"

elif [ -z "$1" ] || [ "$1" == "m" ]; then
	if command -v ninja > /dev/null 2>&1; then
		ninjaComp "$@"
		exit
	else
		echo -e "$BRIGHT_RED Please have 'ninja' installed $RESET"
	fi

elif [ "$1" == "settings" ]; then
	echo -e "[ * ] Compilation settings"

	if [ ! -d "tmp" ]; then
		echo -e "${BOLD}[ NOTE ] Creating \`tmp/\` dir${RESET}"
		mkdir tmp
	fi

	# COMPILATION CORES
	echo -e "${BOLD}[ PROMPT ] Enter the number of cores to compile the program ${RESET}"
	read cores

	if [[ ! -s "$COMPILATION_FILE_PATH" ]]; then
		echo "{}" > "$COMPILATION_FILE_PATH"
	fi

	jq ".cores = $cores" "$COMPILATION_FILE_PATH" > tmp/tmp_dev_compilation.json

	# ENABLE BACKUP (YES OR NO)
	echo -e "${BOLD}[ PROMPT ] Enable backup? 'YES' or 'NO' ${RESET}"
	read enableBackup

	# SET MACROS
	echo -e "${BOLD}[ PROMPT ] Macros (OPTIONAL)\n(done: Stop adding to macro list)${RESET}"
	programMacros=()

	while IFS= read -r macroVal; do
		if [ "$macroVal" == "done" ]; then
			break
		fi
		programMacros+=("$macroVal")
	done

	echo -e "${BRIGHT_BLUE}${BOLD}[ * ] TOTAL MACROS [${programMacros[@]}]${RESET}"

	# Write macros to JSON
	macrosJson=$(printf '%s\n' "${programMacros[@]}" | jq -R . | jq -s .)
	jq --arg val "$enableBackup" --argjson macros "$macrosJson" \
	   ".cores = $cores | .enableBackup = \$val | .macros = \$macros" \
	   "$COMPILATION_FILE_PATH" > tmp/tmp_dev_compilation.json

	mv tmp/tmp_dev_compilation.json "$COMPILATION_FILE_PATH"

	echo -e "[ DONE ]"
	exit
fi

