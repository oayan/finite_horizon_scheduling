add_library(common STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/edge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/noise.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tree.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/block.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/cxxpool.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/edge.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/gaussiannoise.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/helpers.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/link.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/node.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/noise.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/threadpool.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/Common/tree.h
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/Common)
