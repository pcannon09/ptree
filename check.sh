#!/bin/bash

# PCANNON CHECK.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards

cppcheck --project=build/compile_commands.json \
	--enable=all \
	--inconclusive \
	--suppress=missingIncludeSystem \
	--suppress='*:vendor/*' \
	$@

