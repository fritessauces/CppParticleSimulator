cmake_minimum_required(VERSION 3.15)

project(labo01)

# Setup language requirements
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add .cpp and .h files
file(GLOB_RECURSE TESTS_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE MAIN_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "src/*.h")

#--------------------------------------------------
# Define math lib (header-only library)
#--------------------------------------------------
add_library(labo01 INTERFACE)
target_sources( labo01 INTERFACE ${MAIN_SOURCES} )
target_include_directories(labo01 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)


#--------------------------------------------------
# Define test executable
#--------------------------------------------------
add_executable(TestLabo01 main.cpp ${MAIN_SOURCES} ${TESTS_SOURCES})
target_link_libraries(TestLabo01 gtest)

# Set labo01 as the startup project for Visual Studio
if(MSVC)
	set_property(TARGET TestLabo01 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/labo01)
endif()