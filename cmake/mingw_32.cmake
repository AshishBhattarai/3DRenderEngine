# toolchain for cross compiling to windows
# usage#1 : first clear build. cd build && rm -rf *
# usage#2 : cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw_32.cmake ..
set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX i686-w64-mingw32)

# corss c/c++ compilers
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
SET(CMAKE_RANLIB	${TOOLCHAIN_PREFIX}-ranlib)

# root directory with the cross compiler's c/c++ headers/libs
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# modify default behaviour of find_xxx(ex: find_package()) commands
# NEVER - CMAKE_FIND_ROOT_PATH will be ignored and only the host system root will be used.
# ONLY	-	only the roots in CMAKE_FIND_ROOT_PATH will be searched.
# BOTH 	-	both host and CMAKE_FIND_ROOT_PATH will be searched.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # find_program()
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)	 # find_library()
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  # find_include()
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)  # find_package()