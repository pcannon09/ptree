#!/bin/bash

# PCANNON GEN.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards

source ./utils/inc/sh/colors.sh

DIR_GEN=(
		".private"
		".private/dev"
)

FILE_GEN=(
		".private/dev/compilation.json"
)

function changeDir {
    cd $1

    echo -e "${UNDERLINE}[ * ] Moved to \`$1\`${RESET}"
}

if [ "$1" == "ungen" ]; then
    echo -e "${BOLD}[ * ] Removing generated files"
    echo -e "${RED}[ WARN ] Are you sure that you want to continue? This will remove all existing data [ Y / N / GET ]${RESET}"

    read answer

    if [ "${answer,,}" == "yes" ] || [ "${answer,,}" == "y" ]; then
        printf "[ DEL ] Deleting all from \`.private/\` " ; find .private -mindepth 1 ! -name "project.json" -delete ; echo "[ DONE ]"

        echo -e "[ DONE ]${RESET}"

    elif [ "${answer,,}" == "get" ]; then
        tree .private/
        
        echo "[ DONE ]"

    else
        echo -e "${RESET}Abort."
    fi

    echo -e "${BOLD}${CYAN}[ NOTE ] Execute this file without the \`ungen\` flag to generate all files"
    exit

elif [ "$1" == "doxygen" ]; then
	doxygen Doxyfile
fi

for dir in "${DIR_GEN[@]}"; do
	printf "${BOLD}${BRIGHT_GREEN}[ MKDIR ] Generating \`$dir\` directory${RESET} " ; mkdir -p $dir ; echo -e "${BOLD}${BRIGHT_GREEN}[ OK ]${RESET}"
done

for file in "${FILE_GEN[@]}"; do
	printf "${BOLD}${BRIGHT_GREEN}[ TOUCH ] Generating \`$file\` file${RESET} " ; touch $file ; echo -e "${BOLD}${BRIGHT_GREEN}[ OK ]${RESET}"
done

if [ -f "Doxyfile" ]; then
	doxygen Doxyfile
fi

echo -e "[ DONE ]"

