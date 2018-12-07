cmake_minimum_required(VERSION 2.8.3)
project(spinnaker_sdk_camera_driver)

add_compile_options(-std=c++11)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

###
# camera Acquisition specific 
###
#SET("OpenCV_DIR" "~/apps/opencv-2.4.13/")
set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" CACHE PATH "Project Include Directory")
set(SPINNAKER_INCLUDE_DIR "/usr/include/spinnaker" CACHE PATH "Spinnaker Include Directory")
set(SPINNAKER_LIB_DIR "/usr/lib" CACHE PATH "Spinnaker Libs Directory")
# set(yaml-cpp_DIR "~/apps/yaml-cpp" CACHE PATH "yaml-cpp Directory")
set(CUDA_USE_STATIC_CUDA_RUNTIME  OFF)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  cv_bridge
  image_transport
  sensor_msgs
)

#find_package(PCL REQUIRED)

###
# Acquisition specific
###
# Find Packages
find_package(OpenCV REQUIRED)
find_package(LibUnwind REQUIRED)

find_package(Boost REQUIRED)
if(Boost_FOUND)
  find_package ( Boost COMPONENTS filesystem system serialization REQUIRED )
  set(Boost_GENERAL ${Boost_LIBRARIES})
elseif(NOT Boost_FOUND)
  message("Boost not found!")
endif()

add_message_files(
  FILES
  SpinnakerImageNames.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs message_runtime
  DEPENDS OpenCV LibUnwind
)

include_directories(
  ${PROJECT_INCLUDE_DIR}
  ${catkin_INCLUDE_DIRS}
  ${SPINNAKER_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${LibUnwind_INCLUDE_DIRS}
)

link_directories( ${SPINNAKER_LIB_DIR} )

set (LIBS ${LibUnwind_LIBRARIES} Spinnaker ${OpenCV_LIBS} ${Boost_GENERAL})

add_library (acquilib SHARED
  src/capture.cpp
  src/camera.cpp
)

add_dependencies(acquilib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(acquilib ${LIBS} ${catkin_LIBRARIES})

add_executable (acquisition_node src/acquisition_node.cpp)
add_dependencies(acquisition_node acquilib ${catkin_EXPORTED_TARGETS})
target_link_libraries (acquisition_node acquilib ${LIBS} ${catkin_LIBRARIES})

install(TARGETS acquilib acquisition_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)
