# spinnaker_camera_driver

These are the ros drivers for running the Pt Grey (FLIR) cameras that use the Spinnaker SDK.  This code has been tested with various Point Grey Blackfly S (BFS) cameras. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The pre-requisites for this repo include:
* ros-kinetic-desktop-full
* spinnaker (download from [Pt Grey's website](https://www.ptgrey.com/support/downloads))
* ros-kinetic-cv-bridge
* ros-kinetic-image-transport
* libunwind-dev

```bash
# after installing spinnaker verify that you can run your cameras with SpinView

# after installing ros, install other pre-requisites with: 

sudo apt install libunwind-dev ros-kinetic-cv-bridge ros-kinetic-image-transport
```

### Installing
To install the spinnaker drivers
```bash
mkdir -p ~/spinnaker_ws/src
cd spinnaker_ws/src
git clone https://github.com/neufieldrobotics/spinnaker_camera_driver.git
cd ~/spinnaker_ws/
catkin_make
source ~/spinnaker_ws/devel/setup.bash
# add this to ~/.bashrc to make this permanent 
```

## Running the drivers

Modify the `params/test_params.yaml` file replacing the cam-ids and master cam serial number to match your camera's serial number. Then run the code as:
```bash
roslaunch spinnaker_camera_driver acquisition.launch
# Test that the images are being published by running
rqt_image_view
```
## Parmeters
All the parameters can be set via the launch file or via the yaml config_file.  It is good practice to specify all the 'task' specific parameters via launch file and all the 'system configuration' specific parameters via a config_file.  

### Task Specific Parameters
* ~binning (int, default: 1)  
  Binning for cameras, when changing from 2 to 1 cameras need to be unplugged and replugged
* ~color (bool, default: false)  
  Should color images be used (only works on models that support color images)
* ~exp (int, default: 0)  
  Exposure setting for cameras
* ~frames (int, default: 50)  
  Number of frames to save/view 0=ON
* ~live (bool, default: false)  
  Show images on screen GUI
* ~live_grid (bool, default: false)  
  Show images on screen GUI in a grid
* ~save (bool, default: false)  
  Flag whether images should be saved or not (via opencv mat objects to disk)
* ~save_path (string, default: "\~/projects/data")  
  Location to save the image data
* \~save_type (string, default: "bmp")  
  Type of file type to save to when saving images locally: binary, tiff, bmp, jpeg etc.
* ~soft_framerate (int, default: 20)  
  When hybrid software triggering is used, this controls the FPS, 0=as fast as possible
* ~time (bool, default=false)  
  Show time/FPS on output
* ~to_ros (bool, default: true)  
  Flag whether images should be published to ROS.  When manually selecting frames to send to rosbag, set this to False.  In that case, frames will only be sent when 'space bar' is pressed
* ~utstamps (bool, default:false)  
  Flag whether each image should have Unique timestamps vs the master cams time stamp for all
* ~max_rate_save (bool, default: false)  
  Flag for max rate mode which is when the master triggers the slaves and saves images at maximum rate possible.  This is the multithreaded mode"

### System configuration parameters
* ~cam_ids (yaml sequence or array)  
  This is a list of camera serial numbers in the order which it would be organized.  The convention is to start from left to right.
* ~cam_aliases (yaml squence or array)  
This is the names that would be given to the cameras for filenames and rostopics in the order specified above eg. cam0, cam1 etc.
* ~master_cam (int, default: )  
  This is the serial number of the camera to be used as master for triggering the other cameras.
* ~skip (int)  
  Number of frames to be skipped initially to flush the buffer
* ~delay (float)  
  Secs to wait in the deinit/init sequence

### Camera info message details
* ~image_width (int)
* ~image_height (int)
* ~distortion_model (string)  
  Distortion model for the camera calibration.  Typical is 'plumb_bob'
* ~distortion_coeffs (array of arrays)  
  Distortion coefficients of all the cameras in the array.  Must match the number of cam_ids provided.
* ~intrinsic_coeff (array of arrays)  
  Intrinsic coefficients of all the cameras in the array.  Must match the number of cam_ids provided.
  Specified as [fx  0 cx 0 fy cy 0  0  1]
* ~projection_coeffs (array of arrays)  
  Projection coefficients of all the cameras in the array.  Must match the number of cam_ids provided.
* ~rectification_coeffs (array of arrays)  
  Rectification coefficients of all the cameras in the array.  Must match the number of cam_ids provided.

## Multicamera Master-Slave Setup
When using multiple cameras, we have found that the only way to keep images between different cameras synched is by using a master-slave setup using the GPIO connector. So this is the only way we support multicamera operation with this code. A general guide for multi camera setup is available at https://www.ptgrey.com/tan/11052, however note that we use a slightly different setup with our package.
Refer to the `params/multi-cam_example.yaml` for an example on how to setup the configuration. You must specify a master_cam which must be one of the cameras in the cam_ids list. This master camera is the camera that is either explicitly software triggered by the code or triggered internally via a counter at a given frame rate. All the other cameras are triggered externally when the master camera triggers. In order to make this work, the wiring must be such that the external signal from the master camera **Line2** is connected to **Line3** on all slave cameras. To connect cameras in this way:
* Connect the primary (master) camera's pin 3 (red wire, GPIO) to each secondary (slave) camera's pin 1 (green wire, GPI).
* Connect the primary (master) camera's pin 5 (blue wire, opto ground) and pin 6 (brown wire, ground) to each secondary (slave) camera's pin 6 (brown wire, ground).

**GPIO Pinouts for Blackfly S**  
<img src="docs/images/bfs_GPIO.png" alt="GPIO Pinouts for Blackfly S" width="640" align="middle">

**GPIO Connections for Master/Slave config**  
<img src="docs/images/gpio_connections.png" alt="GPIO Connections for Master/Slave setup" width="360" align="middle">  

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
