cmake_minimum_required(VERSION 2.8.3)
project(autodrive_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  std_msgs
)


## Generate messages in the 'msg' folder
add_message_files(
  DIRECTORY msg
  FILES
  AutodriveCmd.msg
  AutodriveRpt.msg
  UserCmd.msg
  DiagStatus.msg
  ObjectRpt.msg
  CipvObjectRpt.msg
  ObjectArrRpt.msg
  CanData.msg
  GnssRpt.msg
  VisualAngle.msg
  GpsMapCmd.msg
  GpsMapArray.msg
  EsrObjectRpt.msg
  EsrNearObjectsRpt.msg
  TrafficSignal.msg
  SpeedCtrlCmdRpt.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
  std_msgs
)
