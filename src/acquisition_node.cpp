#include "spinnaker_sdk_camera_driver/capture.h"
#include <nodelet/loader.h>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

int main(int argc, char** argv) {
    
    // Initializing the ros node
    ros::init(argc, argv, "acquisition_node");
    
    nodelet::Loader nodelet;
    nodelet::M_string remap(ros::names::getRemappings());
    nodelet::V_string nargv;
    std::string nodelet_name = ros::this_node::getName();
    nodelet.load(nodelet_name, "acquisition/Capture", remap, nargv);

    ros::waitForShutdown();

    return 0;        
}
