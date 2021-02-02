# Find the Spinnaker library
#
#  Spinnaker_FOUND        - True if Spinnaker was found.
#  Spinnaker_LIBRARIES    - The libraries needed to use Spinnaker
#  Spinnaker_INCLUDE_DIRS - Location of Spinnaker.h

unset(Spinnaker_FOUND)
unset(Spinnaker_INCLUDE_DIRS)
unset(Spinnaker_LIBRARIES)

find_path(Spinnaker_INCLUDE_DIRS NAMES
  Spinnaker.h
  PATHS
  /opt/spinnaker/include
  )
find_library(Spinnaker_LIBRARIES NAMES Spinnaker
  PATHS
  /opt/spinnaker/lib
)

if(NOT Spinnaker_INCLUDE_DIRS OR NOT Spinnaker_LIBRARIES)
  message(STATUS "Couldnt find Spinnaker 2.2.x. Checking lower versions")
  find_path(Spinnaker_INCLUDE_DIRS NAMES
  Spinnaker.h
  PATHS
  /usr/include/spinnaker/
  /usr/local/include/spinnaker/
  )
  find_library(Spinnaker_LIBRARIES NAMES Spinnaker
  PATHS
  /usr/lib
  /usr/local/lib
  )
  if(NOT Spinnaker_INCLUDE_DIRS OR NOT Spinnaker_LIBRARIES)
     message(STATUS "Couldnt find Spinnaker.")
  endif()
endif()

if (Spinnaker_INCLUDE_DIRS AND Spinnaker_LIBRARIES)
  message(STATUS "Spinnaker found in the system")
  set(Spinnaker_FOUND 1)
endif (Spinnaker_INCLUDE_DIRS AND Spinnaker_LIBRARIES)
