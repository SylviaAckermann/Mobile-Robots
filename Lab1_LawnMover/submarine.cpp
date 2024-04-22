
#include "eyebot++.h"

#define SPEED 70
#define DIST2WALL 0.15 * 1e3 //[mm]

// PSD IDs.
const int PSD_DOWN = 6;
// Thruster IDs.
const int LEFT = 1;
const int FRONT = 2;
const int RIGHT = 3;
const int BACK = 4;

void up(float secs)
{
    LCDPrintf("UP\n");
    MOTORDrive(FRONT, SPEED);
    MOTORDrive(BACK, SPEED);
    sleep(secs);
    MOTORDrive(FRONT, 0);
    MOTORDrive(BACK, 0);
}
void down(float secs)
{
    LCDPrintf("DOWN\n");
    MOTORDrive(FRONT, -SPEED);
    MOTORDrive(BACK, -SPEED);
    sleep(secs);
    MOTORDrive(FRONT, 0);
    MOTORDrive(BACK, 0);
}
void forward(float secs)
{
    // LCDPrintf("FORWARD\n");
    MOTORDrive(LEFT, SPEED);
    MOTORDrive(RIGHT, SPEED);
    sleep(secs);
    MOTORDrive(LEFT, 0);
    MOTORDrive(RIGHT, 0);
}
void backward(float secs)
{
    // LCDPrintf("BACKWARD\n");
    MOTORDrive(LEFT, -SPEED);
    MOTORDrive(RIGHT, -SPEED);
    sleep(secs);
    MOTORDrive(LEFT, 0);
    MOTORDrive(RIGHT, 0);
}
void turn_left(float secs)
{
    // LCDPrintf("LEFT\n");
    MOTORDrive(LEFT, -SPEED);
    MOTORDrive(RIGHT, SPEED);
    sleep(secs);
    MOTORDrive(LEFT, 0);
    MOTORDrive(RIGHT, 0);
}
void turn_right(float secs)
{
    // LCDPrintf("RIGHT\n");
    MOTORDrive(LEFT, SPEED);
    MOTORDrive(RIGHT, -SPEED);
    sleep(secs);
    MOTORDrive(LEFT, 0);
    MOTORDrive(RIGHT, 0);
}

void moveForward2Wall()
{
    // move straight in front direction until wall within 15 cm distance
    LCDPrintf("--- moving forward---\n");
    while (PSDGet(PSD_FRONT) > 2 * DIST2WALL)
    {
        // LCDPrintf("Distance F = %5d mm\n", PSDGet(PSD_FRONT));
        forward(1);
    }
    LCDPrintf("STOP\n");
}

void rightTurnAlign()
{
    LCDPrintf("--- Turning right ---\n");
    turn_right(1);
    int distF;
    do
    {
        distF = PSDGet(PSD_FRONT);
        turn_right(0.1);
        // LCDPrintf("Distance F = %3d\n", distF);
    } while (abs(distF - PSDGet(PSD_FRONT)) > 2);
}

void followWall(int stopDist)
{
    LCDPrintf("--- follow wall ---\n");
    int distLeft, distRight;
    while (PSDGet(PSD_FRONT) > stopDist)
    {
        distLeft = PSDGet(PSD_LEFT);
        distRight = PSDGet(PSD_RIGHT);
        forward(1);

        // LCDPrintf("Measured Distance[0]   L = %3d, R = %3d\n", distLeft, distRight);
        // LCDPrintf("Measured Distance[1]   L = %3d, R = %3d\n", PSDGet(PSD_LEFT), PSDGet(PSD_RIGHT));

        if (PSDGet(PSD_LEFT) < distLeft && PSDGet(PSD_RIGHT) > distRight) // too close
        {
            LCDPrintf("turn right\n");
            turn_right(1);
        }
        if (PSDGet(PSD_LEFT) > distLeft && PSDGet(PSD_RIGHT) < distRight) // too far away
        {
            LCDPrintf("turn left\n");
            turn_left(1);
        }

        if (KEYRead() == KEY2)
        {
            KEYWait(KEY3);
        }
    }
    LCDPrintf("STOP\n");
}

int main()
{
    LCDPrintf("Let's watch the submarine in the pool \n");
    LCDMenu("START DIVE", "STOP", "CONTINUE", "END");
    KEYWait(KEY1);
    // int cornerCount = 0;

    // move to wall and align
    moveForward2Wall();
    while (PSDGet(PSD_FRONT) < 2 * DIST2WALL)
    {
        rightTurnAlign();
    }
    for (size_t cornerCount = 0; cornerCount <= 4; cornerCount++)
    {
        followWall(2 * DIST2WALL);
        LCDPrintf("%d. corner\n", cornerCount);
        rightTurnAlign();
    }
    MOTORSpeed(1, 0);
    LCDPrintf("The loop around the pool is finished \n");

    // aligning2Wall();
    // lineMove();

    /*while (true)
    {f
        if (PSDGet(PSD_FRONT) < 2000)
        {
            ++cornerCount;
            turn_right(5);
            LCDPrintf("%d. corner\n", cornerCount);
        }
        else if (PSDGet(PSD_LEFT) > 2000)
        {
            turn_left(1);
            LCDPrintf("turn left\n");
        }
        else if (PSDGet(PSD_LEFT) < 2000)
        {
            turn_right(1);
            LCDPrintf("turn right\n");
        }
        forward(1);
        LCDPrintf("move forward\n");
        if (cornerCount >= 4)
            MOTORSpeed(1, 0);
    } */

    KEYWait(KEY4);
    return 0;
}
