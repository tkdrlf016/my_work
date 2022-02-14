# Constant
uint8 QUALITY_NONE = 0
uint8 QUALITY_LOW = 1
uint8 QUALITY_MED = 2
uint8 QUALITY_HIGH = 3

uint8 LANE_TYPE_DASHED = 0
uint8 LANE_TYPE_SOLID = 1
uint8 LANE_TYPE_NONE = 2
uint8 LANE_TYPE_ROAD_EDGE = 3
uint8 LANE_TYPE_DOUBLE_LANE_MARK = 4
uint8 LANE_TYPE_BOTTS_DOTS = 5
uint8 LANE_TYPE_INVALID = 6

# Message
Header header

uint8 lane_type
uint8 quality
uint8 model_degree
float64 position_parameter_c0
float64 curvature_parameter_c2
float64 curvature_derivative_parameter_c3
float64 marking_width
float64 heading_angle_parameter_c1
float64 view_range
bool view_range_availability
