//
// Created by auv on 1/10/19.
//

#include "ros/ros.h"
#include "spinnaker_sdk_camera_driver/std_include.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include "sensor_msgs/Image.h"

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const sensor_msgs::Image::ConstPtr& msg)
{
    ROS_INFO_STREAM("diff time is "<< ros::Time::now().toSec() - msg->header.stamp.toSec());
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "listener");


    ros::NodeHandle node;

    image_transport::ImageTransport it_(node);
    image_transport::Subscriber image_sub_ = it_.subscribe("/camera_array/cam0/image_raw",1, chatterCallback) ;

    ros::spin();

    return 0;
}