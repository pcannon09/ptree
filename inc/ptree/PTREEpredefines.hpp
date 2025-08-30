/**
 * EXAMPLEpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 * NOTICE: 
 * - RENAMED TO PTREEpredefines.hpp
 */

#pragma once

// Project setup
#define PTREE_DEFAULT_CPP_STD			201703L

// Versioning
#define PTREE_VERSION_MAJOR            0
#define PTREE_VERSION_MINOR            0
#define PTREE_VERSION_PATCH            0

#define PTREE_VERSION_STD              0

// Version states:
// * dev
// * beta
// * build
#define PTREE_VERSION_STATE          "dev"

#define PTREE_VERSION                ((PTREE_VERSION_MAJOR<<16)|(PTREE_VERSION_MINOR<<8)|(PTREE_VERSION_PATCH)|(FDX_VERSION_STATE << 24))

#define PTREE_VERSION_CHECK(PTREE_VERSION_MAJOR, PTREE_VERSION_MINOR, PTREE_VERSION_PATCH, FDX_VERSION_STATE) \
    (((PTREE_VERSION_MAJOR)<<16)|((PTREE_VERSION_MINOR)<<8)|(PTREE_VERSION_PATCH)|((PTREE_VERSION_STATE) << 24))

// Macro utils
#define PTREE_STRINGIFY(x) #x
#define PTREE_TOSTRING(x) PTREE_STRINGIFY(x)

#ifndef PTREE_DEV
#   define PTREE_DEV true
#endif // !defined(PTREE_DEV)

// Macro Checks
#if PTREE_DEV
#	define __PTREE_MAKE_TESTS
#endif // PTREE_DEV

#define __PTREE_BYTES_TO_MB(_bytes)  		(_bytes / (1024.0 * 1024.0))
#define __PTREE_BYTES_TO_GB(_bytes)  		(_bytes / (1024.0 * 1024.0 * 1024.0))
#define __PTREE_BYTES_TO_TB(_bytes)  		(_bytes / (1024.0 * 1024.0 * 1024.0 * 1024.0))

// OS Checks
#ifdef WIN32
#	define PTREE_OS_WIN32
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define PTREE_OS_MACOS
#elif defined(__linux__) || defined(__unix) || defined(__unix__)
#	define PTREE_OS_UNIX_LINUX
#elif defined(__FreeBSD__)
#	define PTREE_OS_FREEBSD
#else
#	error "Current platform is not supported"
#endif // defined(WIN32) // Platform check

