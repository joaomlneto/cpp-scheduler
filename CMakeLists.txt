cmake_minimum_required(VERSION 3.8)
project(cpp-scheduler VERSION 1.0.0 LANGUAGES CXX C)

include(GNUInstallDirs)
include(CTest)

# require pthreads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#########################
# cpp-scheduler library #
################################################################################

# get list of source files
file(GLOB_RECURSE cpp-scheduler_src relative ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp" "src/*c")
add_library(cpp-scheduler SHARED ${cpp-scheduler_src})

set_property(TARGET cpp-scheduler PROPERTY POSITION_INDEPENDENT_CODE on)
target_compile_options(cpp-scheduler PRIVATE -g -Wall -pedantic -Wshadow -Wfatal-errors)

target_compile_features(cpp-scheduler
  PRIVATE cxx_std_17
)

target_include_directories(cpp-scheduler
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PUBLIC $<INSTALL_INTERFACE:include>
	PRIVATE src
)

target_link_libraries(cpp-scheduler
	Threads::Threads
)

# 'make install' to the correct locations (provided by GNUInstallDirs)
install(TARGETS cpp-scheduler
	EXPORT cpp-scheduler_config
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'
install(EXPORT cpp-scheduler_config DESTINATION share/cpp-scheduler/cmake)

# uninstall target
if(NOT TARGET uninstall)
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
		IMMEDIATE @ONLY)
	add_custom_target(uninstall
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

################################################################################
# tests

# simple test -- schedule independent tasks
add_executable(SimpleTest test/simple.cpp)
target_link_libraries(SimpleTest cpp-scheduler)
add_test(SimpleTest SimpleTest)

# dependency test -- schedule tasks with dependencies
add_executable(DependencyTest test/dependencies.cpp)
target_link_libraries(DependencyTest cpp-scheduler)
add_test(DependencyTest DependencyTest)

# bake-a-cake test
add_executable(RecipeTest test/recipe.cpp)
target_link_libraries(RecipeTest cpp-scheduler)
add_test(RecipeTest RecipeTest)
