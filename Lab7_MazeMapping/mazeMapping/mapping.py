#!/usr/bin/env python3
from eye import *
import math

SIM_WORLD_SIZE = 2000
SPEED = 100
WALLDIST = 300


def drawLine(x, y, lidar_x, lidar_y):
    if lidar_x > SIM_WORLD_SIZE:
        lidar_x = SIM_WORLD_SIZE
    if lidar_x < 0:
        lidar_x = 0
    if lidar_y > SIM_WORLD_SIZE:
        lidar_y = SIM_WORLD_SIZE
    if lidar_y < 0:
        lidar_y = 0
    LCDLine(2*int(128*x/SIM_WORLD_SIZE), int(128 * 2 * (1-y/SIM_WORLD_SIZE)), 2 *
            int(128*lidar_x/SIM_WORLD_SIZE), 256-2*int(128*lidar_y/SIM_WORLD_SIZE), WHITE)
    LCDCircle(2*int(128*lidar_x/SIM_WORLD_SIZE), 256-2 *
              int(128*lidar_y/SIM_WORLD_SIZE), 5, GREEN, 1)


def mapping():

    lidarValues = LIDARGet()  # (360) elements
    # left: lidarValues[90]
    # front: lidarValues[180]
    # right: lidarValues[270]

    x, y, phi = VWGetPosition()
    # LCDCircle(2*int(128*x/SIM_WORLD_SIZE), int(128* 2 * (1-y/SIM_WORLD_SIZE)), 5, RED, 1)

    # 360 elements: [-180°... 9° 180]
    for angle, lidarValue in enumerate(lidarValues):
        if lidarValue > SIM_WORLD_SIZE:  # Maximum value, so don't bother drawing a line until we can see the end
            continue

        angle_to_object = phi - angle + 180

        lidar_x = int(x+lidarValue*math.cos(angle_to_object*math.pi/180))
        lidar_y = int(y+lidarValue*math.sin(angle_to_object*math.pi/180))

        drawLine(x, y, lidar_x, lidar_y)
        # KEYWait(KEY3)


def followWall():
    VWSetSpeed(SPEED, 0)
    aligned2Wall = False

    while (KEYRead() != KEY2):

        OSWait(10)
        mapping()
        distance = LIDARGet()

        if (distance[180] < 1.5*WALLDIST):
            print("aligning to wall")
            # find minimal distance to wall
            minimum = 100000
            minIdx = -1
            for i in range(0, len(distance)):
                if distance[i] < minimum:
                    minimum = distance[i]
                    minIdx = i
            obstacleAngle = minIdx - 180
            print("obstacleAngle: ", obstacleAngle)
            if obstacleAngle < -90:
                obstacleAngle += 360
            VWTurn(obstacleAngle, 90)
            VWWait()
            VWSetSpeed(SPEED, 0)
            aligned2Wall = True
        # closer to wall than minimum distance
        elif (distance[270] < WALLDIST):
            print("turn right")
            VWTurn(1, 90)
            VWWait()
            VWSetSpeed(SPEED, 0)
        elif aligned2Wall and (distance[250] > distance[290]):  # too far away
            print("turn left")
            VWTurn(-1, 90)
            VWWait()
            VWSetSpeed(SPEED, 0)
        elif aligned2Wall and (distance[250] > distance[290]):  # // too close
            print("")
            VWTurn(1, 90)
            VWWait()
            VWSetSpeed(SPEED, 0)
    else:
        VWSetSpeed(0, 0)


if __name__ == "__main__":
    # SIMSetRobot(0, 300, 300, 100, -90)
    # VWSetPosition(180,180,0)
    VWSetPosition(1200, 1200, 90)

    LCDMenu("Start", "Mapping", "Stop", "End")

    # Initialise the map with a grey box
    KEYWait(KEY1)
    LCDArea(0, 0, 256, 256, NAVY, 1)
    x = 180
    y = 180
    LCDCircle(2*int(128*x/SIM_WORLD_SIZE),
              int(128 * 2 * (1-y/SIM_WORLD_SIZE)), 5, RED, 1)

    KEYWait(KEY2)
    followWall()

    KEYWait(KEY4)
