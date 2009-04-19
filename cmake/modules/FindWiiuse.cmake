# - Try to find Wiisue 
# One done, this will define
#
# Wiiuse_FOUND - system has Wiiuse
# Wiiuse_INCLUDE_DIR - the Wiiuse include directories
# Wiiuse_LIBRARY - link these to use Wiiuse

include(LibFindMacros)

# Include dir
find_path(Wiiuse_INCLUDE_DIR
    NAMES wiiuse.h
    PATHS /usr/include /usr/local/include /usr/local/wiiuse ENV PATH)

# Finally the library itself
find_library(Wiiuse_LIBRARY
    NAMES wiiuse
    PATHS /usr/lib /usr/local/lib /usr/local/wiiuse)


libfind_process(Wiiuse)
