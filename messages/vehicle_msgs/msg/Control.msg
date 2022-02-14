std_msgs/Header header

uint8 emergency_stop
uint8 auto_mode
uint8 steer_auto
uint8 speed_auto
uint8 shift_auto

float32 target_speed             # 0~speed [km/h]
int32   target_steer_angle       # -700deg ~ 700deg
uint8   target_shift_gear        # P,R,N,D [0, 1, 2, 3]
int16   target_accel_pedal       # 0~100%
int16   target_break_pedal       # 0~100%

uint8 left_lane_change           # 0:no command 1:lane change command
uint8 right_lane_change          # 0:no command 1:lane change command

# pid control variable
float32 ster_kp
float32 ster_ki
float32 ster_kd

float32 acel_kp
float32 acel_ki
float32 acel_kd

float32 decl_kp
float32 decl_ki
float32 decl_kd
