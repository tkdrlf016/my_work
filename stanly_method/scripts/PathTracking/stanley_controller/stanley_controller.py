#!/usr/bin/env python
"""

Path tracking simulation with Stanley steering control and PID speed control.

author: Atsushi Sakai (@Atsushi_twi)

Ref:
    - [Stanley: The robot that won the DARPA grand challenge](http://isl.ecst.csuchico.edu/DOCS/darpa2005/DARPA%202005%20Stanley.pdf)
    - [Autonomous Automobile Path Tracking](https://www.ri.cmu.edu/pub_files/2009/2/Automatic_Steering_Methods_for_Autonomous_Automobile_Path_Tracking.pdf)

"""
from visualization_msgs.msg import MarkerArray
from visualization_msgs.msg import Marker
from nav_msgs.msg import Path
from sensor_msgs.msg import NavSatFix
import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import rospy
import math
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose
from geometry_msgs.msg import QuaternionStamped
sys.path.append(os.path.dirname(os.path.abspath(__file__)) +
                "/../../PathPlanning/CubicSpline/")

try:
    import cubic_spline_planner
except:
    raise


k = 0.5  # control gain
Kp = 1.0  # speed proportional gain
dt = 0.1  # [s] time difference
L = 2.9  # [m] Wheel base of vehicle
max_steer = np.radians(30.0)  # [rad] max steering angle

show_animation = True


class State(object):
    """
    Class representing the state of a vehicle.

    :param x: (float) x-coordinate
    :param y: (float) y-coordinate
    :param yaw: (float) yaw angle
    :param v: (float) speed
    """

    def __init__(self, x=100.0, y=100.0, yaw=0.0, v=0.0):
        """Instantiate the object."""
        super(State, self).__init__()
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v
        self.curr_x = 0
        self.curr_y = 0 
        rospy.init_node("stanley_controll")
        self.nav_msg = NavSatFix()
        self.path_publisher = rospy.Publisher("/path",NavSatFix,queue_size = 100)
        self.ros_msg = NavSatFix()
        self.gnss_bool = 1
        self.gnss_subscriber = rospy.Subscriber("/gnss",NavSatFix,self.gnsscallback)
        self.car_yaw = rospy.Subscriber("/filter/quaternion",QuaternionStamped,self.yawcallback)
        
    def update(self, acceleration, delta):
        """
        Update the state of the vehicle.

        Stanley Control uses bicycle model.

        :param acceleration: (float) Acceleration
        :param delta: (float) Steering
        """
        delta = np.clip(delta, -max_steer, max_steer)

        #self.x += self.v * np.cos(self.yaw) * dt
        #self.y += self.v * np.sin(self.yaw) * dt
        #self.x = self.curr_x
        #self.y = self.curr_y
        #self.yaw += self.v / L * np.tan(delta) * dt
        #self.yaw = normalize_angle(self.yaw)
        self.v += acceleration * dt

    def set_xy(self,x,y):
        self.nav_msg.longitude = x
        self.nav_msg.latitude = y
        self.path_publisher.publish(self.nav_msg)

    def gnsscallback(self,data):
        #print("-=-=-=-=")
        self.gnss_bool = 0
        #print(data.longitude)
        #print(data.latitude)
        #print(self.yaw)
        self.x = data.longitude
        self.y = data.latitude
    
    def yawcallback(self,data):
        sinr = 0
        cosr = 0
        car_pos_ang_w = data.quaternion.w
        car_pos_ang_x = data.quaternion.x
        car_pos_ang_y = data.quaternion.y
        car_pos_ang_z = data.quaternion.z

        sinr = 2*(car_pos_ang_w*car_pos_ang_z + car_pos_ang_y*car_pos_ang_x)
        cosr = 1-2*(car_pos_ang_z*car_pos_ang_z+car_pos_ang_y*car_pos_ang_y)
        self.yaw = math.atan(sinr/cosr)

        if(car_pos_ang_z>=0) and (car_pos_ang_w>=0):
        
            if self.yaw<0 :
            
                self.yaw = self.yaw + np.pi

        elif car_pos_ang_z<0 and car_pos_ang_w<0:
        
            if self.yaw<0 :
                self.yaw = self.yaw + np.pi
        else:
            if self.yaw >=0 :
            
                self.yaw = self.yaw - np.pi
        

    def func_gnss_bool(self):
        return self.gnss_bool
        


