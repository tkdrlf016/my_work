cmake_minimum_required(VERSION 2.8.3)
project(mobileye_msgs)

find_package(catkin REQUIRED
  COMPONENTS
    message_generation
    std_msgs
)

add_message_files(DIRECTORY msg
  FILES
    AwsDisplay.msg
    Tsr.msg
    TsrArray.msg
    SignType.msg
    Ahbc.msg
    Lane.msg
    ObstacleStatus.msg
    ObstacleData.msg
    Obstacle.msg
    LaneExt.msg
    LaneLKA.msg
    ReferencePoints.msg
    LaneLkaArray.msg
    TrzPosLane.msg
)

generate_messages(
  DEPENDENCIES
    std_msgs
)

catkin_package(
  CATKIN_DEPENDS
    message_runtime
    std_msgs
)
