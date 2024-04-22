// ----------------------------------------------
// C++ example program
// Thomas Braunl 2018
// ----------------------------------------------

#include "eyebot++.h"

int main()
{ QVGAcol img;

  CAMInit(QVGA);
  LCDMenu( "", "", "", "END");

  while (KEYRead() != KEY4)
  { CAMGet(img);
    LCDImage(img);
  }
  return 0;
}

