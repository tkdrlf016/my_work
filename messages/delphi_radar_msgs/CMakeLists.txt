cmake_minimum_required(VERSION 2.8.3)
project(delphi_radar_msgs)

find_package(catkin REQUIRED
  COMPONENTS
    message_generation
    std_msgs
)

add_message_files(DIRECTORY msg
  FILES
    EsrTrack.msg
    EsrTrackArray.msg
    EsrStatus1.msg
    EsrStatus2.msg
    EsrStatus3.msg
    EsrStatus4.msg
    EsrStatus9.msg
    SrrTrack.msg
    SrrTrackArray.msg
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
