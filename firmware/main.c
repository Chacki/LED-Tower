/* Name: main.c
 * Project: Firmware for my custom LED tower
 * Author: Malte Sönnichsen
 */
#include <stdint.h>
#include "tower.h"
#include <avr/interrupt.h> /* for sei() */
#include <avr/io.h>
#include <avr/wdt.h>

int main(void) {
  wdt_enable(WDTO_1S);
  sei();
  initTower();

  for (;;) {
    wdt_reset();
  }
  return 0;
}
