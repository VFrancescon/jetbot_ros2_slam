from jetbot import Robot
import time

robot = Robot()
print("Running test now. Setting speed to 0.5")
robot.foward(speed=0.5)
time.sleep(5)
robot.stop()

