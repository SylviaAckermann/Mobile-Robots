
#include "eyebot++.h"

int main()
{
    LCDPrintf("This program will measure the expansion of the square \n");
    LCDPrintf("The robot is moved in the middle of the square\n and oriented parallel to the walls \n");
    LCDMenu("MEASURE", "", "", "END");
    KEYWait(KEY1);

    int width = PSDGet(PSD_FRONT) + PSDGet(PSD_BACK);
    int height = PSDGet(PSD_LEFT) + PSDGet(PSD_RIGHT);

    LCDPrintf("Square size: width = %4d mm, height = %4d mm \n", width, height);

    KEYWait(KEY4);
    return 0;
}