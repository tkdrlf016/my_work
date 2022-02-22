#!/usr/bin/env python
import time
import serial
import rospy
from std_msgs.msg import Int32

rospy.init_node("uart_driver_node")
pub = rospy.Publisher("/lory/emergency",Int32)
print("UART Demonstration Program!!")
print("NVIDIA Jetson Nano Developer Kit!!")
msg = 0

serial_port = serial.Serial(
    port="/dev/ttyUSB0",
    baudrate=9600,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
)
# Wait a second to let the port initialize
#time.sleep(1)

try:
    # Send a simple header
    serial_port.write("UART Demonstration Program\r\n".encode())
    serial_port.write("NVIDIA Jetson Nano Developer Kit\r\n".encode())
    while True:
        if serial_port.inWaiting() > 0:
            data = serial_port.read()
            #print(data)
            if data == '1':
                print('not ok')
                msg = 1
            else :
                print('ok')
                msg = 0
            pub.publish(msg)
            serial_port.write(data)
            # if we get a carriage return, add a line feed too
            # \r is a carriage return; \n is a line feed
            # This is to help the tty program on the other end 
            # Windows is \r\n for carriage return, line feed
            # Macintosh and Linux use \n
            if data == "\r".encode():
                # For Windows boxen on the other end
                serial_port.write("\n".encode())


except KeyboardInterrupt:
    print("Exiting Program")

except Exception as exception_error:
    print("Error occurred. Exiting Program")
    print("Error: " + str(exception_error))

finally:
    serial_port.close()
    pass
