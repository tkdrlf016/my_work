std_msgs/Header header

# STR_CMD
int16     ul_steer_pos
uint8       ul_steer_auto_mode

# ACC_G_CMD
float32     ul_acc_g_cmd
uint8       ul_acc_g_auto_mode

# ACC_CMD
uint8       ul_aps_cmd
uint8       ul_aps_auto_mode

# SCC_CMD
uint8       ul_scc_mode
uint8       ul_scc_acc
uint8       ul_scc_takeover

# BRK_MOT_CMD
int16       ul_brake_pos_cmd
int16       ul_brake_vel_cmd
uint16      ul_brake_acc_cmd
uint16      ul_brake_dec_cmd

# BRK_STA_CMD
uint8       ul_brake_auto_mode
uint8       ul_brake_mtr_alarm_reset_cmd
uint8       ul_brake_mtr_pos_init_cmd
uint8       ul_encoder_init_cmd

# SHIFT_CMD
uint8       ul_shift_pos
uint8       ul_shift_auto_mode
uint8       ul_shift_reboot_cmd

# TL_CMD
uint8       ul_left_turnlamp
uint8       ul_right_turnlamp
uint8       ul_hazard
uint8       ul_lamp_auto_mode

# CHM1115
