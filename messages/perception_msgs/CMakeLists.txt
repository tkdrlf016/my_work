cmake_minimum_required(VERSION 2.8.3)
project(perception_msgs)

find_package(catkin REQUIRED
  COMPONENTS
    message_generation
    std_msgs
    geometry_msgs
    sensor_msgs
    jsk_recognition_msgs
)

add_message_files(DIRECTORY msg
  FILES
    CIPV.msg
    LCA.msg
    Centroids.msg
    CloudCluster.msg
    CloudClusterArray.msg
    DetectedObject.msg
    DetectedObjectArray.msg
)

generate_messages(
  DEPENDENCIES
    std_msgs
    geometry_msgs
    sensor_msgs
    jsk_recognition_msgs
)

catkin_package(
  CATKIN_DEPENDS
    message_runtime
    std_msgs
)
