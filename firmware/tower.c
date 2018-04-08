#include <stdint.h>
#include <avr/io.h>
#include "tower.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} SColorRGB;

uint32_t useconds = 0; // useconds since start
SColorRGB layer[8]; 
int li = 0; // actual layer index

void initTower(void) {
  DDRC = 0x3E;
  DDRD = 0x68;
  PORTC = 0x02;
  PORTC = 0x06;
  _delay_ms(100);
  PORTC = 0x02;
  DDRC = 0x3A;
  PORTD = 0x68;

  /* -------------------TIMER INTERRUPT---------------------------------------*/
  TIMSK1 = _BV(OCIE1A);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  OCR1A = 2500; // 1kHz = 20000000 Clock / 8 Prescaler / 2500 Comparematch

  /* -------------------PULSE-WIDTH MODULATION--------------------------------*/
  TCCR0A =
      _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00) |
      _BV(WGM00); // Non inverting mode on OC0A and OC0B, Mode = Mode 3 FAST PWM
  TCCR0B = _BV(CS00); // 1 prescaling

  TCCR2A = _BV(COM2B1) | _BV(WGM20) |
           _BV(WGM20); // Non inverting mode on OC2A, Mode = Mode 3 FAST PWM
  TCCR2B = _BV(CS20);  // 1 prescaling

  /* -------------------ANALOG DIGITAL CONVERTER------------------------------*/
  ADMUX = _BV(REFS0);
  ADMUX = (ADMUX & 0xf0) | 7;
  ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  ADCSRB = _BV(ADTS2) | _BV(ADTS0);
  layer[1].red = 255;
}

uint32_t getTime() {
  return useconds;
}

void setColor(int i, int red, int green, int blue) {
  layer[i].red = red;
  layer[i].green = green;
  layer[i].blue = blue;
}

void nextLayer() {
  li++;
  li = li % 8;
  PORTC &= (0 << LEDCLK);
  OCR0A = 0;
  OCR0B = 0;
  OCR2B = 0;
  _delay_us(30); // Wait for PWM output to be zero
  PORTC |= (1 << LEDCLK);
  OCR0A = layer[li].red;
  OCR0B = layer[li].blue;
  OCR2B = layer[li].green;
}

ISR(TIMER1_COMPA_vect) {
  sei();
  useconds++;
  nextLayer();
}
