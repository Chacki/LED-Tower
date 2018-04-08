#include <stdint.h>
#ifndef TOWER_H
#define TOWER_H
#ifndef F_CPU
#define F_CPU 20000000
#endif
#define TASTER PC0
#define LEDCLK PC1
#define LEDRES PC2
#define LEDENA PC3
void initTower(void);
uint32_t getTime();
void setColor(int i, int red, int green, int blue);
#endif
