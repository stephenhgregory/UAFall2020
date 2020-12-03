#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
import random
import time

class ForceMapper(): 
    def __init__(self):
        self.r = rospy.Rate(250) # 250hz
        self.linear_speed = 0.8
        self.angular_speed = 0.8
        self.max_velocity = 0.8
        self.min_velocity = -0.2
        self.max_range = 5
        self.turn_right_probability = 0.5
        self.at_start = True
        # self.currently_stopped_and_turning = True
        # self.turning_direction = 'left'
        self.currently_stopped_and_turning = False
        self.t1 = time.time()
        self.move_cmd = Twist()
        self.sub = rospy.Subscriber('/scan', LaserScan, self.callBack)
        self.cmd_pub = rospy.Publisher('cmd_vel_mux/input/navi', Twist, queue_size=1)
        self.start()

    def forward(self):
        self.move_cmd.linear.x = self.linear_speed

    def forward_slight_left(self):
        self.move_cmd.linear.x = self.linear_speed
        self.move_cmd.angular.z = 0.1

    def turn_stochastically(self, right_probability):
        if random.random() < right_probability:
            # Turn right
            self.move_cmd.angular.z = 2.0 * -self.angular_speed
            self.turning_direction = 'right'
        else:
            # Turn left
            self.move_cmd.angular.z = 2.0 * self.angular_speed
            self.turning_direction = 'left'

    def turn_left(self):
        self.move_cmd.angular.z = 2 * self.angular_speed

    def turn_right(self):
        self.move_cmd.angular.z = 2 * -self.angular_speed

    def stop_turning(self):
        self.move_cmd.angular.z = 0

    def stop(self):
        self.move_cmd.linear.x = 0

    def set_proportional_velocity(self, percentage, max_velocity=0.8):
        if not self.move_cmd.linear.x >= max_velocity:
            self.move_cmd.linear.x = max_velocity * percentage

    def handle_about_to_collide(self, left, right):
        self.stop()
        if left < right:
            self.turn_right_probability = 0.85
        elif right < left:
            self.turn_right_probability = 0.15
        self.turn_stochastically(right_probability=self.turn_right_probability)
        return

    def keep_turning_in_special_case(self):
        if self.turning_direction == 'right':
            self.turn_right()
        elif self.turning_direction == 'left':
            self.turn_left()

    def start_behavior(self, msg):
        size = len(msg.ranges)
        front = msg.ranges[size/2]

        if (front < 5):
            self.turn_left()
        else:
            self.at_start = False

    def master_behavior(self, msg):
        size = len(msg.ranges)
        right = msg.ranges[0]
        front_right = msg.ranges[size/4]
        front = msg.ranges[size/2]
        front_left = msg.ranges[3*size/4]
        left = msg.ranges[size-1]
        print("l,f,r: ", left, front, right)

        # Handle if we are in the special case of currently turning
        if front < 2.0 and self.currently_stopped_and_turning:
            self.keep_turning_in_special_case()
            return

        # Reset the special case variable (currently stopped and turning)
        self.currently_stopped_and_turning = False

        if front < 2.5 or right < 0.8 or left < 0.8:

            # Handle if we are about to hit a wall
            if front < 1.0 or right < 0.5 or left < 0.5:
                self.handle_about_to_collide(left, right)
                self.currently_stopped_and_turning = True
                return

            # Set the velocity of the robot proportional to its proximity to a wall in front of itself
            self.set_proportional_velocity(percentage=(self.max_range / front), max_velocity=self.max_velocity)

            # Handle turning left or right when encountering a wall            
            if front_right < front:
                self.turn_left()
            elif front_left < front:
                self.turn_right()
            else:
                self.turn_stochastically(right_probability=0.5)

        else:
            self.stop_turning()
            # self.forward_slight_left()
            self.forward()
        
    def start(self):
        while not rospy.is_shutdown():
            self.cmd_pub.publish(self.move_cmd)
            self.r.sleep()

    def callBack(self, msg):
        if self.at_start:
            self.start_behavior(msg)
        else:
            self.master_behavior(msg)


def main():
    rospy.init_node('ForceMapper')
    try:
        force = ForceMapper()
    except rospy.ROSInterruptException:
        pass

if __name__ == '__main__':
    main()
