// ----------------------------------------------
// C++ example program
// Thomas Braunl 2018
// ----------------------------------------------

#include "eyebot++.h"

int main()
{ LCDPrintf("Hello from C++");
  LCDMenu("DONE", "BYE", "EXIT", "END");
  KEYWait(KEY4); 
  return 0;
}

