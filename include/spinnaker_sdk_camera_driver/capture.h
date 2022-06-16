#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER

#include "std_include.h"
#include "serialization.h"
#include "camera.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
//ROS
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"
//Dynamic reconfigure
#include <dynamic_reconfigure/server.h>
#include <spinnaker_sdk_camera_driver/spinnaker_camConfig.h>

#include "spinnaker_sdk_camera_driver/SpinnakerImageNames.h"

#include <sstream>
#include <image_transport/image_transport.h>
// nodelets
#include <nodelet/nodelet.h>
#include <nodelet/loader.h>
#include "pluginlib/class_list_macros.h"

#ifdef trigger_msgs_FOUND
  #include <trigger_msgs/sync_trigger.h>
#endif 

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace cv;
using namespace std;


namespace acquisition {
    
    class Capture : public nodelet::Nodelet {

    public:
    
        ~Capture();
        Capture();
        virtual void onInit();
        
        std::shared_ptr<boost::thread> pubThread_;

        void load_cameras();
        void init_variables_register_to_ros();
        void init_array();
        void init_cameras(bool);
        void start_acquisition();
        void end_acquisition();
        void deinit_cameras();
        void acquire_mat_images(int);
        void run();
        void run_external_trig();
        void run_soft_trig();
        void run_mt();
        void publish_to_ros(int, char**, float);

        void read_parameters();
        std::string todays_date();

        
        void write_queue_to_disk(queue<ImagePtr>*, int);
        void acquire_images_to_queue(vector<queue<ImagePtr>>*);
    
    private:

        void set_frame_rate(CameraPtr, float);
    
        void create_cam_directories();
        void save_mat_frames(int);
        void save_binary_frames(int);
        void get_mat_images();
        void update_grid();
        void export_to_ROS();
        void dynamicReconfigureCallback(spinnaker_sdk_camera_driver::spinnaker_camConfig &config, uint32_t level);
       
        float mem_usage();
    
        SystemPtr system_;    
        CameraList camList_;
        vector<acquisition::Camera> cams;
        vector<string> cam_ids_;
        vector<string> cam_names_;
        string master_cam_id_;
        unsigned int numCameras_;
        vector<CameraPtr> pCams_;
        vector<ImagePtr> pResultImages_;
        vector<Mat> frames_;
        vector<string> time_stamps_;
        vector< vector<Mat> > mem_frames_;
        vector<vector<double>> intrinsic_coeff_vec_;
        vector<vector<double>> distortion_coeff_vec_;
        vector<vector<double>> rect_coeff_vec_;
        vector<vector<double>> proj_coeff_vec_;
        vector<string> imageNames;
        vector<bool> flip_horizontal_vec_;
        vector<bool> flip_vertical_vec_;
           
        string path_;
        string todays_date_;

        time_t time_now_;
        double grab_time_, save_time_, toMat_time_, save_mat_time_, export_to_ROS_time_, achieved_time_;

        int nframes_;
        float init_delay_;
        int skip_num_;
        int binning_;
        bool color_;
        string pixel_format_;
        string dump_img_;
        string ext_;
        float exposure_time_;
        float gain_;
        double target_grey_value_;
        bool first_image_received;
        // int decimation_;
        string tf_prefix_;        
        int soft_framerate_; // Software (ROS) frame rate
        
        int MASTER_CAM_;
        int CAM_; // active cam during live
        int image_width_;
        int image_height_;

        bool FIXED_NUM_FRAMES_;
        bool TIME_BENCHMARK_;
        bool MASTER_TIMESTAMP_FOR_ALL_;
        bool EXTERNAL_TRIGGER_;
        bool SAVE_;
        bool SAVE_BIN_;
        bool LIVE_;
        bool CAM_DIRS_CREATED_;
        bool GRID_VIEW_;
//        bool MEM_SAVE_;
        bool SOFT_FRAME_RATE_CTRL_;
        bool EXPORT_TO_ROS_;
        bool MAX_RATE_SAVE_;
        bool PUBLISH_CAM_INFO_;
        bool VERIFY_BINNING_;
        uint64_t SPINNAKER_GET_NEXT_IMAGE_TIMEOUT_;
        
        #ifdef trigger_msgs_FOUND
            ros::Time latest_imu_trigger_time_;
            uint32_t prev_imu_trigger_count_ = 0; 
            uint32_t latest_imu_trigger_count_;

            void assignTimeStampCallback(const trigger_msgs::sync_trigger::ConstPtr& msg);
            struct SyncInfo_{
                uint32_t latest_imu_trigger_count_;
                ros::Time latest_imu_trigger_time_;
            };
            std::vector<std::queue<SyncInfo_>> sync_message_queue_vector_;
            ros::Subscriber timeStamp_sub;
        #endif
        
        
        bool region_of_interest_set_;
        int region_of_interest_width_;
        int region_of_interest_height_;
        int region_of_interest_x_offset_;
        int region_of_interest_y_offset_;

        // grid view related variables
        bool GRID_CREATED_;
        Mat grid_;
    
        // ros variables
        ros::NodeHandle nh_;
        ros::NodeHandle nh_pvt_;
        std::shared_ptr<image_transport::ImageTransport> it_;

        dynamic_reconfigure::Server<spinnaker_sdk_camera_driver::spinnaker_camConfig>* dynamicReCfgServer_;

        ros::Publisher acquisition_pub;
        //vector<ros::Publisher> camera_image_pubs;
        vector<image_transport::CameraPublisher> camera_image_pubs;
        //vector<ros::Publisher> camera_info_pubs;

		
        vector<sensor_msgs::ImagePtr> img_msgs;
        vector<sensor_msgs::CameraInfoPtr> cam_info_msgs;
        spinnaker_sdk_camera_driver::SpinnakerImageNames mesg;
        boost::mutex queue_mutex_;  
    };

}

#endif
