std_msgs/Header header

uint8 GEAR_P = 0
uint8 GEAR_R = 1
uint8 GEAR_N = 2
uint8 GEAR_D = 3
uint8 GEAR_INVALID = 4

uint8 TS_NONE = 0
uint8 TS_LEFT = 1
uint8 TS_RIGHT = 2
uint8 TS_HAZARD = 3

# Message

bool valid                     # false: data invalid, true: data valid

float64 steering_angle         # [deg] -cw ~ +ccw
float64 steering_angle_rate    # [deg/s] 0 ~ rate

uint8 gear

float32 accel_pedal            # [%] 0 ~ 100
float32 brake_pedal            # [%] 0 ~ 100
uint8 brake_act		       # 1: Off, 2: On

float64 speed_kph              # [km/h] 0 ~ speed
float64 linear_velocity        # [m/s] 0 ~ vel
float64 angular_velocity       # [deg/s] -cw ~ ccw
float64 longitude_acceleration # [m/s^2] -backward ~ +forward
float64 lateral_acceleration   # [m/s^2] +left ~ -right

bool driving_stopped           # standstill. false: moving, true: stopped

uint8 turn_signal

bool acc_obj_status            # false: no target, true: target
float32 acc_obj_dist           # [m]
float32 acc_obj_rel_speed      # [m/s]
float32 acc_obj_lat_pos        # [m] +left ~ -right

bool left_blindspot            # false: free, true: warning
bool right_blindspot           # false: free, true: warning

bool door_open_warning
bool driver_seatbelt_warning
float32 rpm
float32 bat_soc                # [%] 0 ~ 100
float32 bat_volt               # [v]
float32 odometer               # [km]
