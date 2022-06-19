// Header to include most of the standard headers
// required by rest of the application

// Spinnaker SDK
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include "spinnaker_configure.h"
// OpenCV
#if (OPENCV_VERSION < 4)
    #include <cv.h>
    auto destroyAllWindows = cvDestroyAllWindows;
    auto waitKey = cvWaitKey;
    auto WINDOW_NORMAL = CV_WINDOW_NORMAL;
    auto WINDOW_KEEPRATIO = CV_WINDOW_KEEPRATIO;
#endif

#include <opencv2/highgui/highgui.hpp>


// ROS
#include <ros/ros.h>
// #include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "sensor_msgs/Image.h"
#include "sensor_msgs/CameraInfo.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include "sensor_msgs/Image.h"


// Standard Libs
#include <iostream>
#include <fstream>
#include <sstream> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <signal.h>
#include <cstdlib>

#include <queue> 
#include <boost/thread.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


// gflags
//#include <gflags/gflags.h>

// glog
//#include <glog/logging.h>

// yaml-cpp
//#include <yaml-cpp/yaml.h>

// gperftools
// #include <gperftools/profiler.h>
