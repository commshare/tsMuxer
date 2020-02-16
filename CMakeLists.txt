cmake_minimum_required (VERSION 3.1)
project (tsmuxer_main)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

execute_process(
  COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  RESULT_VARIABLE GIT_REV_SHORT_RV
  OUTPUT_VARIABLE GIT_REV_SHORT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(TSMUXER_RELEASE FALSE)
set(VERSION_NUMBER "2.6.16")
if(${TSMUXER_RELEASE})
  set(TSMUXER_VERSION ${VERSION_NUMBER})
elseif(${GIT_REV_SHORT_RV} EQUAL 0)
  set(TSMUXER_VERSION "git-${GIT_REV_SHORT}")
else()
  set(TSMUXER_VERSION "${VERSION_NUMBER}-dev")
endif()
add_definitions("-DTSMUXER_VERSION=\"${TSMUXER_VERSION}\"")

add_subdirectory (libmediation)
add_subdirectory (tsMuxer)

##add by me https://www.cnblogs.com/rickyk/p/3872568.html
option(TSMUXER_GUI "This is a option for address" ON)
set(TSMUXER_GUI ON)
##find qt5 cmake module :https://blog.csdn.net/aiyun5666/article/details/101915628
if(TSMUXER_GUI)
	add_subdirectory (tsMuxerGUI)
endif()