def pid_control(target, current):
    """
    Proportional control for the speed.

    :param target: (float)
    :param current: (float)
    :return: (float)
    """
    return Kp * (target - current)


def stanley_control(state, cx, cy, cyaw, last_target_idx):
    """
    Stanley steering control.

    :param state: (State object)
    :param cx: ([float])
    :param cy: ([float])
    :param cyaw: ([float])
    :param last_target_idx: (int)
    :return: (float, int)
    """
    current_target_idx, error_front_axle = calc_target_index(state, cx, cy)

    if last_target_idx >= current_target_idx:
        current_target_idx = last_target_idx

    # theta_e corrects the heading error
    theta_e = normalize_angle(cyaw[current_target_idx] - state.yaw)
    # theta_d corrects the cross track error
    theta_d = np.arctan2(k * error_front_axle, state.v)
    # Steering control
    delta = theta_e + theta_d

    return delta, current_target_idx


def normalize_angle(angle):
    """
    Normalize an angle to [-pi, pi].

    :param angle: (float)
    :return: (float) Angle in radian in [-pi, pi]
    """
    while angle > np.pi:
        angle -= 2.0 * np.pi

    while angle < -np.pi:
        angle += 2.0 * np.pi

    return angle


def calc_target_index(state, cx, cy):
    """
    Compute index in the trajectory list of the target.

    :param state: (State object)
    :param cx: [float]
    :param cy: [float]
    :return: (int, float)
    """
    # Calc front axle position
    fx = state.x  + L * np.cos(state.yaw)
    fy = state.y  + L * np.sin(state.yaw)

    # Search nearest point index
    dx = [fx - icx for icx in cx]
    dy = [fy - icy for icy in cy]
    d = np.hypot(dx, dy)
    target_idx = np.argmin(d)

    # Project RMS error onto front axle vector
    front_axle_vec = [-np.cos(state.yaw + np.pi / 2),
                      -np.sin(state.yaw + np.pi / 2)]
    error_front_axle = np.dot([dx[target_idx], dy[target_idx]], front_axle_vec)

    return target_idx, error_front_axle



