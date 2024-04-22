// **************************************************************************
// Sourcecode from Sylvia Ackermann
// Written in March 2023
// **************************************************************************

#include "eyebot++.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdio.h>

// Spline parameters
float interval, k;
int speed;

//***************************************************************************//

void SplineDrive(int bx, int by, int alpha0)
{
  LCDPrintf("\n\nSplineDrive(%d, %d, %d)\n", bx, by, alpha0);
  float alpha = alpha0 * M_PI / 180.0;
  // LCDPrintf("alpha = %3f\n", alpha);
  int ax, ay, phi;
  VWGetPosition(&ax, &ay, &phi);

  int lastX = ax;
  int lastY = ay;
  int steps = ceil(1 / interval); // number of steps

  for (float u = 0.0; u <= 1.0001; u += interval)
  {

    // calculate hermite spline parameters
    float h1 = 2 * std::pow(u, 3) - 3 * std::pow(u, 2) + 1;
    float h2 = -2 * std::pow(u, 3) + 3 * std::pow(u, 2);
    float h3 = std::pow(u, 3) - 2 * std::pow(u, 2) + u;
    float h4 = std::pow(u, 3) - std::pow(u, 2);

    float len = k * std::sqrt(std::pow(bx - ax, 2) + std::pow(by - ay, 2));
    float Dax = len;
    float Day = 0;
    float Dbx = len * std::cos(alpha);
    float Dby = len * std::sin(alpha);

    // calculate coordinate of next waypoint
    float sx = ax * h1 + bx * h2 + Dax * h3 + Dbx * h4;
    float sy = ay * h1 + by * h2 + Day * h3 + Dby * h4;

    // calculate heading towards waypoint
    int sphi = round(atan2(sy - lastY, sx - lastX) * 180.0 / M_PI);

    // move toward next waypoint
    float sDist = len / (steps * k);
    int rx, ry, rphi;
    VWGetPosition(&rx, &ry, &rphi);
    VWCurve(sDist, sphi - rphi, speed);
    VWWait();

    lastX = sx;
    lastY = sy;

    // LCDPrintf("Waypoint: u = %2f:  sx = %3f,  sy = %3f, sphi = %d \n", u, sx, sy, sphi);
  }
}

void readWaypoints(int points[20][2])
{
  FILE *file = fopen("way.txt", "r");
  int i = 0;
  while (fscanf(file, "%d %d", &points[i][0], &points[i][1]) == 2)
  {
    // printf("x:%d y:%d\n", points[i][0], points[i][1]);
    i++;
  }
  fclose(file);
}

int main()
{
  LCDPrintf("Lets move along the spline \n");
  LCDMenu("Task 1", "Task 2", "CONTINUE", "END");
  int d = 1000;
  SIMSetRobot(0, d, d, 0, 0); // SIMSetRobot (int id, int  x, int  y, int  z, int  phi);

  while (true)
  {
    int key = KEYRead();
    if (key == KEY1)
    {
      VWSetPosition(0, 0, 0); // VWSetPosition(int x, int y, int phi);
      LCDPrintf("Task 1\n");
      int spline;
      std::cout << "Enter Spline-Case-Nr ";
      std::cin >> spline;
      switch (spline)
      {
      case 1:
        interval = 0.1;
        k = 1.5;
        speed = 200;
        SplineDrive(1000, 0, 0); // K = 1.5
        break;

      case 2:
        interval = 0.1;
        k = 1.5;
        speed = 200;
        SplineDrive(0, 1000, 90); // k = 1.5
        break;

      case 3:
        interval = 0.1;
        k = 2;
        speed = 200;
        SplineDrive(0, 1000, 0); // K = 1.5
        break;

      case 4:
        interval = 0.06; // 0.06
        k = 4;           // 4
        speed = 50;      // 100
        SplineDrive(-500, -10, 0);
        break;

      case 5:
        interval = 0.1; // 0.06
        k = 3.0;         // 4.0
        speed = 100;     // 100
        SplineDrive(-1000, -1000, -45);
      }
      VWSetPosition(-d, -d, 0);
      break;
    }
    if (key == KEY2)
    {
      VWSetPosition(d, d, 0); // VWSetPosition(int x, int y, int phi);
      LCDPrintf("Task 2\n");
      int points[20][2];
      readWaypoints(points);

      while (true)
      {
        for (size_t n = 0; n < 20; n++)
        {
          if (KEYRead() == KEY4)
          {
            break;
          }

          if (points[n][0] == 0 && points[n][1] == 0)
          {
            LCDPrintf("\nDriving to all waypoints done!\n");
            break;
          }
          else
          {
            interval = 0.05; // 0.05
            k = 1.5;         // 1.5
            speed = 100;
            SplineDrive(points[n][0], points[n][1], 0);
            // KEYWait(KEY3);
          }
        }
      }
    }
    if (key == KEY4)
    {
      break;
    }
  }

  KEYWait(KEY4);
  return 0;
}
