cmake_minimum_required(VERSION 2.8.3)
project(spinnaker_sdk_camera_driver)

add_compile_options(-std=c++11)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

###
# camera Acquisition specific 
###
set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" CACHE PATH "Project Include Directory")
set(CUDA_USE_STATIC_CUDA_RUNTIME  OFF)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  cv_bridge
  image_transport
  sensor_msgs
  dynamic_reconfigure
  nodelet
)
find_package(trigger_msgs)
#find_package(PCL REQUIRED)

###
# Acquisition specific
###
# Find Packages
find_package(Spinnaker REQUIRED)
message("spinnaker lib :   " ${Spinnaker_LIBRARIES})
find_package(OpenCV REQUIRED)
# use LibUnwind only for x86_64 or x86_32 architecture
# do not use LibUnwind for arm architecture
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64 OR x86_32)
  message("uses LibUnwind for x86_64 or x86_32 architecture")
  find_package(LibUnwind REQUIRED)
endif(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64 OR x86_32)
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES aarch64 OR arm)
  message("Detected ARM architecture")
endif(${CMAKE_SYSTEM_PROCESSOR} MATCHES aarch64 OR arm)

find_package(Boost REQUIRED)
if(Boost_FOUND)
  find_package ( Boost COMPONENTS filesystem system serialization REQUIRED )
  set(Boost_GENERAL ${Boost_LIBRARIES})
elseif(NOT Boost_FOUND)
  message("Boost not found!")
endif()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/spinnaker_sdk_camera_driver/spinnaker_configure.h.in"
  "${PROJECT_SOURCE_DIR}/include/spinnaker_sdk_camera_driver/spinnaker_configure.h"
)

add_message_files(
  FILES
  SpinnakerImageNames.msg
)

generate_dynamic_reconfigure_options(
  cfg/spinnaker_cam.cfg

)

generate_messages(
  DEPENDENCIES
  std_msgs
)
## setting catkin_package, include_directories and libs based on architecture
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64 OR x86_32)
    catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS roscpp std_msgs message_runtime nodelet
    DEPENDS OpenCV LibUnwind
    )

    include_directories(
      ${PROJECT_INCLUDE_DIR}
      ${catkin_INCLUDE_DIRS}
      SYSTEM ${Spinnaker_INCLUDE_DIRS}
      SYSTEM ${OpenCV_INCLUDE_DIRS}
      SYSTEM ${Boost_INCLUDE_DIR}
      ${LibUnwind_INCLUDE_DIRS}
    )

    link_directories( ${Spinnaker_LIBRARIES} )

    set (LIBS ${LibUnwind_LIBRARIES} ${Spinnaker_LIBRARIES} ${OpenCV_LIBS} ${Boost_GENERAL})
    
endif(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64 OR x86_32)
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES aarch64 OR arm)
    catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS roscpp std_msgs message_runtime nodelet
    DEPENDS OpenCV
    )

    include_directories(
      ${PROJECT_INCLUDE_DIR}
      ${catkin_INCLUDE_DIRS}
      SYSTEM ${Spinnaker_INCLUDE_DIRS}
      SYSTEM ${OpenCV_INCLUDE_DIRS}
      SYSTEM ${Boost_INCLUDE_DIR}
    )

    link_directories( ${Spinnaker_LIBRARIES} )

    set (LIBS ${Spinnaker_LIBRARIES} ${OpenCV_LIBS} ${Boost_GENERAL})
    
endif(${CMAKE_SYSTEM_PROCESSOR} MATCHES aarch64 OR arm)

# Make package version available in CPP as a defination
add_definitions("-D${PROJECT_NAME}_VERSION=\"${${PROJECT_NAME}_VERSION}\"")

add_library (acquilib SHARED
  src/capture.cpp
  src/camera.cpp
)
add_dependencies(acquilib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_gencfg)
target_link_libraries(acquilib ${LIBS} ${catkin_LIBRARIES})

add_executable (acquisition_node src/acquisition_node.cpp)
add_dependencies(acquisition_node acquilib ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_gencfg)
target_link_libraries (acquisition_node acquilib ${LIBS} ${catkin_LIBRARIES})

## subscriber_example for subscribing as nodelet
add_library (subscriber_example examples/subscriber_nodelet.cpp)
add_dependencies(subscriber_example ${catkin_EXPORTED_TARGETS})
target_link_libraries(subscriber_example ${catkin_LIBRARIES})


install(TARGETS acquilib acquisition_node subscriber_example
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)

install(FILES nodelet_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
