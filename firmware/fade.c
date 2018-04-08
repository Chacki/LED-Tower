#include "tower.h"
uint32_t oldTime = 0;
const int uI = 100;

void fade() {
  if(oldTime > getTime() - uI) return;
  oldTime = getTime();
  setColor(0,255,255,255);
}

