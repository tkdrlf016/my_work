# Constant
uint8 LANE_CONFIDENCE_NONE = 0
uint8 LANE_CONFIDENCE_LOW = 1
uint8 LANE_CONFIDENCE_MED = 2
uint8 LANE_CONFIDENCE_HIGH = 3

# Message
Header header

uint8 lane_confidence_left
bool ldw_available_left
uint8 lane_type_left
float64 distance_to_left_lane

uint8 lane_confidence_right
bool ldw_available_right
uint8 lane_type_right
float64 distance_to_right_lane

