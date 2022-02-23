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
from std_msgs.msg import Int32
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

    def __init__(self, x=0.0, y=100.0, yaw=0.0, v=0.0):
        super(State, self).__init__()
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v
        self.curr_x = 0
        self.curr_y = 0 
        self.curr_section = 0
        rospy.init_node("stanley_controll")
        self.nav_msg = NavSatFix()
        self.path_publisher = rospy.Publisher("/path",NavSatFix,queue_size = 100)
        self.ros_msg = NavSatFix()
        self.vel_msg = Int32()
        self.gnss_bool = 1
        self.gnss_subscriber = rospy.Subscriber("/gnss",NavSatFix,self.gnsscallback)
        self.car_yaw = rospy.Subscriber("/filter/quaternion",QuaternionStamped,self.yawcallback)
    def find_curr_section(self):
        A =1


    def switch_vel(self):
        B =1

    def set_xy(self,x,y):
        self.nav_msg.longitude = x
        self.nav_msg.latitude = y
        self.path_publisher.publish(self.nav_msg)

    def gnsscallback(self,data):
        self.gnss_bool = 0
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

    return Kp * (target - current)


def stanley_control(state, cx, cy, cyaw, last_target_idx):

    current_target_idx, error_front_axle = calc_target_index(state, cx, cy)
    current_target_idx += 5
    if last_target_idx >= current_target_idx:
        current_target_idx = last_target_idx

    theta_e = normalize_angle(cyaw[current_target_idx] - state.yaw)
    theta_d = np.arctan2(k * error_front_axle, state.v)
    delta = theta_e + theta_d

    return delta, current_target_idx


def normalize_angle(angle):

    while angle > np.pi:
        angle -= 2.0 * np.pi

    while angle < -np.pi:
        angle += 2.0 * np.pi

    return angle


def calc_target_index(state, cx, cy):
    fx = state.x  + L * np.cos(state.yaw)
    fy = state.y  + L * np.sin(state.yaw)

    dx = [fx - icx for icx in cx]
    dy = [fy - icy for icy in cy]
    d = np.hypot(dx, dy)
    target_idx = np.argmin(d)

    front_axle_vec = [-np.cos(state.yaw + np.pi / 2),
                      -np.sin(state.yaw + np.pi / 2)]
    error_front_axle = np.dot([dx[target_idx], dy[target_idx]], front_axle_vec)

    return target_idx, error_front_axle



def main():
    ax = [0, 40 , 80, 120, 160]
    ay = [0, 150, 0, 150, 0]
    between_vel = [10,11,12,13]
    cx, cy, cyaw, ck, s ,a = cubic_spline_planner.calc_spline_course(
        ax, ay, ds=2.5)
    target_speed = 0 / 3.6  # [m/s]

    max_simulation_time = 100.0

    state = State(x=0.0, y=20.0, yaw=np.radians(90.0), v=0.0)
    
    last_idx = len(cx) - 1
    time = 0.0
    v = [state.v]
    t = [0.0]
    x = [state.x]
    y = [state.y]
    yaw = [state.yaw]
    
    target_idx, _ = calc_target_index(state, cx, cy)
    rate = rospy.Rate(5)
    while 1: 
        ai = pid_control(target_speed, state.v)
 
        time += dt
        state.x += 0.2
        x.append(state.x)
        y.append(state.y)
 
        yaw.append(state.yaw)
        v.append(state.v)
        t.append(time)

        di, target_idx = stanley_control(state, cx, cy, cyaw, target_idx)
        print((state.yaw*180/3.141592)-(math.atan2((cy[target_idx]-state.y),(cx[target_idx]-state.x)))*180/3.141592)
        for i in range(1,len(a)):
            if (target_idx <= a[i]) and (target_idx >= a[i-1]):
                print(i)
                print(between_vel[i-1])
        rate.sleep()



        if show_animation:
            plt.cla()
            plt.plot(cx, cy, ".r", label="course")
            plt.plot(x, y, ".b", label="trajectory")
            plt.plot(cx[target_idx], cy[target_idx], "xg", label="target")
            state.set_xy(cx[target_idx],cy[target_idx])
            plt.axis("equal")
            plt.grid(True)
            plt.title("Speed[km/h]:" + str(state.v * 3.6)[:4])
            plt.pause(0.001)

if __name__ == '__main__':
    main()
