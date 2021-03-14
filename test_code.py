from jetbot import Robot
import time

robot = Robot()
print("Running test now. Setting speed to 0.3")
robot.left(speed=0.3)
time.sleep(0.5)
robot.stop()
print("And done")
quit()
