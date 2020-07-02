void validator()
{
std::shared_ptr<camera_info_manager::CameraInfoManager> camera_info;


camera_info = std::shared_ptr<camera_info_manager::CameraInfoManager>(
      new camera_info_manager::CameraInfoManager(pnh));



  if (camera_info->validateURL(camera_info_url))
  {
    camera_info->loadCameraInfo(camera_info_url);
  }
  else
  {
    ROS_INFO("flir_boson_usb - camera_info_url could not be validated. Publishing with unconfigured camera.");
  }

  sensor_msgs::CameraInfoPtr
    ci(new sensor_msgs::CameraInfo(camera_info->getCameraInfo()));

  ci->header.frame_id = frame_id;
}