
#include "eyebot++.h"
#include <string>

#define SPEED 300
#define DIST2WALL 0.15 * 1e3 //[mm]

void aligning2Wall();
void moveForward2Wall();
void cornerTurning(std::string direction);
void lineMove(int stopDist = DIST2WALL);

int main()
{
  LCDPrintf("Welcome to this Lawn Mower demonstration \n");
  LCDMenu("RUN", "STOP", "|>", "END");
  KEYWait(KEY1);

  // move robot to starting position in the corner
  moveForward2Wall();
  aligning2Wall();
  lineMove();             // followWall(DIST2WALL, "left");
  cornerTurning("right"); // turn right

  // start lawn-mowing-pattern
  float squareWidth = 1900; // [mm]

  LCDPrintf("\n# # # # # # # Start lawn moving pattern # # # # # # # \n\n");
  lineMove(); // followWall(DIST2WALL, "left");

  // First Line
  cornerTurning("right"); // turn right
  float step = 450;       // [mm]
  float frontDist = squareWidth - 2 * DIST2WALL - step;
  LCDPrintf("Distance to move: frontDist = %3f\n", frontDist);
  lineMove(frontDist); // followWall(frontDist, "left");
  VWTurn(-90, 90);     // turn right / clock-wise
  VWWait();
  lineMove();

  // Second Line
  VWTurn(90, 90); // turn left
  VWWait();
  frontDist -= step;
  LCDPrintf("Distance to move: frontDist = %3f\n", frontDist);
  lineMove(frontDist); // followWall(frontDist, "right");
  VWTurn(90, 90);      // turn left
  VWWait();
  lineMove();

  // bottom Line
  VWTurn(-90, 90); // turn right
  VWWait();
  lineMove();             // followWall(DIST2WALL, "left");
  cornerTurning("right"); // aligning2Wall();
  lineMove();

  KEYWait(KEY4);
  return 0;
}

//***************************************************************************//

void moveForward2Wall()
{
  // move straight in front direction until wall within 15 cm distance
  VWSetSpeed(SPEED, 0); // drive straight 300mm/s
  LCDPrintf("--- moving forward---\n");
  while (PSDGet(PSD_FRONT) > DIST2WALL)
  {
    OSWait(100);
  }
  VWSetSpeed(0, 0);
}

void aligning2Wall()
{
  int distL = PSDGet(PSD_LEFT);
  int distR = PSDGet(PSD_RIGHT);
  int distF = PSDGet(PSD_FRONT);
  LCDPrintf("Distances: L = %3d, R = %3d, F = %3d\n", distL, distR, distF);

  // calculate angle between robot's path and wall
  if (distL > distR)
  {
    LCDPrintf("L > R \n");
    float alpha = atan2(distR, distF) * 180 / M_PI;
    LCDPrintf("alpha = %2f\n", alpha);
    // rotate to be parallel to wall
    float beta = 90 - alpha;
    VWTurn(-(90 + beta), 90);
    VWWait();
  }
  else if (distR > distL)
  {
    LCDPrintf("R > L \n");
    float alpha = atan2(distL, distF) * 180 / M_PI;
    LCDPrintf("alpha = %2f\n", alpha);
    // rotate to be parallel to wall
    VWTurn(-alpha, 90);
    VWWait();
  }
}

void cornerTurning(std::string direction)
{
  LCDPrintf("--- Turning in corner ---\n");
  int distF;
  if (direction == "right")
  {
    VWTurn(-80, 90);
    VWWait();
    do
    {
      distF = PSDGet(PSD_FRONT);
      VWTurn(-0.1, 90);
      VWWait();
    } while (abs(distF - PSDGet(PSD_FRONT)) > 2);
  }
  else if (direction == "left")
  {
    VWTurn(80, 90);
    VWWait();
    do
    {
      distF = PSDGet(PSD_FRONT);
      VWTurn(0.1, 90);
      VWWait();
    } while (abs(distF - PSDGet(PSD_FRONT)) > 2);
  }
}

void lineMove(int stopDist)
{
  LCDPrintf("--- mowing along path ---\n");
  VWSetSpeed(SPEED, 0); // drive straight 300mm/s
  int distLeft, distRight;
  while (PSDGet(PSD_FRONT) > stopDist)
  {
    distLeft = PSDGet(PSD_LEFT);
    distRight = PSDGet(PSD_RIGHT);
    OSWait(10); // OSWait(10)

    // LCDPrintf("Measured Distance[0]   L = %3d, R = %3d\n", distLeft, distRight);
    // LCDPrintf("Measured Distance[1]   L = %3d, R = %3d\n", PSDGet(PSD_LEFT), PSDGet(PSD_RIGHT));

    if (PSDGet(PSD_LEFT) < distLeft && PSDGet(PSD_RIGHT) > distRight) // too close
    {
      LCDPrintf("move to right\n");
      VWTurn(-1, 90);
      VWWait();
      VWSetSpeed(SPEED, 0);
    }
    if (PSDGet(PSD_LEFT) > distLeft && PSDGet(PSD_RIGHT) < distRight) // too far away
    {
      LCDPrintf("move to left\n");
      VWTurn(10, 90);
      VWWait();
      VWSetSpeed(SPEED, 0);
    }

    if (KEYRead() == KEY2)
    {
      VWSetSpeed(0, 0);
      KEYWait(KEY3);
      VWSetSpeed(SPEED, 0);
    }
  }
  LCDPrintf("STOP\n");
  VWSetSpeed(0, 0);
}
