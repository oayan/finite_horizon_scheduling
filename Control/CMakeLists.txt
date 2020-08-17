add_library(control STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/InvertedPendulum/invertedpendulumconfig.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/InvertedPendulum/invertedpendulum.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/InvertedPendulum/invertedpendulumstable.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/1x1Plants/A125.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/1x1Plants/A075.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/1x1Plants/A150.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/1x1Plants/A100.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/ControlBase/controller.h 
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/ControlBase/controlloop.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/ControlBase/estimator.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/ControlBase/plant.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/ControlBase/sensor.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/InvertedPendulum/invertedpendulumconfig.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/InvertedPendulum/invertedpendulum.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/InvertedPendulum/invertedpendulumstable.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/1x1Plants/A125.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/1x1Plants/A075.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/1x1Plants/A150.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/1x1Plants/A100.h
)

target_include_directories(control 
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Control/InvertedPendulum
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(control PUBLIC common network)
