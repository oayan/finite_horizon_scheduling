add_library(network STATIC
${CMAKE_CURRENT_SOURCE_DIR}/src/NetworkBase/gateway.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/NetworkBase/mobileterminal.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/aoischeduler.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/aoiapplication.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/basestation.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/controlapplication.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/finitehorizonscheduler.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/networkconfig.cpp 
# ${CMAKE_CURRENT_SOURCE_DIR}/NetworkBase/scheduler.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/NetworkBase/gateway.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/NetworkBase/mobileterminal.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/NetworkBase/application.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/NetworkBase/packet.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/finitehorizonscheduler.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/txgateway.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/rxgateway.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/basestation.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/networkconfig.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/controlapplication.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/aoischeduler.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/aoiapplication.h
)

target_include_directories(network 
  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Network 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Network/NetworkBase
)

target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(network PUBLIC common)
