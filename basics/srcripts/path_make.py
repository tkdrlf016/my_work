import math
import numpy as np
import matplotlib.pyplot as plt
import reeds_shepp as rs
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped

class C:
     # PID config
    Kp = 0.3  # proportional gain

    # system config
    Ld = 2.6  # look ahead distance
    kf = 0.1  # look forward gain
    dt = 0.1  # T step
    dist_stop = 0.7  # stop distance
    dc = 0.0

    # vehicle config
    RF = 3.3  # [m] distance from rear to vehicle front end of vehicle
    RB = 0.8  # [m] distance from rear to vehicle back end of vehicle
    W = 2.4  # [m] width of vehicle
    WD = 0.7 * W  # [m] distance between left-right wheels
    WB = 1.0  # [m] Wheel base
    TR = 0.44  # [m] Tyre radius
    TW = 0.7  # [m] Tyre width
    MAX_STEER = 0.30
    MAX_ACCELERATION = 5.0

def generate_path(s):

    max_c = math.tan(C.MAX_STEER) / C.WB  # max curvature

    path_x, path_y, yaw, direct = [], [], [], []
    x_rec, y_rec, yaw_rec, direct_rec = [], [], [], []
    direct_flag = 1.0

    for i in range(len(s) - 1):
        s_x, s_y, s_yaw = s[i][0], s[i][1], np.deg2rad(s[i][2])
        g_x, g_y, g_yaw = s[i + 1][0], s[i + 1][1], np.deg2rad(s[i + 1][2])

        path_i = rs.calc_optimal_path(s_x, s_y, s_yaw,
                                      g_x, g_y, g_yaw, max_c)
        
        #print(path_i.x)
        #print(path_i.y)
         
        ix = path_i.x
        iy = path_i.y
        iyaw = path_i.yaw
        idirect = path_i.directions

        for j in range(len(ix)):
            if idirect[j] == direct_flag:
                x_rec.append(ix[j])
                y_rec.append(iy[j])
                yaw_rec.append(iyaw[j])
                direct_rec.append(idirect[j])
            else:
                if len(x_rec) == 0 or direct_rec[0] != direct_flag:
                    direct_flag = idirect[j]
                    continue

                path_x.append(x_rec)
                path_y.append(y_rec)
                yaw.append(yaw_rec)
                direct.append(direct_rec)
                x_rec, y_rec, yaw_rec, direct_rec = \
                    [x_rec[-1]], [y_rec[-1]], [yaw_rec[-1]], [-direct_rec[-1]]

    path_x.append(x_rec)
    path_y.append(y_rec)
    yaw.append(yaw_rec)
    direct.append(direct_rec)

    x_all, y_all = [], []

    for ix, iy in zip(path_x, path_y):
        x_all += ix
        y_all += iy

    return path_x, path_y, yaw, direct, x_all, y_all


def main():
    # generate path: [x, y, yaw]
    #states = [(0, 0, 0), (20, 15, 0), (35, 20, 90), (40, 0, 0),
    #         (20, 0, 120), (5, -10, 180), (15, 5, 30)]
    #states = [(0,0,0),(20,0,10),(30,0,0),(40,20,30),(60,40,0),(80,20,-90),(80,-40,-90),(60,-60,180),(40,-40,90),(20,-20,180),(0,-20,180)]
    states = [(0,0,90),(5,5,45),(20,10,180),(40,5,-45),(60,0,180)]
    #states = [[0,0,0],[10,0,0],[20,20,0]]
    #states = [(0,0,0),(10,0,0),(20,0,0),(30,0,0),(40,0,0),(50,0,0)]
    #states = [(-3, 3, 120), (10, -7, 30), (10, 13, 30), (20, 5, -25),
    #           (35, 10, 180), (30, -10, 160), (5, -12, 90)]

    x, y, yaw, direct, path_x, path_y = generate_path(states)
    pick_x = 0
    pick_y = 0
    path = Path()
    pose_msg = PoseStamped()
    for i in path_x[:]:
        if pick_x != 0:
            path_x.remove(i)
        pick_x += 1
        if pick_x == 5:
            pick_x = 0
            pose_msg.pose.position.x = i

    for i in path_y[:]:
        if pick_y != 0:
            path_y.remove(i)
        pick_y += 1
        if pick_y == 5:
            pick_y = 0
            pose_msg.pose.position.y = i
            path.poses.append(pose_msg)
            
    print(path_x)
    print(path_y)
    plt.plot(path_x, path_y, color='gray', linewidth=2)
    plt.show()
    return path
