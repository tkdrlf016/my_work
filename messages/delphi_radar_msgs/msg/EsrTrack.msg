std_msgs/Header header

# Constants
uint8 TRACK_STATUS_NO_TARGET = 0
uint8 TRACK_STATUS_NEW_TARGET = 1
uint8 TRACK_STATUS_NEW_UPDATED_TARGET = 2
uint8 TRACK_STATUS_UPDATED_TARGET = 3
uint8 TRACK_STATUS_COASTED_TARGET = 4
uint8 TRACK_STATUS_MERGED_TARGET = 5
uint8 TRACK_STATUS_INVALID_COASTED_TARGET = 6
uint8 TRACK_STATUS_NEW_COASTED_TARGET = 7

uint8 TRACK_MED_RANGE_MODE_NO_UPDATE = 0
uint8 TRACK_MED_RANGE_MODE_MR_UPDATE_ONLY = 1
uint8 TRACK_MED_RANGE_MODE_LR_UPDATE_ONLY = 2
uint8 TRACK_MED_RANGE_MODE_BOTH_UPDATE = 3

# ESR Track
uint8         track_id
bool          track_oncoming
bool          track_group_changed
float32       track_lat_rate       # m/s
uint8         track_status
float32       track_angle          # deg
float32       track_range          # m
float32       track_width          # m
bool          track_rolling_count
bool          track_bridge_object
float32       track_range_accel    # m/ss
uint8         track_med_range_mode
float32       track_range_rate     # m/s

# ESR Track MotionPower
int8          track_power          # dB, amplitude
bool          track_moving
bool          track_movable_slow
bool          track_movable_fast
