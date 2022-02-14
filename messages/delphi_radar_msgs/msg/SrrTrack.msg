std_msgs/Header header

# Constant
uint8     DETECT_VALID_LEVEL_SUSPECT_DETECTION=0
uint8     DETECT_VALID_LEVEL_LEVEL_1=1
uint8     DETECT_VALID_LEVEL_LEVEL_2=2
uint8     DETECT_VALID_LEVEL_LEVEL_3=3
uint8     DETECT_VALID_LEVEL_LEVEL_4=4
uint8     DETECT_VALID_LEVEL_LEVEL_5=5
uint8     DETECT_VALID_LEVEL_LEVEL_6=6
uint8     DETECT_VALID_LEVEL_LEVEL_7=7

uint8     DETECT_STATUS_NO_DATA=0
uint8     DETECT_STATUS_VALID_DATA_PRESENT=1

# Message
uint8     detect_id
uint8     detect_valid_level
uint8     detect_status
float32   detect_range_rate                 # m/s
float32   detect_range                      # m
float32   detect_angle                      # deg
float32   detect_amplitude                  # dbsm
