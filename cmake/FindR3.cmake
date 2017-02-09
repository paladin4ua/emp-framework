# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Kapec <kapecp@gmail.com>
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
#	Note:
#		Searching headers and libraries is very simple and is NOT as powerful as scripts
#		distributed with CMake, because LuaDist defines directories to search for.
#		Everyone is encouraged to contact the author with improvements. Maybe this file
#		becomes part of CMake distribution sometimes.

# - Find pcre
# Find the native R3 headers and libraries.
#
# R3_INCLUDE_DIRS	- where to find pcre.h, etc.
# R3_LIBRARIES	- List of libraries when using pcre.
# R3_FOUND	- True if pcre found.

# Look for the header file.
FIND_PATH(R3_INCLUDE_DIR NAMES r3.h)

# Look for the library.
FIND_LIBRARY(R3_LIBRARY NAMES r3)

# Handle the QUIETLY and REQUIRED arguments and set R3_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(R3 DEFAULT_MSG R3_LIBRARY R3_INCLUDE_DIR)

# Copy the results to the output variables.
IF(R3_FOUND)
	SET(R3_LIBRARIES ${R3_LIBRARY})
	SET(R3_INCLUDE_DIRS ${R3_INCLUDE_DIR})
ELSE(R3_FOUND)
	SET(R3_LIBRARIES)
	SET(R3_INCLUDE_DIRS)
ENDIF(R3_FOUND)

MARK_AS_ADVANCED(R3_INCLUDE_DIRS R3_LIBRARIES)
