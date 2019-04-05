cmake_minimum_required(VERSION 2.6)
project(RWSfromFile)
SET(PACKAGE_NAME RWSfromFile)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# C++14: set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
cmake_minimum_required(VERSION 2.8)

SET(SRC
            log.h
            main.cpp 
            readfiles.cpp
            makedesctext.cpp
            gpx.cpp

	)
IF(UNIX)
 INCLUDE("cmake/FindTinyXML.cmake")
 FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX)

SET(SRC_LTINYXML
            tinyxml/tinyxml.cpp
            tinyxml/tinyxmlparser.cpp
            tinyxml/tinyxmlerror.cpp
 )
 ADD_DEFINITIONS(-DTIXML_USE_STL)
 
 set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 

add_executable(${PACKAGE_NAME} ${SRC} ${SRC_LTINYXML})
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PACKAGE_NAME} ${CURL_LIBRARIES})

install(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION bin)
