std_msgs/Header header

# Constants
uint8 MRLR_MODE_RESERVED = 0
uint8 MRLR_MODE_MR_ONLY = 1
uint8 MRLR_MODE_LR_ONLY = 2
uint8 MRLR_MODE_MR_LR = 3


# ESR status4
uint8       rolling_count3
uint8       mrlr_mode
bool        patial_blockage
bool        side_lobe_blockage
bool        lr_only_grating_lobe_det
bool        truck_target_det
uint8       path_id_acc
uint8       path_id_cmmb_move
uint8       path_id_cmmb_stat
uint8       path_id_fcw_move
uint8       path_id_fcw_stat
uint8       path_id_acc_stat
float32     auto_algin_angle # deg, (+)=clockwise