def main():
    """Plot an example of Stanley steering control on a cubic spline."""
    #  target course
    #ax = [0.0, 100.0, 100.0, 50.0, 60.0]
    #ay = [0.0, 0.0, -30.0, -20.0, 0.0]
    #ax = [0.0, 50.0, 25.0, 0.0, 25.0]
    #ay = [0.0, 30.0, 70.0, 70.0, -10.0]
    #rospy.init_node("stanley_controll")
    '''
    mark_msg = MarkerArray()
    marker = Marker()
    marker.header.frame_id = "/map"
    marker.type = 2
    marker.action = 2
    marker.pose = Pose()
    marker.color.r = 0.0
    marker.color.g = 0.0
    marker.color.b = 0.0
    marker.color.a = 0.0
    marker.scale.x = 0.1
    marker.scale.y = 0.1
    marker.scale.z = 0.1
    marker.frame_locked = False
    marker.ns = "Goal"
    path_publisher = rospy.Publisher("/path", MarkerArray, queue_size=1)
    '''
    '''
    path_msg = Path()
    path_msg.header.frame_id = "/map"
    pose_msg = PoseStamped()
    path_publisher = rospy.Publisher("/path",Path,queue_size = 1000)
    '''
    
    #nav_msg = NavSatFix()
    #path_publisher = rospy.Publisher("/path",NavSatFix,queue_size = 100)
    #os_msg = NavSatFix()
    #gnss_subscriber = rospy.Subscriber("/gnss",NavSatFix,gnsscallback)

    #ax = [0.0, 100.0,300.0,500.0,900.0, 1000.0, 1500.0, 1000.0,500.0,0.0,-500.0,-10.0]
    #ay = [0.0, 0.0,0.0,0.0,0.0, 0.0, 500.0, 1000.0,1000,1000,500,-10.0]
    ax = [0.0, 10.0,30.0,50.0,90.0, 100.0, 150.0, 100.0,50.0,0.0,-50.0,-1.0]
    ay = [0.0, 0.0,0.0,0.0,0.0, 0.0, 50.0, 100.0,100,100,50,-1.0]
    #ay = [37.11589,37.11589,37.11589,37.11589]
    #ax = [427.1,30,20,0]
    #ay = [37.11589,37.11589]
    #ax = [0 , 100]
    #ay = [0.0,100.0]
    #ax = [0.0,100.0]
    #ax = [126 , 186]
    #ay = [37,97]	
    cx, cy, cyaw, ck, s = cubic_spline_planner.calc_spline_course(
        ax, ay, ds=2.5)
    #print(cx)
    #print(cy)
    '''
    for i in cx[:]:
        pose_msg.pose.position.x = i
    for i in cy[:]:
        pose_msg.pose.position.y = i
        path_msg.poses.append(pose_msg)
    ''' 
    

    '''
    for i in range(0,len(cx)):
        
        nav_msg.longitude = cx[i]
        nav_msg.latitude = cy[i]
        nav_publisher.publish(nav_msg)
        
    	marker.pose.position.x = cx[i]
    	marker.pose.position.y = cy[i]
        mark_msg.markers.append(marker)
    '''
    #path_publisher.publish(mark_msg)  
    #path_publisher.publish(path_msg)
    target_speed = 1.0 / 3.6  # [m/s]

    max_simulation_time = 100.0

    # Initial state
    state = State(x=100.0, y=100.0, yaw=np.radians(20.0), v=0.0)
    
    last_idx = len(cx) - 1
    time = 0.0
    #x = [state.x]
    #y = [state.y]
    #x = []
    #y = []
    #yaw = [state.yaw]
    v = [state.v]
    t = [0.0]
    
    while state.func_gnss_bool():
        print("gnss is not sended")
    
    x = [state.x]
    y = [state.y]
    yaw = [state.yaw]
    
    target_idx, _ = calc_target_index(state, cx, cy)
    while 1: #max_simulation_time >= time and last_idx > target_idx:
        ai = pid_control(target_speed, state.v)
        print("-=-=-=-=-=-=")
        print(state.x)
        print(state.y)
        di, target_idx = stanley_control(state, cx, cy, cyaw, target_idx)
        state.update(ai, di)
        #state.update(ai, di)
        time += dt
       
        x.append(state.x)
        y.append(state.y)
        #x.append(state.x)
        #y.append(state.y)
        yaw.append(state.yaw)
        v.append(state.v)
        t.append(time)
        #path_publisher.publish(mark_msg)
        #path_publisher.publish(path_msg)
        if show_animation:  # pragma: no cover
            plt.cla()
            # for stopping simulation with the esc key.
            plt.gcf().canvas.mpl_connect('key_release_event',
                    lambda event: [exit(0) if event.key == 'escape' else None])
            plt.plot(cx, cy, ".r", label="course")
            plt.plot(x, y, ".b", label="trajectory")
            plt.plot(cx[target_idx], cy[target_idx], "xg", label="target")
            #nav_msg.longitude = cx[target_idx]
            #nav_msg.latitude = cy[target_idx]
            state.set_xy(cx[target_idx],cy[target_idx])
            #path_publisher.publish(nav_msg)
            plt.axis("equal")
            plt.grid(True)
            plt.title("Speed[km/h]:" + str(state.v * 3.6)[:4])
            plt.pause(0.001)

    # Test
'''
    assert last_idx >= target_idx, "Cannot reach goal"

    if show_animation:  # pragma: no cover
        plt.plot(cx, cy, ".r", label="course")
        plt.plot(x, y, "-b", label="trajectory")
        plt.legend()
        plt.xlabel("x[m]")
        plt.ylabel("y[m]")
        plt.axis("equal")
        plt.grid(True)

        plt.subplots(1)
        plt.plot(t, [iv * 3.6 for iv in v], "-r")
        plt.xlabel("Time[s]")
        plt.ylabel("Speed[km/h]")
        plt.grid(True)
        plt.show()
'''

if __name__ == '__main__':
    main()
