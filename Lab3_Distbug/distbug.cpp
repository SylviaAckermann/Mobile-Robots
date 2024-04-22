// **************************************************************************
// Sourcecode from Sylvia Ackermann
// Written in April 2023
// **************************************************************************

#include "eyebot++.h"
#include <cmath>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>

#include <stdio.h>

// global variables of the goal coorinates
int goalX = 3800 - 500;
int goalY = 4700 - 500;
int speed = 200;
int wallDist = 300;
int step = 1000; // 1000, increase step to move around big obstacles
int buffer = 50;

std::array<int, 360> distance;
int x, y, phi;
int hitX, hitY;

enum Enum
{
  DRIVE,
  ROTATE,
  FOLLOW
};
Enum state;

//***************************************************************************//
int heading2goal()
{
  // get heading to goal
  LIDARGet(&distance[0]);
  VWGetPosition(&x, &y, &phi);
  int theta = round(atan2(goalY - y, goalX - x) * 180.0 / M_PI);
  return theta;
}

int angle2goal()
{
  // get difference in current heading and heading to goal
  int theta = heading2goal();
  if (theta > 180.0)
    theta -= 360;
  int diff2goal = round(theta - phi);
  return diff2goal;
}

void turn2goal()
{
  // get difference in heading to goal
  int diff2goal = angle2goal();
  LCDSetPrintf(2, 0, "Angle to goal: %d\n", diff2goal);
  // KEYWait(KEY2);

  // rotate towards goal
  VWTurn(diff2goal, 90); // check if direction is right
  VWWait();
}

bool leaveConditionReached()
{
  // leave condition
  float d_min = 10000.0;
  VWGetPosition(&x, &y, &phi);
  int dx = goalX - x;
  int dy = goalY - y;
  float d = sqrt(dx * dx + dy * dy);
  // LCDPrintf("dx = %d, dy = %d, distance = %3f\n", dx, dy, d);

  // Update minimum distance
  if (d < d_min)
    d_min = d;
  // Calculate free space towards goal
  int theta = heading2goal();
  int angle = 180 - (theta - phi);
  if (angle < 0)
    angle += 360;
  LIDARGet(&distance[0]);
  int f = distance[angle]; // free space
  // LCDPrintf("a=%d d=%f f=%d m=%f \n", angle, d, f, d_min);

  // Check leave condition
  if (d - f <= d_min - step)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void followWall()
{
  int counter = 0;
  bool goalUnreachable = false;
  while (leaveConditionReached() == false)
  {
    OSWait(10);

    counter++;
    // hit point reached? => goal unreachable
    VWGetPosition(&x, &y, &phi);
    LCDSetPrintf(4, 0, "%d. current Position: x = %d, y = %d\n", counter, x, y);
    LCDSetPrintf(5, 0, "Distance to leave point: dx = %d, dy = %d\n", abs(hitX - x), abs(hitY - y));
    if (counter > 100 && abs(hitX - x) < 3 * buffer && abs(hitY - y) < 3 * buffer)
    {
      VWSetSpeed(0, 0);
      LCDPrintf("Goal unreachable!\n");
      exit(EXIT_FAILURE);
      KEYWait(KEY4);
    }

    LIDARGet(&distance[0]);
    if (distance[180] < wallDist)
    {
      VWTurn(90, 90);
      VWWait();
      VWSetSpeed(speed, 0);
    }
    else if (distance[270] < wallDist) // closer to wall than minimum distance
    {
      VWTurn(1, 90);
      VWWait();
      VWSetSpeed(speed, 0);
    }
    else if (distance[250] > distance[290]) // too far away
    {
      VWTurn(-1, 90);
      VWWait();
      VWSetSpeed(speed, 0);
    }
    else if (distance[250] > distance[290]) // too close
    {
      VWTurn(1, 90);
      VWWait();
      VWSetSpeed(speed, 0);
    }
  }

  if (goalUnreachable == false)
  {
    // leave condition reached
    VWSetSpeed(0, 0);
    LCDSetPrintf(1, 0, "     LEAVING     \n");
    VWStraight(300, 100); // move away from point
    VWWait();

    // turn towards goal
    turn2goal();

    state = DRIVE;
  }
}

//***************************************************************************//

int main()
{
  LCDPrintf("Lets start ... \n");
  LCDMenu("Start", "Stop", "", "End");

  while (true)
  {
    int key = KEYRead();
    if (key == KEY1)
    {
      // rotate towards goal
      turn2goal();

      state = DRIVE;
      int obstacleAngle;

      while (true)
      {
        // check if goal found
        if (abs(goalX - x) < buffer && abs(goalY - y) < buffer)
        {
          LCDSetPrintf(3, 0, "Goal found !");
          VWSetSpeed(0, 0);
          break;
        }

        // calculate angle to goal
        int diff2goal = angle2goal();

        switch (state)
        {
        case DRIVE:
          LCDSetPrintf(1, 0, "     DRIVE     \n");
          LIDARGet(&distance[0]);
          if (distance[180] < 1.5 * wallDist || distance[150] < wallDist || distance[210] < wallDist)
          // front: distance[180], left distance[150], right distance[210]
          {
            VWSetSpeed(0, 0);

            // find minimal distance to wall
            auto min = std::min_element(distance.cbegin(), distance.cend());
            int minIdx = std::distance(min, distance.cbegin());
            // LCDPrintf("angle to wall: %d", minIdx);
            obstacleAngle = minIdx - 180;
            if (obstacleAngle < -90)
              obstacleAngle += 360;
            // LCDPrintf("obstacle angle: %d", obstacleAngle);

            // save hit point
            VWGetPosition(&hitX, &hitY, &phi);
            hitX = x;
            hitY = y;
            LCDSetPrintf(3, 0, "Hitpoint: hitX = %d, hitY = %d\n", hitX, hitY);
            state = ROTATE;
          }
          else if (abs(diff2goal) > 1.0)
            VWSetSpeed(speed, diff2goal);
          else
            VWSetSpeed(speed, 0);
          continue;

        case ROTATE:
          LCDSetPrintf(1, 0, "     ROTATE     \n");
          VWTurn(90 + obstacleAngle, 90);
          VWWait();
          state = FOLLOW;
          continue;

        case FOLLOW:
          VWSetSpeed(speed, 0);
          LCDSetPrintf(1, 0, "     FOLLOW     \n");
          // wall following
          followWall();
        }
      }
    }
    if (key == KEY4)
    {
      break;
    }
  }

  return 0;
}
