#include "spinnaker_sdk_camera_driver/capture.h"

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

int main(int argc, char** argv) {
    
    int result = 0;

    // Initializing the ros node
    ros::init(argc, argv, "acquisition_node");
    
    acquisition::Capture cobj;
    cobj.init_array();
    cobj.run();
    
    return 0;
        
}
