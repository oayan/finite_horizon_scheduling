cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(NCS_framework_cpp) 

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1")
            message("hello")
            set(NCS_FSimpl "stdc++fs") 
        endif()
    endif()
endif()

find_package(Threads)

include_directories(third_party)
add_subdirectory(Common)
add_subdirectory(Network)
add_subdirectory(Control)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
    common network control 
    ${NCS_FSimpl}
    ${CMAKE_THREAD_LIBS_INIT})
