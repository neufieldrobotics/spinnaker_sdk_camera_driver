//
// Created by pushyami on 1/15/19.
//
//cpp
#include <iostream>
// ROS
#include <ros/ros.h>
#include <image_transport/image_transport.h>
//#include <cv_bridge/cv_bridge.h>
// msgs
#include "sensor_msgs/Image.h"
//#include "sensor_msgs/CameraInfo.h"

// nodelets
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>



namespace subscriber_nodelet_ns
{
    class subscriber_nodelet: public nodelet::Nodelet
    {
    //This is a test nodelet for  measuring nodelet performance
        public:
            subscriber_nodelet(){}
            ~subscriber_nodelet()
            {
                it_.reset();
            }

            std::shared_ptr<image_transport::ImageTransport> it_;
            image_transport::Subscriber image_sub_;
            
            virtual void onInit()
            {
                NODELET_INFO("Initializing Subscriber nodelet");
                ros::NodeHandle& node = getNodeHandle();
                ros::NodeHandle& private_nh = getPrivateNodeHandle();

                it_.reset(new image_transport::ImageTransport(node));
                image_sub_ = it_->subscribe("/camera_array/cam0/image_raw",1, &subscriber_nodelet::imgCallback, this);
                NODELET_INFO("onInit for Subscriber nodelet Initialized");
            }

            void imgCallback(const sensor_msgs::Image::ConstPtr& msg)
            {
                // dont modify the input msg in any way
                sensor_msgs::Image tmp_img = *msg;
                NODELET_INFO_STREAM("diff time is "<< ros::Time::now().toSec() - tmp_img.header.stamp.toSec());
                // copy input img to cv::mat and do any cv stuff
                // dont do time intense stuff in callbacks
            }
    };
}

PLUGINLIB_EXPORT_CLASS(subscriber_nodelet_ns::subscriber_nodelet, nodelet::Nodelet)