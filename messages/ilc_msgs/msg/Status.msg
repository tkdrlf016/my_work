std_msgs/Header header

bool        valid

# STR_INFO
bool        lu_mdps_control
bool        lu_pd_error
bool        lu_mdps_error
bool        lu_obd_error
bool        lu_sas_error
int16       lu_steer_output
uint8       lu_mdps_takeover
uint8       lu_slave_alive

# SAS_INFO
float32     lu_steer_pos_info
uint8       lu_steer_vel_info
uint8       lu_sas_state_info
uint8       lu_sas_message
uint8       lu_sas_checksum

# ACC_INFO
uint8       lu_aps_master_vol_info
uint8       lu_aps_slave_vol_info
uint16      lu_aps_error_vol_info
uint8       lu_aps_auto_mode_info
uint8       lu_aps_alive

# BRK_INFO
int16       lu_brake_pos_info
int16       lu_brake_vel_info
uint16      lu_brake_state_info
uint16      lu_encoder_pos_info

# GEAR_INFO
uint8       lu_shift_pos_info
uint8       lu_shift_auto_info
uint8       lu_shift_state_info
uint8       lu_shift_alive

# TL_INFO
uint8       lu_left_turnlamp
uint8       lu_right_turnlamp
uint8       lu_hazard
uint8       lu_turnlamp_auto_mode

# C_CAN
float32     lu_c_can

# ILC_CAN_READY
uint8       lu_can_ready


