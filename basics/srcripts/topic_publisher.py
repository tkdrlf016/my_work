#!/usr/bin/env python
import rospy
from std_msgs.msg import Int32
import numpy as np
import matplotlib.pyplot as plt
import path_make as pm
from nav_msgs.msg import Path


rospy.init_node("topic_publisher")
path_msg = Path()
#pub = rospy.Publisher('counter',Int32,queue_size=10)
path = rospy.Publisher('Path',Path,queue_size=1000)
rate = rospy.Rate(2)
print("do!")
path_msg = pm.main()


while not rospy.is_shutdown():
	#pub.publish(count)
	#count +=1
	path.publish(path_msg)
	print("ss")
	rate.sleep()
